#include "drv_uart.h"
#include <string.h>

/* ==================== 类型定义和全局变量 ============================================ */
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

#if (UART_USE_DMA == 1)
    uart_dma_idle_callback_t dma_idle_callback;
    void *dma_idle_callback_arg;
#endif

    /* 状态标志 */
    volatile bool tx_busy;
    volatile bool rx_busy;

    /* 统计信息 */
    uint32_t tx_total;    // 发送总字节数
    uint32_t rx_total;    // 接收总字节数
    uint32_t error_count; // 发生的错误次数

#if (UART_USE_DMA == 1)
    /* DMA特定字段 */
    DMA_HandleTypeDef hdma_tx;
    DMA_HandleTypeDef hdma_rx;

    uint8_t dma_rx_buffer[UART_DMA_BUFFER_SIZE];
    uint8_t dma_tx_buffer[UART_DMA_BUFFER_SIZE];

    volatile uint16_t dma_rx_write_pos;  // DMA写入位置
    volatile uint16_t dma_rx_read_pos;   // 应用读取位置
    volatile uint16_t dma_rx_data_count; // 当前接收数据量
    bool dma_rx_circular_mode;           // 循环模式标志
    bool dma_tx_busy;                    // DMA发送忙标志
    bool dma_rx_busy;                    // DMA接收忙标志

    /* 环形缓冲区（使用drv_tool中的实现） */
    ring_buffer_t rx_ring_buffer;
    ring_buffer_t tx_ring_buffer;
    uint8_t rx_software_buffer[UART_RX_BUFFER_SIZE];
    uint8_t tx_software_buffer[UART_TX_BUFFER_SIZE];
#endif

} uart_device_t;

/* 全局设备数组 */
static uart_device_t uart_devices[UART_INSTANCE_MAX];

/* UART基地址映射 */
static USART_TypeDef *const uart_bases[UART_INSTANCE_MAX] = {
    USART1, USART2, USART3, USART4, USART5};

/* UART实例名称 */
static const char *uart_instance_names[UART_INSTANCE_MAX] = {
    "USART1", "USART2", "USART3", "USART4", "USART5"};

/* UART中断号映射 */
static const IRQn_Type uart_irqs[UART_INSTANCE_MAX] = {
    USART1_IRQn, USART2_IRQn, USART3_IRQn, USART4_IRQn, USART5_IRQn};

/* ==================== 静态函数前向声明 ==================== */
static uart_err_t uart_configure_irq_priority(uart_instance_t instance);
#if (UART_USE_DMA == 1)
static uart_err_t uart_dma_init(uart_instance_t instance);
static uart_err_t uart_dma_deinit(uart_instance_t instance);
static uint16_t uart_dma_buffer_to_ring(uart_instance_t instance);
static uart_err_t uart_start_tx_from_ring_buffer(uart_instance_t instance);
#endif
/* ================================ 工具函数 ============================================ */
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

/* ================================ 初始化/反初始化 ============================================ */

/** UART初始化
 * @brief  配置并初始化指定的UART实例
 * @param  instance: UART实例索引(UART_INSTANCE_MAX内)
 * @param  baudrate: 波特率(如9600, 115200等)
 * @retval
 *          UART_OK: 初始化成功
 *          UART_ERROR_PARAM: 无效的实例索引
 *          UART_ERROR_BUSY: 实例已被初始化
 *          UART_ERROR: HAL初始化失败
 */
uart_err_t uart_init(uart_instance_t instance, uint32_t baudrate)
{
    if (instance >= UART_INSTANCE_MAX)
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];

    /* 初始化检查 */
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
    dev->huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; // 无高级功能初始化
    /* 初始化UART */
    if (HAL_UART_Init(&dev->huart) != HAL_OK)
    {
        return UART_ERROR;
    }

#if (UART_USE_DMA == 1)
    /* 初始化DMA */
    uart_err_t dma_result = uart_dma_init(instance);
    if (dma_result != UART_OK)
    {
        HAL_UART_DeInit(&dev->huart);
        return dma_result;
    }
#endif

    /* 配置中断优先级 */
    //    uart_configure_irq_priority(instance);

    return UART_OK;
}

