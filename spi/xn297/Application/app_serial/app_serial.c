#include "app_sreial.h"

/**
 * @brief 初始化串口通信
 */
void serial_init(void)
{
    // 配置串口参数
    UART_Config_TypeDef uart_config;

    uart_config.BaudRate = 115200;                  // 设置波特率
    uart_config.Mode = UART_MODE_DMA;               // DMA模式
    uart_config.Parity = UART_PARITY_NONE;          // 无校验
    uart_config.StopBits = UART_STOP_BITS_1;        // 停止位
    uart_config.WordLength = UART_WORD_LENGTH_8B;   // 数据位

    // 初始化串口1
    if (BSP_UART_Init(UART_NUM_1, &uart_config, UART_MODE_DMA) != BSP_UART_OK) {
        // 错误处理
    }

    // 初始化串口2
    if (BSP_UART_Init(UART_NUM_2, &uart_config, UART_MODE_DMA) != BSP_UART_OK) {
        // 错误处理
    }

    // 初始化串口3
    if (BSP_UART_Init(UART_NUM_3, &uart_config, UART_MODE_DMA) != BSP_UART_OK) {
        // 错误处理
    }
}
