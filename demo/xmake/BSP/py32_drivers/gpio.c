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
//#include "userConfig.h"
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
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10 , GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11 , GPIO_PIN_RESET);


  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_2, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;            
  GPIO_InitStruct.Pull = GPIO_PULLUP;                    
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;         
  /* GPIO Initialization */
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);


}
/* USER CODE BEGIN 2 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_8)
	{
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_14);
	}
	
	if(GPIO_Pin == GPIO_PIN_9)
	{
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_15);
	}
}
/* USER CODE END 2 */