/** UART反初始化
 * @brief  反初始化指定的UART实例
 * @param  instance: UART实例索引(UART_INSTANCE_MAX内)
 * @retval
 *          UART_OK: 反初始化成功
 *          UART_ERROR_PARAM: 无效的实例索引
 */
uart_err_t uart_deinit(uart_instance_t instance)
{
    if (!is_uart_initialized(instance))
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];

#if (UART_USE_DMA == 1)
    /* 反初始化DMA */
    uart_dma_deinit(instance);
#endif

    /* 停止UART */
    HAL_UART_DeInit(&dev->huart);

    /* 清除设备结构 */
    memset(dev, 0, sizeof(uart_device_t));

    return UART_OK;
}

#if (UART_USE_DMA == 1)
/* DMA初始化 */
static uart_err_t uart_dma_init(uart_instance_t instance)
{
    uart_device_t *dev = get_uart_device(instance);
    if (dev == NULL)
    {
        return UART_ERROR_PARAM;
    }

    /* 初始化TX DMA */
    memset(&dev->hdma_tx, 0, sizeof(DMA_HandleTypeDef));
    dev->hdma_tx.Instance = UART1_TX_DMA_CHANNEL; // 根据实例动态设置
    dev->hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    dev->hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    dev->hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
    dev->hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dev->hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dev->hdma_tx.Init.Mode = DMA_NORMAL;
    dev->hdma_tx.Init.Priority = DMA_PRIORITY_MEDIUM;

    if (HAL_DMA_Init(&dev->hdma_tx) != HAL_OK)
    {
        return UART_ERROR_DMA;
    }

    /* 关联DMA到UART */
    __HAL_LINKDMA(&dev->huart, hdmatx, dev->hdma_tx);

    /* 初始化RX DMA */
    memset(&dev->hdma_rx, 0, sizeof(DMA_HandleTypeDef));
    dev->hdma_rx.Instance = UART1_RX_DMA_CHANNEL; // 根据实例动态设置
    dev->hdma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dev->hdma_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    dev->hdma_rx.Init.MemInc = DMA_MINC_ENABLE;
    dev->hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dev->hdma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dev->hdma_rx.Init.Mode = DMA_CIRCULAR; // 默认循环模式
    dev->hdma_rx.Init.Priority = DMA_PRIORITY_HIGH;

    if (HAL_DMA_Init(&dev->hdma_rx) != HAL_OK)
    {
        HAL_DMA_DeInit(&dev->hdma_tx);
        return UART_ERROR_DMA;
    }

    __HAL_LINKDMA(&dev->huart, hdmarx, dev->hdma_rx);

    /* 初始化环形缓冲区 */
    ring_buffer_init(&dev->rx_ring_buffer, dev->rx_software_buffer, UART_RX_BUFFER_SIZE);
    ring_buffer_init(&dev->tx_ring_buffer, dev->tx_software_buffer, UART_TX_BUFFER_SIZE);

    /* 初始化DMA缓冲区 */
    memset(dev->dma_rx_buffer, 0, UART_DMA_BUFFER_SIZE);
    memset(dev->dma_tx_buffer, 0, UART_DMA_BUFFER_SIZE);
    dev->dma_rx_write_pos = 0;
    dev->dma_rx_read_pos = 0;
    dev->dma_rx_data_count = 0;
    dev->dma_rx_circular_mode = true;
    dev->dma_tx_busy = false;
    dev->dma_rx_busy = false;

    return UART_OK;
}

/* DMA反初始化 */
static uart_err_t uart_dma_deinit(uart_instance_t instance)
{
    uart_device_t *dev = get_uart_device(instance);
    if (dev == NULL)
    {
        return UART_ERROR_PARAM;
    }

    /* 停止DMA传输 */
    HAL_DMA_Stop(&dev->hdma_tx);
    HAL_DMA_Stop(&dev->hdma_rx);

    /* 反初始化DMA */
    HAL_DMA_DeInit(&dev->hdma_tx);
    HAL_DMA_DeInit(&dev->hdma_rx);

    return UART_OK;
}
#endif

