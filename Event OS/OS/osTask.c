/*-----------------------------------------------File Info------------------------------------------------
** File Name:               osTask.c  
** Last modified date:      2016.07.22
** Last version:            V0.1
** Description:             
**--------------------------------------------------------------------------------------------------------            
** Created date:            2016.09.07
** author:                  YLY
** Version:                 V0.1
** Descriptions:            系统任务注册/调度. 
**--------------------------------------------------------------------------------------------------------
*/

/*************************************************************
* 	include 
* 	头文件	
*************************************************************/
/* 系统配置文件 */ 
#include "osConfig.h" 
/* 重定义头文件 */
#include "osTypedef.h"
/*消息队列,用户可以此文件创建所需的各种类型的消息队列*/
#include "osQueue.h"
/* 定时器 */
#include "osTimer.h"
/* 事件 */
#include "osEvent.h" 
/*任务创建/调度用户需要加载*/
#include "osTask.h"
/*OS硬件相关*/
#include "osHardware.h"

#include "osScheduler.h"

#if DEFAULT_IDLE_TASK == 0
	extern void idle_task(uint8_t taskId, uint8_t msg, void *pData, uint16_t dataSize);
#endif
/* 声名任务结构体变量 */
tsTask gtTask;
static tsTaskMsg gtTaskBuffer[MAX_TASK_MSG_BUF_NUM];	
tsQueue gtTaskQueue;
/**
*	function:	 
*	作用:	创建一个任务 
*	parame 1:	
*	参数1:	taskId为接收消息的任务ID
*	parame 2:	
*	参数2:	msg为发送的消息内容
*	parame 3:	
*	参数3:	pData传递的数据, 
*	parame 4:	
*	参数4:	dataSize传递数据的大小
*	return :	
*	返回:	消息发送成功返回true,否则返回false.
**/
bool send_msg_to_task(uint8_t taskId, uint8_t msg, void *pData, uint16_t dataSize)
{
	tsTaskMsg sMsg;	
	uint16_t tempMsgNum;
	
	sMsg.taskId = taskId;	
	sMsg.message =  msg;
	sMsg.pData	= pData;
	sMsg.dataSize = dataSize;
	
	tempMsgNum = get_queue_data_num(&gtTaskQueue);	/* 获取消息数量 */	
	if(tempMsgNum >= MAX_TASK_MSG_BUF_NUM)
	{
		OS_ERR_PRINTF("\r\n\r\ntaskMsgSendFalse,please check \"MAX_TASK_MSG_BUF_NUM\"=%d\r\n\r\n",MAX_TASK_MSG_BUF_NUM);
		gtTask.taskMsgMax = tempMsgNum;
		return false;
	}
	tempMsgNum++;	/* 还有一个消息未发送 */
	if(gtTask.taskMsgMax < tempMsgNum)
	{
		gtTask.taskMsgMax = tempMsgNum;
		OS_DEBUG_PRINTF("gtTask.taskMsgMax=%d,Usage percentage=%d%%\r\n",gtTask.taskMsgMax,(uint8_t)((gtTask.taskMsgMax*100)/MAX_TASK_MSG_BUF_NUM));
	}
	OS_INFO_PRINTF("SendMsgToTaskSuccess,TaskId=%d,Msg=%x,dataAddress=%x,size=%d\r\n",taskId,msg,(uint32_t )pData,dataSize);	
	return push_qu_one(gtTaskQueue,sMsg);	/* 发送消息 */ 		
}

/**
*	function:	Create task 
*	作用:	创建一个任务 
*	parame 1:	Function pointer
*	参数1:	回调函数指针,即任务函数指针,任务函数原型	void taskFunc(uint8_t id, uint8_t msg, void *pData, uint16_t dataSize);  
*	return :	ID was created as a function of the number, type uint32_t, 
*				if the return null, then create a failure,
*				When you fail to create a task, 
*				check the maximum number of tasks "max_task_num" in the osconfig.
*	返回:	被创建函数的ID号,类型为uint32_t,如果返回NULL,则创建失败,
*           任务创建失败时,请检查osConfig中任务最大数量"MAX_TASK_NUM".
**/
uint8_t create_task(tpfTaskFunc pfFunction)
{
	static uint8_t firstCreateTask = 0;
	uint32_t i;
	
	if(!firstCreateTask)	/* 第一次创建任务  */
	{
		/* 初始化任务相关标识 */
		firstCreateTask = 1;
		create_qu(gtTaskQueue,gtTaskBuffer);
		memset((uint8_t *)&gtTask,0x00,sizeof(gtTask));
		/*  初始化空闲任务 */
		#if EN_IDLE_TASK == 1
			gtTask.pfFunc[gtTask.taskIdMax] = idle_task;
			send_msg_to_task(gtTask.taskIdMax, TASK_INIT, NULL, 0);/* 给当前任务发送初始化消息 */ 
		#endif
	}
	if(pfFunction == NULL)
		return 0;
	#if EN_IDLE_TASK == 1
		if((gtTask.taskIdMax+1) >= MAX_TASK_NUM)
		{
			OS_ERR_PRINTF("\r\n\r\ntaskCreateFalse,please check \"MAX_TASK_NUM\"=%d\r\n\r\n",MAX_TASK_NUM);
			return 0;
		}
	#else
		if((gtTask.taskIdMax) >= MAX_TASK_NUM)
		{
			OS_ERR_PRINTF("\r\n\r\ntaskCreateFalse,please check \"MAX_TASK_NUM\"=%d\r\n\r\n",MAX_TASK_NUM);
			return 0;
		}		
	#endif
		
	/* 判断此任务是否被创建过 */
	for(i=0; i<gtTask.taskIdMax; i++)
	{
		if(gtTask.pfFunc[i] == pfFunction)	//此任务已经被创建
		{
			OS_ERR_PRINTF("\r\ntaskIsCreated\r\n");
			return i;
		}
	}

	/* 查找是否有被删除的任务*/
	for(i=0; i<gtTask.taskIdMax; i++)
	{
		if(gtTask.pfFunc[i] == NULL)
		{	
			gtTask.pfFunc[i] = pfFunction;
			send_msg_to_task(i, TASK_INIT, NULL, 0);/* 给当前任务发送初始化消息 */ 	
			OS_DEBUG_PRINTF("taskCreateSuccess,TaskId=%d\r\n",gtTask.taskIdMax);				
			return i;
		}
	}
	
	gtTask.taskIdMax++;	
	gtTask.pfFunc[gtTask.taskIdMax] = pfFunction;	
	send_msg_to_task(gtTask.taskIdMax, TASK_INIT, NULL, 0);/* 给当前任务发送初始化消息 */ 
	OS_DEBUG_PRINTF("taskCreateSuccess,TaskId=%d\r\n",gtTask.taskIdMax);	
	return gtTask.taskIdMax;	
}

