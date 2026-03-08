/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include "board.h"

/**
 * @brief  系统时钟配置
 * @param  None
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef OscInitstruct = {0};
  RCC_ClkInitTypeDef ClkInitstruct = {0};

  // 配置振荡器类型，启用多种振荡器检测
  OscInitstruct.OscillatorType =
      RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE |
      RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSI48M;
  OscInitstruct.HSEState = RCC_HSE_ON;       // 启用外部高速时钟 (HSE)
  OscInitstruct.HSEFreq = RCC_HSE_16_32MHz;  // 选择HSE频率为16-32MHz
  OscInitstruct.HSI48MState = RCC_HSI48M_ON; // 禁用48MHz内部RC振荡器 (HSI48M)
  OscInitstruct.HSIState = RCC_HSI_ON;       // 启用内部高速时钟 (HSI)
  OscInitstruct.LSEState = RCC_LSE_OFF;      // 禁用外部低速时钟 (LSE)
  // OscInitstruct.LSEDriver = RCC_LSEDRIVE_HIGH;                   //
  // 设置驱动能力等级为高
  OscInitstruct.LSIState = RCC_LSI_ON;             // 启用内部低速时钟 (LSI)
  OscInitstruct.PLL.PLLState = RCC_PLL_ON;         // 启用PLL
  OscInitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE; // PLL时钟源选择HSE
  OscInitstruct.PLL.PLLMUL = RCC_PLL_MUL9; // PLL倍频系数为9   16*9=144MHz

  // 配置振荡器
  if (HAL_RCC_OscConfig(&OscInitstruct) != HAL_OK) {
    Error_Handler(); // 错误处理函数
  }

  // 配置系统时钟
  ClkInitstruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                            RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  ClkInitstruct.SYSCLKSource =
      RCC_SYSCLKSOURCE_PLLCLK;                   // 系统时钟源选择PLL输出PLLCLK
  ClkInitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1; // AHB总线不分频
  ClkInitstruct.APB1CLKDivider = RCC_HCLK_DIV1;  // APB1总线不分频
  ClkInitstruct.APB2CLKDivider = RCC_HCLK_DIV2;  // APB2总线分频2

  /*
   * 重新初始化RCC时钟
   * -- clock <= 24MHz: FLASH_LATENCY_0
   * -- clock > 24MHz:  FLASH_LATENCY_1
   */
  // 配置时钟
  if (HAL_RCC_ClockConfig(&ClkInitstruct, FLASH_LATENCY_6) != HAL_OK) {
    Error_Handler(); // 错误处理函数
  }
}