/* ==================================== 中断相关函数 ==================================================== */
/* 配置中断优先级 */
static uart_err_t uart_configure_irq_priority(uart_instance_t instance)
{
    if (instance >= UART_INSTANCE_MAX)
    {
        return UART_ERROR_PARAM;
    }

    IRQn_Type irq = uart_irqs[instance];

    /* 设置UART中断优先级 */
    HAL_NVIC_SetPriority(irq, UART_IRQ_PRIORITY, UART_IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(irq);

#if (UART_USE_DMA == 1)
    /* 设置DMA中断优先级 - 需要根据具体DMA通道配置 */
    uart_device_t *dev = &uart_devices[instance];
    HAL_NVIC_SetPriority(dev->hdma_tx.IRQn, UART_DMA_IRQ_PRIORITY, UART_DMA_IRQ_SUB_PRIORITY);
    HAL_NVIC_SetPriority(dev->hdma_rx.IRQn, UART_DMA_IRQ_PRIORITY, UART_DMA_IRQ_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(dev->hdma_tx.IRQn);
    HAL_NVIC_EnableIRQ(dev->hdma_rx.IRQn);
#endif

    return UART_OK;
}

/** 中断优先级配置API */
uart_err_t uart_set_irq_priority(uart_instance_t instance, uint8_t preempt_priority, uint8_t sub_priority)
{
    if (instance >= UART_INSTANCE_MAX)
    {
        return UART_ERROR_PARAM;
    }

    IRQn_Type irq = uart_irqs[instance];

    /* 禁用中断 */
    HAL_NVIC_DisableIRQ(irq);

    /* 设置新优先级 */
    HAL_NVIC_SetPriority(irq, preempt_priority, sub_priority);

    /* 重新启用中断 */
    HAL_NVIC_EnableIRQ(irq);

    return UART_OK;
}

/** DMA中断优先级配置API */
uart_err_t uart_set_dma_irq_priority(uart_instance_t instance, uint8_t preempt_priority, uint8_t sub_priority)
{
#if (UART_USE_DMA == 1)
    if (!is_uart_initialized(instance))
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];

    /* 设置TX DMA中断优先级 */
    HAL_NVIC_DisableIRQ(dev->hdma_tx.IRQn);
    HAL_NVIC_SetPriority(dev->hdma_tx.IRQn, preempt_priority, sub_priority);
    HAL_NVIC_EnableIRQ(dev->hdma_tx.IRQn);

    /* 设置RX DMA中断优先级 */
    HAL_NVIC_DisableIRQ(dev->hdma_rx.IRQn);
    HAL_NVIC_SetPriority(dev->hdma_rx.IRQn, preempt_priority, sub_priority);
    HAL_NVIC_EnableIRQ(dev->hdma_rx.IRQn);

    return UART_OK;
#else
    return UART_ERROR_MODE;
#endif
}

/* ==================================== 数据传输 ==================================================== */
/** 发送数据
 * @brief  通过指定UART实例发送数据
 * @param  instance: UART实例索引(UART_INSTANCE_MAX内)
 * @param  data: 指向要发送数据的指针
 * @param  size: 要发送的数据字节数
 * @param  timeout: 超时时间(毫秒)，仅轮询模式有效
 * @retval
 *          UART_OK: 发送请求已接受
 *          UART_ERROR_PARAM: 无效参数
 *          UART_ERROR_BUSY: UART正忙
 *          UART_ERROR: 发送失败
 * @note    参数检查：确保UART已初始化、数据指针有效、数据长度>0
 *          状态检查：确保当前没有进行中的发送操作
 *          根据编译选项选择传输模式：
 *              - 轮询模式：阻塞直到完成
 *              - 中断模式：启动传输后立即返回，完成时触发中断
 *              - DMA模式：使用DMA控制器进行零负载数据传输
 *          更新状态和统计信息
 */
uart_err_t uart_send(uart_instance_t instance, const uint8_t *data, uint16_t size, uint32_t timeout)
{
    if (!is_uart_initialized(instance) || data == NULL || size == 0)
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];

    /* 根据模式选择发送方式 */
#if (UART_USE_POLLING == 1)
    /* 轮询模式 */

    HAL_StatusTypeDef status = HAL_UART_Transmit(&dev->huart, (uint8_t *)data, size, timeout);
    if (status == HAL_OK)
    {
        dev->tx_total += size;
        return UART_OK;
    }
#elif (UART_USE_INTERRUPT == 1)
    /* 中断模式 */
    if (dev->tx_busy)
    {
        return UART_ERROR_BUSY;
    }
    HAL_StatusTypeDef status = HAL_UART_Transmit_IT(&dev->huart, (uint8_t *)data, size);
    if (status == HAL_OK)
    {
        dev->tx_busy = true;
        dev->tx_total += size;
        return UART_OK;
    }
#elif (UART_USE_DMA == 1)
    /* DMA模式 */
    if (dev->tx_busy || dev->dma_tx_busy)
    {
        return UART_ERROR_BUSY;
    }
    HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(&dev->huart, (uint8_t *)data, size);
    if (status == HAL_OK)
    {
        dev->tx_busy = true;
        dev->dma_tx_busy = true;
        dev->tx_total += size;
        return UART_OK;
    }
#endif

    return UART_ERROR;
}

