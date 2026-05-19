add_rules("mode.debug", "mode.release")

set_project("osal-py32f403")
set_version("1.0.0")
set_plat("cross")
set_arch("arm")

includes("../../sdk/toolchains/arm-none-eabi.lua")
set_toolchains("arm-none-eabi", {sdkdir = "D:/APP/path/arm-gcc-15.2"})

add_moduledirs("../../sdk/scripts")

target("osal")
    set_kind("binary")
    set_filename("osal.elf")
    set_targetdir("dist")

    add_defines("PY32F403xD", "USE_HAL_DRIVER")

    add_files("Application/Src/*.c")
    add_files("../../sdk/PY32f403_Firmware_Library/startup/gcc/startup_py32f403xx.s")

    add_files("Task/*.c")
    remove_files("Task/protocol.c")
    add_includedirs("Task")

    add_files("Sensor/QMI8658A/*.c")
    add_includedirs("Sensor/QMI8658A")

    add_files("data_protocol/data_protocol.c")
    add_includedirs("data_protocol")

    add_includedirs("Application/Inc")
    add_includedirs("../../sdk/PY32f403_Firmware_Library/CMSIS/Include")
    add_includedirs("../../sdk/PY32f403_Firmware_Library/CMSIS/Device/PUYA/PY32F403/Include")
    add_includedirs("../../sdk/PY32f403_Firmware_Library/PY32F403_HAL_Driver/Inc")

    add_files("../../sdk/PY32f403_Firmware_Library/PY32F403_HAL_Driver/Src/*.c")

    add_files("../../sdk/py32_drivers/Src/*.c")
    add_includedirs("../../sdk/py32_drivers")
    add_includedirs("../../sdk/py32_drivers/Inc")

    add_files("../../LIB/OSAL/*.c")
    remove_files("../../LIB/OSAL/osal_pt.c")
    add_files("../../LIB/OSAL/hal/*.c")
    add_files("../../LIB/OSAL/MemMang/*.c")
    add_includedirs("../../LIB/OSAL")
    add_includedirs("../../LIB/OSAL/hal")
    add_includedirs("../../LIB/OSAL/Protothreads")
    add_includedirs("../../LIB/Event OS/osConfig")
    add_includedirs("../../LIB/Event OS/osHardware")
    add_includedirs("../../LIB/Event OS/OS")

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
                "-Wl,--defsym,__errno=0",
                {force = true})

    after_build(function(target)
        import("post_build")
        post_build.main(target)
    end)
