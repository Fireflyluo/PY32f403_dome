#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include <string.h>
#include <board.h>

#include "drv_hard_i2c.h"
#include "drv_config.h"

#if defined(RT_USING_I2C) && defined(BSP_USING_I2C) && \
    (defined(BSP_USING_HARD_I2C1) || defined(BSP_USING_HARD_I2C2) || defined(BSP_USING_HARD_I2C3))

//#define DRV_DEBUG
#define LOG_TAG "drv.i2c.hw"
#include <drv_log.h>

#define I2C_DMA_TRANS_MIN_LEN 1U

enum
{
#ifdef BSP_USING_HARD_I2C1
    I2C1_INDEX,
#endif
#ifdef BSP_USING_HARD_I2C2
    I2C2_INDEX,
#endif
#ifdef BSP_USING_HARD_I2C3
    I2C3_INDEX,
#endif
};

static struct py32_i2c_config i2c_config[] =
{
#ifdef BSP_USING_HARD_I2C1
    I2C1_BUS_CONFIG,
#endif
#ifdef BSP_USING_HARD_I2C2
    I2C2_BUS_CONFIG,
#endif
#ifdef BSP_USING_HARD_I2C3
    I2C3_BUS_CONFIG,
#endif
};

static struct py32_i2c i2c_objs[sizeof(i2c_config) / sizeof(i2c_config[0])] = {0};

static void py32_i2c_enable_dma_clock(const struct dma_config *cfg)
{
    rt_uint32_t tmpreg = 0U;

    if (cfg == RT_NULL)
    {
        return;
    }

    SET_BIT(RCC->AHB1ENR, cfg->dma_rcc);
    tmpreg = READ_BIT(RCC->AHB1ENR, cfg->dma_rcc);
    UNUSED(tmpreg);
}

static rt_err_t py32_i2c_dma_init(struct py32_i2c *i2c_drv)
{
    if (i2c_drv->i2c_dma_flag & I2C_USING_TX_DMA_FLAG)
    {
        DMA_HandleTypeDef *hdma_tx = &i2c_drv->dma.handle_tx;
        struct dma_config *tx_cfg = i2c_drv->config->dma_tx;

        py32_i2c_enable_dma_clock(tx_cfg);
        py32_dma_apply_remap(tx_cfg);

        rt_memset(hdma_tx, 0, sizeof(*hdma_tx));
        hdma_tx->Instance = tx_cfg->Instance;
        hdma_tx->Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_tx->Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_tx->Init.MemInc = DMA_MINC_ENABLE;
        hdma_tx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_tx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_tx->Init.Mode = DMA_NORMAL;
        hdma_tx->Init.Priority = DMA_PRIORITY_LOW;

        if (HAL_DMA_DeInit(hdma_tx) != HAL_OK)
        {
            return -RT_ERROR;
        }
        if (HAL_DMA_Init(hdma_tx) != HAL_OK)
        {
            return -RT_ERROR;
        }

        __HAL_LINKDMA(&i2c_drv->handle, hdmatx, i2c_drv->dma.handle_tx);
        HAL_NVIC_SetPriority(tx_cfg->dma_irq, 1, 0);
        HAL_NVIC_EnableIRQ(tx_cfg->dma_irq);
    }

    if (i2c_drv->i2c_dma_flag & I2C_USING_RX_DMA_FLAG)
    {
        DMA_HandleTypeDef *hdma_rx = &i2c_drv->dma.handle_rx;
        struct dma_config *rx_cfg = i2c_drv->config->dma_rx;

        py32_i2c_enable_dma_clock(rx_cfg);
        py32_dma_apply_remap(rx_cfg);

        rt_memset(hdma_rx, 0, sizeof(*hdma_rx));
        hdma_rx->Instance = rx_cfg->Instance;
        hdma_rx->Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_rx->Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_rx->Init.MemInc = DMA_MINC_ENABLE;
        hdma_rx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_rx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_rx->Init.Mode = DMA_NORMAL;
        hdma_rx->Init.Priority = DMA_PRIORITY_HIGH;

        if (HAL_DMA_DeInit(hdma_rx) != HAL_OK)
        {
            return -RT_ERROR;
        }
        if (HAL_DMA_Init(hdma_rx) != HAL_OK)
        {
            return -RT_ERROR;
        }

        __HAL_LINKDMA(&i2c_drv->handle, hdmarx, i2c_drv->dma.handle_rx);
        HAL_NVIC_SetPriority(rx_cfg->dma_irq, 0, 0);
        HAL_NVIC_EnableIRQ(rx_cfg->dma_irq);
    }

    return RT_EOK;
}

