#ifndef __DRV_DMA_H_
#define __DRV_DMA_H_

#include <board.h>
#include <rtthread.h>
#include <stdint.h>

#if defined(SOC_SERIES_PY32F4)
#include <py32f403_ll_dma.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define DMA_INSTANCE_TYPE DMA_Channel_TypeDef

struct dma_config {
  DMA_INSTANCE_TYPE *Instance;
  rt_uint32_t dma_rcc;
  IRQn_Type dma_irq;
  rt_uint32_t dma_remap;
};

static inline void py32_dma_apply_remap(const struct dma_config *cfg)
{
#if defined(SOC_SERIES_PY32F4)
  DMA_TypeDef *dmax;
  uint32_t ll_channel;
  uintptr_t addr;
  uintptr_t dma1_base;
  uintptr_t dma2_base;
  uintptr_t channel_stride;
  uint32_t channel_index;

  if ((cfg == RT_NULL) || (cfg->Instance == RT_NULL) || (cfg->dma_remap == 0U))
  {
    return;
  }

  addr = (uintptr_t)cfg->Instance;
  dma1_base = (uintptr_t)DMA1_Channel1;
  dma2_base = (uintptr_t)DMA2_Channel1;
  channel_stride = (uintptr_t)DMA1_Channel2 - (uintptr_t)DMA1_Channel1;

  if ((addr >= dma1_base) && (addr <= (uintptr_t)DMA1_Channel7))
  {
    dmax = DMA1;
    channel_index = (uint32_t)((addr - dma1_base) / channel_stride) + 1U;
  }
  else if ((addr >= dma2_base) && (addr <= (uintptr_t)DMA2_Channel5))
  {
    dmax = DMA2;
    channel_index = (uint32_t)((addr - dma2_base) / channel_stride) + 1U;
  }
  else
  {
    return;
  }

  switch (channel_index)
  {
  case 1U:
    ll_channel = LL_DMA_CHANNEL_1;
    break;
  case 2U:
    ll_channel = LL_DMA_CHANNEL_2;
    break;
  case 3U:
    ll_channel = LL_DMA_CHANNEL_3;
    break;
  case 4U:
    ll_channel = LL_DMA_CHANNEL_4;
    break;
  case 5U:
    ll_channel = LL_DMA_CHANNEL_5;
    break;
  case 6U:
    ll_channel = LL_DMA_CHANNEL_6;
    break;
  case 7U:
    ll_channel = LL_DMA_CHANNEL_7;
    break;
  default:
    return;
  }

  LL_SYSCFG_SetDMARemap(dmax, ll_channel, cfg->dma_remap);
#else
  RT_UNUSED(cfg);
#endif
}

#ifdef __cplusplus
}
#endif

#endif
