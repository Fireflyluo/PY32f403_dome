/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2025-03-08     RT-Thread         first version for PY32
 */

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
#endif

#ifdef __cplusplus
}
#endif

#endif