static rt_err_t py32_i2c_init(struct py32_i2c *i2c_drv)
{
    I2C_HandleTypeDef *handle;

    RT_ASSERT(i2c_drv != RT_NULL);

    handle = &i2c_drv->handle;
    rt_memset(handle, 0, sizeof(*handle));
    handle->Instance = i2c_drv->config->Instance;

#if defined(SOC_SERIES_PY32F4)
    handle->Init.ClockSpeed = 100000;
    handle->Init.DutyCycle = I2C_DUTYCYCLE_2;
#endif
    handle->Init.OwnAddress1 = 0;
    handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    handle->Init.OwnAddress2 = 0;
    handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_DeInit(handle) != HAL_OK)
    {
        return -RT_ERROR;
    }
    if (HAL_I2C_Init(handle) != HAL_OK)
    {
        LOG_E("%s init failed", i2c_drv->config->name);
        return -RT_ERROR;
    }

    rt_completion_init(&i2c_drv->completion);

    if ((i2c_drv->i2c_dma_flag & (I2C_USING_TX_DMA_FLAG | I2C_USING_RX_DMA_FLAG)) != 0U)
    {
        if (py32_i2c_dma_init(i2c_drv) != RT_EOK)
        {
            LOG_E("%s dma init failed", i2c_drv->config->name);
            return -RT_ERROR;
        }
    }

    HAL_NVIC_SetPriority(i2c_drv->config->evirq_type, 2, 0);
    HAL_NVIC_EnableIRQ(i2c_drv->config->evirq_type);
    HAL_NVIC_SetPriority(i2c_drv->config->erirq_type, 2, 1);
    HAL_NVIC_EnableIRQ(i2c_drv->config->erirq_type);

    return RT_EOK;
}

static rt_ssize_t py32_i2c_master_xfer(struct rt_i2c_bus_device *bus,
                                       struct rt_i2c_msg msgs[],
                                       rt_uint32_t num)
{
    struct py32_i2c *i2c_obj;
    I2C_HandleTypeDef *handle;
    rt_tick_t timeout_tick;
    rt_uint32_t i;

    if ((bus == RT_NULL) || (msgs == RT_NULL) || (num == 0U))
    {
        return 0;
    }

    i2c_obj = rt_container_of(bus, struct py32_i2c, i2c_bus);
    handle = &i2c_obj->handle;
    timeout_tick = rt_tick_from_millisecond(bus->timeout ? bus->timeout : 100);
    if (timeout_tick == 0)
    {
        timeout_tick = 1;
    }

    for (i = 0; i < num; i++)
    {
        struct rt_i2c_msg *msg = &msgs[i];
        rt_uint16_t dev_addr = (rt_uint16_t)(msg->addr << 1);
        rt_uint32_t mode;
        HAL_StatusTypeDef hal_ret;
        rt_bool_t use_dma;
        rt_bool_t is_read;

        is_read = ((msg->flags & RT_I2C_RD) != 0U) ? RT_TRUE : RT_FALSE;

        if (msg->len == 0U)
        {
            hal_ret = HAL_I2C_IsDeviceReady(handle, dev_addr, 2, bus->timeout ? bus->timeout : 100);
            if (hal_ret != HAL_OK)
            {
                return (i == 0U) ? -RT_ERROR : (rt_ssize_t)i;
            }
            continue;
        }

        if (i == (num - 1U))
        {
            mode = (msg->flags & RT_I2C_NO_STOP) ? I2C_LAST_FRAME_NO_STOP : I2C_LAST_FRAME;
        }
        else
        {
            mode = I2C_LAST_FRAME_NO_STOP;
            if (msgs[i + 1U].flags & RT_I2C_NO_START)
            {
                if (((msgs[i + 1U].flags & RT_I2C_RD) != 0U) == is_read)
                {
                    mode = I2C_FIRST_AND_NEXT_FRAME;
                }
            }
        }

        use_dma = RT_FALSE;
        if (is_read && (i2c_obj->i2c_dma_flag & I2C_USING_RX_DMA_FLAG) && (msg->len >= I2C_DMA_TRANS_MIN_LEN))
        {
            use_dma = RT_TRUE;
        }
        else if ((!is_read) && (i2c_obj->i2c_dma_flag & I2C_USING_TX_DMA_FLAG) && (msg->len >= I2C_DMA_TRANS_MIN_LEN))
        {
            use_dma = RT_TRUE;
        }

        if (use_dma)
        {
            rt_completion_init(&i2c_obj->completion);
            if (is_read)
            {
                hal_ret = HAL_I2C_Master_Seq_Receive_DMA(handle, dev_addr, msg->buf, msg->len, mode);
            }
            else
            {
                hal_ret = HAL_I2C_Master_Seq_Transmit_DMA(handle, dev_addr, msg->buf, msg->len, mode);
            }
            if (hal_ret != HAL_OK)
            {
                return (i == 0U) ? -RT_ERROR : (rt_ssize_t)i;
            }
            if (rt_completion_wait(&i2c_obj->completion, timeout_tick) != RT_EOK)
            {
                HAL_I2C_Master_Abort_IT(handle, dev_addr);
                return (i == 0U) ? -RT_ETIMEOUT : (rt_ssize_t)i;
            }
        }
        else
        {
            rt_completion_init(&i2c_obj->completion);
            if (is_read)
            {
                hal_ret = HAL_I2C_Master_Seq_Receive_IT(handle, dev_addr, msg->buf, msg->len, mode);
            }
            else
            {
                hal_ret = HAL_I2C_Master_Seq_Transmit_IT(handle, dev_addr, msg->buf, msg->len, mode);
            }
            if (hal_ret != HAL_OK)
            {
                return (i == 0U) ? -RT_ERROR : (rt_ssize_t)i;
            }
            if (rt_completion_wait(&i2c_obj->completion, timeout_tick) != RT_EOK)
            {
                HAL_I2C_Master_Abort_IT(handle, dev_addr);
                return (i == 0U) ? -RT_ETIMEOUT : (rt_ssize_t)i;
            }
        }
    }

    return (rt_ssize_t)num;
}

