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
#include "bsp_uasrt.h"
#include "bsp_spi.h"
#include "bsp_core.h"

#include "usart.h"
#include "bsp_common.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern void BSP_UART_IRQHandler(UART_Num_TypeDef uart_num);
extern void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim3;
//extern DMA_HandleTypeDef hdma_usart1_tx;
//extern DMA_HandleTypeDef hdma_usart1_rx;
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

void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}



void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);  // 对应 RF_IRQ_Pin
}

/**
 * USART1全局中断处理函数
 * 处理UART空闲标志，进行DMA接收缓冲区管理
 */
void USART1_IRQHandler(void)
{
    BSP_UART_IRQHandler(UART_NUM_1);

    // 检查是否发生空闲帧中断
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        BSP_UART_ReceiveIdle(&huart1);  // 调用封装后的统一处理函数
    }
}
void USART2_IRQHandler(void)
{
    BSP_UART_IRQHandler(UART_NUM_2);

    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart2);
        BSP_UART_ReceiveIdle(&huart2);  // 调用封装后的统一处理函数
    }
}
void USART3_IRQHandler(void)
{
    BSP_UART_IRQHandler(UART_NUM_3);

    if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);
        BSP_UART_ReceiveIdle(&huart3);  // 调用封装后的统一处理函数
    }
}
void SPI1_IRQHandler(void)
{
    BSP_SPI_IRQHandler(SPI_NUM_1);
}

void SPI2_IRQHandler(void)
{
    BSP_SPI_IRQHandler(SPI_NUM_2);
}

void SPI3_IRQHandler(void)
{
    BSP_SPI_IRQHandler(SPI_NUM_3);
}
/* USER CODE BEGIN 1 */


void DMA1_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

void DMA1_Channel2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

void DMA1_Channel3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
}

void DMA1_Channel4_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
}

void DMA1_Channel5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi2_tx);
}

void DMA2_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
}

void DMA2_Channel2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
}
/* USER CODE END 1 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3) {
//        BSP_TIM_IRQHandler(&bsp_htim3);
    }
}
void TIM6_DAC_IRQHandler(void)
{
    BSP_TIM_IRQHandler(DELAY_US_TIM);
}

void TIM7_IRQHandler(void)
{
    BSP_TIM_IRQHandler(DELAY_MS_TIM);
}

void TIM2_IRQHandler(void)
{
    BSP_TIM_IRQHandler(PWM_LED_TIM);
}