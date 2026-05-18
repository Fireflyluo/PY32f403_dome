/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-03-08     RT-Thread    first version for PY32F4xx
 */

#ifndef __DMA_CONFIG_H__
#define __DMA_CONFIG_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DMA1 Channel1 */
#if defined(BSP_SPI1_RX_USING_DMA) && !defined(SPI1_RX_DMA_INSTANCE)
#define SPI1_DMA_RX_IRQHandler DMA1_Channel1_IRQHandler
#define SPI1_RX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define SPI1_RX_DMA_INSTANCE DMA1_Channel1
#define SPI1_RX_DMA_IRQ DMA1_Channel1_IRQn
#elif defined(BSP_SPI3_RX_USING_DMA) && !defined(SPI3_RX_DMA_INSTANCE)
#define SPI3_DMA_RX_IRQHandler DMA1_Channel1_IRQHandler
#define SPI3_RX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define SPI3_RX_DMA_INSTANCE DMA1_Channel1
#define SPI3_RX_DMA_IRQ DMA1_Channel1_IRQn
#endif

/* DMA1 Channel2 */
#if defined(BSP_SPI1_TX_USING_DMA) && !defined(SPI1_TX_DMA_INSTANCE)
#define SPI1_DMA_TX_IRQHandler DMA1_Channel2_IRQHandler
#define SPI1_TX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define SPI1_TX_DMA_INSTANCE DMA1_Channel2
#define SPI1_TX_DMA_IRQ DMA1_Channel2_IRQn
#elif defined(BSP_SPI3_TX_USING_DMA) && !defined(SPI3_TX_DMA_INSTANCE)
#define SPI3_DMA_TX_IRQHandler DMA1_Channel2_IRQHandler
#define SPI3_TX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define SPI3_TX_DMA_INSTANCE DMA1_Channel2
#define SPI3_TX_DMA_IRQ DMA1_Channel2_IRQn
#endif

/* DMA1 Channel3 */
#if defined(BSP_SPI2_RX_USING_DMA) && !defined(SPI2_RX_DMA_INSTANCE)
#define SPI2_DMA_RX_IRQHandler DMA1_Channel3_IRQHandler
#define SPI2_RX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define SPI2_RX_DMA_INSTANCE DMA1_Channel3
#define SPI2_RX_DMA_IRQ DMA1_Channel3_IRQn
#elif defined(BSP_UART3_RX_USING_DMA) && !defined(UART3_RX_DMA_INSTANCE)
#define UART3_DMA_RX_IRQHandler DMA1_Channel3_IRQHandler
#define UART3_RX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define UART3_RX_DMA_INSTANCE DMA1_Channel3
#define UART3_RX_DMA_IRQ DMA1_Channel3_IRQn
#endif

/* DMA1 Channel4 */
#if defined(BSP_SPI2_TX_USING_DMA) && !defined(SPI2_TX_DMA_INSTANCE)
#define SPI2_DMA_TX_IRQHandler DMA1_Channel4_IRQHandler
#define SPI2_TX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define SPI2_TX_DMA_INSTANCE DMA1_Channel4
#define SPI2_TX_DMA_IRQ DMA1_Channel4_IRQn
#elif defined(BSP_UART4_RX_USING_DMA) && !defined(UART4_RX_DMA_INSTANCE)
#define UART4_DMA_RX_IRQHandler DMA1_Channel4_IRQHandler
#define UART4_RX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define UART4_RX_DMA_INSTANCE DMA1_Channel4
#define UART4_RX_DMA_IRQ DMA1_Channel4_IRQn
#elif defined(BSP_USART3_TX_USING_DMA) && !defined(USART3_TX_DMA_INSTANCE)
#define USART3_DMA_TX_IRQHandler DMA1_Channel4_IRQHandler
#define USART3_TX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define USART3_TX_DMA_INSTANCE DMA1_Channel4
#define USART3_TX_DMA_IRQ DMA1_Channel4_IRQn
#endif

