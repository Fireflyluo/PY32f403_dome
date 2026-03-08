/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-03-08     RT-Thread    first version for PY32F4
 */

#ifndef __DRV_FLASH_H__
#define __DRV_FLASH_H__

#include "rtdevice.h"
#include <drv_common.h>
#include <rthw.h>
#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

int py32_flash_read(rt_uint32_t addr, rt_uint8_t *buf, size_t size);
int py32_flash_write(rt_uint32_t addr, const rt_uint8_t *buf, size_t size);
int py32_flash_erase(rt_uint32_t addr, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_FLASH_H__ */
