/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
/* USER CODE END 0 */

 
/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance           = USART1;               /* 选择USART1实例 */
  huart1.Init.BaudRate      = 115200;               /* 设置波特率为115200 */
  huart1.Init.WordLength    = UART_WORDLENGTH_8B;                 /* 设置数据位为8位 */
  huart1.Init.StopBits      = UART_STOPBITS_1;                    /* 设置停止位为1位 */
  huart1.Init.Parity        = UART_PARITY_NONE;                   /* 设置无校验位 */
  huart1.Init.Mode          = UART_MODE_TX_RX;                    /* 设置为收发模式 */
  huart1.Init.HwFlowCtl     = UART_HWCONTROL_NONE;                /* 禁用硬件流控制 */
  huart1.Init.OverSampling  = UART_OVERSAMPLING_16;               /* 设置过采样率为16 */
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;   // 无高级功能初始化
	
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
      APP_ErrorHandler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);		// 开启串口空闲中断，必须调用

  /* USER CODE END USART1_Init 2 */

}
void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart2.Instance           = USART2;               /* 选择USART1实例 */
  huart2.Init.BaudRate      = 115200;               /* 设置波特率为115200 */
  huart2.Init.WordLength    = UART_WORDLENGTH_8B;                 /* 设置数据位为8位 */
  huart2.Init.StopBits      = UART_STOPBITS_1;                    /* 设置停止位为1位 */
  huart2.Init.Parity        = UART_PARITY_NONE;                   /* 设置无校验位 */
  huart2.Init.Mode          = UART_MODE_TX_RX;                    /* 设置为收发模式 */
  huart2.Init.HwFlowCtl     = UART_HWCONTROL_NONE;                /* 禁用硬件流控制 */
  huart2.Init.OverSampling  = UART_OVERSAMPLING_16;               /* 设置过采样率为16 */
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;   // 无高级功能初始化
	
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
      APP_ErrorHandler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);		// 开启串口空闲中断，必须调用

  /* USER CODE END USART1_Init 2 */

}
void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart2.Instance           = USART3;               /* 选择USART1实例 */
  huart2.Init.BaudRate      = 115200;               /* 设置波特率为115200 */
  huart2.Init.WordLength    = UART_WORDLENGTH_8B;                 /* 设置数据位为8位 */
  huart2.Init.StopBits      = UART_STOPBITS_1;                    /* 设置停止位为1位 */
  huart2.Init.Parity        = UART_PARITY_NONE;                   /* 设置无校验位 */
  huart2.Init.Mode          = UART_MODE_TX_RX;                    /* 设置为收发模式 */
  huart2.Init.HwFlowCtl     = UART_HWCONTROL_NONE;                /* 禁用硬件流控制 */
  huart2.Init.OverSampling  = UART_OVERSAMPLING_16;               /* 设置过采样率为16 */
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;   // 无高级功能初始化
	
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
      APP_ErrorHandler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);		// 开启串口空闲中断，必须调用
  /* USER CODE END USART1_Init 2 */

}

