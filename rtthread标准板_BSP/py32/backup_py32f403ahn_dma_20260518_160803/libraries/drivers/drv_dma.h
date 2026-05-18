/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-03-08     RT-Thread    first version for PY32
 */

#ifndef __DRV_DMA_H_
#define __DRV_DMA_H_

#include <board.h>
#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DMA_INSTANCE_TYPE DMA_Channel_TypeDef

struct dma_config {
  DMA_INSTANCE_TYPE *Instance;
  rt_uint32_t dma_rcc;
  IRQn_Type dma_irq;
};

#ifdef __cplusplus
}
#endif

#endif /*__DRV_DMA_H_ */