static const struct rt_i2c_bus_device_ops py32_i2c_ops =
{
    .master_xfer = py32_i2c_master_xfer,
    RT_NULL,
    RT_NULL,
};

static void py32_get_dma_info(void)
{
#ifdef BSP_USING_HARD_I2C1
#ifdef BSP_I2C1_RX_USING_DMA
    i2c_objs[I2C1_INDEX].i2c_dma_flag |= I2C_USING_RX_DMA_FLAG;
    static struct dma_config i2c1_dma_rx = I2C1_RX_DMA_CONFIG;
    i2c_config[I2C1_INDEX].dma_rx = &i2c1_dma_rx;
#endif
#ifdef BSP_I2C1_TX_USING_DMA
    i2c_objs[I2C1_INDEX].i2c_dma_flag |= I2C_USING_TX_DMA_FLAG;
    static struct dma_config i2c1_dma_tx = I2C1_TX_DMA_CONFIG;
    i2c_config[I2C1_INDEX].dma_tx = &i2c1_dma_tx;
#endif
#endif

#ifdef BSP_USING_HARD_I2C2
#ifdef BSP_I2C2_RX_USING_DMA
    i2c_objs[I2C2_INDEX].i2c_dma_flag |= I2C_USING_RX_DMA_FLAG;
    static struct dma_config i2c2_dma_rx = I2C2_RX_DMA_CONFIG;
    i2c_config[I2C2_INDEX].dma_rx = &i2c2_dma_rx;
#endif
#ifdef BSP_I2C2_TX_USING_DMA
    i2c_objs[I2C2_INDEX].i2c_dma_flag |= I2C_USING_TX_DMA_FLAG;
    static struct dma_config i2c2_dma_tx = I2C2_TX_DMA_CONFIG;
    i2c_config[I2C2_INDEX].dma_tx = &i2c2_dma_tx;
#endif
#endif

#ifdef BSP_USING_HARD_I2C3
#ifdef BSP_I2C3_RX_USING_DMA
    i2c_objs[I2C3_INDEX].i2c_dma_flag |= I2C_USING_RX_DMA_FLAG;
    static struct dma_config i2c3_dma_rx = I2C3_RX_DMA_CONFIG;
    i2c_config[I2C3_INDEX].dma_rx = &i2c3_dma_rx;
#endif
#ifdef BSP_I2C3_TX_USING_DMA
    i2c_objs[I2C3_INDEX].i2c_dma_flag |= I2C_USING_TX_DMA_FLAG;
    static struct dma_config i2c3_dma_tx = I2C3_TX_DMA_CONFIG;
    i2c_config[I2C3_INDEX].dma_tx = &i2c3_dma_tx;
#endif
#endif
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    struct py32_i2c *i2c_drv = rt_container_of(hi2c, struct py32_i2c, handle);
    rt_completion_done(&i2c_drv->completion);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    struct py32_i2c *i2c_drv = rt_container_of(hi2c, struct py32_i2c, handle);
    rt_completion_done(&i2c_drv->completion);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    struct py32_i2c *i2c_drv = rt_container_of(hi2c, struct py32_i2c, handle);
    rt_completion_done(&i2c_drv->completion);
}

