#include "board.h"
#include "drv_usart.h"
#include "drv_config.h"
#include <string.h>
#include <drivers/serial.h>

#ifdef RT_USING_SERIAL

//#define DRV_DEBUG
#define LOG_TAG             "drv.usart"
#include <drv_log.h>

#if !defined(BSP_USING_UART1) && !defined(BSP_USING_UART2) && !defined(BSP_USING_UART3) && \
    !defined(BSP_USING_UART4) && !defined(BSP_USING_UART5) && !defined(BSP_USING_UART6)
#error "Please define at least one BSP_USING_UARTx"
#endif

#ifdef RT_SERIAL_USING_DMA
static void py32_dma_config(struct rt_serial_device *serial, rt_ubase_t flag);
#endif

enum
{
#ifdef BSP_USING_UART1
    UART1_INDEX,
#endif
#ifdef BSP_USING_UART2
    UART2_INDEX,
#endif
#ifdef BSP_USING_UART3
    UART3_INDEX,
#endif
#ifdef BSP_USING_UART4
    UART4_INDEX,
#endif
#ifdef BSP_USING_UART5
    UART5_INDEX,
#endif
#ifdef BSP_USING_UART6
    UART6_INDEX,
#endif
};

static struct py32_uart_config uart_config[] =
{
#ifdef BSP_USING_UART1
    UART1_CONFIG,
#endif
#ifdef BSP_USING_UART2
    UART2_CONFIG,
#endif
#ifdef BSP_USING_UART3
    UART3_CONFIG,
#endif
#ifdef BSP_USING_UART4
    UART4_CONFIG,
#endif
#ifdef BSP_USING_UART5
    UART5_CONFIG,
#endif
#ifdef BSP_USING_UART6
    UART6_CONFIG,
#endif
};

static struct py32_uart uart_obj[sizeof(uart_config) / sizeof(uart_config[0])] = {0};

rt_uint32_t py32_uart_get_mask(rt_uint32_t word_length, rt_uint32_t parity)
{
    rt_uint32_t mask = 0x00FFU;
    if (word_length == UART_WORDLENGTH_8B)
    {
        if (parity == UART_PARITY_NONE)
        {
            mask = 0x00FFU ;
        }
        else
        {
            mask = 0x007FU ;
        }
    }
    else if (word_length == UART_WORDLENGTH_9B)
    {
        if (parity == UART_PARITY_NONE)
        {
            mask = 0x01FFU ;
        }
        else
        {
            mask = 0x00FFU ;
        }
    }
    return mask;
}

static rt_err_t py32_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct py32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    uart = rt_container_of(serial, struct py32_uart, serial);
    uart->handle.Instance          = uart->config->Instance;
    uart->handle.Init.BaudRate     = cfg->baud_rate;
    uart->handle.Init.Mode         = UART_MODE_TX_RX;
    uart->handle.Init.OverSampling = UART_OVERSAMPLING_16;

    switch (cfg->flowcontrol)
    {
    case RT_SERIAL_FLOWCONTROL_NONE:
        uart->handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        break;
    case RT_SERIAL_FLOWCONTROL_CTSRTS:
        uart->handle.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
        break;
    default:
        uart->handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        break;
    }

    switch (cfg->data_bits)
    {
    case DATA_BITS_8:
        if (cfg->parity == PARITY_ODD || cfg->parity == PARITY_EVEN)
            uart->handle.Init.WordLength = UART_WORDLENGTH_9B;
        else
            uart->handle.Init.WordLength = UART_WORDLENGTH_8B;
        break;
    case DATA_BITS_9:
        uart->handle.Init.WordLength = UART_WORDLENGTH_9B;
        break;
    default:
        uart->handle.Init.WordLength = UART_WORDLENGTH_8B;
        break;
    }

    switch (cfg->stop_bits)
    {
    case STOP_BITS_1:
        uart->handle.Init.StopBits   = UART_STOPBITS_1;
        break;
    case STOP_BITS_2:
        uart->handle.Init.StopBits   = UART_STOPBITS_2;
        break;
    default:
        uart->handle.Init.StopBits   = UART_STOPBITS_1;
        break;
    }

    switch (cfg->parity)
    {
    case PARITY_NONE:
        uart->handle.Init.Parity     = UART_PARITY_NONE;
        break;
    case PARITY_ODD:
        uart->handle.Init.Parity     = UART_PARITY_ODD;
        break;
    case PARITY_EVEN:
        uart->handle.Init.Parity     = UART_PARITY_EVEN;
        break;
    default:
        uart->handle.Init.Parity     = UART_PARITY_NONE;
        break;
    }

