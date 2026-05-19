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

#if (UART_EXAMPLE_MODE == UART_EXAMPLE_DMA)
/* 配置为DMA模式 */

#if (UART_WORK_MODE != UART_MODE_DMA)
#error "此示例需要配置为DMA模式,请检查drv_uart_config.h中的UART_WORK_MODE设置"
#endif

static uart_instance_t g_uart_instance = UART_INSTANCE_2;

/**
 * @brief 初始化UART2为DMA模式
 */
uart_err_t uart_dma_init(void)
{
    uart_err_t ret = uart_init(g_uart_instance, 115200,UART_MODE_DMA);
    if (ret != UART_OK)
    {
        return ret;
    }

    // 启动DMA接收
    uart_dma_start_rx(UART_INSTANCE_2, UART_DMA_MODE_CIRCULAR);
   
    return UART_OK;
}

/**
 * @brief DMA模式发送字符串
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
 * @brief DMA模式接收指定长度数据
 */
void uart_process_received_data(void)
{
    uint8_t rxbuffer[64];  // 适当大小的缓冲区
    static uint32_t last_process_time = 0;
    uint32_t current_time = HAL_GetTick();
    
    // 流量控制：每10ms处理一次
    if (current_time - last_process_time < 10)
    {
        return;
    }
    last_process_time = current_time;
    
    // 读取可用数据
    uint16_t received_bytes = uart_read_from_ring_buffer(g_uart_instance, rxbuffer, sizeof(rxbuffer));
    
    if (received_bytes > 0)
    {
        // 立即回显接收到的数据
        uart_send_async(g_uart_instance, rxbuffer, received_bytes);
        
        // 添加换行符分隔不同的数据包
        uart_send_string("\r\n");
        
        // 调试信息
        char debug_msg[32];
        snprintf(debug_msg, sizeof(debug_msg), "[%d bytes]\r\n", received_bytes);
        uart_send_string(debug_msg);
    }
}
/**
 * @brief DMA模式回环测试示例
 */
void uart_echo_test(void)
{
    // 完整发送提示信息
    uart_send_string("UART2 DMA Echo Test Started\r\n");
    uart_send_string("Type something and press Enter:\r\n");
    uart_send_string("Each input will be echoed immediately.\r\n");
    uart_send_string("===================================\r\n");
    
    uint32_t test_start_time = HAL_GetTick();
    
    while (1)
    {
        // 实时处理接收数据
        uart_process_received_data();
        
        // 每30秒显示状态
        if (HAL_GetTick() - test_start_time > 30000)
        {
            uart_send_string("Echo test is still running...\r\n");
            test_start_time = HAL_GetTick();
        }
        
        HAL_Delay(1);  
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