#include <rtthread.h>
#include <board.h>

#ifdef RT_USING_SFUD
#include "spi_flash_sfud.h"
#endif

#define LED_PIN GET_PIN(B, 2)

#ifdef RT_USING_SFUD
static int cmd_sfud(int argc, char **argv)
{
    /* 手动附加 CS 片选 */
    rt_base_t cs_pin = GET_PIN(A, 4);
    rt_pin_mode(cs_pin, PIN_MODE_OUTPUT);
    rt_pin_write(cs_pin, PIN_HIGH);

    static struct rt_spi_device spi_dev_w25q128;
    rt_err_t ret = rt_spi_bus_attach_device(
        &spi_dev_w25q128, "spi10", "spi1", RT_NULL);
    if (ret != RT_EOK) {
        rt_kprintf("attach failed: %d\n", ret);
        return -1;
    }
    spi_dev_w25q128.cs_pin = cs_pin;

    rt_spi_flash_device_t flash = rt_sfud_flash_probe("W25Q128", "spi10");
    if (flash == RT_NULL) {
        rt_kprintf("sfud probe failed\n");
        return -1;
    }
    rt_kprintf("sfud probe ok\n");
    return 0;
}
MSH_CMD_EXPORT_ALIAS(cmd_sfud, sfud_test, probe W25Q128 via SFUD);
#endif

int main(void)
{
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}
