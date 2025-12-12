/*-----------------------------------------------File Info------------------------------------------------
** File Name:               osEvent.h  
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
#ifndef _OS_EVENT_H_
#define _OS_EVENT_H_

/*************************************************************
* 	include 
* 	头文件	
*************************************************************/ 
#include "osConfig.h" 
#include "osTypedef.h"
/* Defining task function pointer array types   */
/* 定义事件函数指针数组类型*/
typedef void(*tpEventFunc)(uint32_t eventId, uint32_t param);

/*************************************************************
* Defining task function pointer array types 
* 结构体	
*************************************************************/ 
typedef struct setEvent
{
	volatile uint32_t delayTime[EVNT_CONCURRENT];	/* 延时时间 */
	uint32_t id[EVNT_CONCURRENT];					/* ID号 */
	tpEventFunc pfFunc[EVNT_CONCURRENT];			/* 事件回调函数指针 */
	uint32_t param[EVNT_CONCURRENT];				/* 参数 */
	#if (EN_IDLE_TASK == 1) && (DEFAULT_IDLE_TASK == 1) && \
		(OS_DEBUG_LEVEL != CLOSE) && (OS_DEBUG_LEVEL != ERROR)
		uint16_t maxEvent;			/* 标识最大并发数,便于调试 */
	#endif
}tsEvent;
extern tsEvent gtEvent;



/*************************************************************
* 函数声明	
*************************************************************/ 
//以下API是采用回调函数方式操作事件,同一个回调函数不能被创建多次
extern bool create_event(uint32_t delayMs, tpEventFunc pFunction, uint32_t param);
extern bool clean_event(tpEventFunc pFunction);
extern void event_scheduler(void);

//以下API是采用事件ID方式操作事件,同一个回调函数可能被创建多次
extern uint32_t create_events(uint32_t delayMs, tpEventFunc pFunction, uint32_t param);	//事件创建
extern bool search_event_for_id(uint32_t eventId);												//事件ID
extern bool rst_event_for_id(uint32_t eventId, uint32_t delayMs);								//事件重置
extern bool clean_event_for_id(uint32_t eventId);												//事件清除

//注意,采用回调函数和事件ID操作事件的方法不能混用
/* 系统移植时请将此函数放置于TICK定时器器 */
extern void os_event_timer_isr(void);



#endif /* _OS_EVNT_H_ */