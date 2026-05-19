/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-5      SummerGift   first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include "drv_common.h"
#include "drv_gpio.h"
#include <py32f4xx.h>
#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PY32_FLASH_START_ADRESS ((uint32_t)0x08000000)
#define PY32_FLASH_SIZE (384 * 1024)
#define PY32_FLASH_END_ADDRESS                                                 \
  ((uint32_t)(PY32_FLASH_START_ADRESS + PY32_FLASH_SIZE))

#define PY32_SRAM_SIZE 64
#define PY32_SRAM_END (0x20000000 + PY32_SRAM_SIZE * 1024)

#if defined(__ARMCC_VERSION)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN (&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section = "CSTACK"
#define HEAP_BEGIN (__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN (&__bss_end)
#endif

#define HEAP_END PY32_SRAM_END

void SystemClock_Config(void);

#ifdef __cplusplus
}
#endif

#endif
