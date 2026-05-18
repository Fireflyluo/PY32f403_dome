{
    files = {
        [[D:\MCU\6.OS\RTT\rt-thread-v5.1.0\libcpu\arm\cortex-m4\context_gcc.S]]
    },
    depfiles = "build\\.objs\\rtthread\\windows\\x64\\release\\289be06fc0a1947697e23af26a024a48\\context_gcc.S.obj:  D:\\MCU\\6.OS\\RTT\\rt-thread-v5.1.0\\libcpu\\arm\\cortex-m4\\context_gcc.S\
",
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
            [[-ID:\MCU\6.OS\RTT\rt-thread-v5.1.0\include]],
            [[-ID:\MCU\6.OS\RTT\rt-thread-v5.1.0\libcpu\arm\common]],
            [[-ID:\MCU\6.OS\RTT\rt-thread-v5.1.0\libcpu\arm\cortex-m4]],
            [[-ID:\MCU\6.OS\RTT\rt-thread-v5.1.0\components\finsh]],
            [[-ID:\MCU\6.OS\RTT\rt-thread-v5.1.0\components\drivers\include]],
            [[-ID:\MCU\6.OS\RTT\rt-thread-v5.1.0\components\drivers\include\drivers]],
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