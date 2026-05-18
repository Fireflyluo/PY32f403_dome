#ifndef __DRV_CONFIG_H__
#define __DRV_CONFIG_H__

#include <board.h>
#include <rtdevice.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(SOC_SERIES_PY32F4)
#include "py32f4/dma_config.h"
#include "py32f4/uart_config.h"
#include "py32f4/spi_config.h"
#include "py32f4/tim_config.h"
#include "py32f4/pwm_config.h"
#include "py32f4/adc_config.h"
#include "py32f4/i2c_hard_config.h"
#endif

#ifdef __cplusplus
}
#endif

#endif
