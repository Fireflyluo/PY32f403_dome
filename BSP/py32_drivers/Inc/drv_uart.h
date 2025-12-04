#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#include "drv_uart_config.h"
#include "main.h"
#include "board.h"
#include "py32f4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* 错误码定义 */
    typedef enum
    {
        UART_OK = 0,
        UART_ERROR = -1,
        UART_ERROR_PARAM = -2,
        UART_ERROR_BUSY = -3,
        UART_ERROR_TIMEOUT = -4,
        UART_ERROR_MODE = -5,
        UART_ERROR_DMA = -6,
        UART_ERROR_BUFFER = -7,
    } uart_err_t;

    /* UART实例枚举 */
    typedef enum
    {
        UART_INSTANCE_1 = 0,
        UART_INSTANCE_2,
        UART_INSTANCE_3,
        UART_INSTANCE_4,
        UART_INSTANCE_5,
        UART_INSTANCE_MAX
    } uart_instance_t;

    /* DMA模式配置 */
    typedef enum
    {
        UART_DMA_MODE_NORMAL = 0,   // 单次传输模式
        UART_DMA_MODE_CIRCULAR = 1, // 循环传输模式
    } uart_dma_mode_t;

    /* 回调函数类型定义 */
    typedef void (*uart_tx_complete_callback_t)(uart_instance_t instance, void *arg);
    typedef void (*uart_rx_complete_callback_t)(uart_instance_t instance, void *arg);
    typedef void (*uart_rx_half_complete_callback_t)(uart_instance_t instance, void *arg);
    typedef void (*uart_error_callback_t)(uart_instance_t instance, void *arg);
    typedef void (*uart_dma_idle_callback_t)(uart_instance_t instance, uint16_t data_size, void *arg);

    /* API函数声明 */
    /* 基础功能 */
    uart_err_t uart_init(uart_instance_t instance, uint32_t baudrate);
    uart_err_t uart_deinit(uart_instance_t instance);

    /* 数据传输 */
    uart_err_t uart_send(uart_instance_t instance, const uint8_t *data, uint16_t size, uint32_t timeout);
    uart_err_t uart_receive(uart_instance_t instance, uint8_t *buffer, uint16_t size, uint32_t timeout);
    uart_err_t uart_send_async(uart_instance_t instance, const uint8_t *data, uint16_t size);
    uart_err_t uart_receive_async(uart_instance_t instance, uint8_t *buffer, uint16_t size);

/* DMA特定功能 */
#if (UART_USE_DMA == 1)
    uart_err_t uart_dma_start_rx(uart_instance_t instance, uart_dma_mode_t mode);
    uart_err_t uart_dma_stop_rx(uart_instance_t instance);
    uart_err_t uart_dma_start_tx(uart_instance_t instance, const uint8_t *data, uint16_t size);
    uart_err_t uart_dma_stop_tx(uart_instance_t instance);
    bool uart_dma_is_rx_busy(uart_instance_t instance);
    bool uart_dma_is_tx_busy(uart_instance_t instance);
    uint16_t uart_dma_get_rx_remaining(uart_instance_t instance);
    uint16_t uart_dma_get_rx_data_count(uart_instance_t instance);
    uart_err_t uart_dma_set_rx_buffer(uart_instance_t instance, uint8_t *buffer, uint16_t size);
#endif

    /* 回调函数注册 */
    uart_err_t uart_register_tx_complete_callback(uart_instance_t instance, uart_tx_complete_callback_t callback, void *arg);
    uart_err_t uart_register_rx_complete_callback(uart_instance_t instance, uart_rx_complete_callback_t callback, void *arg);
    uart_err_t uart_register_error_callback(uart_instance_t instance, uart_error_callback_t callback, void *arg);
#if (UART_USE_DMA == 1)
    uart_err_t uart_register_dma_idle_callback(uart_instance_t instance, uart_dma_idle_callback_t callback, void *arg);
#endif

    /* 状态查询 */
    bool uart_is_tx_busy(uart_instance_t instance);
    bool uart_is_rx_busy(uart_instance_t instance);

    const char *uart_get_instance_name(uart_instance_t instance);
    
    /* 中断优先级配置 */
    uart_err_t uart_set_irq_priority(uart_instance_t instance, uint8_t preempt_priority, uint8_t sub_priority);
    uart_err_t uart_set_dma_irq_priority(uart_instance_t instance, uint8_t preempt_priority, uint8_t sub_priority);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_UART_H__ */