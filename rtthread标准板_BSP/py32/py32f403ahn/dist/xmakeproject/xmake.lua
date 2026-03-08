add_rules("mode.debug", "mode.release")

toolchain("arm-none-eabi")
    set_kind("standalone")
    set_sdkdir("C:/APP/Env/tools/gnu_gcc/arm_gcc/mingw")
toolchain_end()

target("rt-thread")
    set_kind("binary")
    set_toolchains("arm-none-eabi")

    add_files(
        	"applications/main.c",
	"rt-thread/components/libc/compilers/common/cctype.c",
	"rt-thread/components/libc/compilers/common/cstdlib.c",
	"rt-thread/components/libc/compilers/common/cstring.c",
	"rt-thread/components/libc/compilers/common/ctime.c",
	"rt-thread/components/libc/compilers/common/cunistd.c",
	"rt-thread/components/libc/compilers/common/cwchar.c",
	"rt-thread/components/libc/compilers/newlib/syscalls.c",
	"rt-thread/components/drivers/core/device.c",
	"rt-thread/components/drivers/ipc/completion.c",
	"rt-thread/components/drivers/ipc/condvar.c",
	"rt-thread/components/drivers/ipc/dataqueue.c",
	"rt-thread/components/drivers/ipc/pipe.c",
	"rt-thread/components/drivers/ipc/ringblk_buf.c",
	"rt-thread/components/drivers/ipc/ringbuffer.c",
	"rt-thread/components/drivers/ipc/waitqueue.c",
	"rt-thread/components/drivers/ipc/workqueue.c",
	"rt-thread/components/drivers/pin/pin.c",
	"rt-thread/components/drivers/serial/serial.c",
	"PY32F403_HAL/CMSIS/Device/PUYA/PY32F403/Source/Templates/gcc/startup_py32f403xx.s",
	"board/CubeMX_Config/Src/py32f403_hal_msp.c",
	"board/board.c",
	"libraries/drivers/drv_gpio.c",
	"libraries/drivers/drv_gpio.c",
	"libraries/drivers/drv_usart.c",
	"libraries/drivers/drv_usart.c",
	"libraries/drv_common.c",
	"libraries/drv_common.c",
	"rt-thread/components/finsh/cmd.c",
	"rt-thread/components/finsh/msh.c",
	"rt-thread/components/finsh/msh_parse.c",
	"rt-thread/components/finsh/shell.c",
	"rt-thread/src/clock.c",
	"rt-thread/src/components.c",
	"rt-thread/src/idle.c",
	"rt-thread/src/ipc.c",
	"rt-thread/src/irq.c",
	"rt-thread/src/klibc/kstdio.c",
	"rt-thread/src/klibc/kstring.c",
	"rt-thread/src/kservice.c",
	"rt-thread/src/mem.c",
	"rt-thread/src/mempool.c",
	"rt-thread/src/object.c",
	"rt-thread/src/scheduler_comm.c",
	"rt-thread/src/scheduler_up.c",
	"rt-thread/src/thread.c",
	"rt-thread/src/timer.c",
	"rt-thread/libcpu/arm/common/atomic_arm.c",
	"rt-thread/libcpu/arm/common/div0.c",
	"rt-thread/libcpu/arm/common/showmem.c",
	"rt-thread/libcpu/arm/cortex-m4/context_gcc.S",
	"rt-thread/libcpu/arm/cortex-m4/cpuport.c",
	"PY32F403_HAL/CMSIS/Device/PUYA/PY32F403/Source/Templates/system_py32f403.c",
	"PY32F403_HAL/CMSIS/Device/PUYA/PY32F403/Source/Templates/system_py32f403.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_cortex.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_cortex.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_crc.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_crc.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_dma.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_dma.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_gpio.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_gpio.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_pwr.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_pwr.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_pwr_ex.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_pwr_ex.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_rcc.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_rcc.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_rcc_ex.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_rcc_ex.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_uart.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_uart.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_usart.c",
	"PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_usart.c"
    )

    add_includedirs(
        	"applications",
	"rt-thread/components/libc/compilers/common/include",
	"rt-thread/components/libc/compilers/newlib",
	"rt-thread/components/drivers/include",
	"board",
	"board/CubeMX_Config/Inc",
	"libraries/drivers",
	"libraries/drivers/config",
	"libraries",
	"libraries/CMSIS/Include",
	"libraries/HAL_Drivers/CMSIS/Include",
	"libraries/PY32F403_HAL/PY32F403_HAL_Driver/Inc",
	"libraries/PY32F403_HAL/CMSIS/Device/PUYA/PY32F403/Include",
	"rt-thread/components/finsh",
	".",
	"rt-thread/include",
	"rt-thread/libcpu/arm/common",
	"rt-thread/libcpu/arm/cortex-m4",
	"PY32F403_HAL/PY32F403_HAL_Driver/Inc",
	"PY32F403_HAL/CMSIS/Device/PUYA/PY32F403/Include",
	"rt-thread/components/libc/posix/io/epoll",
	"rt-thread/components/libc/posix/io/eventfd",
	"rt-thread/components/libc/posix/io/poll",
	"rt-thread/components/libc/posix/ipc"
    )

    add_defines(
        	"PY32F403xD",
	"RT_USING_LIBC",
	"RT_USING_NEWLIBC",
	"USE_HAL_DRIVER",
	"_POSIX_C_SOURCE=1",
	"__RTTHREAD__"
    )

    add_cflags(
        " -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g" ,{force = true}
    )
    add_cxxflags(
        " -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g" ,{force = true}
    )

    add_asflags(
        " -c -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -x assembler-with-cpp -Wa,-mimplicit-it=thumb  -gdwarf-2" ,{force = true}
    )

    add_ldflags(
        " -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Wl,--gc-sections,-Map=rt-thread.map,-cref,-u,Reset_Handler -T board/linker_scripts/link.lds" ,{force = true}
    )

    set_targetdir("./")
    set_filename("rtthread.elf")

    after_build(function(target)
        os.exec("C:/APP/Env/tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-objcopy -O ihex rtthread.elf rtthread.hex")
        os.exec("C:/APP/Env/tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-objcopy -O binary rtthread.elf rtthread.bin")
        os.exec("C:/APP/Env/tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-size rtthread.elf")
    end)