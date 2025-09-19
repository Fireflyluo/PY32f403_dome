/*-----------------------------------------------File Info------------------------------------------------
** File Name:               bsp_exti.h  
** Last modified date:      2021.4.9
** Last version:            V1.0.0
** Description:             外部中断管理接口定义
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2021.4.9
** author:                  Fireflyluo
** Version:                 V1.0.0
** Descriptions:            定义GPIO中断类型与回调函数原型；
**                          声明中断初始化与回调注册接口。
**--------------------------------------------------------------------------------------------------------*/
#ifndef __BSP_EXTI_H__
#define __BSP_EXTI_H__

/* Includes ------------------------------------------------------------------*/
#include "bsp_common.h"


//#include <stdbool.h>
/* Public Function Prototypes ------------------------------------------------*/
typedef void(*bsp_gpio_exti_callback)(GPIO_TypeDef *gpiox, uint16_t gpio_pin);


void bsp_exit_set(uint8_t irqn, uint32_t priority,uint32_t SubPriority, GPIO_TypeDef *gpiox, uint16_t pin);
void bsp_exit_clear_set(uint8_t irqn);
void bsp_exit_clear_flag( uint16_t gpio_pin_x);
 // 引脚外部中断回调

bool bsp_gpio_exit_irq_register_callback(GPIO_TypeDef *gpiox, uint16_t pin, bsp_gpio_exti_callback callback);

#endif