#ifdef RT_SERIAL_USING_DMA
    if (!(serial->parent.open_flag & RT_DEVICE_OFLAG_OPEN)) {
        uart->dma_rx.remaining_cnt = cfg->bufsz;
    }
#endif

    if (HAL_UART_Init(&uart->handle) != HAL_OK)
    {
        return -RT_ERROR;
    }
    uart->DR_mask = py32_uart_get_mask(uart->handle.Init.WordLength, uart->handle.Init.Parity);

    return RT_EOK;
}

static rt_err_t py32_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct py32_uart *uart;
#ifdef RT_SERIAL_USING_DMA
    rt_ubase_t ctrl_arg = (rt_ubase_t)arg;
#endif

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct py32_uart, serial);

    switch (cmd)
    {
    case RT_DEVICE_CTRL_CLR_INT:
        NVIC_DisableIRQ(uart->config->irq_type);
        __HAL_UART_DISABLE_IT(&(uart->handle), UART_IT_RXNE);
#ifdef RT_SERIAL_USING_DMA
        if (ctrl_arg == RT_DEVICE_FLAG_DMA_RX)
        {
            HAL_NVIC_DisableIRQ(uart->config->dma_rx->dma_irq);
            if (HAL_DMA_Abort(&(uart->dma_rx.handle)) != HAL_OK)
            {
                RT_ASSERT(0);
            }
            if (HAL_DMA_DeInit(&(uart->dma_rx.handle)) != HAL_OK)
            {
                RT_ASSERT(0);
            }
        }
        else if(ctrl_arg == RT_DEVICE_FLAG_DMA_TX)
        {
            HAL_NVIC_DisableIRQ(uart->config->dma_tx->dma_irq);
            if (HAL_DMA_DeInit(&(uart->dma_tx.handle)) != HAL_OK)
            {
                RT_ASSERT(0);
            }
        }
#endif
        break;

    case RT_DEVICE_CTRL_SET_INT:
        HAL_NVIC_SetPriority(uart->config->irq_type, 1, 0);
        HAL_NVIC_EnableIRQ(uart->config->irq_type);
        __HAL_UART_ENABLE_IT(&(uart->handle), UART_IT_RXNE);
        break;

#ifdef RT_SERIAL_USING_DMA
    case RT_DEVICE_CTRL_CONFIG:
        py32_dma_config(serial, ctrl_arg);
        break;
#endif

    case RT_DEVICE_CTRL_CLOSE:
        if (HAL_UART_DeInit(&(uart->handle)) != HAL_OK )
        {
            RT_ASSERT(0)
        }
        break;
    }
    return RT_EOK;
}

static int py32_putc(struct rt_serial_device *serial, char c)
{
    struct py32_uart *uart;
    RT_ASSERT(serial != RT_NULL);

    uart = rt_container_of(serial, struct py32_uart, serial);
    UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_TC);
    uart->handle.Instance->DR = c;
    while (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TC) == RESET);
    return 1;
}

static int py32_getc(struct rt_serial_device *serial)
{
    int ch;
    struct py32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct py32_uart, serial);

    ch = -1;
    if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_RXNE) != RESET)
    {
        ch = uart->handle.Instance->DR & uart->DR_mask;
    }
    return ch;
}

