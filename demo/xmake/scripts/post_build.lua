-- scripts/post_build.lua
function main(target)
    import("core.base.task")
    import("lib.detect.find_tool")

    -- 定义彩色输出函数
    local function info(msg)
        cprint("${green}[信息] %s", msg)
    end
    
    local function success(msg)
        cprint("${bright green}[成功] %s", msg)
    end
    
    local function warning(msg)
        cprint("${yellow}[警告] %s", msg)
    end
    
    local function error(msg)
        cprint("${bright red}[错误] %s", msg)
    end
    
    local function debug(msg)
        cprint("${blue}[调试] %s", msg)
    end

    local targetfile = target:targetfile()
    info("构建完成，正在检查生成的文件...")
    cprint("${cyan}输出文件: %s", targetfile)
    
    -- 显示详细的内存使用情况
    local function show_memory_usage()
        local size_tool = find_tool("arm-none-eabi-size")
        if size_tool then
            os.exec(format("%s -A %s", size_tool.program, targetfile))
            print("")
        else
            warning("未找到 arm-none-eabi-size 工具")
        end
        
        if size_tool then
            local result = os.iorun(format("%s -B %s", size_tool.program, targetfile))
            if result then
                info("内存使用摘要:")
                print(result)
            end
        end
    end
    
    -- 生成 .bin 和 .hex 文件（采用旧脚本的宽松检查方式）
    local function generate_bin_hex()
        local arm_objcopy = "D:/APP/path/arm-gcc-14.2/bin/arm-none-eabi-objcopy.exe"
        
        -- 检查工具是否存在
        if not os.isfile(arm_objcopy) then
            error("ARM objcopy 工具不存在: " .. arm_objcopy)
            warning("请检查路径是否正确")
            return false
        end
        
        -- 检查目标文件是否存在
        if not os.isfile(targetfile) then
            error("目标文件不存在: " .. targetfile)
            return false
        end
        
        local base_name = path.join(path.directory(targetfile), target:name())
        local all_success = true
        
        -- 生成 .bin 文件
        local bin_file = base_name .. ".bin"
        local bin_cmd = format('"%s" -O binary "%s" "%s"', arm_objcopy, targetfile, bin_file)
        debug("执行命令: " .. bin_cmd)
        
        -- 采用旧脚本的执行方式：不检查os.exec返回值，直接检查结果文件
        os.exec(bin_cmd)
        if os.isfile(bin_file) then
            local bin_size = os.filesize(bin_file)
            if bin_size and bin_size > 0 then
                success("生成二进制文件 [完成]")
                info("文件大小: " .. bin_size .. " 字节")
                info("文件路径: " .. bin_file)
            else
                error("生成的二进制文件为空")
                all_success = false
            end
        else
            error("生成二进制文件 [失败]")
            all_success = false
        end
        
        -- 生成 .hex 文件
        local hex_file = base_name .. ".hex"
        local hex_cmd = format('"%s" -O ihex "%s" "%s"', arm_objcopy, targetfile, hex_file)
        debug("执行命令: " .. hex_cmd)
        
        os.exec(hex_cmd)
        if os.isfile(hex_file) then
            success("生成十六进制文件 [完成]")
            info("文件路径: " .. hex_file)
        else
            error("生成十六进制文件 [失败]")
            all_success = false
        end



        return all_success
    end
    
    -- 计算并显示内存使用百分比
    local function show_memory_percentage()
        local size_tool = find_tool("arm-none-eabi-size")
        if size_tool then
            local result = os.iorun(format("%s -B %s", size_tool.program, targetfile))
            if result then
                local text, data, bss = result:match("(%d+)%s+(%d+)%s+(%d+)")
                if text and data and bss then
                    text = tonumber(text)
                    data = tonumber(data)
                    bss = tonumber(bss)
                    
                    local flash_size = 512 * 1024
                    local ram_size = 128 * 1024
                    
                    local flash_used = text + data
                    local ram_used = data + bss
                    
                    local flash_percent = (flash_used / flash_size) * 100
                    local ram_percent = (ram_used / ram_size) * 100
                    
                    info("内存区域使用情况:")
                    cprint("           FLASH: %8d B %10d B ${green}%6.2f%%", 
                        flash_used, flash_size, flash_percent)
                    cprint("             RAM: %8d B %10d B ${green}%6.2f%%", 
                        ram_used, ram_size, ram_percent)
                end
            end
        end
    end
    
    -- 显示构建信息摘要
    local function show_build_summary()
        cprint("${bright cyan}================================================")
        cprint("${bright cyan}              构建完成摘要")
        cprint("${bright cyan}================================================")
        cprint("${cyan}目标名称: ${white}%s", target:name())
        cprint("${cyan}目标类型: ${white}%s", target:kind())
        cprint("${cyan}平台架构: ${white}%s", target:plat() .. "/" .. target:arch())
        cprint("${cyan}构建模式: ${white}%s", "debug")  -- 直接使用字符串，避免mode()方法调用
        cprint("${cyan}输出路径: ${white}%s", targetfile)
        cprint("${bright cyan}================================================")
    end
    
    -- 主执行流程
    info("开始后构建处理...")
    
    show_memory_usage()
    show_memory_percentage()
    
    info("开始生成输出文件...")
    local bin_hex_result = generate_bin_hex()
    
    show_build_summary()
    
    if bin_hex_result then
        success("后构建处理完成")
    else
        warning("后构建处理完成，但文件生成存在错误")
    end
end