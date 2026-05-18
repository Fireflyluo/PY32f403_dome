#ifndef __I2C_HARD_CONFIG_H__
#define __I2C_HARD_CONFIG_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BSP_USING_HARD_I2C1
#ifndef I2C1_BUS_CONFIG
#define I2C1_BUS_CONFIG                             \
    {                                               \
        .Instance = I2C1,                           \
        .timing   = 0x10707DBC,                     \
        .timeout  = 0x1000,                         \
        .name     = "hwi2c1",                       \
        .evirq_type = I2C1_EV_IRQn,                 \
        .erirq_type = I2C1_ER_IRQn,                 \
    }
#endif
#endif

#ifdef BSP_I2C1_TX_USING_DMA
#ifndef I2C1_TX_DMA_CONFIG
#define I2C1_TX_DMA_CONFIG                          \
    {                                               \
        .dma_rcc = I2C1_TX_DMA_RCC,                 \
        .Instance = I2C1_TX_DMA_INSTANCE,           \
        .dma_irq = I2C1_TX_DMA_IRQ,                 \
        .dma_remap = I2C1_TX_DMA_REMAP,             \
    }
#endif
#endif

#ifdef BSP_I2C1_RX_USING_DMA
#ifndef I2C1_RX_DMA_CONFIG
#define I2C1_RX_DMA_CONFIG                          \
    {                                               \
        .dma_rcc = I2C1_RX_DMA_RCC,                 \
        .Instance = I2C1_RX_DMA_INSTANCE,           \
        .dma_irq = I2C1_RX_DMA_IRQ,                 \
        .dma_remap = I2C1_RX_DMA_REMAP,             \
    }
#endif
#endif

#ifdef BSP_USING_HARD_I2C2
#ifndef I2C2_BUS_CONFIG
#define I2C2_BUS_CONFIG                             \
    {                                               \
        .Instance = I2C2,                           \
        .timing   = 0x10707DBC,                     \
        .timeout  = 0x1000,                         \
        .name     = "hwi2c2",                       \
        .evirq_type = I2C2_EV_IRQn,                 \
        .erirq_type = I2C2_ER_IRQn,                 \
    }
#endif
#endif

#ifdef BSP_I2C2_TX_USING_DMA
#ifndef I2C2_TX_DMA_CONFIG
#define I2C2_TX_DMA_CONFIG                          \
    {                                               \
        .dma_rcc = I2C2_TX_DMA_RCC,                 \
        .Instance = I2C2_TX_DMA_INSTANCE,           \
        .dma_irq = I2C2_TX_DMA_IRQ,                 \
        .dma_remap = I2C2_TX_DMA_REMAP,             \
    }
#endif
#endif

#ifdef BSP_I2C2_RX_USING_DMA
#ifndef I2C2_RX_DMA_CONFIG
#define I2C2_RX_DMA_CONFIG                          \
    {                                               \
        .dma_rcc = I2C2_RX_DMA_RCC,                 \
        .Instance = I2C2_RX_DMA_INSTANCE,           \
        .dma_irq = I2C2_RX_DMA_IRQ,                 \
        .dma_remap = I2C2_RX_DMA_REMAP,             \
    }
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
