/**
 * @file gpio_example.c
 * @brief GPIO使用示例
 * @version 1.0
 * @date 2025-12-04
 */

#include "drv_gpio.h"
#include "main.h"

#define LED_PIN GET_PIN(B, 2)

int gpio_example(void)
{
    /* 初始化LED引脚为输出模式 */
    gpio_init();
    gpio_mode(LED_PIN, GPIO_MODE_OUTPUT_PP);
    
    while (1)
    {
        /* 点亮LED */
        gpio_write(LED_PIN, GPIO_PIN_SET);
        HAL_Delay(500);

        /* 熄灭LED */
        gpio_write(LED_PIN, GPIO_PIN_RESET);
        HAL_Delay(500);

        /* 翻转LED */
        gpio_toggle(LED_PIN);
        HAL_Delay(500);
    }

    return 0;
}