#include "LED.h"

//LED_2 -->PB3
//LED_3 -->PA8
void LED_Init(void)
{
	
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED2_Pin , GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED1_Pin , GPIO_PIN_RESET);

}

void LED2_on(void)
{
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin , GPIO_PIN_SET);
}

void LED2_off(void)
{
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin , GPIO_PIN_RESET);
}

void LED2_Turn(void)
{
	if (HAL_GPIO_ReadPin(LED2_GPIO_Port, LED2_Pin) == 0)		
	{
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin , GPIO_PIN_SET);			
	}
	else													
	{
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin , GPIO_PIN_RESET);		
	}
}

void LED3_on(void)
{
	 HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin , GPIO_PIN_SET);
}

void LED3_off(void)
{
	 HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin , GPIO_PIN_RESET);
}

void LED3_Turn(void)
{
	if (HAL_GPIO_ReadPin(LED1_GPIO_Port, LED1_Pin) == 0)		
	{
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin , GPIO_PIN_SET);			
	}
	else													
	{
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin , GPIO_PIN_RESET);		
	}
}