/** 接收数据
 * @brief  通过指定UART实例接收数据
 * @param  instance: UART实例索引(UART_INSTANCE_MAX内)
 * @param  buffer: 指向接收缓冲区的指针
 * @param  size: 要接收的数据字节数
 * @param  timeout: 超时时间(毫秒)，仅轮询模式有效
 * @retval
 *          UART_OK: 接收请求已接受
 *          UART_ERROR_PARAM: 无效参数
 *          UART_ERROR_BUSY: UART正忙
 *          UART_ERROR: 发送失败
 * @note    参数检查：确保UART已初始化、缓冲区指针有效、数据长度>0
 *          状态检查：确保当前没有进行中的接收操作
 *          根据编译选项选择传输模式：
 *              - 轮询模式：阻塞直到完成
 *              - 中断模式：启动传输后立即返回，完成时触发中断
 *              - DMA模式：使用DMA控制器进行零负载数据传输
 *          更新状态和统计信息
 */
uart_err_t uart_receive(uart_instance_t instance, uint8_t *buffer, uint16_t size, uint32_t timeout)
{
    if (!is_uart_initialized(instance) || buffer == NULL || size == 0)
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];

    /* 根据模式选择接收方式 */
#if (UART_USE_POLLING == 1)
    /* 轮询模式 */
    HAL_StatusTypeDef status = HAL_UART_Receive(&dev->huart, buffer, size, timeout);
    if (status == HAL_OK)
    {
        dev->tx_total += size;
        return UART_OK;
    }
    else if (status == HAL_TIMEOUT)
    {
        return UART_ERROR_TIMEOUT;
    }

#elif (UART_USE_INTERRUPT == 1)
    /* 中断模式 */
    if (dev->rx_busy)
    {
        return UART_ERROR_BUSY;
    }
    HAL_StatusTypeDef status = HAL_UART_Receive_IT(&dev->huart, buffer, size);
    if (status == HAL_OK)
    {
        dev->rx_busy = true;
        return UART_OK;
    }
#elif (UART_USE_DMA == 1)
    /* DMA模式 */
    if (dev->rx_busy || dev->dma_rx_busy)
    {
        return UART_ERROR_BUSY;
    }
    HAL_StatusTypeDef status = HAL_UART_Receive_DMA(&dev->huart, buffer, size);
    if (status == HAL_OK)
    {
        dev->rx_busy = true;
        dev->dma_rx_busy = true;
        return UART_OK;
    }
#endif

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

/* ==================================== 回调函数管理 ==================================================== */

/** 注册发送完成回调函数
 * @brief  注册发送完成回调函数
 * @param  instance: UART实例索引(UART_INSTANCE_MAX内)
 * @param  callback: 发送完成回调函数指针
 * @param  arg: 回调函数参数指针
 * @retval
 *          UART_OK: 注册成功
 *          UART_ERROR_PARAM: 无效参数
 */
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