/**
*	function:	Create task 
*	作用:	删除一个任务 
*	parame 1:	Function pointer
*	参数1:	回调函数指针,即任务函数指针,任务函数原型	void taskFunc(uint8_t id, uint8_t msg, void *pData, uint16_t dataSize);  
*	return :	ID was created as a function of the number, type uint32_t, 
*				if the return null, then create a failure,
*				When you fail to create a task, 
*				check the maximum number of tasks "max_task_num" in the osconfig.
*	返回:	被创建函数的ID号,类型为uint32_t,如果返回NULL,则创建失败,
*           任务创建失败时,请检查osConfig中任务最大数量"MAX_TASK_NUM".
**/
uint8_t del_task(tpfTaskFunc pfFunction)
{
	uint32_t i;
	
	if(pfFunction == NULL)
		return 0;
	
	/* 判断此任务是否被创建过 */
	for(i=0; i<gtTask.taskIdMax; i++)
	{
		if(gtTask.pfFunc[i] == pfFunction)	//此任务已经被创建
		{
			gtTask.pfFunc[i] = NULL;
			OS_DEBUG_PRINTF("taskDelSuccess,TaskId=%d\r\n",i);	
			return i;
		}
	}
	OS_DEBUG_PRINTF("taskDelFail\r\n");	
	return 0;	
}
/*
*	function:	task Scheduler
*	作用:	任务调度,放置于主函数大循环中,用于调度已经注册的任务
* 	此处写成内联函数,目前是考虑任务阻塞,通过内联函数的方式,
*	可以实现在一个任务阻塞时,仍然可以调度其它任务. 
*	parame :	void
*	参数:	空
*	return :	not return
*	返回:	未处理的任务数量
**/
uint8_t task_scheduler(void)
{
	tsTaskMsg sMsg;	
	if(!get_queue_data_num(&gtTaskQueue))	/* 没有消息发给任务返回 */
		return 0;
	pull_qu_one(gtTaskQueue,&sMsg);
	if(sMsg.taskId > gtTask.taskIdMax)		/*  取ID号不对,不调度此任务直接返回  */	
		return get_queue_data_num(&gtTaskQueue);
	if(gtTask.pfFunc[sMsg.taskId])
		gtTask.pfFunc[sMsg.taskId](sMsg.taskId,sMsg.message,sMsg.pData,sMsg.dataSize);		/* 执行调度 */
	return get_queue_data_num(&gtTaskQueue);
}


/**
*	function:	default idle task
*	作用:	默认空闲任务,可用于分析CPU的使用率
*	return :	not
*	返回:  无 
**/
#if DEFAULT_IDLE_TASK == 1

void idle_task(uint8_t taskId,uint8_t msg, void *pData, uint16_t dataSize)
{
	static uint32_t idleTimes = 0;
	static uint32_t idleTimesMax = 0;
	if(idleTimes < 0xffffffff)
		idleTimes++;
	switch(msg)
	{	
		case TASK_INIT:	/* 系统初始化 */
		{
			OS_INFO_PRINTF("idle Init\r\n");
		}break;
		
		case OUTPUT_OS_INFO:
		{
			if(idleTimesMax < gCpuRunBase)
				idleTimesMax = gCpuRunBase;			
			OS_DEBUG_PRINTF("\r\n");
			OS_DEBUG_PRINTF("cpuUsePercentage=%d%%,idle=%x,base=%x\r\n",100-((idleTimes*100)/idleTimesMax),idleTimes,idleTimesMax);	
			OS_DEBUG_PRINTF("MsgMax=%d,UsePercentage=%d%%\r\n",gtTask.taskMsgMax,(uint8_t)((gtTask.taskMsgMax*100)/MAX_TASK_MSG_BUF_NUM));	
			OS_DEBUG_PRINTF("TaskMax=%d,UsePercentage=%d%%\r\n",gtTask.taskIdMax+1,(uint8_t)(((gtTask.taskIdMax+1)*100)/MAX_TASK_NUM));
			OS_DEBUG_PRINTF("MaxTimer=%d,UsePercentage=%d%%\r\n",gtTimer.maxTimer,(gtTimer.maxTimer*100)/MAX_TIMER_NUM);
			OS_DEBUG_PRINTF("MaxEvent=%d,UsePercentage=%d%%\r\n",gtEvent.maxEvent,(gtEvent.maxEvent*100)/EVNT_CONCURRENT);	
			OS_DEBUG_PRINTF("\r\n");			
			idleTimes = 0;
			gCpuRunBase = 0;			
		}break;
		
		default:
		{			
			
		}break;
	}
}
#endif