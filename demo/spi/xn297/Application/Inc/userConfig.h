 /*-----------------------------------------------File Info------------------------------------------------
** File Name:               userConfig.h  
** Last modified date:      2025.7.1
** Last version:            V0.1
** Description:             
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            应用程序配置
**--------------------------------------------------------------------------------------------------------*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdio.h>
#include <stdbool.h>
/* MCU_Driver */
//片上外设驱动
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "bsp_core.h"


/* Module_Driver */
//板级外设驱动
#include "SI24R1.h"
//#include "xl2400.h"
#include "kfifo.h"
#include "xn297L.h"

/* 全局宏定义 */
/* *****GPIO*************************** */

/* LED引脚定义
*		PA1 -> LED1
*		PB2 -> LED2
*/
	
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_2
#define LED2_GPIO_Port GPIOB

/* 射频引脚定义
*		PB3 -> RF_CLK
*		PB4 -> RF_MISO
*		PB5 -> RF_MOSI
*
*		PB6 -> RF_IRQ		//射频模块si24r1 / nRF24Loi+
*		PB7 -> RF_CE
*		PA15 -> RF1_CSN 
*		
*		PB1 -> RF2_CSN	//xl2400p
*/
#define RF_IRQ_Pin GPIO_PIN_6   //si24r1中断引脚
#define RF_IRQ_GPIO_Port GPIOB
#define RF_IRQ_EXTI_IRQn EXTI15_10_IRQn

#define RF_CE_Pin GPIO_PIN_7    //si24r1模块使能引脚
#define RF_CE_GPIO_Port GPIOB
#define RF1_CSN_Pin GPIO_PIN_15  //si24r1模块片选引脚
#define RF1_CSN_GPIO_Port GPIOA
#define RF2_CSN_Pin GPIO_PIN_1   //xl2400p模块片选引脚
#define RF2_CSN_GPIO_Port GPIOB

/* LCD引脚定义
*		PB12 -> SRC_CS
*		PB13 -> SRC_SCL
*		PB14 -> SRC_DC
*		PB15 -> SRC_SDA		
*		PA8 -> SRC_RES
*		
*/
#define SRC_RES_Pin GPIO_PIN_8
#define SRC_RES_GPIO_Port GPIOA
#define SRC_DC_Pin GPIO_PIN_15
#define SRC_DC_GPIO_Port GPIOB
#define SRC_SDA_Pin GPIO_PIN_14
#define SRC_SDA_GPIO_Port GPIOB
#define SRC_SCL_Pin GPIO_PIN_13
#define SRC_SCL_GPIO_Port GPIOB
#define SRC_CS_Pin GPIO_PIN_12
#define SRC_CS_GPIO_Port GPIOB

/* 按键定义
*		PB8 -> KEY1
*		PB9 -> KEY2
*/
#define KEY1_Pin GPIO_PIN_8
#define KEY1_GPIO_Port GPIOB
#define KEY1_EXTI_IRQn EXTI9_5_IRQn

#define KEY2_Pin GPIO_PIN_9
#define KEY2_GPIO_Port GPIOB
#define KEY2_EXTI_IRQn EXTI9_5_IRQn

#endif	/* __CONFIG_H_ */