static rt_ssize_t py32_dma_transmit(struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction)
{
    struct py32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(buf != RT_NULL);
    uart = rt_container_of(serial, struct py32_uart, serial);

    if (size == 0)
    {
        return 0;
    }

    if (RT_SERIAL_DMA_TX == direction)
    {
        if (HAL_UART_Transmit_DMA(&uart->handle, buf, size) == HAL_OK)
        {
            return size;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

#ifdef RT_SERIAL_USING_DMA
static void dma_recv_isr(struct rt_serial_device *serial, rt_uint8_t isr_flag)
{
    struct py32_uart *uart;
    rt_base_t level;
    rt_size_t recv_len, counter;

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct py32_uart, serial);

    level = rt_hw_interrupt_disable();
    recv_len = 0;
    counter = __HAL_DMA_GET_COUNTER(&(uart->dma_rx.handle));

    switch (isr_flag)
    {
    case UART_RX_DMA_IT_IDLE_FLAG:
        if (counter <= uart->dma_rx.remaining_cnt)
            recv_len = uart->dma_rx.remaining_cnt - counter;
        else
            recv_len = serial->config.bufsz + uart->dma_rx.remaining_cnt - counter;
        break;

    case UART_RX_DMA_IT_HT_FLAG:
        if (counter < uart->dma_rx.remaining_cnt)
            recv_len = uart->dma_rx.remaining_cnt - counter;
        break;

    case UART_RX_DMA_IT_TC_FLAG:
        if(counter >= uart->dma_rx.remaining_cnt)
            recv_len = serial->config.bufsz + uart->dma_rx.remaining_cnt - counter;
        break;

    default:
        break;
    }

    if (recv_len)
    {
        uart->dma_rx.remaining_cnt = counter;
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_DMADONE | (recv_len << 8));
    }
    rt_hw_interrupt_enable(level);
}
#endif

static void uart_isr(struct rt_serial_device *serial)
{
    struct py32_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct py32_uart, serial);

    if ((__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_RXNE) != RESET) &&
            (__HAL_UART_GET_IT_SOURCE(&(uart->handle), UART_IT_RXNE) != RESET))
    {
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
    }
#ifdef RT_SERIAL_USING_DMA
    else if ((uart->uart_dma_flag) && (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_IDLE) != RESET)
             && (__HAL_UART_GET_IT_SOURCE(&(uart->handle), UART_IT_IDLE) != RESET))
    {
        dma_recv_isr(serial, UART_RX_DMA_IT_IDLE_FLAG);
        __HAL_UART_CLEAR_IDLEFLAG(&uart->handle);
    }
    else if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TC) &&
            (__HAL_UART_GET_IT_SOURCE(&(uart->handle), UART_IT_TC) != RESET))
    {
        if (((serial->parent.open_flag & RT_DEVICE_FLAG_DMA_TX) != 0) || (uart->dma_tx_test_active != 0U))
        {
            HAL_UART_IRQHandler(&(uart->handle));
        }
        UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_TC);
    }
#endif
    else
    {
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_ORE) != RESET)
        {
            __HAL_UART_CLEAR_OREFLAG(&uart->handle);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_NE) != RESET)
        {
            __HAL_UART_CLEAR_NEFLAG(&uart->handle);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_FE) != RESET)
        {
            __HAL_UART_CLEAR_FEFLAG(&uart->handle);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_PE) != RESET)
        {
            __HAL_UART_CLEAR_PEFLAG(&uart->handle);
        }
    }
}

#ifdef BSP_USING_UART1
void USART1_IRQHandler(void)
{
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART1_INDEX].serial));
    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_RX_USING_DMA)
