/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-03-08     RT-Thread    first version for PY32
 */

#ifndef __DRV_USART_H__
#define __DRV_USART_H__

#include "drv_dma.h"
#include "rtdevice.h"
#include <drv_common.h>
#include <rthw.h>
#include <rtthread.h>

int rt_hw_usart_init(void);

#define DMA_INSTANCE_TYPE DMA_Channel_TypeDef
#define UART_INSTANCE_CLEAR_FUNCTION __HAL_UART_CLEAR_FLAG

#define UART_RX_DMA_IT_IDLE_FLAG 0x00
#define UART_RX_DMA_IT_HT_FLAG 0x01
#define UART_RX_DMA_IT_TC_FLAG 0x02

/* stm32 config class */
struct py32_uart_config {
  const char *name;
  USART_TypeDef *Instance;
  IRQn_Type irq_type;
  struct dma_config *dma_rx;
  struct dma_config *dma_tx;
};

/* stm32 uart dirver class */
struct py32_uart {
  UART_HandleTypeDef handle;
  struct py32_uart_config *config;
  rt_uint32_t DR_mask;

#ifdef RT_SERIAL_USING_DMA
  struct {
    DMA_HandleTypeDef handle;
    rt_size_t remaining_cnt;
  } dma_rx;
  struct {
    DMA_HandleTypeDef handle;
  } dma_tx;
#endif
  rt_uint16_t uart_dma_flag;
  struct rt_serial_device serial;
};

#endif /* __DRV_USART_H__ */
