 /*-----------------------------------------------File Info------------------------------------------------
** File Name:               osScheduler.h  
** Last modified date:      2016.09.8
** Last version:            V0.1
** Description:             OS相关配置,以及OS所用到的变量别名     
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2016.09.8
** author:                  YLY
** Version:                 V0.1
** Descriptions:             事件创建/事件调度 实现 
**--------------------------------------------------------------------------------------------------------*/
#ifndef _OS_SCHEDULER_H_
#define _OS_SCHEDULER_H_

/*************************************************************
* 	include 
* 	头文件	
*************************************************************/ 
#include "osTypedef.h"

#if (EN_IDLE_TASK == 1) && (DEFAULT_IDLE_TASK == 1) && \
	(OS_DEBUG_LEVEL != CLOSE) && (OS_DEBUG_LEVEL != ERROR)
	extern uint32_t gCpuRunBase;
#endif
void os_scheduler_timer_isr(void);	
/* 系统移植时请将此内联函数放置于TICK定时器器 */

extern uint32_t get_os_time(void);

extern void os_scheduler(void);

/////////////////////////////////////////////////////////////////////////
/* 特别声明:由于osDelayMs时当前运行的函数会阻塞,
** 但为了做多任务,系统在函数阻塞时,调度其它任务和事件.
** 同时可能调用已经阻塞的任务,发生递归调用的情况;
** 这样压栈的数据就会大大增加,严重时发生栈溢出,导致程序崩溃.
** 用户调用此延时程序时,需要判断返回值,如果返回值为fals,说明延时失败,
** 此时用户需要自行设法解决.
*/
/////////////////////////////////////////////////////////////////////////
extern bool os_delay_ms(uint16_t delayMs);

/*
**  系统指令延时,调用此函数,会使所有任务和事件阻塞,非必要情况下尽量少用
*/
extern void os_directive_delay(uint16_t delay);
extern void os_directive_delay_us(uint16_t delayUs);
extern void os_directive_delay_ms(uint16_t delayMs);


/////////////////////////////////////////////////////////////////////////
/* 特别声明:由于taskDelayMs taskDelayUs时所有任务会阻塞,但事件会被调度.*/
/////////////////////////////////////////////////////////////////////////
extern bool task_delay_ms(uint16_t delayMs);

extern void task_delay_us(uint16_t delayUs);
#endif /* _OS_Scheduler_H_ */