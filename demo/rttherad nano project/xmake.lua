add_rules("mode.debug", "mode.release")

set_project("rttherad-nano-py32f403")
set_version("1.0.0")
set_plat("cross")
set_arch("arm")

includes("../../sdk/toolchains/arm-none-eabi.lua")
set_toolchains("arm-none-eabi", {sdkdir = "D:/APP/path/arm-gcc-15.2"})

add_moduledirs("../../sdk/scripts")

target("rttherad_nano")
    set_kind("binary")
    set_filename("rttherad_nano.elf")
    set_targetdir("dist")

    add_defines("PY32F403xD", "USE_HAL_DRIVER")

    add_files("Application/Src/*.c")
    add_files("../../sdk/PY32f403_Firmware_Library/startup/gcc/startup_py32f403xx.s")

    add_includedirs("Application/Inc")
    add_includedirs("Include")
    add_includedirs("../../sdk/PY32f403_Firmware_Library/CMSIS/Include")
    add_includedirs("../../sdk/PY32f403_Firmware_Library/CMSIS/Device/PUYA/PY32F403/Include")
    add_includedirs("../../sdk/PY32f403_Firmware_Library/PY32F403_HAL_Driver/Inc")

    add_files("../../sdk/PY32f403_Firmware_Library/PY32F403_HAL_Driver/Src/*.c")

    add_files("../../sdk/py32_drivers/Src/*.c")
    add_includedirs("../../sdk/py32_drivers/Inc")

    add_cflags("-mcpu=cortex-m4", "-mthumb", "-mfpu=fpv4-sp-d16", "-mfloat-abi=hard",
               "-ffunction-sections", "-fdata-sections", "-Wall", "-Wno-unused-parameter",
               {force = true})

    if is_mode("debug") then
        add_cflags("-Og", "-g", {force = true})
    else
        add_cflags("-Os", {force = true})
    end

    add_asflags("-c", "-mcpu=cortex-m4", "-mthumb", "-mfpu=fpv4-sp-d16", "-mfloat-abi=hard",
                "-x", "assembler-with-cpp", "-g", {force = true})

    add_ldflags("-mcpu=cortex-m4", "-mthumb", "-mfpu=fpv4-sp-d16", "-mfloat-abi=hard",
                "-ffunction-sections", "-fdata-sections",
                "-Wl,--gc-sections",
                "-T", "../../sdk/PY32f403_Firmware_Library/startup/gcc/py32f403xd.ld",
                "--specs=nano.specs", "--specs=nosys.specs",
                {force = true})

    after_build(function(target)
        import("post_build")
        post_build.main(target)
    end)
