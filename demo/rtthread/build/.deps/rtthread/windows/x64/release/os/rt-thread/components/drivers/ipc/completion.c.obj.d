{
    depfiles = "completion.o: os\\rt-thread\\components\\drivers\\ipc\\completion.c  os\\rt-thread\\include/rtdbg.h os/rtconfig.h os\\rt-thread\\include/rthw.h  os\\rt-thread\\include/rtdef.h os\\rt-thread\\include/rtsched.h  os\\rt-thread\\include/rttypes.h os\\rt-thread\\include/rtcompiler.h  os\\rt-thread\\components\\drivers\\include/rtdevice.h  os\\rt-thread\\include/rtthread.h os\\rt-thread\\include/rtservice.h  os\\rt-thread\\include/rtm.h os\\rt-thread\\include/rtatomic.h  os\\rt-thread\\include/rtklibc.h os\\rt-thread\\components\\finsh/finsh.h  os\\rt-thread\\components\\drivers\\include/drivers/core/driver.h  os\\rt-thread\\components\\drivers\\include/drivers/core/bus.h  os\\rt-thread\\components\\drivers\\include/drivers/classes/block.h  os\\rt-thread\\components\\drivers\\include/drivers/classes/char.h  os\\rt-thread\\components\\drivers\\include/drivers/classes/graphic.h  os\\rt-thread\\components\\drivers\\include/drivers/classes/mtd.h  os\\rt-thread\\components\\drivers\\include/drivers/classes/net.h  os\\rt-thread\\components\\drivers\\include/ipc/ringbuffer.h  os\\rt-thread\\components\\drivers\\include/ipc/completion.h  os\\rt-thread\\components\\drivers\\include/ipc/dataqueue.h  os\\rt-thread\\components\\drivers\\include/ipc/workqueue.h  os\\rt-thread\\components\\drivers\\include/ipc/condvar.h  os\\rt-thread\\components\\drivers\\include/ipc/waitqueue.h  os\\rt-thread\\components\\drivers\\include/ipc/pipe.h  os\\rt-thread\\components\\drivers\\include/ipc/condvar.h  os\\rt-thread\\components\\drivers\\include/ipc/poll.h  os\\rt-thread\\components\\drivers\\include/ipc/ringblk_buf.h  os\\rt-thread\\components\\drivers\\include/drivers/spi.h  os\\rt-thread\\components\\drivers\\include/drivers/pin.h  os\\rt-thread\\components\\drivers\\include/drivers/serial.h  os\\rt-thread\\components\\drivers\\include/drivers/i2c.h  os\\rt-thread\\components\\drivers\\include/drivers/i2c_dev.h  os\\rt-thread\\components\\drivers\\include/drivers/pin.h  os\\rt-thread\\components\\drivers\\include/drivers/rt_drv_pwm.h\
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
        [[os\rt-thread\components\drivers\ipc\completion.c]]
    },
    depfiles_format = "gcc"
}