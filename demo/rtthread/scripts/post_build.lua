-- scripts/post_build.lua
-- PY32F403 + RT-Thread 构建后处理

function _parse_ldscript(ldscript_path)
    local flash = 384
    local ram   = 64
    local f = io.open(ldscript_path, "r")
    if not f then return flash, ram end
    for line in f:lines() do
        local name, len = line:match("(%a+)%s+%(.*%)%s*:%s*ORIGIN%s*=%s*[^,]+,%s*LENGTH%s*=%s*(%d+)")
        if name == "ROM" then flash = tonumber(len) or flash
        elseif name == "RAM" then ram = tonumber(len) or ram end
    end
    f:close()
    return flash, ram
end

-- ===================== size -A 逐段解析 =====================
function _analyze_sections(elf_path, tc_bin)
    local size_tool = path.join(tc_bin, "arm-none-eabi-size.exe")
    if not size_tool then return nil end

    local out = os.iorun(format("%s -A %s", size_tool, elf_path))
    if not out then return nil end

    local sec = {text = 0, rodata = 0, data = 0, bss = 0}
    for line in out:gmatch("[^\r\n]+") do
        local name, sz = line:match("^(%S+)%s+(%d+)")
        if name and sz then
            sz = tonumber(sz)
            if name == ".text" or name == ".isr_vector" or name == ".ARM.exidx" then
                sec.text = sec.text + sz
            elseif name == ".rodata" or name == ".ARM" then
                sec.rodata = sec.rodata + sz
            elseif name == ".data" or name == ".init_array" or name == ".fini_array" then
                sec.data = sec.data + sz
            elseif name == ".bss" or name == "._user_heap_stack" then
                sec.bss = sec.bss + sz
            end
        end
    end
    return sec
end

-- ===================== 文件统计 =====================
function _stat_files(target)
    local c, cpp, asm = 0, 0, 0
    for _, src in ipairs(target:sourcefiles()) do
        local ext = path.extension(src)
        if ext == ".c" then c = c + 1
        elseif ext == ".cpp" or ext == ".cxx" or ext == ".cc" then cpp = cpp + 1
        elseif ext == ".s" or ext == ".S" or ext == ".asm" then asm = asm + 1 end
    end
    local total = c + cpp + asm
    if total == 0 then return end
    cprint("+---------+-----------+-----------+--------+")
    cprint("| C 文件  | C++ 文件  | 汇编文件  |  总计  |")
    cprint("+---------+-----------+-----------+--------+")
    cprint("| %-7d | %-9d | %-9d | %-6d |", c, cpp, asm, total)
    cprint("+---------+-----------+-----------+--------+")
    cprint("")
end

-- ===================== EIDE 风格内存分析 =====================
function _show_memory(elf_path, flash_kb, ram_kb, tc_bin)
    local sec = _analyze_sections(elf_path, tc_bin)
    if not sec then
        cprint("${yellow}[ 警告 ]${clear} 无法获取段信息")
        return
    end

    local code    = sec.text
    local ro_data = sec.rodata
    local rw_data = sec.data
    local zi_data = sec.bss

    local total_ro  = code + ro_data
    local total_rw  = rw_data + zi_data
    local total_rom = code + ro_data + rw_data

    local ram_total = ram_kb * 1024
    local rom_total = flash_kb * 1024

    local function draw_bar(pct, w)
        w = w or 20
        local f = math.floor(pct * w / 100 + 0.5)
        local e = w - f
        local bar = ""
        for _ = 1, f do bar = bar .. "#" end
        for _ = 1, e do bar = bar .. " " end
        return bar
    end

    local ram_pct = (total_rw / ram_total) * 100
    local rom_pct = (total_rom / rom_total) * 100

    local ram_color = ram_pct > 80 and "${red}" or (ram_pct > 50 and "${yellow}" or "${green}")
    local rom_color = rom_pct > 80 and "${red}" or (rom_pct > 50 and "${yellow}" or "${green}")

    cprint("Program Size: Code=%d RO-data=%d RW-data=%d ZI-data=%d",
           code, ro_data, rw_data, zi_data)
    cprint("")
    cprint("Total RO  Size (Code + RO Data)          %8d (%8.2f kB)", total_ro, total_ro / 1024)
    cprint("Total RW  Size (RW Data + ZI Data)       %8d (%8.2f kB)", total_rw, total_rw / 1024)
    cprint("Total ROM Size (Code + RO Data + RW Data)%8d (%8.2f kB)", total_rom, total_rom / 1024)
    cprint("")

    cprint("Total Memory Usage:")
    cprint("")
    cprint("  RAM: " .. ram_color .. "[" .. draw_bar(ram_pct, 24) .. "]${clear} %5.1f%%  %6.1fKB/%6.1fKB",
           ram_pct, total_rw / 1024, ram_total / 1024)
    cprint("  ROM: " .. rom_color .. "[" .. draw_bar(rom_pct, 24) .. "]${clear} %5.1f%%  %6.1fKB/%6.1fKB",
           rom_pct, total_rom / 1024, rom_total / 1024)
    cprint("")

    cprint("Section Memory Usage:")
    cprint("")
    cprint("  Flash (0x08000000): " .. rom_color .. "[" .. draw_bar(rom_pct, 24) .. "]${clear} %5.1f%%  %6.1fKB/%6.1fKB",
           rom_pct, total_rom / 1024, rom_total / 1024)
    cprint("    - .text/.isr_vector : %6d bytes", code)
    cprint("    - .rodata           : %6d bytes", ro_data)
    cprint("    - .data             : %6d bytes", rw_data)
    cprint("")
    cprint("  RAM   (0x20000000): " .. ram_color .. "[" .. draw_bar(ram_pct, 24) .. "]${clear} %5.1f%%  %6.1fKB/%6.1fKB",
           ram_pct, total_rw / 1024, ram_total / 1024)
    cprint("    - .data             : %6d bytes", rw_data)
    cprint("    - .bss              : %6d bytes", zi_data)
    cprint("")
end

-- ===================== 主入口 =====================
function main(target)
    local elf = path.translate(target:targetfile())
    if not os.isfile(elf) then
        cprint("${yellow}[ 警告 ]${clear} ELF 不存在: " .. elf)
        return
    end

    local ld = path.join(os.projectdir(), "bsp/linker_scripts/link.lds")
    local flash_kb, ram_kb = _parse_ldscript(ld)

    local tc_bin = "D:/APP/path/arm-gcc-15.2/bin"
    local objcopy = path.join(tc_bin, "arm-none-eabi-objcopy.exe")
    local objdump = path.join(tc_bin, "arm-none-eabi-objdump.exe")

    local base = elf:gsub("%.elf$", "")
    local hex = base .. ".hex"
    local bin = base .. ".bin"
    local lst = base .. ".lst"

    cprint("${cyan}[ 工具 ]${clear} 生成 hex / bin / lst ...")
    cprint("")
    os.execv(objcopy, {"-O", "ihex", elf, hex})
    os.execv(objcopy, {"-O", "binary", elf, bin})

    local lst_out = os.iorunv(objdump, {"-d", "-S", elf})
    if lst_out then
        io.writefile(lst, lst_out)
        cprint("  %s", path.filename(lst))
    end

    cprint("")
    _stat_files(target)
    _show_memory(elf, flash_kb, ram_kb, tc_bin)

    cprint("${bright green}[ 完成 ]${clear} 构建完成")
    cprint("")
end
