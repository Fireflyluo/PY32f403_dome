 /*-----------------------------------------------File Info------------------------------------------------
** File Name:               osTimer.h  
** Last modified date:      2016.09.8
** Last version:            V0.1
** Description:                 
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2016.09.8
** author:                  YLY
** Version:                 V0.1
** Descriptions:            事件创建/事件调度,这是一个基于系统TICK的真正定时器,于TICK中断中处理,非必要的情况尽量不要使用
**--------------------------------------------------------------------------------------------------------*/
#ifndef _OS_TIMER_H_
#define _OS_TIMER_H_

/*************************************************************
* 	include 
* 	头文件	
*************************************************************/ 
#include "osConfig.h" 
#include "osTypedef.h"

/* 定义定时器函数指针数组类型*/
typedef void(*tpfTimerFunc)(void);

typedef struct strTimer
{
	uint8_t status[MAX_TIMER_NUM];			/* 状态，单次或周期，运行、暂停 */
	uint32_t setTime[MAX_TIMER_NUM];		/* 定时时间 */	
	uint32_t runTime[MAX_TIMER_NUM];		/* 运行时间 */
	tpfTimerFunc pfFunc[MAX_TIMER_NUM];		/* 定时器回调函数指针 */
	#if (EN_IDLE_TASK == 1) && (DEFAULT_IDLE_TASK == 1) && \
		(OS_DEBUG_LEVEL != CLOSE) && (OS_DEBUG_LEVEL != ERROR)
		uint16_t maxTimer;			/* 标识最大并发数,便于调试 */
	#endif
}tsTimer;

extern tsTimer gtTimer;

extern bool create_timer(uint32_t timeMs, tpfTimerFunc pfFunction);//创建定时器
extern bool del_timer(tpfTimerFunc pfFunction);

extern void timer_scheduler(void);
#endif /* _OS_TIMER_H_ */