/* DMA1 Channel5 */
#if defined(BSP_UART2_RX_USING_DMA) && !defined(UART2_RX_DMA_INSTANCE)
#define UART2_DMA_RX_IRQHandler DMA1_Channel5_IRQHandler
#define UART2_RX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define UART2_RX_DMA_INSTANCE DMA1_Channel5
#define UART2_RX_DMA_IRQ DMA1_Channel5_IRQn
#elif defined(BSP_UART5_RX_USING_DMA) && !defined(UART5_RX_DMA_INSTANCE)
#define UART5_DMA_RX_IRQHandler DMA1_Channel5_IRQHandler
#define UART5_RX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define UART5_RX_DMA_INSTANCE DMA1_Channel5
#define UART5_RX_DMA_IRQ DMA1_Channel5_IRQn
#elif defined(BSP_SPI3_TX_USING_DMA) && !defined(SPI3_TX_DMA_INSTANCE)
#define SPI3_DMA_TX_IRQHandler DMA1_Channel5_IRQHandler
#define SPI3_TX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define SPI3_TX_DMA_INSTANCE DMA1_Channel5
#define SPI3_TX_DMA_IRQ DMA1_Channel5_IRQn
#endif

/* DMA1 Channel6 */
#if defined(BSP_UART2_TX_USING_DMA) && !defined(UART2_TX_DMA_INSTANCE)
#define UART2_DMA_TX_IRQHandler DMA1_Channel6_IRQHandler
#define UART2_TX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define UART2_TX_DMA_INSTANCE DMA1_Channel6
#define UART2_TX_DMA_IRQ DMA1_Channel6_IRQn
#elif defined(BSP_UART5_TX_USING_DMA) && !defined(UART5_TX_DMA_INSTANCE)
#define UART5_DMA_TX_IRQHandler DMA1_Channel6_IRQHandler
#define UART5_TX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define UART5_TX_DMA_INSTANCE DMA1_Channel6
#define UART5_TX_DMA_IRQ DMA1_Channel6_IRQn
#endif

/* DMA1 Channel7 */
#if defined(BSP_UART1_TX_USING_DMA) && !defined(UART1_TX_DMA_INSTANCE)
#define UART1_DMA_TX_IRQHandler DMA1_Channel7_IRQHandler
#define UART1_TX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define UART1_TX_DMA_INSTANCE DMA1_Channel7
#define UART1_TX_DMA_IRQ DMA1_Channel7_IRQn
#elif defined(BSP_UART3_TX_USING_DMA) && !defined(UART3_TX_DMA_INSTANCE)
#define UART3_DMA_TX_IRQHandler DMA1_Channel7_IRQHandler
#define UART3_TX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define UART3_TX_DMA_INSTANCE DMA1_Channel7
#define UART3_TX_DMA_IRQ DMA1_Channel7_IRQn
#elif defined(BSP_SPI2_RX_USING_DMA) && !defined(SPI2_RX_DMA_INSTANCE)
#define SPI2_DMA_RX_IRQHandler DMA1_Channel7_IRQHandler
#define SPI2_RX_DMA_RCC RCC_AHB1ENR_DMA1EN
#define SPI2_RX_DMA_INSTANCE DMA1_Channel7
#define SPI2_RX_DMA_IRQ DMA1_Channel7_IRQn
#endif

/* DMA2 Channel1 */
#if defined(BSP_SPI1_RX_USING_DMA) && !defined(SPI1_RX_DMA_INSTANCE)
#define SPI1_DMA_RX_IRQHandler DMA2_Channel1_IRQHandler
#define SPI1_RX_DMA_RCC RCC_AHB1ENR_DMA2EN
#define SPI1_RX_DMA_INSTANCE DMA2_Channel1
#define SPI1_RX_DMA_IRQ DMA2_Channel1_IRQn
#elif defined(BSP_UART6_RX_USING_DMA) && !defined(UART6_RX_DMA_INSTANCE)
#define UART6_DMA_RX_IRQHandler DMA2_Channel1_IRQHandler
#define UART6_RX_DMA_RCC RCC_AHB1ENR_DMA2EN
#define UART6_RX_DMA_INSTANCE DMA2_Channel1
#define UART6_RX_DMA_IRQ DMA2_Channel1_IRQn
#endif

