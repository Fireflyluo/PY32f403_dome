#include "drv_uart.h"
#include <string.h>

/* UART设备结构 */
typedef struct
{
    UART_HandleTypeDef huart;

    /* 回调函数 */
    uart_tx_complete_callback_t tx_complete_callback;
    void *tx_complete_callback_arg;

    uart_rx_complete_callback_t rx_complete_callback;
    void *rx_complete_callback_arg;

    uart_rx_half_complete_callback_t rx_half_complete_callback;
    void *rx_half_complete_callback_arg;

    uart_error_callback_t error_callback;
    void *error_callback_arg;

    /* 状态标志 */
    volatile bool tx_busy;
    volatile bool rx_busy;

    /* 统计信息 */
    uint32_t tx_total;
    uint32_t rx_total;
    uint32_t error_count;
} uart_device_t;

/* 全局设备数组 */
static uart_device_t uart_devices[UART_INSTANCE_MAX];

/* UART基地址映射 */
static USART_TypeDef *const uart_bases[UART_INSTANCE_MAX] = {
    USART1, USART2, USART3};

/* UART实例名称 */
static const char *uart_instance_names[UART_INSTANCE_MAX] = {
    "USART1", "USART2", "USART3"};

/* 获取UART设备指针 */
static uart_device_t *get_uart_device(uart_instance_t instance)
{
    if (instance >= UART_INSTANCE_MAX)
    {
        return NULL;
    }
    return &uart_devices[instance];
}

/* 检查UART是否已初始化 */
static bool is_uart_initialized(uart_instance_t instance)
{
    uart_device_t *dev = get_uart_device(instance);
    return (dev != NULL && dev->huart.Instance != NULL);
}

/* UART初始化 */
uart_err_t uart_init(uart_instance_t instance, uint32_t baudrate)
{
    if (instance >= UART_INSTANCE_MAX)
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];

    /* 检查是否已初始化 */
    if (dev->huart.Instance != NULL)
    {
        return UART_ERROR_BUSY;
    }

    /* 初始化设备结构 */
    memset(dev, 0, sizeof(uart_device_t));

    /* 配置UART句柄 */
    dev->huart.Instance = uart_bases[instance];
    dev->huart.Init.BaudRate = baudrate;
    dev->huart.Init.WordLength = UART_WORDLENGTH_8B;
    dev->huart.Init.StopBits = UART_STOPBITS_1;
    dev->huart.Init.Parity = UART_PARITY_NONE;
    dev->huart.Init.Mode = UART_MODE_TX_RX;
    dev->huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    dev->huart.Init.OverSampling = UART_OVERSAMPLING_16;

    /* 初始化UART */
    if (HAL_UART_Init(&dev->huart) != HAL_OK)
    {
        return UART_ERROR;
    }

    return UART_OK;
}

/* UART反初始化 */
uart_err_t uart_deinit(uart_instance_t instance)
{
    if (!is_uart_initialized(instance))
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];

    /* 停止任何进行中的传输 */
    HAL_UART_DeInit(&dev->huart);

    /* 清除设备结构 */
    memset(dev, 0, sizeof(uart_device_t));

    return UART_OK;
}

/* 发送数据 */
uart_err_t uart_send(uart_instance_t instance, const uint8_t *data, uint16_t size, uint32_t timeout)
{
    if (!is_uart_initialized(instance) || data == NULL || size == 0)
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];

    /* 检查是否正在发送 */
    if (dev->tx_busy)
    {
        return UART_ERROR_BUSY;
    }

    /* 根据模式选择发送方式 */
#if (UART_USE_POLLING == 1)
    /* 轮询模式 */
    HAL_StatusTypeDef status = HAL_UART_Transmit(&dev->huart, (uint8_t *)data, size, timeout);

#elif (UART_USE_INTERRUPT == 1)
    /* 中断模式 */
    HAL_StatusTypeDef status = HAL_UART_Transmit_IT(&dev->huart, (uint8_t *)data, size);

#elif (UART_USE_DMA == 1)
    /* DMA模式 */
    HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(&dev->huart, (uint8_t *)data, size);
#endif

    if (status == HAL_OK)
    {
        dev->tx_busy = true;
        dev->tx_total += size;
        return UART_OK;
    }

    return UART_ERROR;
}

/* 异步发送 */
uart_err_t uart_send_async(uart_instance_t instance, const uint8_t *data, uint16_t size)
{
#if (UART_USE_POLLING == 1)
    /* 轮询模式不支持异步发送 */
    return UART_ERROR_MODE;
#else
    return uart_send(instance, data, size, HAL_MAX_DELAY);
#endif
}