/** 注册接收完成回调函数
 * @brief  注册接收完成回调函数
 * @param  instance: UART实例索引(UART_INSTANCE_MAX内)
 * @param  callback: 接收完成回调函数指针
 * @param  arg: 回调函数参数指针
 * @retval
 *          UART_OK: 注册成功
 *          UART_ERROR_PARAM: 无效参数
 */
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

/** 注册错误回调函数
 * @brief  注册错误回调函数
 * @param  instance: UART实例索引(UART_INSTANCE_MAX内)
 * @param  callback: 错误回调函数指针
 * @param  arg: 回调函数参数指针
 * @retval
 *          UART_OK: 注册成功
 *          UART_ERROR_PARAM: 无效参数
 */
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

/* ==================================== 状态查询 ================================================== */
/* 发送状态查询 */
bool uart_is_tx_busy(uart_instance_t instance)
{
    if (!is_uart_initialized(instance))
    {
        return false;
    }
    return uart_devices[instance].tx_busy;
}
/* 接收状态查询 */
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

/* =========================================== HAL回调处理 ======================================================== */
/* 发送完成回调 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_INSTANCE_MAX; i++)
    {
        if (&uart_devices[i].huart == huart)
        {
            uart_device_t *dev = &uart_devices[i];
            dev->tx_busy = false;

#if (UART_USE_DMA == 1)
            dev->dma_tx_busy = false;

            /* 从环形缓冲区中移除已发送的数据 */
            uint16_t sent_size = huart->TxXferSize;
            ring_buffer_skip(&dev->tx_ring_buffer, sent_size);

            /* 如果环形缓冲区中还有数据，继续发送 */
            if (ring_buffer_available(&dev->tx_ring_buffer) > 0)
            {
                uart_start_tx_from_ring_buffer((uart_instance_t)i);
            }
#endif

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

#if (UART_USE_DMA == 1)
            dev->dma_rx_busy = false;

            /* 只有在非循环模式下才更新统计 */
            if (!dev->dma_rx_circular_mode)
            {
                dev->rx_total += huart->RxXferSize;

                /* 将数据从DMA缓冲区转移到环形缓冲区 */
                uart_dma_buffer_to_ring((uart_instance_t)i);
            }
#endif

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

#if (UART_USE_DMA == 1)
            /* 处理前半部分数据 */
            uart_dma_buffer_to_ring((uart_instance_t)i);
#endif

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

#if (UART_USE_DMA == 1)
            dev->dma_tx_busy = false;
            dev->dma_rx_busy = false;
#endif

            if (dev->error_callback != NULL)
            {
                dev->error_callback((uart_instance_t)i, dev->error_callback_arg);
            }
            break;
        }
    }
}
#if (UART_USE_DMA == 1) && (UART_USE_DMA_IDLE_DETECTION == 1)
/* UART空闲中断回调（用于DMA空闲检测） */
void HAL_UART_IdleCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_INSTANCE_MAX; i++)
    {
        if (&uart_devices[i].huart == huart)
        {
            uart_device_t *dev = &uart_devices[i];

            /* 计算空闲期间接收的数据量 */
            uint16_t idle_data_size = uart_dma_buffer_to_ring((uart_instance_t)i);

            if (dev->dma_idle_callback != NULL && idle_data_size > 0)
            {
                dev->dma_idle_callback((uart_instance_t)i, idle_data_size, dev->dma_idle_callback_arg);
            }
            break;
        }
    }
}
#endif

/* =========================== DMA模式下的环形缓冲区操作 =============================================================== */
#if (UART_USE_DMA == 1)

/** 从DMA缓冲区读取数据到环形缓冲区 */
static uint16_t uart_dma_buffer_to_ring(uart_instance_t instance)
{
    uart_device_t *dev = &uart_devices[instance];
    uint16_t bytes_read = 0;

    /* 计算DMA当前写入位置 */
    uint16_t current_dma_pos = UART_DMA_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&dev->hdma_rx);

    /* 处理环形缓冲区边界 */
    if (current_dma_pos >= dev->dma_rx_write_pos)
    {
        /* 正常情况：新数据在旧数据之后 */
        bytes_read = current_dma_pos - dev->dma_rx_write_pos;
    }
    else
    {
        /* 缓冲区回绕：数据跨越缓冲区边界 */
        bytes_read = (UART_DMA_BUFFER_SIZE - dev->dma_rx_write_pos) + current_dma_pos;
    }

    if (bytes_read > 0)
    {
        /* 将数据从DMA缓冲区复制到环形缓冲区 */
        for (uint16_t i = 0; i < bytes_read; i++)
        {
            uint16_t dma_index = (dev->dma_rx_write_pos + i) % UART_DMA_BUFFER_SIZE;
            if (!ring_buffer_put(&dev->rx_ring_buffer, dev->dma_rx_buffer[dma_index]))
            {
                /* 环形缓冲区满，停止复制 */
                break;
            }
        }

        dev->dma_rx_write_pos = current_dma_pos;
        dev->dma_rx_data_count += bytes_read;
    }

    return bytes_read;
}

