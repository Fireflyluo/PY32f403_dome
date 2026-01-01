/*Info------------------------------------------------
** File Name:               board.c
** Created date:            2025.9.21
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            板极初始化程序
**                          所有板级硬件初始化程序
**--------------------------------------------------------------------------------------------------------
*/

#include "main.h"
#include "board.h"
#include "drv_include.h"

static uart_instance_t log_uart_instance = UART_INSTANCE_2;

void board_init(void)
{
    // led初始化
    gpio_init();
    gpio_mode(LED_PIN, GPIO_MODE_OUTPUT_PP);
    // 串口初始化 中断模式
    uart_init(log_uart_instance, 115200, UART_MODE_INTERRUPT);
    // i2c 初始化 I2C2_SCL -> PB10 I2C2_SDA -> PB11
    MX_I2C2_Init();
}

/***********************************Printf重定向********************************************************/
int fputc(int ch, FILE *f)
{
    uart_send(log_uart_instance, (const uint8_t *)&ch, 1, 1000);
    return (ch);
}
int fgetc(FILE *f)
{

    uint8_t ch = 0;
    uart_receive(log_uart_instance, &ch, 1, 0xFFFFFFFFU);
    return ch;
}