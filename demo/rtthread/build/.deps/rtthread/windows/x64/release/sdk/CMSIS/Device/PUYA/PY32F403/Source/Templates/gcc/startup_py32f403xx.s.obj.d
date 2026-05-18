{
    depfiles = "build\\.objs\\rtthread\\windows\\x64\\release\\sdk\\CMSIS\\Device\\PUYA\\PY32F403\\Source\\Templates\\gcc\\startup_py32f403xx.s.obj:  sdk\\CMSIS\\Device\\PUYA\\PY32F403\\Source\\Templates\\gcc\\startup_py32f403xx.s\
",
    files = {
        [[sdk\CMSIS\Device\PUYA\PY32F403\Source\Templates\gcc\startup_py32f403xx.s]]
    },
    depfiles_format = "gcc",
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
            "-c",
            "-mcpu=cortex-m4",
            "-mthumb",
            "-mfpu=fpv4-sp-d16",
            "-mfloat-abi=hard",
            "-x",
            "assembler-with-cpp",
            "-Wa,-mimplicit-it=thumb",
            "-g"
        }
    }
}