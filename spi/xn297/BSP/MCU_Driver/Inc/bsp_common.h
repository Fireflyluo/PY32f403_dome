/*-----------------------------------------------File Info------------------------------------------------
** File Name:               bsp_common.h  
** Last modified date:      2025.7.1
** Last version:            V0.1
** Description:             公共类型定义与枚举
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            定义通用GPIO状态、上下拉、输出类型等枚举；
**                          定义DMA传输状态、中断触发类型等公共结构；
**                          引入标准库与BSP相关头文件。
**--------------------------------------------------------------------------------------------------------*/
#ifndef __BSP_COMMON_H
#define __BSP_COMMON_H

#include "main.h"
#include "py32f403_it.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>     // NULL
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "bsp_exti.h"
/* ------------------------ 公共枚举定义 ------------------------ */

/**
 * @brief  GPIO引脚状态枚举，表示引脚高低电平
 */
typedef enum
{
    BSP_GPIO_PIN_RESET = 0U,
    BSP_GPIO_PIN_SET
} bsp_gpio_pin_state_t;

/**
 * @brief 定义GPIO引脚的上下拉类型
 */
typedef enum
{
    BSP_GPIO_PIN_NOPULL = 0U,  // 不使用上下拉电阻
    BSP_GPIO_PIN_PULLUP,       // 使用上拉电阻
    BSP_GPIO_PIN_PULLDOWN      // 使用下拉电阻
} bsp_gpio_pin_pull_t;

/**
 * @brief  GPIO引脚输出类型枚举
 */
typedef enum
{
    BSP_GPIO_PIN_OUT_OD = 0U,     // 开漏输出
    BSP_GPIO_PIN_OUT_PP,          // 推挽输出
    BSP_GPIO_PIN_AF_OD,           // 复用开漏
    BSP_GPIO_PIN_AF_PP            // 复用推挽
} bsp_gpio_pin_out_t;

/**
 * @brief 定义GPIO外部中断的触发模式
 */
typedef enum
{
    BSP_GPIO_EXTI_RISING = 0U,          // 上升沿触发中断
    BSP_GPIO_EXTI_IT_FALLING,           // 下降沿触发中断
    BSP_GPIO_EXTI_IT_RISING_FALLING,    // 双边沿触发中断
    BSP_GPIO_EXTI_EVT_FALLING,          // 下降沿事件
    BSP_GPIO_EXTI_EVT_RISING,           // 上升沿事件
    BSP_GPIO_EXTI_EVT_RISING_FALLING    // 双边沿事件
} bsp_gpio_exti_int_type_t;


/**
 * @brief DMA传输状态枚举
 */
typedef enum {
    BSP_DMA_IDLE = 0,       // DMA空闲
    BSP_DMA_TX_BUSY,        // DMA发送中
    BSP_DMA_RX_BUSY,         // DMA接收中
    BSP_DMA_TXRX_BUSY
} bsp_dma_status_t;

#endif /* __BSP_COMMON_H__ */
 