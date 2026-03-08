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
	"../../../components/libc/compilers/common/cctype.c",
	"../../../components/libc/compilers/common/cstdlib.c",
	"../../../components/libc/compilers/common/cstring.c",
	"../../../components/libc/compilers/common/ctime.c",
	"../../../components/libc/compilers/common/cunistd.c",
	"../../../components/libc/compilers/common/cwchar.c",
	"../../../components/libc/compilers/newlib/syscalls.c",
	"../../../components/drivers/core/device.c",
	"../../../components/drivers/ipc/completion.c",
	"../../../components/drivers/ipc/condvar.c",
	"../../../components/drivers/ipc/dataqueue.c",
	"../../../components/drivers/ipc/pipe.c",
	"../../../components/drivers/ipc/ringblk_buf.c",
	"../../../components/drivers/ipc/ringbuffer.c",
	"../../../components/drivers/ipc/waitqueue.c",
	"../../../components/drivers/ipc/workqueue.c",
	"../../../components/drivers/pin/pin.c",
	"../../../components/drivers/serial/serial.c",
	"../PY32F403_HAL/CMSIS/Device/PUYA/PY32F403/Source/Templates/gcc/startup_py32f403xx.s",
	"../libraries/drivers/drv_gpio.c",
	"../libraries/drivers/drv_usart.c",
	"../libraries/drv_common.c",
	"board/CubeMX_Config/Src/py32f403_hal_msp.c",
	"board/board.c",
	"../../../components/finsh/msh_parse.c",
	"../../../components/finsh/msh.c",
	"../../../components/finsh/shell.c",
	"../../../components/finsh/cmd.c",
	"../../../src/clock.c",
	"../../../src/components.c",
	"../../../src/idle.c",
	"../../../src/ipc.c",
	"../../../src/irq.c",
	"../../../src/klibc/kstdio.c",
	"../../../src/klibc/kstring.c",
	"../../../src/kservice.c",
	"../../../src/mem.c",
	"../../../src/mempool.c",
	"../../../src/object.c",
	"../../../src/scheduler_comm.c",
	"../../../src/scheduler_up.c",
	"../../../src/thread.c",
	"../../../src/timer.c",
	"../../../libcpu/arm/common/atomic_arm.c",
	"../../../libcpu/arm/common/div0.c",
	"../../../libcpu/arm/common/showmem.c",
	"../../../libcpu/arm/cortex-m4/context_gcc.S",
	"../../../libcpu/arm/cortex-m4/cpuport.c",
	"../PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_uart.c",
	"../PY32F403_HAL/CMSIS/Device/PUYA/PY32F403/Source/Templates/system_py32f403.c",
	"../PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_pwr.c",
	"../PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_pwr_ex.c",
	"../PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_rcc_ex.c",
	"../PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_usart.c",
	"../PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_crc.c",
	"../PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal.c",
	"../PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_dma.c",
	"../PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_cortex.c",
	"../PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_gpio.c",
	"../PY32F403_HAL/PY32F403_HAL_Driver/Src/py32f403_hal_rcc.c"
    )

    add_includedirs(
        	"applications",
	"../../../components/libc/compilers/common/include",
	"../../../components/libc/compilers/newlib",
	"../../../components/drivers/include",
	"board",
	"board/CubeMX_Config/Inc",
	"../libraries/drivers",
	"../libraries/drivers/config",
	"../libraries",
	"../libraries/CMSIS/Include",
	"../libraries/HAL_Drivers/CMSIS/Include",
	"../libraries/PY32F403_HAL/PY32F403_HAL_Driver/Inc",
	"../libraries/PY32F403_HAL/CMSIS/Device/PUYA/PY32F403/Include",
	"../../../components/finsh",
	".",
	"../../../include",
	"../../../libcpu/arm/common",
	"../../../libcpu/arm/cortex-m4",
	"../PY32F403_HAL/PY32F403_HAL_Driver/Inc",
	"../PY32F403_HAL/CMSIS/Device/PUYA/PY32F403/Include",
	"../../../components/libc/posix/io/epoll",
	"../../../components/libc/posix/io/eventfd",
	"../../../components/libc/posix/io/poll",
	"../../../components/libc/posix/ipc"
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