void UART1_DMA_RX_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_DMA_IRQHandler(&uart_obj[UART1_INDEX].dma_rx.handle);
    rt_interrupt_leave();
}
#endif
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_TX_USING_DMA)
void UART1_DMA_TX_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_DMA_IRQHandler(&uart_obj[UART1_INDEX].dma_tx.handle);
    rt_interrupt_leave();
}
#endif
#endif /* BSP_USING_UART1 */

#ifdef BSP_USING_UART2
void USART2_IRQHandler(void)
{
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART2_INDEX].serial));
    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_RX_USING_DMA)
void UART2_DMA_RX_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_DMA_IRQHandler(&uart_obj[UART2_INDEX].dma_rx.handle);
    rt_interrupt_leave();
}
#endif
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_TX_USING_DMA)
void UART2_DMA_TX_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_DMA_IRQHandler(&uart_obj[UART2_INDEX].dma_tx.handle);
    rt_interrupt_leave();
}
#endif
#endif /* BSP_USING_UART2 */

#ifdef BSP_USING_UART3
void USART3_IRQHandler(void)
{
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART3_INDEX].serial));
    rt_interrupt_leave();
}
#endif /* BSP_USING_UART3 */

static void py32_uart_get_dma_config(void)
{
#ifdef BSP_USING_UART1
    uart_obj[UART1_INDEX].uart_dma_flag = 0;
#ifdef BSP_UART1_RX_USING_DMA
    uart_obj[UART1_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart1_dma_rx = UART1_DMA_RX_CONFIG;
    uart_config[UART1_INDEX].dma_rx = &uart1_dma_rx;
#endif
#ifdef BSP_UART1_TX_USING_DMA
    uart_obj[UART1_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart1_dma_tx = UART1_DMA_TX_CONFIG;
    uart_config[UART1_INDEX].dma_tx = &uart1_dma_tx;
#endif
#endif

#ifdef BSP_USING_UART2
    uart_obj[UART2_INDEX].uart_dma_flag = 0;
#ifdef BSP_UART2_RX_USING_DMA
    uart_obj[UART2_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart2_dma_rx = UART2_DMA_RX_CONFIG;
    uart_config[UART2_INDEX].dma_rx = &uart2_dma_rx;
#endif
#ifdef BSP_UART2_TX_USING_DMA
    uart_obj[UART2_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart2_dma_tx = UART2_DMA_TX_CONFIG;
    uart_config[UART2_INDEX].dma_tx = &uart2_dma_tx;
#endif
#endif

#ifdef BSP_USING_UART3
    uart_obj[UART3_INDEX].uart_dma_flag = 0;
#ifdef BSP_UART3_RX_USING_DMA
    uart_obj[UART3_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart3_dma_rx = UART3_DMA_RX_CONFIG;
    uart_config[UART3_INDEX].dma_rx = &uart3_dma_rx;
#endif
#ifdef BSP_UART3_TX_USING_DMA
    uart_obj[UART3_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart3_dma_tx = UART3_DMA_TX_CONFIG;
    uart_config[UART3_INDEX].dma_tx = &uart3_dma_tx;
#endif
#endif
}

#ifdef RT_SERIAL_USING_DMA
static void py32_dma_config(struct rt_serial_device *serial, rt_ubase_t flag)
{
    struct rt_serial_rx_fifo *rx_fifo;
    DMA_HandleTypeDef *DMA_Handle;
    struct dma_config *dma_config;
    struct py32_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(flag == RT_DEVICE_FLAG_DMA_TX || flag == RT_DEVICE_FLAG_DMA_RX);
    uart = rt_container_of(serial, struct py32_uart, serial);

    if (RT_DEVICE_FLAG_DMA_RX == flag)
    {
        DMA_Handle = &uart->dma_rx.handle;
        dma_config = uart->config->dma_rx;
    }
    else
    {
        DMA_Handle = &uart->dma_tx.handle;
        dma_config = uart->config->dma_tx;
    }
    LOG_D("%s dma config start", uart->config->name);

    {
        rt_uint32_t tmpreg = 0x00U;
        SET_BIT(RCC->AHB1ENR, dma_config->dma_rcc);
        tmpreg = READ_BIT(RCC->AHB1ENR, dma_config->dma_rcc);
        UNUSED(tmpreg);
    }

    if (RT_DEVICE_FLAG_DMA_RX == flag)
    {
        __HAL_LINKDMA(&(uart->handle), hdmarx, uart->dma_rx.handle);
    }
    else if (RT_DEVICE_FLAG_DMA_TX == flag)
    {
        __HAL_LINKDMA(&(uart->handle), hdmatx, uart->dma_tx.handle);
    }

    DMA_Handle->Instance                 = dma_config->Instance;
    DMA_Handle->Init.PeriphInc           = DMA_PINC_DISABLE;
    DMA_Handle->Init.MemInc              = DMA_MINC_ENABLE;
    DMA_Handle->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    DMA_Handle->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;

    if (RT_DEVICE_FLAG_DMA_RX == flag)
    {
        DMA_Handle->Init.Direction           = DMA_PERIPH_TO_MEMORY;
        DMA_Handle->Init.Mode                = DMA_CIRCULAR;
    }
    else if (RT_DEVICE_FLAG_DMA_TX == flag)
    {
        DMA_Handle->Init.Direction           = DMA_MEMORY_TO_PERIPH;
        DMA_Handle->Init.Mode                = DMA_NORMAL;
    }

    DMA_Handle->Init.Priority            = DMA_PRIORITY_MEDIUM;
    py32_dma_apply_remap(dma_config);
    if (HAL_DMA_DeInit(DMA_Handle) != HAL_OK)
    {
        RT_ASSERT(0);
    }

    if (HAL_DMA_Init(DMA_Handle) != HAL_OK)
    {
        RT_ASSERT(0);
    }

    if (flag == RT_DEVICE_FLAG_DMA_RX)
    {
        rx_fifo = (struct rt_serial_rx_fifo *)serial->serial_rx;
        if (HAL_UART_Receive_DMA(&(uart->handle), rx_fifo->buffer, serial->config.bufsz) != HAL_OK)
        {
            RT_ASSERT(0);
        }
        CLEAR_BIT(uart->handle.Instance->CR3, USART_CR3_EIE);
        __HAL_UART_ENABLE_IT(&(uart->handle), UART_IT_IDLE);
    }

    HAL_NVIC_SetPriority(dma_config->dma_irq, 0, 0);
    HAL_NVIC_EnableIRQ(dma_config->dma_irq);

    HAL_NVIC_SetPriority(uart->config->irq_type, 1, 0);
    HAL_NVIC_EnableIRQ(uart->config->irq_type);

    LOG_D("%s dma config done", uart->config->name);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    RT_ASSERT(huart != NULL);
    struct py32_uart *uart = (struct py32_uart *)huart;
    LOG_D("%s: %s %d\n", __FUNCTION__, uart->config->name, huart->ErrorCode);
    UNUSED(uart);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    struct py32_uart *uart;
    RT_ASSERT(huart != NULL);
    uart = (struct py32_uart *)huart;
    dma_recv_isr(&uart->serial, UART_RX_DMA_IT_TC_FLAG);
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    struct py32_uart *uart;
    RT_ASSERT(huart != NULL);
    uart = (struct py32_uart *)huart;
    dma_recv_isr(&uart->serial, UART_RX_DMA_IT_HT_FLAG);
}

static void _dma_tx_complete(struct rt_serial_device *serial)
{
    struct py32_uart *uart;
    rt_size_t trans_total_index;
    rt_base_t level;

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct py32_uart, serial);

    level = rt_hw_interrupt_disable();
    trans_total_index = __HAL_DMA_GET_COUNTER(&(uart->dma_tx.handle));
    rt_hw_interrupt_enable(level);

    if (trans_total_index == 0)
    {
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_TX_DMADONE);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    struct py32_uart *uart;
    RT_ASSERT(huart != NULL);
    uart = (struct py32_uart *)huart;
    if ((uart->serial.parent.open_flag & RT_DEVICE_FLAG_DMA_TX) != 0)
    {
        _dma_tx_complete(&uart->serial);
    }

    if (uart->dma_tx_test_active != 0U)
    {
        uart->dma_tx_test_active = 0U;
        rt_completion_done(&uart->dma_tx_test_done);
    }
}
#endif  /* RT_SERIAL_USING_DMA */

#ifdef RT_SERIAL_USING_DMA
static struct py32_uart *py32_find_uart(const char *name)
{
    rt_size_t i;

    if (name == RT_NULL)
    {
        return RT_NULL;
    }

    for (i = 0; i < sizeof(uart_obj) / sizeof(uart_obj[0]); i++)
    {
        if ((uart_obj[i].config != RT_NULL) && (strcmp(uart_obj[i].config->name, name) == 0))
        {
            return &uart_obj[i];
        }
    }

    return RT_NULL;
}

rt_err_t rt_hw_usart_dma_test_send(const char *name, const rt_uint8_t *buf, rt_size_t size)
{
    struct py32_uart *uart;
    rt_tick_t timeout_tick;
    rt_base_t level;

    if ((buf == RT_NULL) || (size == 0U))
    {
        return -RT_EINVAL;
    }

    uart = py32_find_uart(name);
    if ((uart == RT_NULL) || ((uart->uart_dma_flag & RT_DEVICE_FLAG_DMA_TX) == 0U))
    {
        return -RT_ENOSYS;
    }

    level = rt_hw_interrupt_disable();
    if (uart->dma_tx_test_active != 0U)
    {
        rt_hw_interrupt_enable(level);
        return -RT_EBUSY;
    }
    uart->dma_tx_test_active = 1U;
    rt_hw_interrupt_enable(level);

    py32_dma_config(&uart->serial, RT_DEVICE_FLAG_DMA_TX);
    rt_completion_init(&uart->dma_tx_test_done);

    if (HAL_UART_Transmit_DMA(&uart->handle, (rt_uint8_t *)buf, size) != HAL_OK)
    {
        uart->dma_tx_test_active = 0U;
        return -RT_ERROR;
    }

    timeout_tick = rt_tick_from_millisecond(1000);
    if (timeout_tick == 0)
    {
        timeout_tick = 1;
    }

    if (rt_completion_wait(&uart->dma_tx_test_done, timeout_tick) != RT_EOK)
    {
        uart->dma_tx_test_active = 0U;
        HAL_UART_AbortTransmit(&uart->handle);
        return -RT_ETIMEOUT;
    }

    return RT_EOK;
}
#endif

static const struct rt_uart_ops py32_uart_ops =
{
    .configure = py32_configure,
    .control = py32_control,
    .putc = py32_putc,
    .getc = py32_getc,
    .dma_transmit = py32_dma_transmit
};

int rt_hw_usart_init(void)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    rt_err_t result = 0;

    py32_uart_get_dma_config();

    for (rt_size_t i = 0; i < sizeof(uart_obj) / sizeof(struct py32_uart); i++)
    {
        uart_obj[i].config = &uart_config[i];
        uart_obj[i].serial.ops    = &py32_uart_ops;
        uart_obj[i].serial.config = config;

        result = rt_hw_serial_register(&uart_obj[i].serial, uart_obj[i].config->name,
                                       RT_DEVICE_FLAG_RDWR
                                       | RT_DEVICE_FLAG_INT_RX
                                       | RT_DEVICE_FLAG_INT_TX
                                       | uart_obj[i].uart_dma_flag
                                       , NULL);
        RT_ASSERT(result == RT_EOK);
    }

    return result;
}
INIT_BOARD_EXPORT(rt_hw_usart_init);

#endif /* RT_USING_SERIAL */
