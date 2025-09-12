/*-----------------------------------------------File Info------------------------------------------------
** File Name:               bsp_gpio.h  
** Last modified date:      2021.4.9
** Last version:            V1.0.0
** Description:             GPIO控制接口定义
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2021.4.9
** author:                  Fireflyluo
** Version:                 V1.0.0
** Descriptions:            定义GPIO引脚状态、上下拉、输出类型等枚举；
**                          声明GPIO初始化、控制、状态获取等函数接口。
**--------------------------------------------------------------------------------------------------------*/
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_common.h"
/* Public enum ---------------------------------------------------------------*/
/* 函数返回值枚举 */
typedef enum {
    BSP_GPIO_OK = 0,           // UART操作成功
    BSP_GPIO_ERR_INVALID_NUM,  // 无效的GPIO编号
    BSP_GPIO_ERR_NULL_PTR,     // 空指针错误
} BSP_GPIO_Status_TypeDef;

/* Public Function Prototypes ------------------------------------------------*/

// 常规引脚初始化
void BSP_GPIO_DeInit(void *gpiox, uint16_t pin);
BSP_GPIO_Status_TypeDef BSP_GPIO_Init_Output(void *gpiox, uint16_t pin, bsp_gpio_pin_out_t out_mode);
BSP_GPIO_Status_TypeDef BSP_GPIO_Init_Input(void *gpiox, uint16_t pin, bsp_gpio_pin_pull_t pull);
BSP_GPIO_Status_TypeDef BSP_GPIO_Init_Exti(void *gpiox, uint16_t pin, uint8_t irqn, bsp_gpio_exti_int_type_t exti_type, bsp_gpio_pin_pull_t pull);


// 引脚控制和状态获取
void BSP_GPIO_SetPin(void *gpiox, uint16_t pin, bsp_gpio_pin_state_t state);
void BSP_GPIO_SetToggle(void *gpiox, uint16_t pin);
bool BSP_GPIO_GetState(void *gpiox, uint16_t pin);

#endif
