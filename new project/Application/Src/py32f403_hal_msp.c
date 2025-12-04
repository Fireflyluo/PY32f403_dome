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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
}

/**
 * @brief Initialize UART MSP
 * @param  huart：UART handle
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if (huart->Instance == USART2)
  {
    /* Enable clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    /* GPIO Initialization
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Alternate = GPIO_AF2_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}
