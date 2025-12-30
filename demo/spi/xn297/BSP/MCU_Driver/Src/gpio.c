/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "userConfig.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // 配置 PA0 和 PB2 为输出模式（LED）
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
	 /*初始化引脚电平 */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin , GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin , GPIO_PIN_RESET);
  HAL_GPIO_WritePin(RF1_CSN_GPIO_Port, RF1_CSN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RF_CE_GPIO_Port, RF_CE_Pin, GPIO_PIN_RESET);
	
	//配置LED输出模式
  GPIO_InitStruct.Pin = GPIO_PIN_1; // PA0 -- LED2
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_2; // PB2 -- LED3
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// 配置 PA15 -- RF_CSN 和 PB7 -- RF_CE 为输出模式
	GPIO_InitStruct.Pin = GPIO_PIN_15; //RF_CSN
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_1; // RF_CE|RF_CSN2
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
  // 配置 PB6 为输入模式（RF外部中断）
  GPIO_InitStruct.Pin = GPIO_PIN_6; // RF_IRQ
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // 下降沿触发中断
  GPIO_InitStruct.Pull = GPIO_PULLUP;          // 上拉电阻
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//配置LCD软件spi输出模式
	GPIO_InitStruct.Pin = GPIO_PIN_8; //SRC_RES 
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_14 |GPIO_PIN_13 | GPIO_PIN_12; // SRC_DC SRC_SDA SRC_SCL SRC_CS
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 配置 EXTI 线路
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 3); // 设置 EXTI15_10 中断优先级
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         // 启用 EXTI15_10 中断
}
/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
