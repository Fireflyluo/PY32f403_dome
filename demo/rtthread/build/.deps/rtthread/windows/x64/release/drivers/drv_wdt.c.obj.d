{
    depfiles = "drv_wdt.o: drivers\\drv_wdt.c bsp/board.h drivers/drv_common.h  os\\rt-thread\\include/rthw.h os\\rt-thread\\include/rtdef.h  os\\rt-thread\\include/rtsched.h os\\rt-thread\\include/rttypes.h  os/rtconfig.h os\\rt-thread\\include/rtcompiler.h  os\\rt-thread\\include/rtthread.h os\\rt-thread\\include/rtservice.h  os\\rt-thread\\include/rtm.h os\\rt-thread\\include/rtatomic.h  os\\rt-thread\\include/rtklibc.h os\\rt-thread\\components\\finsh/finsh.h  os\\rt-thread\\components\\drivers\\include/rtdevice.h  os\\rt-thread\\components\\drivers\\include/drivers/core/driver.h  os\\rt-thread\\components\\drivers\\include/drivers/core/bus.h  os\\rt-thread\\components\\drivers\\include/drivers/classes/block.h  os\\rt-thread\\components\\drivers\\include/drivers/classes/char.h  os\\rt-thread\\components\\drivers\\include/drivers/classes/graphic.h  os\\rt-thread\\components\\drivers\\include/drivers/classes/mtd.h  os\\rt-thread\\components\\drivers\\include/drivers/classes/net.h  os\\rt-thread\\components\\drivers\\include/ipc/ringbuffer.h  os\\rt-thread\\components\\drivers\\include/ipc/completion.h  os\\rt-thread\\components\\drivers\\include/ipc/dataqueue.h  os\\rt-thread\\components\\drivers\\include/ipc/workqueue.h  os\\rt-thread\\components\\drivers\\include/ipc/condvar.h  os\\rt-thread\\components\\drivers\\include/ipc/waitqueue.h  os\\rt-thread\\components\\drivers\\include/ipc/pipe.h  os\\rt-thread\\components\\drivers\\include/ipc/condvar.h  os\\rt-thread\\components\\drivers\\include/ipc/poll.h  os\\rt-thread\\components\\drivers\\include/ipc/ringblk_buf.h  os\\rt-thread\\components\\drivers\\include/drivers/spi.h  os\\rt-thread\\components\\drivers\\include/drivers/pin.h  os\\rt-thread\\components\\drivers\\include/drivers/serial.h  os\\rt-thread\\components\\drivers\\include/drivers/i2c.h  os\\rt-thread\\components\\drivers\\include/drivers/i2c_dev.h  os\\rt-thread\\components\\drivers\\include/drivers/pin.h  os\\rt-thread\\components\\drivers\\include/drivers/rt_drv_pwm.h  drivers/drv_gpio.h sdk\\CMSIS\\Device\\PUYA\\PY32F403\\Include/py32f4xx.h  sdk\\CMSIS\\Device\\PUYA\\PY32F403\\Include/py32f403xD.h  sdk\\CMSIS\\Include/core_cm4.h sdk\\CMSIS\\Include/cmsis_version.h  sdk\\CMSIS\\Include/cmsis_compiler.h sdk\\CMSIS\\Include/cmsis_gcc.h  sdk\\CMSIS\\Device\\PUYA\\PY32F403\\Include/system_py32f4xx.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f4xx_hal.h  bsp\\CubeMX_Config\\Inc/py32f403_hal_conf.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_def.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f4xx_hal.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_cortex.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_def.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_dma.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_dma_ex.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_exti.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_flash.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_gpio.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_gpio_ex.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_rcc.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_rcc_ex.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_i2c.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_spi.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_spi_ex.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_pwr.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_pwr_ex.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_tim.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_tim_ex.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_uart.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_iwdg.h\
",
    values = {
        [[C:\APP\Env\tools\gnu_gcc\arm_gcc\mingw\bin\arm-none-eabi-gcc]],
        {
            "-fvisibility=hidden",
            "-O3",
            "-Iapp",
            "-Ibsp",
            [[-Ibsp\CubeMX_Config\Inc]],
            "-Idrivers",
            [[-Idrivers\config]],
            [[-Isdk\CMSIS\Include]],
            [[-Isdk\CMSIS\Device\PUYA\PY32F403\Include]],
            [[-Isdk\PY32F403_HAL_Driver\Inc]],
            "-Ios",
            [[-Ios\rt-thread\include]],
            [[-Ios\rt-thread\libcpu\arm\common]],
            [[-Ios\rt-thread\libcpu\arm\cortex-m4]],
            [[-Ios\rt-thread\components\drivers\include]],
            [[-Ios\rt-thread\components\drivers\include\drivers]],
            [[-Ios\rt-thread\components\finsh]],
            "-DPY32F403xD",
            "-DUSE_HAL_DRIVER",
            "-D__RTTHREAD__",
            "-mcpu=cortex-m4",
            "-mthumb",
            "-mfpu=fpv4-sp-d16",
            "-mfloat-abi=hard",
            "-ffunction-sections",
            "-fdata-sections",
            "-Wall",
            "-Wno-unused-parameter",
            "-g",
            "-Os",
            "-DNDEBUG"
        }
    },
    files = {
        [[drivers\drv_wdt.c]]
    },
    depfiles_format = "gcc"
}