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
    } uart_err_t;

    /* UART实例枚举 */
    typedef enum
    {
        UART_INSTANCE_1 = 0,
        UART_INSTANCE_2,
        UART_INSTANCE_3,
        UART_INSTANCE_4,
        UART_INSTANCE_5,
        UART_INSTANCE_6,
        UART_INSTANCE_7,
        UART_INSTANCE_8,
        UART_INSTANCE_MAX
    } uart_instance_t;

    /* 回调函数类型定义 */
    typedef void (*uart_tx_complete_callback_t)(uart_instance_t instance, void *arg);
    typedef void (*uart_rx_complete_callback_t)(uart_instance_t instance, void *arg);
    typedef void (*uart_rx_half_complete_callback_t)(uart_instance_t instance, void *arg);
    typedef void (*uart_error_callback_t)(uart_instance_t instance, void *arg);

    /* API函数声明 */
    uart_err_t uart_init(uart_instance_t instance, uint32_t baudrate);
    uart_err_t uart_deinit(uart_instance_t instance);

    uart_err_t uart_send(uart_instance_t instance, const uint8_t *data, uint16_t size, uint32_t timeout);
    uart_err_t uart_receive(uart_instance_t instance, uint8_t *buffer, uint16_t size, uint32_t timeout);

    uart_err_t uart_send_async(uart_instance_t instance, const uint8_t *data, uint16_t size);
    uart_err_t uart_receive_async(uart_instance_t instance, uint8_t *buffer, uint16_t size);

    uart_err_t uart_register_tx_complete_callback(uart_instance_t instance, uart_tx_complete_callback_t callback, void *arg);
    uart_err_t uart_register_rx_complete_callback(uart_instance_t instance, uart_rx_complete_callback_t callback, void *arg);
    uart_err_t uart_register_error_callback(uart_instance_t instance, uart_error_callback_t callback, void *arg);

    bool uart_is_tx_busy(uart_instance_t instance);
    bool uart_is_rx_busy(uart_instance_t instance);

    const char *uart_get_instance_name(uart_instance_t instance);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_UART_H__ */