local RTT_ROOT = "os/rt-thread"

function config_add(target)
    target:add("defines",
        "PY32F403xD",
        "USE_HAL_DRIVER",
        "__RTTHREAD__"
    )
end

function kernel_add(target)
    target:add("includedirs",
        "os",
        path.join(RTT_ROOT, "include"),
        path.join(RTT_ROOT, "libcpu/arm/common"),
        path.join(RTT_ROOT, "libcpu/arm/cortex-m4")
    )

    target:add("files",
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
        path.join(RTT_ROOT, "libcpu/arm/cortex-m4/context_gcc.S")
    )
end

function components_auto_add(target)
    target:add("includedirs",
        path.join(RTT_ROOT, "components/drivers/include"),
        path.join(RTT_ROOT, "components/drivers/include/drivers"),
        path.join(RTT_ROOT, "components/finsh")
    )

    target:add("files",
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
        path.join(RTT_ROOT, "components/drivers/misc/rt_drv_pwm.c"),
        path.join(RTT_ROOT, "components/finsh/cmd.c"),
        path.join(RTT_ROOT, "components/finsh/msh.c"),
        path.join(RTT_ROOT, "components/finsh/msh_parse.c"),
        path.join(RTT_ROOT, "components/finsh/shell.c")
    )
end

return {
    config_add = config_add,
    kernel_add = kernel_add,
    components_auto_add = components_auto_add
}
