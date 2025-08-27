/*-----------------------------------------------File Info------------------------------------------------
** File Name:               py32f403_hal_msp.c  
** Last modified date:      2025.7.1
** Last version:            V0.1
** Description:             HAL库MSP层初始化代码
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            包含SPI、UART、DMA、GPIO等外设的底层时钟启用与NVIC中断优先级配置；
**                          实现HAL库要求的MspInit/MspDeInit函数；
**                          配置DMA通道映射与中断服务函数关联。
**--------------------------------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//DMA_HandleTypeDef hdma_spi2_tx;
/* Private function prototypes -----------------------------------------------*/
/* External functions --------------------------------------------------------*/




/**
  * @brief Initialize global MSP
  */
void HAL_MspInit(void)
{
  // 启用基础硬件模块时钟
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  // 启用 GPIOA、GPIOB 时钟（用于 LED 和 IRQ）
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

