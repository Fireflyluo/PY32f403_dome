{
    depfiles = "build\\.objs\\bare-test\\windows\\x64\\debug\\sdk\\CMSIS\\Device\\PUYA\\PY32F403\\Source\\Templates\\gcc\\startup_py32f403xx.s.obj:  sdk\\CMSIS\\Device\\PUYA\\PY32F403\\Source\\Templates\\gcc\\startup_py32f403xx.s\
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
            "-c",
            "-mcpu=cortex-m4",
            "-mthumb",
            "-x",
            "assembler-with-cpp",
            "-Wa,-mimplicit-it=thumb",
            "-g"
        }
    },
    depfiles_format = "gcc",
    files = {
        [[sdk\CMSIS\Device\PUYA\PY32F403\Source\Templates\gcc\startup_py32f403xx.s]]
    }
}