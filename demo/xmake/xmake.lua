set_project("arm_m4_project")

add_rules("mode.debug", "mode.release")

set_plat("cross")
set_arch("arm")

includes("../../sdk/toolchains/arm-none-eabi.lua")
set_toolchains("arm-none-eabi", {sdkdir = "D:/APP/path/arm-gcc-15.2"})

set_languages("c17", "c++20")
add_moduledirs("../../sdk/scripts")

task("post-build")
    on_run(function ()
        import("core.project.project")
        local target = project.target("arm_mcu")
        if target then
            import("post_build")
            post_build.main(target)
        else
            print("错误: 找不到目标 'arm_mcu'")
        end
    end)
    set_menu {
        usage = "xmake post-build",
        description = "执行构建后处理(生成bin/hex,显示内存使用情况)"
    }

target("arm_mcu")
    set_kind("binary")
    set_extension(".elf")

    add_files("../../sdk/PY32f403_Firmware_Library/startup/gcc/startup_py32f403xx.s")
    add_files("src/*.c")
    add_files("src/syscalls.c")
    add_files("../../sdk/py32_drivers/Src/*.c")
    add_files("../../sdk/PY32f403_Firmware_Library/PY32F403_HAL_Driver/Src/*.c")

    add_includedirs("inc")
    add_includedirs("../../sdk/py32_drivers")
    add_includedirs("../../sdk/py32_drivers/Inc")
    add_includedirs("../../sdk/PY32f403_Firmware_Library/CMSIS/Include")
    add_includedirs("../../sdk/PY32f403_Firmware_Library/PY32F403_HAL_Driver/Inc")
    add_includedirs("../../sdk/PY32f403_Firmware_Library/CMSIS/Device/PUYA/PY32F403/Include")

    add_defines("ARM_MATH_CM4", "USE_STDPERIPH_DRIVER", "PY32F403xD")

    add_cflags("-mcpu=cortex-m4",
               "-mthumb", 
               "-mfpu=fpv4-sp-d16",
               "-mfloat-abi=hard",
               "-Wall",
               "-Wextra",
               "-Wno-unused-parameter", 
               "-ffunction-sections",
               "-fdata-sections",
               {force = true})

    add_cxxflags(
        "-fno-exceptions",
        "-fno-rtti",
        "-fno-use-cxa-atexit",
        {force = true}
    )

    if is_mode("debug") then
        add_cflags("-g", "-O0", {force = true})
        add_defines("DEBUG=1")
    else
        add_cflags("-Os", {force = true})
        add_defines("NDEBUG=1")
    end

    add_ldflags("-mcpu=cortex-m4",
                "-mthumb",
                "-mfpu=fpv4-sp-d16",
                "-mfloat-abi=hard",
                "-Wl,--gc-sections",
                {force = true})

    add_ldflags("-W", {force = true})
    add_ldflags("-T../../sdk/PY32f403_Firmware_Library/startup/gcc/py32f403xd.ld", {force = true})
    add_ldflags("--specs=nano.specs", "--specs=nosys.specs", {force = true})
    add_ldflags("-u_printf_float", "-u_scanf_float", {force = true})

    after_build(function(target)
        import("post_build")
        post_build.main(target)
    end)