void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c)
{
    struct py32_i2c *i2c_drv = rt_container_of(hi2c, struct py32_i2c, handle);
    rt_completion_done(&i2c_drv->completion);
}

#ifdef BSP_USING_HARD_I2C1
void I2C1_EV_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_I2C_EV_IRQHandler(&i2c_objs[I2C1_INDEX].handle);
    rt_interrupt_leave();
}

void I2C1_ER_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_I2C_ER_IRQHandler(&i2c_objs[I2C1_INDEX].handle);
    rt_interrupt_leave();
}
#endif

#ifdef BSP_USING_HARD_I2C2
void I2C2_EV_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_I2C_EV_IRQHandler(&i2c_objs[I2C2_INDEX].handle);
    rt_interrupt_leave();
}

void I2C2_ER_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_I2C_ER_IRQHandler(&i2c_objs[I2C2_INDEX].handle);
    rt_interrupt_leave();
}
#endif

#ifdef BSP_USING_HARD_I2C3
void I2C3_EV_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_I2C_EV_IRQHandler(&i2c_objs[I2C3_INDEX].handle);
    rt_interrupt_leave();
}

void I2C3_ER_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_I2C_ER_IRQHandler(&i2c_objs[I2C3_INDEX].handle);
    rt_interrupt_leave();
}
#endif

#if defined(BSP_USING_HARD_I2C1) && defined(BSP_I2C1_RX_USING_DMA)
void I2C1_DMA_RX_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_DMA_IRQHandler(&i2c_objs[I2C1_INDEX].dma.handle_rx);
    rt_interrupt_leave();
}
#endif

#if defined(BSP_USING_HARD_I2C1) && defined(BSP_I2C1_TX_USING_DMA)
void I2C1_DMA_TX_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_DMA_IRQHandler(&i2c_objs[I2C1_INDEX].dma.handle_tx);
    rt_interrupt_leave();
}
#endif

#if defined(BSP_USING_HARD_I2C2) && defined(BSP_I2C2_RX_USING_DMA)
void I2C2_DMA_RX_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_DMA_IRQHandler(&i2c_objs[I2C2_INDEX].dma.handle_rx);
    rt_interrupt_leave();
}
#endif

#if defined(BSP_USING_HARD_I2C2) && defined(BSP_I2C2_TX_USING_DMA)
void I2C2_DMA_TX_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_DMA_IRQHandler(&i2c_objs[I2C2_INDEX].dma.handle_tx);
    rt_interrupt_leave();
}
#endif

#if defined(BSP_USING_HARD_I2C3) && defined(BSP_I2C3_RX_USING_DMA)
void I2C3_DMA_RX_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_DMA_IRQHandler(&i2c_objs[I2C3_INDEX].dma.handle_rx);
    rt_interrupt_leave();
}
#endif

#if defined(BSP_USING_HARD_I2C3) && defined(BSP_I2C3_TX_USING_DMA)
void I2C3_DMA_TX_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_DMA_IRQHandler(&i2c_objs[I2C3_INDEX].dma.handle_tx);
    rt_interrupt_leave();
}
#endif

static int rt_hw_hw_i2c_init(void)
{
    rt_size_t i;

    py32_get_dma_info();

    for (i = 0; i < sizeof(i2c_objs) / sizeof(i2c_objs[0]); i++)
    {
        rt_err_t ret;

        i2c_objs[i].config = &i2c_config[i];
        i2c_objs[i].i2c_bus.ops = &py32_i2c_ops;
        i2c_objs[i].i2c_bus.timeout = i2c_config[i].timeout;

        ret = py32_i2c_init(&i2c_objs[i]);
        if (ret != RT_EOK)
        {
            return ret;
        }

        ret = rt_i2c_bus_device_register(&i2c_objs[i].i2c_bus, i2c_config[i].name);
        if (ret != RT_EOK)
        {
            LOG_E("register %s failed", i2c_config[i].name);
            return ret;
        }

        LOG_I("%s bus init done", i2c_config[i].name);
    }

    return RT_EOK;
}
INIT_BOARD_EXPORT(rt_hw_hw_i2c_init);

#endif