/** 从环形缓冲区读取数据 */
uint16_t uart_read_from_ring_buffer(uart_instance_t instance, uint8_t *buffer, uint16_t size)
{
    if (!is_uart_initialized(instance) || buffer == NULL || size == 0)
    {
        return 0;
    }

    uart_device_t *dev = &uart_devices[instance];

    /* 先将DMA缓冲区中的数据转移到环形缓冲区 */
    uart_dma_buffer_to_ring(instance);

    /* 从环形缓冲区读取数据 */
    return ring_buffer_get_multiple(&dev->rx_ring_buffer, buffer, size);
}

/** 向环形缓冲区写入数据用于发送 */
uint16_t uart_write_to_ring_buffer(uart_instance_t instance, const uint8_t *data, uint16_t size)
{
    if (!is_uart_initialized(instance) || data == NULL || size == 0)
    {
        return 0;
    }

    uart_device_t *dev = &uart_devices[instance];

    /* 向环形缓冲区写入数据 */
    uint16_t bytes_written = ring_buffer_put_multiple(&dev->tx_ring_buffer, data, size);

    /* 如果DMA发送空闲，自动启动发送 */
    if (!dev->dma_tx_busy && bytes_written > 0)
    {
        uart_start_tx_from_ring_buffer(instance);
    }

    return bytes_written;
}

/** 从环形缓冲区启动DMA发送 */
uart_err_t uart_start_tx_from_ring_buffer(uart_instance_t instance)
{
    if (!is_uart_initialized(instance))
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];

    if (dev->dma_tx_busy)
    {
        return UART_ERROR_BUSY;
    }

    /* 获取环形缓冲区中的数据量 */
    uint16_t available_data = ring_buffer_available(&dev->tx_ring_buffer);
    if (available_data == 0)
    {
        return UART_OK; // 没有数据可发送
    }

    /* 计算实际发送大小（不超过DMA缓冲区大小） */
    uint16_t send_size = (available_data > UART_DMA_BUFFER_SIZE) ? UART_DMA_BUFFER_SIZE : available_data;

    /* 从环形缓冲区读取数据到DMA发送缓冲区 */
    ring_buffer_peek_multiple(&dev->tx_ring_buffer, dev->dma_tx_buffer, send_size, 0);

    /* 启动DMA发送 */
    if (HAL_UART_Transmit_DMA(&dev->huart, dev->dma_tx_buffer, send_size) != HAL_OK)
    {
        return UART_ERROR;
    }

    dev->dma_tx_busy = true;
    return UART_OK;
}

/** 获取环形缓冲区统计信息 */
uart_err_t uart_get_ring_buffer_stats(uart_instance_t instance,
                                      uint16_t *rx_available,
                                      uint16_t *tx_available,
                                      uint16_t *rx_free_space)
{
    if (!is_uart_initialized(instance))
    {
        return UART_ERROR_PARAM;
    }

    uart_device_t *dev = &uart_devices[instance];

    if (rx_available != NULL)
    {
        *rx_available = ring_buffer_available(&dev->rx_ring_buffer);
    }

    if (tx_available != NULL)
    {
        *tx_available = ring_buffer_available(&dev->tx_ring_buffer);
    }

    if (rx_free_space != NULL)
    {
        *rx_free_space = ring_buffer_free_space(&dev->rx_ring_buffer);
    }

    return UART_OK;
}

#endif /* UART_USE_DMA */