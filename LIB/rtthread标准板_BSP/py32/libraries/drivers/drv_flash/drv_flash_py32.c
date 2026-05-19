/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-03-08     RT-Thread    first version for PY32F4
 */

#include "drv_flash.h"

#if defined(BSP_USING_ON_CHIP_FLASH)

#include "py32f403_hal.h"

#define PY32_FLASH_START_ADDR FLASH_BASE
#define PY32_FLASH_SIZE (256 * 1024) /* 256KB for PY32F403 */
#define PY32_FLASH_PAGE_SIZE 1024    /* 1KB page size */
#define PY32_FLASH_LOCK_TIMEOUT 50000

/**
 * Read data from flash.
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read size
 * @return success: RT_EOK, fail: -RT_ERROR
 */
int py32_flash_read(rt_uint32_t addr, rt_uint8_t *buf, size_t size) {
  int result = RT_EOK;

  if ((addr + size) > (PY32_FLASH_START_ADDR + PY32_FLASH_SIZE)) {
    return -RT_ERROR;
  }

  rt_memcpy(buf, (void *)addr, size);

  return result;
}

/**
 * Write data to flash.
 * @param addr flash address
 * @param buf data buffer
 * @param size write size
 * @return success: RT_EOK, fail: -RT_ERROR
 */
int py32_flash_write(rt_uint32_t addr, const rt_uint8_t *buf, size_t size) {
  int result = RT_EOK;
  rt_uint32_t write_addr = addr;
  rt_uint32_t end_addr = addr + size;
  rt_uint32_t write_data = 0;
  rt_uint32_t i;
  HAL_StatusTypeDef status;

  if ((end_addr) > (PY32_FLASH_START_ADDR + PY32_FLASH_SIZE)) {
    return -RT_ERROR;
  }

  /* Unlock flash */
  HAL_FLASH_Unlock();

  /* Write data by word (4 bytes) */
  for (i = 0; i < size; i += 4) {
    /* Prepare data (handle unaligned size) */
    write_data = 0xFFFFFFFF;
    rt_memcpy(&write_data, buf + i, (size - i >= 4) ? 4 : (size - i));

    /* Program word */
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, write_addr, write_data);
    if (status != HAL_OK) {
      result = -RT_ERROR;
      break;
    }

    write_addr += 4;
  }

  /* Lock flash */
  HAL_FLASH_Lock();

  return result;
}

/**
 * Erase flash pages.
 * @param addr start address
 * @param size erase size
 * @return success: RT_EOK, fail: -RT_ERROR
 */
int py32_flash_erase(rt_uint32_t addr, size_t size) {
  int result = RT_EOK;
  rt_uint32_t page_addr;
  rt_uint32_t page_num;
  rt_uint32_t i;
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef EraseInitStruct;
  rt_uint32_t page_error = 0;

  if ((addr + size) > (PY32_FLASH_START_ADDR + PY32_FLASH_SIZE)) {
    return -RT_ERROR;
  }

  /* Calculate page address and number */
  page_addr = addr - (addr % PY32_FLASH_PAGE_SIZE);
  page_num = (size + PY32_FLASH_PAGE_SIZE - 1) / PY32_FLASH_PAGE_SIZE;
  if (addr % PY32_FLASH_PAGE_SIZE != 0) {
    page_num += 1;
  }

  /* Unlock flash */
  HAL_FLASH_Unlock();

  /* Erase pages */
  for (i = 0; i < page_num; i++) {
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = page_addr + i * PY32_FLASH_PAGE_SIZE;
    EraseInitStruct.NbPages = 1;

    status = HAL_FLASHEx_Erase(&EraseInitStruct, &page_error);
    if (status != HAL_OK) {
      result = -RT_ERROR;
      break;
    }
  }

  /* Lock flash */
  HAL_FLASH_Lock();

  return result;
}

#ifdef RT_USING_FAL
#include <fal.h>

static int fal_flash_read(long offset, rt_uint8_t *buf, size_t size);
static int fal_flash_write(long offset, const rt_uint8_t *buf, size_t size);
static int fal_flash_erase(long offset, size_t size);

static const struct fal_flash_dev py32_onchip_flash = {
    "onchip_flash",
    PY32_FLASH_START_ADDR,
    PY32_FLASH_SIZE,
    PY32_FLASH_PAGE_SIZE,
    {NULL, fal_flash_read, fal_flash_write, fal_flash_erase}};

static int fal_flash_read(long offset, rt_uint8_t *buf, size_t size) {
  return py32_flash_read(py32_onchip_flash.addr + offset, buf, size);
}

static int fal_flash_write(long offset, const rt_uint8_t *buf, size_t size) {
  return py32_flash_write(py32_onchip_flash.addr + offset, buf, size);
}

static int fal_flash_erase(long offset, size_t size) {
  return py32_flash_erase(py32_onchip_flash.addr + offset, size);
}
#endif /* RT_USING_FAL */

#endif /* BSP_USING_ON_CHIP_FLASH */
