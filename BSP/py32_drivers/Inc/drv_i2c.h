#ifndef __DRV_IIC_H
#define __DRV_IIC_H

#include "main.h"
#include "board.h"
#include "py32f4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define I2C_SPEEDCLOCK   100000             /* Communication speed 100K */


    /* 错误码定义 */
    typedef enum
    {
        I2C_OK = 0,
        I2C_ERROR = -1,
        I2C_ERROR_PARAM = -2,
        I2C_ERROR_BUSY = -3,
        I2C_ERROR_TIMEOUT = -4,
        I2C_ERROR_MODE = -5,
        I2C_ERROR_DMA = -6,
        I2C_ERROR_BUFFER = -7,
    } i2c_err_t;
    /* 工作模式枚举 */
    typedef enum
    {
        I2C_MODE_POLLING = 0, /* 轮询模式 */
        I2C_MODE_INTERRUPT,   /* 中断模式 */
        I2C_MODE_DMA          /* DMA模式 */
    } i2c_mode_t;
    /* i2c实例枚举 */
    typedef enum
    {
        I2C_INSTANCE_1 = 0,
        I2C_INSTANCE_2,
        I2C_INSTANCE_MAX
    } i2c_instance_t;

    // 回调函数类型定义
    typedef void (*i2c_tx_complete_callback_t)(i2c_instance_t instance, void *arg);
    typedef void (*i2c_rx_complete_callback_t)(i2c_instance_t instance, void *arg);
    typedef void (*i2c_error_callback_t)(i2c_instance_t instance, void *arg);
#if (I2C_USE_DMA == 1)
    typedef void (*i2c_dma_idle_callback_t)(i2c_instance_t instance, uint16_t data_size, void *arg);
#endif

    void MX_I2C2_Init(void);
    void I2C2_ScanDevices(void);

#ifdef __cplusplus
}
#endif

#endif
