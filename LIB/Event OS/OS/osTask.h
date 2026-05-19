 /*-----------------------------------------------File Info------------------------------------------------
** File Name:               osConfig.h  
** Last modified date:      2016.09.8
** Last version:            V0.1
** Description:                
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2016.09.8
** author:                  YLY
** Version:                 V0.1
** Descriptions:            任务创建/任务调度 实现 
**--------------------------------------------------------------------------------------------------------*/
#ifndef _OS_TASK_H_
#define _OS_TASK_H_

/*************************************************************
* 	include 
* 	头文件	
*************************************************************/ 
#include <stdlib.h>
#include "osConfig.h" 
#include "osTypedef.h"
#include "osTask.h"

/* 动态内存定义 */
//#define osMalloc(x)		malloc(x)
//#define osFree(x)			free(x)

extern void *pvPortMalloc( size_t xWantedSize );
extern void vPortFree( void *pv );
extern size_t xPortGetFreeHeapSize( void );
extern size_t xPortGetMinimumEverFreeHeapSize( void );

#define osMalloc(x)		pvPortMalloc(x)
#define osFree(x)		vPortFree(x)

#define osGetFreeSize()		xPortGetFreeHeapSize()
#define osGetMinimum()		xPortGetMinimumEverFreeHeapSize()

/* Defining task function pointer array types   */
/* 定义任务函数指针数组类型*/
typedef void(*tpfTaskFunc)(uint8_t id, uint8_t msg, void *pData, uint16_t dataSize);

/* Define task structure  */
/* 定义任务结构 */
typedef struct strTask 
{
	uint8_t taskMsgMax;
	uint8_t taskIdMax;
	tpfTaskFunc pfFunc[MAX_TASK_NUM];
}tsTask;

/* Define task structure  */
/* 定义任务消息结构 */
typedef struct strTaskMessage 
{
	uint8_t  taskId;
	uint8_t  message;
	uint16_t dataSize;
	void 	*pData;
}tsTaskMsg;

/* 全君变量声明 */
extern tsTask gtTask;





/** 
*	任务函数必需是以下类型 
* 	void vTaskFunc(void *param); 
**/
extern uint8_t create_task(tpfTaskFunc pfFunction);
extern uint8_t del_task(tpfTaskFunc pfFunction);

extern bool send_msg_to_task(uint8_t taskId, uint8_t msg, void *pData, uint16_t dataSize);
extern uint8_t task_scheduler(void);

#if DEFAULT_IDLE_TASK == 1
extern void idle_task(uint8_t taskId,uint8_t msg, void *pData, uint16_t dataSize);
#endif




#endif /* _OS_TASK_H_ */