/**
 * @file uart_dma_example.c
 * @brief UART2 DMA模式使用示例
 * @version 1.0
 * @date 2025-12-04
 */

#include "drv_uart.h"
#include "drv_tool.h"
#include "uart_example.h"
#include "main.h"
#include <string.h>
static uint8_t dummy_rx_buffer[256];
#if (UART_WORK_MODE == UART_MODE_DMA)
/* 配置为轮询模式 */

#if (UART_WORK_MODE != UART_MODE_DMA)
#error "此示例需要配置为DMA模式,请检查drv_uart_config.h中的UART_WORK_MODE设置"
#endif

static uart_instance_t g_uart_instance = UART_INSTANCE_2;

/**
 * @brief 初始化UART2为轮询模式
 */
uart_err_t uart_dma_init(void)
{
    uart_err_t ret = uart_init(g_uart_instance, 115200);
    if (ret != UART_OK)
    {
        //        printf("UART2初始化失败: %d\n", ret);
        return ret;
    }
    //    printf("UART2轮询模式初始化成功,波特率: 115200\n");

    // 启动DMA接收，使用一个较大的缓冲区

    uart_receive_async(g_uart_instance, dummy_rx_buffer, sizeof(dummy_rx_buffer));
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
void uart_process_received_data(void)
{
    // 保持原有实现不变
    uint8_t buffer[32];
    uint16_t received_bytes;

    // 从环形缓冲区读取数据
    received_bytes = uart_read_from_ring_buffer(g_uart_instance, buffer, sizeof(buffer));

    if (received_bytes > 0)
    {
        // 处理接收到的数据
        uart_send_async(g_uart_instance, buffer, received_bytes); // 回显

        // 检查是否有回车符
        for (int i = 0; i < received_bytes; i++)
        {
            if (buffer[i] == '\r')
            {
                uart_send_string("\r\nEcho test finished.\r\n");
                break;
            }
        }
    }
}

/**
 * @brief 轮询模式回环测试示例
 */
void uart_echo_test(void)
{
    // 发送提示信息
    uart_send_string("UART2 DMA Echo Test - Type something:\r\n");

    while (1)
    {
        /* 处理接收到的数据 */
        uart_process_received_data();

        HAL_Delay(10);
    }
}

/**
 * @brief 主函数示例
 */
int uart_dma_example(void)
{
    /* 系统初始化 */

    /* UART2初始化 */
    if (uart_dma_init() != UART_OK)
    {
        APP_ErrorHandler();
    }

    /* 发送测试字符串 */
    uart_send_string("Hello from UART2 DMA Mode!\r\n");

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