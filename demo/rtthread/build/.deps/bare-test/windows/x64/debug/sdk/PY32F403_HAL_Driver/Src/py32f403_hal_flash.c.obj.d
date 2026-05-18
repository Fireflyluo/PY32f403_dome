{
    depfiles = "py32f403_hal_flash.o: sdk\\PY32F403_HAL_Driver\\Src\\py32f403_hal_flash.c  sdk\\PY32F403_HAL_Driver\\Inc/py32f4xx_hal.h  bsp\\CubeMX_Config\\Inc/py32f403_hal_conf.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_def.h  sdk\\CMSIS\\Device\\PUYA\\PY32F403\\Include/py32f4xx.h  sdk\\CMSIS\\Device\\PUYA\\PY32F403\\Include/py32f403xD.h  sdk\\CMSIS\\Include/core_cm4.h sdk\\CMSIS\\Include/cmsis_version.h  sdk\\CMSIS\\Include/cmsis_compiler.h sdk\\CMSIS\\Include/cmsis_gcc.h  sdk\\CMSIS\\Device\\PUYA\\PY32F403\\Include/system_py32f4xx.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f4xx_hal.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_cortex.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_def.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_dma.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_dma_ex.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_exti.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_flash.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_gpio.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_gpio_ex.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_rcc.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_rcc_ex.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_i2c.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_pwr.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_pwr_ex.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_tim.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_tim_ex.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_uart.h  sdk\\PY32F403_HAL_Driver\\Inc/py32f403_hal_iwdg.h\
",
    values = {
        [[C:\APP\Env\tools\gnu_gcc\arm_gcc\mingw\bin\arm-none-eabi-gcc]],
        {
            "-O0",
            [[-Ibsp\CubeMX_Config\Inc]],
            [[-Isdk\CMSIS\Include]],
            [[-Isdk\CMSIS\Device\PUYA\PY32F403\Include]],
            [[-Isdk\PY32F403_HAL_Driver\Inc]],
            "-DPY32F403xD",
            "-DUSE_HAL_DRIVER",
            "-mcpu=cortex-m4",
            "-mthumb",
            "-ffunction-sections",
            "-fdata-sections",
            "-Wall",
            "-g"
        }
    },
    depfiles_format = "gcc",
    files = {
        [[sdk\PY32F403_HAL_Driver\Src\py32f403_hal_flash.c]]
    }
}