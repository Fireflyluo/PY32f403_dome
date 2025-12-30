/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */


/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */

    /* 初始化 SPI 外设 */
    hspi1.Instance               = SPI1;                       /* SPI1 */
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;   /* 预分频器 : 16 */
    hspi1.Init.Direction         = SPI_DIRECTION_2LINES;       /* 全双工 */
    hspi1.Init.CLKPolarity       = SPI_POLARITY_LOW;           /* SPI 时钟极性: 低 */
    hspi1.Init.CLKPhase          = SPI_PHASE_1EDGE;            /* 数据采样从第一个时钟边沿开始 */
    hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;          /* SPI 数据大小为 8 位 */
    hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;           /* SPI MSB 传输 */
    hspi1.Init.NSS               = SPI_NSS_HARD_OUTPUT;        /* NSS 硬件模式 */
    hspi1.Init.Mode              = SPI_MODE_MASTER;            /* 配置为主机模式 */
    hspi1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE; /* 禁用 CRC 校验 */
    // Spi1Handle.Init.CRCPolynomial     = 1;                          /* CRC 多项式值 */

if (HAL_SPI_Init(&hspi1) != HAL_OK)
{
  APP_ErrorHandler();
}

  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}
/* SPI2 init function */
//void MX_SPI2_Init(void)
//{

//  /* USER CODE BEGIN SPI1_Init 0 */

//  /* USER CODE END SPI1_Init 0 */

//  /* USER CODE BEGIN SPI1_Init 1 */

//  /* USER CODE END SPI1_Init 1 */

//    /* 初始化 SPI 外设 */
//    hspi2.Instance               = SPI2;                       /* SPI1 */
//    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;   /* 预分频器 : 16 */
//    hspi2.Init.Direction         = SPI_DIRECTION_2LINES;        /* 单线只发送模式 */
//    hspi2.Init.CLKPolarity       = SPI_POLARITY_LOW;           /* SPI 时钟极性: 低 */
//    hspi2.Init.CLKPhase          = SPI_PHASE_1EDGE;            /* 数据采样从第一个时钟边沿开始 */
//    hspi2.Init.DataSize          = SPI_DATASIZE_8BIT;          /* SPI 数据大小为 8 位 */
//    hspi2.Init.FirstBit          = SPI_FIRSTBIT_MSB;           /* SPI MSB 传输 */
//    hspi2.Init.NSS               = SPI_NSS_SOFT;        				/* NSS 硬件模式 */
//    hspi2.Init.Mode              = SPI_MODE_MASTER;            /* 配置为主机模式 */
//    hspi2.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE; /* 禁用 CRC 校验 */
//    hspi2.Init.CRCPolynomial     = 10;                          /* CRC 多项式值 */

//if (HAL_SPI_Init(&hspi2) != HAL_OK)
//{
//  APP_ErrorHandler();
//}

//  /* USER CODE BEGIN SPI1_Init 2 */

//  /* USER CODE END SPI1_Init 2 */

//}


/* SPI3 init function */
void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */

    /* 初始化 SPI 外设 */
    hspi3.Instance               = SPI3;                       /* SPI1 */
    hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;   /* 预分频器 : 16 */
    hspi3.Init.Direction         = SPI_DIRECTION_2LINES;       /* 全双工 */
    hspi3.Init.CLKPolarity       = SPI_POLARITY_LOW;           /* SPI 时钟极性: 低 */
    hspi3.Init.CLKPhase          = SPI_PHASE_1EDGE;            /* 数据采样从第一个时钟边沿开始 */
    hspi3.Init.DataSize          = SPI_DATASIZE_8BIT;          /* SPI 数据大小为 8 位 */
    hspi3.Init.FirstBit          = SPI_FIRSTBIT_MSB;           /* SPI MSB 传输 */
    hspi3.Init.NSS               = SPI_NSS_SOFT;        				/* NSS 软件件模式 */
    hspi3.Init.Mode              = SPI_MODE_MASTER;            /* 配置为主机模式 */
//    hspi3.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE; /* 禁用 CRC 校验 */
//     hspi3.Init.CRCPolynomial     = 10;                          /* CRC 多项式值 */

if (HAL_SPI_Init(&hspi3) != HAL_OK)
{
  APP_ErrorHandler();
}

  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}


/* USER CODE BEGIN 1 */
uint8_t SPI3_ReadWriteByte(uint8_t data)
{
	uint8_t rxdata = 0;
	if(HAL_SPI_TransmitReceive(&hspi3,&data,&rxdata,1,1000) != HAL_OK)
	{
		rxdata=0XFF;
	}
return rxdata;
}




/* USER CODE END 1 */
