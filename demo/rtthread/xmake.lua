add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

set_project("rtthread-py32f403")
set_version("1.0.0")

includes("toolchain.lua")

add_moduledirs("scripts")

local RTT_ROOT = "os/rt-thread"
local TOOLCHAIN_BIN = "C:/APP/Env/tools/gnu_gcc/arm_gcc/mingw/bin"

target("rtthread")
    set_kind("binary")
    set_toolchains("arm-none-eabi")
    set_filename("rtthread.elf")
    set_targetdir("dist")

    add_defines(
        "PY32F403xD",
        "USE_HAL_DRIVER",
        "__RTTHREAD__",
        "__RT_KERNEL_SOURCE__",
        "__RT_IPC_SOURCE__"
    )

    add_includedirs(
        "app",
        "bsp",
        "bsp/CubeMX_Config/Inc",
        "os",
        "lib/drivers",
        "lib/drivers/config",
        "sdk/CMSIS/Include",
        "sdk/CMSIS/Device/PUYA/PY32F403/Include",
        "sdk/PY32F403_HAL_Driver/Inc",
        path.join(RTT_ROOT, "include"),
        path.join(RTT_ROOT, "libcpu/arm/common"),
        path.join(RTT_ROOT, "libcpu/arm/cortex-m4"),
        path.join(RTT_ROOT, "components/drivers/include"),
        path.join(RTT_ROOT, "components/drivers/include/drivers"),
        path.join(RTT_ROOT, "components/drivers/spi"),
        path.join(RTT_ROOT, "components/drivers/spi/sfud/inc"),
        path.join(RTT_ROOT, "components/dfs/include"),
        path.join(RTT_ROOT, "components/dfs/filesystems/elmfat"),
        path.join(RTT_ROOT, "components/finsh"),
        "os/posix_override"
    )

    add_files(
        "app/main.c",
        "bsp/board.c",
        "bsp/CubeMX_Config/Src/py32f403_hal_msp.c",
        "lib/drivers/drv_common.c",
        "lib/drivers/drv_gpio.c",
        "lib/drivers/drv_usart.c",
        "lib/drivers/drv_spi.c",
        "lib/drivers/drv_hard_i2c.c",
        "lib/drivers/drv_tim.c",
        "lib/drivers/drv_pwm.c",
        "lib/drivers/drv_adc.c",
        "lib/drivers/drv_wdt.c",
        "lib/drivers/drv_rtc.c"
    )

    add_files(
        "sdk/CMSIS/Device/PUYA/PY32F403/Source/Templates/gcc/startup_py32f403xx.s",
        "sdk/CMSIS/Device/PUYA/PY32F403/Source/Templates/system_py32f403.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_cortex.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_dma.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_exti.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_flash.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_gpio.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_pwr.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_pwr_ex.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_rcc.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_rcc_ex.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_tim.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_tim_ex.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_uart.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_spi.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_spi_ex.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_i2c.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_adc.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_adc_ex.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_rtc.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_rtc_ex.c",
        "sdk/PY32F403_HAL_Driver/Src/py32f403_hal_iwdg.c"
    )

    add_files(
        path.join(RTT_ROOT, "src/clock.c"),
        path.join(RTT_ROOT, "src/components.c"),
        path.join(RTT_ROOT, "src/idle.c"),
        path.join(RTT_ROOT, "src/ipc.c"),
        path.join(RTT_ROOT, "src/irq.c"),
        path.join(RTT_ROOT, "src/kservice.c"),
        path.join(RTT_ROOT, "src/mem.c"),
        path.join(RTT_ROOT, "src/mempool.c"),
        path.join(RTT_ROOT, "src/object.c"),
        path.join(RTT_ROOT, "src/scheduler_comm.c"),
        path.join(RTT_ROOT, "src/scheduler_up.c"),
        path.join(RTT_ROOT, "src/thread.c"),
        path.join(RTT_ROOT, "src/timer.c"),
        path.join(RTT_ROOT, "src/klibc/kstdio.c"),
        path.join(RTT_ROOT, "src/klibc/kstring.c"),
        path.join(RTT_ROOT, "libcpu/arm/common/atomic_arm.c"),
        path.join(RTT_ROOT, "libcpu/arm/common/div0.c"),
        path.join(RTT_ROOT, "libcpu/arm/common/showmem.c"),
        path.join(RTT_ROOT, "libcpu/arm/cortex-m4/cpuport.c"),
        path.join(RTT_ROOT, "libcpu/arm/cortex-m4/context_gcc.S"),
        path.join(RTT_ROOT, "components/drivers/core/device.c"),
        path.join(RTT_ROOT, "components/drivers/ipc/completion.c"),
        path.join(RTT_ROOT, "components/drivers/ipc/condvar.c"),
        path.join(RTT_ROOT, "components/drivers/ipc/dataqueue.c"),
        path.join(RTT_ROOT, "components/drivers/ipc/pipe.c"),
        path.join(RTT_ROOT, "components/drivers/ipc/ringblk_buf.c"),
        path.join(RTT_ROOT, "components/drivers/ipc/ringbuffer.c"),
        path.join(RTT_ROOT, "components/drivers/ipc/waitqueue.c"),
        path.join(RTT_ROOT, "components/drivers/ipc/workqueue.c"),
        path.join(RTT_ROOT, "components/drivers/pin/pin.c"),
        path.join(RTT_ROOT, "components/drivers/serial/serial.c"),
        path.join(RTT_ROOT, "components/drivers/i2c/i2c_core.c"),
        path.join(RTT_ROOT, "components/drivers/i2c/i2c_dev.c"),
        path.join(RTT_ROOT, "components/drivers/spi/spi_core.c"),
        path.join(RTT_ROOT, "components/drivers/spi/spi_dev.c"),
        path.join(RTT_ROOT, "components/drivers/spi/spi_flash_sfud.c"),
        path.join(RTT_ROOT, "components/drivers/spi/sfud/src/sfud.c"),
        path.join(RTT_ROOT, "components/drivers/spi/sfud/src/sfud_sfdp.c"),
        path.join(RTT_ROOT, "components/drivers/misc/rt_drv_pwm.c"),
        path.join(RTT_ROOT, "components/dfs/src/dfs.c"),
        path.join(RTT_ROOT, "components/dfs/src/dfs_file.c"),
        path.join(RTT_ROOT, "components/dfs/src/dfs_fs.c"),
        path.join(RTT_ROOT, "components/dfs/src/dfs_posix.c"),
        path.join(RTT_ROOT, "components/dfs/filesystems/elmfat/dfs_elm.c"),
        path.join(RTT_ROOT, "components/dfs/filesystems/elmfat/ff.c"),
        path.join(RTT_ROOT, "components/dfs/filesystems/elmfat/ffunicode.c"),
        path.join(RTT_ROOT, "components/dfs/filesystems/devfs/devfs.c"),
        path.join(RTT_ROOT, "components/finsh/cmd.c"),
        path.join(RTT_ROOT, "components/finsh/msh.c"),
        path.join(RTT_ROOT, "components/finsh/msh_file.c"),
        path.join(RTT_ROOT, "components/finsh/msh_parse.c"),
        path.join(RTT_ROOT, "components/finsh/shell.c"),
        "lib/drivers/syscalls_stub.c"
    )

    add_cflags(
        "-mcpu=cortex-m4",
        "-mthumb",
        "-mfpu=fpv4-sp-d16",
        "-mfloat-abi=hard",
        "-ffunction-sections",
        "-fdata-sections",
        "-Wall",
        "-Wno-unused-parameter",
        "-g",
        {force = true}
    )

    add_cxxflags(
        "-mcpu=cortex-m4",
        "-mthumb",
        "-mfpu=fpv4-sp-d16",
        "-mfloat-abi=hard",
        "-ffunction-sections",
        "-fdata-sections",
        "-g",
        {force = true}
    )

    add_asflags(
        "-c",
        "-mcpu=cortex-m4",
        "-mthumb",
        "-mfpu=fpv4-sp-d16",
        "-mfloat-abi=hard",
        "-x", "assembler-with-cpp",
        "-Wa,-mimplicit-it=thumb",
        "-g",
        {force = true}
    )

    add_ldflags(
        "-mcpu=cortex-m4",
        "-mthumb",
        "-mfpu=fpv4-sp-d16",
        "-mfloat-abi=hard",
        "-ffunction-sections",
        "-fdata-sections",
        "-Wl,--gc-sections",
        "-Wl,--print-memory-usage",
        "-T", "bsp/linker_scripts/link.lds",
        {force = true}
    )

    if is_mode("debug") then
        add_cflags("-Og", {force = true})
    else
        add_cflags("-Os", {force = true})
    end

    after_build(function(target)
        import("post_build")
        post_build.main(target)
    end)

task("menuconfig")
    set_menu({
        usage = "xmake menuconfig",
        description = "Run RT-Thread menuconfig (requires Python + kconfiglib)"
    })
    on_run(function()
        local rtt_root = path.join(os.scriptdir(), "os/rt-thread")
        local kconfig_dir = path.join(os.scriptdir(), "os")
        local old_dir = os.curdir()
        os.cd(kconfig_dir)
        os.execv("python", {
            "-c",
            "import os,sys; " ..
            "os.environ['RTT_DIR']=os.environ['RTT_ROOT']=r'" .. rtt_root .. "'; " ..
            "os.environ['BSP_DIR']=os.environ['BSP_ROOT']=r'" .. os.scriptdir() .. "'; " ..
            "os.environ['PKGS_DIR']=os.environ['PKGS_ROOT']=r'" .. path.join(os.scriptdir(), "packages") .. "'; " ..
            "sys.path.insert(0,r'" .. path.join(rtt_root, "tools") .. "'); " ..
            "from menuconfig import guiconfig; guiconfig(r'" .. rtt_root .. "')"
        })
        os.cd(old_dir)
    end)
