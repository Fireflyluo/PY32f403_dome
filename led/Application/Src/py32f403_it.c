/*-----------------------------------------------File Info------------------------------------------------
** File Name:               py32f403_it.c  
** Last modified date:      2025.7.1
** Last version:            V0.1
** Description:             中断服务程序实现
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            实现GPIO外部中断、UART、SPI、DMA、TIM等外设的中断处理函数；
**                          调用BSP层统一中断处理函数；
**                          管理空闲帧中断以实现DMA接收数据解析。
**--------------------------------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "py32f403_it.h"
#include "tim.h"
#include "ws2812.h"
/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*          Cortex-M4 Processor Interruption and Exception Handlers           */
/******************************************************************************/
/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/* PY32F403 Peripheral Interrupt Handlers                                     */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file.                                          */
/******************************************************************************/
/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}
/**
 * @brief  外部声明dev_led_dma_transfer_complete函数
 * @retval None
 */

void WS2812_DMA_Transfer_Complete(void);
void DMA2_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_tim2_ch2_ch4);
        // 检查是否是传输完成中断
    if(__HAL_DMA_GET_FLAG(&hdma_tim2_ch2_ch4, DMA_FLAG_TC1)) {
        // 清除传输完成标志
        __HAL_DMA_CLEAR_FLAG(&hdma_tim2_ch2_ch4, DMA_FLAG_TC1);
        
        // 调用完成处理函数

        WS2812_DMA_Transfer_Complete();
    }
}

/**
 * @brief  DMA传输完成回调函数
 * @param  hdma: DMA句柄
 * @retval None
 */
void HAL_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
{
    if (hdma == &hdma_tim2_ch2_ch4) {
        // TIM2 CH2 DMA传输完成
//        dev_led_dma_transfer_complete();
    }
}