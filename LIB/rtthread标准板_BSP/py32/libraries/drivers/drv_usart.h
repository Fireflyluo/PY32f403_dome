#ifndef __DRV_USART_H__
#define __DRV_USART_H__

#include <py32f4xx_hal.h>
#include "drv_dma.h"
#include "rtdevice.h"
#include <drv_common.h>
#include <rthw.h>
#include <rtthread.h>

int rt_hw_usart_init(void);
#ifdef RT_SERIAL_USING_DMA
rt_err_t rt_hw_usart_dma_test_send(const char *name, const rt_uint8_t *buf, rt_size_t size);
#endif

#define DMA_INSTANCE_TYPE DMA_Channel_TypeDef
#define UART_INSTANCE_CLEAR_FUNCTION __HAL_UART_CLEAR_FLAG

#define UART_RX_DMA_IT_IDLE_FLAG 0x00
#define UART_RX_DMA_IT_HT_FLAG 0x01
#define UART_RX_DMA_IT_TC_FLAG 0x02

struct py32_uart_config {
  const char *name;
  USART_TypeDef *Instance;
  IRQn_Type irq_type;
  struct dma_config *dma_rx;
  struct dma_config *dma_tx;
};

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
  struct rt_completion dma_tx_test_done;
  volatile rt_uint8_t dma_tx_test_active;
#endif
  rt_uint16_t uart_dma_flag;
  struct rt_serial_device serial;
};

#endif
