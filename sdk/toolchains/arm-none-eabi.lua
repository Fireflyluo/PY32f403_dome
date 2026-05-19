toolchain("arm-none-eabi")
    set_homepage("https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain")
    set_description("ARM Embedded GCC toolchain for Cortex-M")
    set_kind("standalone")

    on_check(function (toolchain)
        local sdkdir = toolchain:sdkdir() or toolchain:config("sdkdir") or get_config("sdk")
        local bindir = toolchain:bindir() or toolchain:config("bindir") or get_config("bin")
        local cross = toolchain:cross() or toolchain:config("cross") or get_config("cross")

        if (not bindir or bindir == "") and sdkdir and sdkdir ~= "" then
            bindir = path.join(sdkdir, "bin")
        end

        if (not cross or cross == "") then
            cross = "arm-none-eabi-"
        end

        if not bindir or bindir == "" or not cross or cross == "" then
            raise("ARM GCC toolchain not found. Use: set_toolchains(\"arm-none-eabi\", {sdkdir = \"...\"}) or --sdk=...")
        end

        toolchain:config_set("sdkdir", sdkdir)
        toolchain:config_set("bindir", bindir)
        toolchain:config_set("cross", cross)
        return true
    end)

    on_load(function (toolchain)
        local sdkdir = toolchain:config("sdkdir")
        local bindir = toolchain:config("bindir")
        local cross = toolchain:config("cross")

        local function resolve(name)
            local candidates = { path.join(bindir, cross .. name .. ".exe"), path.join(bindir, cross .. name) }
            for _, c in ipairs(candidates) do
                if os.isfile(c) then return c end
            end
            return candidates[1]
        end

        toolchain:add("toolset", "cc", resolve("gcc"))
        toolchain:add("toolset", "cxx", resolve("g++"))
        toolchain:add("toolset", "as", resolve("gcc"))
        toolchain:add("toolset", "ld", resolve("gcc"))
        toolchain:add("toolset", "sh", resolve("gcc"))
        toolchain:add("toolset", "ar", resolve("ar"))
        toolchain:add("toolset", "ranlib", resolve("ranlib"))
        toolchain:add("toolset", "strip", resolve("strip"))
        toolchain:add("toolset", "objcopy", resolve("objcopy"))
        toolchain:add("toolset", "objdump", resolve("objdump"))
        toolchain:add("toolset", "size", resolve("size"))
        toolchain:add("toolset", "nm", resolve("nm"))

        if bindir and bindir ~= "" and is_host("windows") then
            toolchain:add("runenvs", "PATH", bindir)
        end
    end)
toolchain_end()
