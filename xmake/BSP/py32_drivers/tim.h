/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp_common.h"
/* USER CODE BEGIN Includes */
#define DELAY_US_TIM   BSP_TIM6  // 基本定时器用于微秒延迟
#define DELAY_MS_TIM   BSP_TIM7  // 基本定时器用于毫秒延迟
#define PWM_LED_TIM    BSP_TIM2  // 高级定时器用于PWM

/* LED PWM设置 */
#define LED_PWM_CHANNEL  2       // 使用TIM2通道2
#define LED_PWM_FREQ     1000    // PWM频率1kHz
#define LED_PWM_MAX      100     // PWM最大占空比值

/* 系统时钟频率 */
#define SYSTEM_CORE_CLOCK 168000000  // 168MHz
/* USER CODE END Includes */

extern TIM_HandleTypeDef htim6;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_TIM6_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