/* 接收数据 */
uart_err_t uart_receive(uart_instance_t instance, uint8_t *buffer, uint16_t size, uint32_t timeout)
{
    if (!is_uart_initialized(instance) || buffer == NULL || size == 0)
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];

    /* 检查是否正在接收 */
    if (dev->rx_busy)
    {
        return UART_ERROR_BUSY;
    }

    /* 根据模式选择接收方式 */
#if (UART_USE_POLLING == 1)
    /* 轮询模式 */
    HAL_StatusTypeDef status = HAL_UART_Receive(&dev->huart, buffer, size, timeout);

#elif (UART_USE_INTERRUPT == 1)
    /* 中断模式 */
    HAL_StatusTypeDef status = HAL_UART_Receive_IT(&dev->huart, buffer, size);

#elif (UART_USE_DMA == 1)
    /* DMA模式 */
    HAL_StatusTypeDef status = HAL_UART_Receive_DMA(&dev->huart, buffer, size);
#endif

    if (status == HAL_OK)
    {
        dev->rx_busy = true;
        return UART_OK;
    }

    return UART_ERROR;
}

/* 异步接收 */
uart_err_t uart_receive_async(uart_instance_t instance, uint8_t *buffer, uint16_t size)
{
#if (UART_USE_POLLING == 1)
    /* 轮询模式不支持异步接收 */
    return UART_ERROR_MODE;
#else
    return uart_receive(instance, buffer, size, HAL_MAX_DELAY);
#endif
}

/* 注册回调函数 */
uart_err_t uart_register_tx_complete_callback(uart_instance_t instance, uart_tx_complete_callback_t callback, void *arg)
{
    if (!is_uart_initialized(instance))
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];
    dev->tx_complete_callback = callback;
    dev->tx_complete_callback_arg = arg;

    return UART_OK;
}

uart_err_t uart_register_rx_complete_callback(uart_instance_t instance, uart_rx_complete_callback_t callback, void *arg)
{
    if (!is_uart_initialized(instance))
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];
    dev->rx_complete_callback = callback;
    dev->rx_complete_callback_arg = arg;

    return UART_OK;
}

uart_err_t uart_register_error_callback(uart_instance_t instance, uart_error_callback_t callback, void *arg)
{
    if (!is_uart_initialized(instance))
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];
    dev->error_callback = callback;
    dev->error_callback_arg = arg;

    return UART_OK;
}

/* 状态查询 */
bool uart_is_tx_busy(uart_instance_t instance)
{
    if (!is_uart_initialized(instance))
    {
        return false;
    }
    return uart_devices[instance].tx_busy;
}

bool uart_is_rx_busy(uart_instance_t instance)
{
    if (!is_uart_initialized(instance))
    {
        return false;
    }
    return uart_devices[instance].rx_busy;
}

/* 获取实例名称 */
const char *uart_get_instance_name(uart_instance_t instance)
{
    if (instance >= UART_INSTANCE_MAX)
    {
        return "UNKNOWN";
    }
    return uart_instance_names[instance];
}

/* ========== HAL库回调函数 ========== */

/* 发送完成回调 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_INSTANCE_MAX; i++)
    {
        if (&uart_devices[i].huart == huart)
        {
            uart_device_t *dev = &uart_devices[i];
            dev->tx_busy = false;

            if (dev->tx_complete_callback != NULL)
            {
                dev->tx_complete_callback((uart_instance_t)i, dev->tx_complete_callback_arg);
            }
            break;
        }
    }
}

/* 接收完成回调 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_INSTANCE_MAX; i++)
    {
        if (&uart_devices[i].huart == huart)
        {
            uart_device_t *dev = &uart_devices[i];
            dev->rx_busy = false;
            dev->rx_total += huart->RxXferSize;

            if (dev->rx_complete_callback != NULL)
            {
                dev->rx_complete_callback((uart_instance_t)i, dev->rx_complete_callback_arg);
            }
            break;
        }
    }
}

/* 接收一半完成回调（DMA模式） */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_INSTANCE_MAX; i++)
    {
        if (&uart_devices[i].huart == huart)
        {
            uart_device_t *dev = &uart_devices[i];

            if (dev->rx_half_complete_callback != NULL)
            {
                dev->rx_half_complete_callback((uart_instance_t)i, dev->rx_half_complete_callback_arg);
            }
            break;
        }
    }
}

/* 错误回调 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_INSTANCE_MAX; i++)
    {
        if (&uart_devices[i].huart == huart)
        {
            uart_device_t *dev = &uart_devices[i];
            dev->error_count++;
            dev->tx_busy = false;
            dev->rx_busy = false;

            if (dev->error_callback != NULL)
            {
                dev->error_callback((uart_instance_t)i, dev->error_callback_arg);
            }
            break;
        }
    }
}