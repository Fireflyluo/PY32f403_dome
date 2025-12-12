/*-----------------------------------------------File Info------------------------------------------------
** File Name:               osHardware.c  
** Last modified date:      2016.07.22
** Last version:            V0.1
** Description:             
**--------------------------------------------------------------------------------------------------------            
** Created date:            2016.09.07
** author:                  YLY
** Version:                 V0.1
** Descriptions:            系统与硬件相关部分代码,移植的重点. 
**--------------------------------------------------------------------------------------------------------
*/

/*************************************************************
* 	include 
* 	头文件	
*************************************************************/ 
/*  与硬件相关的库文件  */
#include <stdio.h>
#include "main.h"  // 包含HAL相关头文件
/* 系统配置文件 */ 
#include "osConfig.h"
/* 重定义文件 */
#include "osTypedef.h"
/*消息队列,用户可以此文件创建所需的各种类型的消息队列*/
#include "osTask.h"
/*事件创建/调度用户需要加载*/
#include "osEvent.h" 
/*定时器创建/调度用户需要加载*/
#include "osTimer.h"
/*系统调度和系统延时用户需要加载*/
#include "osScheduler.h"
/*OS驱动*/
#include "osHardware.h"



extern TIM_HandleTypeDef htimx;  // 你的定时器句柄
#ifdef USE_IWDG
extern IWDG_HandleTypeDef hiwdg;
#endif


uint16_t criticalNesting;
bool gClrWdt=false;

/**
 * 看门狗初始化
 */
void wdt_init(void)
{
#ifdef USE_IWDG
    // IWDG定时器1秒
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
    hiwdg.Init.Reload = 1124;  // 1秒超时
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK) {
        Error_Handler();
    }
#endif
}

/**
 * 喂狗
 */
void clr_wdt(void)
{
#ifdef USE_IWDG
    HAL_IWDG_Refresh(&hiwdg);
#endif
}

/**
 * 系统定时器初始化
 */
void sys_tick_init(void)
{
    // HAL库已经初始化了SysTick，这里只需确保配置正确
    // SysTick配置为1ms中断（1000Hz）
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    
    // 设置中断优先级
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

uint32_t get_sys_tick_val(void)
{
    return SysTick->VAL;
}

uint32_t get_sys_tick_load(void)
{
    return SysTick->LOAD;
}

/**
 * SysTick中断处理函数 - 在stm32f4xx_it.c中实现
 */
void os_systick_handler(void)
{
    HAL_IncTick();
    
    static uint16_t tickCount = 0;
    static uint16_t infoTickCount = 0;
    
    tickCount++;
    
    // 每秒清除看门狗
    if(tickCount >= 1000) {  // 1000ms
        tickCount = 0;
        gClrWdt = true;
        
        #if (EN_IDLE_TASK == 1) && (DEFAULT_IDLE_TASK == 1) && \
            (OS_DEBUG_LEVEL != CLOSE) && (OS_DEBUG_LEVEL != ERROR)
        if(++infoTickCount >= OUTPUT_OS_INFO_TIME) {
            infoTickCount = 0;
            send_msg_to_task(IDLE_TASK_ID, OUTPUT_OS_INFO, NULL, 0);
        }
        #endif
    }
    
    // 调用系统调度函数
    timer_scheduler();
    os_event_timer_isr();
    os_scheduler_timer_isr();
}