/* DMA2 Channel2 */
#if defined(BSP_SPI1_TX_USING_DMA) && !defined(SPI1_TX_DMA_INSTANCE)
#define SPI1_DMA_TX_IRQHandler DMA2_Channel2_IRQHandler
#define SPI1_TX_DMA_RCC RCC_AHB1ENR_DMA2EN
#define SPI1_TX_DMA_INSTANCE DMA2_Channel2
#define SPI1_TX_DMA_IRQ DMA2_Channel2_IRQn
#elif defined(BSP_UART6_TX_USING_DMA) && !defined(UART6_TX_DMA_INSTANCE)
#define UART6_DMA_TX_IRQHandler DMA2_Channel2_IRQHandler
#define UART6_TX_DMA_RCC RCC_AHB1ENR_DMA2EN
#define UART6_TX_DMA_INSTANCE DMA2_Channel2
#define UART6_TX_DMA_IRQ DMA2_Channel2_IRQn
#endif

/* DMA2 Channel3 */
#if defined(BSP_SPI2_RX_USING_DMA) && !defined(SPI2_RX_DMA_INSTANCE)
#define SPI2_DMA_RX_IRQHandler DMA2_Channel3_IRQHandler
#define SPI2_RX_DMA_RCC RCC_AHB1ENR_DMA2EN
#define SPI2_RX_DMA_INSTANCE DMA2_Channel3
#define SPI2_RX_DMA_IRQ DMA2_Channel3_IRQn
#elif defined(BSP_UART5_RX_USING_DMA) && !defined(UART5_RX_DMA_INSTANCE)
#define UART5_DMA_RX_IRQHandler DMA2_Channel3_IRQHandler
#define UART5_RX_DMA_RCC RCC_AHB1ENR_DMA2EN
#define UART5_RX_DMA_INSTANCE DMA2_Channel3
#define UART5_RX_DMA_IRQ DMA2_Channel3_IRQn
#endif

/* DMA2 Channel4 */
#if defined(BSP_SPI2_TX_USING_DMA) && !defined(SPI2_TX_DMA_INSTANCE)
#define SPI2_DMA_TX_IRQHandler DMA2_Channel4_IRQHandler
#define SPI2_TX_DMA_RCC RCC_AHB1ENR_DMA2EN
#define SPI2_TX_DMA_INSTANCE DMA2_Channel4
#define SPI2_TX_DMA_IRQ DMA2_Channel4_IRQn
#elif defined(BSP_UART5_TX_USING_DMA) && !defined(UART5_TX_DMA_INSTANCE)
#define UART5_DMA_TX_IRQHandler DMA2_Channel4_IRQHandler
#define UART5_TX_DMA_RCC RCC_AHB1ENR_DMA2EN
#define UART5_TX_DMA_INSTANCE DMA2_Channel4
#define UART5_TX_DMA_IRQ DMA2_Channel4_IRQn
#endif

/* DMA2 Channel5 */
#if defined(BSP_SPI3_RX_USING_DMA) && !defined(SPI3_RX_DMA_INSTANCE)
#define SPI3_DMA_RX_IRQHandler DMA2_Channel5_IRQHandler
#define SPI3_RX_DMA_RCC RCC_AHB1ENR_DMA2EN
#define SPI3_RX_DMA_INSTANCE DMA2_Channel5
#define SPI3_RX_DMA_IRQ DMA2_Channel5_IRQn
#elif defined(BSP_UART4_RX_USING_DMA) && !defined(UART4_RX_DMA_INSTANCE)
#define UART4_DMA_RX_IRQHandler DMA2_Channel5_IRQHandler
#define UART4_RX_DMA_RCC RCC_AHB1ENR_DMA2EN
#define UART4_RX_DMA_INSTANCE DMA2_Channel5
#define UART4_RX_DMA_IRQ DMA2_Channel5_IRQn
#elif defined(BSP_UART6_TX_USING_DMA) && !defined(UART6_TX_DMA_INSTANCE)
#define UART6_DMA_TX_IRQHandler DMA2_Channel5_IRQHandler
#define UART6_TX_DMA_RCC RCC_AHB1ENR_DMA2EN
#define UART6_TX_DMA_INSTANCE DMA2_Channel5
#define UART6_TX_DMA_IRQ DMA2_Channel5_IRQn
#endif

#ifdef __cplusplus
}
#endif

#endif /* __DMA_CONFIG_H__ */
