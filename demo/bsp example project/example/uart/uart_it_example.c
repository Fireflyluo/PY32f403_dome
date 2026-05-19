/**
 * @file uart_it_example.c
 * @brief UART2轮询模式使用示例
 * @version 1.0
 * @date 2025-12-04
 */

#include "drv_uart.h"
#include "uart_example.h"
#include "main.h"
#include <string.h>

#if (UART_EXAMPLE_MODE == UART_EXAMPLE_INTERRUPT)
/* 配置为轮询模式 */

#if (UART_WORK_MODE != UART_MODE_INTERRUPT)
#error "此示例需要配置为中断模式,请检查drv_uart_config.h中的UART_WORK_MODE设置"
#endif

static uart_instance_t g_uart_instance = UART_INSTANCE_2;

/**
 * @brief 初始化UART2为轮询模式
 */
uart_err_t uart_it_init(void)
{
    uart_err_t ret = uart_init(g_uart_instance, 115200,UART_MODE_INTERRUPT);
    if (ret != UART_OK)
    {
        //        printf("UART2初始化失败: %d\n", ret);
        return ret;
    }
    //    printf("UART2轮询模式初始化成功,波特率: 115200\n");
    return UART_OK;
}

/**
 * @brief 轮询模式发送字符串（阻塞式）
 */
uart_err_t uart_send_string(const char *str)
{
    if (str == NULL)
    {
        return UART_ERROR_PARAM;
    }

    uint16_t len = strlen(str);
    return uart_send(g_uart_instance, (const uint8_t *)str, len, 1000);
}

/**
 * @brief 轮询模式接收指定长度数据（阻塞式）
 */
uart_err_t uart_receive_data(uint8_t *buffer, uint16_t size, uint16_t *received_len)
{
    if (buffer == NULL || size == 0)
    {
        return UART_ERROR_PARAM;
    }

    uart_err_t ret = uart_receive(g_uart_instance, buffer, size, 0xFFFFFFFFU);

    if (ret == UART_OK && received_len != NULL)
    {
        *received_len = size;
    }
    return ret;
}

/**
 * @brief 轮询模式回环测试示例
 */
void uart_echo_test(void)
{
    uint8_t rx_buffer[5];
    uint16_t received_len = 0;

    // 发送提示信息
    uart_send_string("UART2 Polling Echo Test - Type something:\r\n");

    while (1)
    {
        /* 接收数据 */
        if (uart_receive_data(rx_buffer, 6, &received_len) == UART_OK)
        {
            // 回显接收到的字符
            uart_send(g_uart_instance, rx_buffer, 6, 1000);
            // 检测回车键，结束测试
            if (rx_buffer[0] == '\r')
            {
                uart_send_string("\r\nEcho test finished.\r\n");
                break;
            }
        }

        HAL_Delay(10);
    }
}

/**
 * @brief 主函数示例
 */
int uart_it_example(void)
{
    /* 系统初始化 */

    /* UART2初始化 */
    if (uart_it_init() != UART_OK)
    {
        APP_ErrorHandler();
    }

    /* 发送测试字符串 */
    uart_send_string("Hello from UART2 Interrupt Mode!\r\n");

    /* 执行回环测试 */
    uart_echo_test();

    while (1)
    {
        uint8_t cmd_buffer[4];
        /* 主循环处理其他任务 */
        HAL_Delay(1000);
        uart_send_string("System running...\r\n");
    }
    return 0;
}

#endif