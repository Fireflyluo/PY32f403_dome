/*-----------------------------------------------File Info------------------------------------------------
** File Name:               bsp_core.h  
** Last modified date:      2025.6.29
** Last version:            V1.0.0
** Description:             BSP系统初始化头文件
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.6.29
** author:                  Fireflyluo
** Version:                 V1.0.0
** Descriptions:            声明BSP_Init()及其他外设初始化接口；
**                          包含所有BSP相关头文件。
**--------------------------------------------------------------------------------------------------------*/
#ifndef __BSP_CORE_H__
#define __BSP_CORE_H__

/* Includes ------------------------------------------------------------------*/
#include "bsp_common.h"
#include "userConfig.h"

#include "bsp_gpio.h"
#include "bsp_exti.h"
#include "bsp_spi.h"
//#include "bsp_i2c.h"
//#include "bsp_adc.h"
#include "bsp_uasrt.h"
#include "bsp_tim.h"
#include "bsp_pwm.h"
#include "tim.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi1_rx;

void BSP_Init(void);
void BSP_Delay_us(uint32_t us);



#endif
