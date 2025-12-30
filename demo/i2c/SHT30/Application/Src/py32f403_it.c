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

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
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
  * @brief This function handles USART2 Interrupt .
  */
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{

  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);

}

