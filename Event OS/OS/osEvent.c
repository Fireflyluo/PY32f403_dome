/*-----------------------------------------------File Info------------------------------------------------
** File Name:               osEvent.c  
** Last modified date:      2016.07.22
** Last version:            V0.1
** Description:             
**--------------------------------------------------------------------------------------------------------            
** Created date:            2016.09.07
** author:                  YLY
** Version:                 V0.1
** Descriptions:            系统事件注册/调度. 
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
/*事件创建/调度用户需要加载*/
#include "osEvent.h" 
/* 硬件配置参数 */
#include "osHardware.h"

#include "osScheduler.h"

/*  */
tsEvent gtEvent;
volatile uint32_t gTimes=0;
static uint8_t gFirstCreate = 0;
/**
*	function:	Create Event 
*	采用回调函数方式操作事件,同一个回调函数不能被创建多次
*	作用:	创建一个事件,可以是及时的,也可以是延时的. 
*	uint32_t uiTime:	
*	参数1: 设置延时时间,以mS为单位.
*	tpfTimerFunc pFunction:	
*	参数2:	定时事件回调函数 
*	uint32_t param	
*	参数3:	传递给回调函数的参数
*	return : true or false
*	返回:	
* 	注意:软件定时器是一个相对定时器,不能完全保证精确度,另外尽量不要用最小单位来创建定时器,那样会非常不准 
**/
bool create_event(uint32_t delayMs, tpEventFunc pFunction, uint32_t param)
{
	uint16_t i;
	uint32_t tempDelay;	
	
	/* 将绝对延时时间转换为相对延时 */	
	#if(TICK_CYCLICITY_US == 1000u)		/* ==1mS */			
		tempDelay = delayMs;
	#elif(TICK_CYCLICITY_US > 1000u)	/* 大于1mS */
		tempDelay = delayMs / (TICK_CYCLICITY_US / 1000u);	
	#else								/* 小于1mS */
		tempDelay = delayMs * (1000u / TICK_CYCLICITY_US);	
	#endif
	
	/* 检查延时周期是否为1,如果为1,再加1,防止运行时间严重变短,程序异常 */
	if(tempDelay == 1)
		tempDelay++;
	
	if(NULL == pFunction)				/* 空指针返回错误  */
		return false;
	
	/* 第一次创建事件,初始化事件相关标识  */	
	if(!gFirstCreate)	
	{
		/* 初始化任务相关标识 */
		gFirstCreate = 1;
		memset((uint8_t *)&gtEvent,0x00,sizeof(gtEvent));
	}
	
	/* 创建事件之前先查找是否有正在运行的事件,如果有事件重置延时,不允许同一事件被多次创建 */
	for(i=0; i < EVNT_CONCURRENT; i++)
	{
		if(gtEvent.pfFunc[i] == pFunction)	/* 此位置的事件是我们要找的事件 */
		{
			OS_INFO_PRINTF("resettingEvent,Id=%d,pFunction=%x\r\n",i,(uint32_t)pFunction);
			gtEvent.delayTime[i] = tempDelay;	/*  重置运行时间 	*/
			gtEvent.param[i] = param;
			return true	;
		}
	}	
	/* 创建事件之前先查找是否有正在运行的事件,如果有事件重置延时,不允许同一事件被多次创建 */
	for(i=0; i < EVNT_CONCURRENT; i++)
	{
		if(gtEvent.pfFunc[i] == NULL)	/* 创建新的事件 */
		{
			OS_INFO_PRINTF("\r\ncreateEventSuccess,Id=%d,pFunction=%x\r\n",i,(uint32_t)pFunction);	
			gtEvent.id[i] = (gTimes<<12) | i;				
			gtEvent.delayTime[i] = tempDelay;	
			gtEvent.pfFunc[i] = pFunction;
			gtEvent.param[i] = param;
			return true;		
		}
	}		
	OS_ERR_PRINTF("\r\n\r\ncreateEventFalse,pleaseCheck \"EVNT_CONCURRENT\"=%d\r\n\r\n",EVNT_CONCURRENT);
	return false;
}



/**
*	function:	clean Event 
*	作用:	清除事件 
*	tpfTimerFunc pFunction:	
*	参数1: 需要的清除事件的函数指针. 
*	return : true or false
*	返回:	
**/
bool clean_event(tpEventFunc pFunction)
{
	uint16_t i;
	/* 不允许创建空指针回调函数 */
	if(pFunction == NULL)
		return false;		
	for(i=0; i<EVNT_CONCURRENT; i++)
	{
		if(gtEvent.pfFunc[i] == pFunction)	/* 事件已经被创建 */
		{
			OS_INFO_PRINTF("CleanEventSuccess,Id=%d,pFunction=%x\r\n",i,(uint32_t)pFunction);			
			gtEvent.pfFunc[i] = NULL;
			gtEvent.id[i] = 0;
			gtEvent.delayTime[i] = 0;
			gtEvent.param[i] = 0;			
			return true;			
		}
	}
	OS_DEBUG_PRINTF("EventIsNotCreated\r\n");
	return false;
}

/**
*	function:	usCreateEvents 
*	采用事件ID方式操作事件,同一个回调函数可能被创建多次
*	作用:	创建一个事件,可以是及时的,也可以是延时的. 
*	uint32_t uiTime:	
*	参数1: 设置延时时间,以mS为单位.
*	tpfTimerFunc pFunction:	
*	参数2:	定时事件回调函数 
*	uint32_t param	
*	参数3:	传递给回调函数的参数
*	return :事件ID
*	返回:	
* 	注意:软件定时器是一个相对定时器,不能完全保证精确度,另外尽量不要用最小单位来创建定时器,那样会非常不准 
**/
uint32_t create_events(uint32_t delayMs, tpEventFunc pFunction, uint32_t param)
{
	uint16_t i;
	uint32_t tempDelay;	
	
	/* 将绝对延时时间转换为相对延时 */	
	#if(TICK_CYCLICITY_US == 1000u)		/* ==1mS */			
		tempDelay = delayMs;
	#elif(TICK_CYCLICITY_US > 1000u)	/* 大于1mS */
		tempDelay = delayMs / (TICK_CYCLICITY_US / 1000u);	
	#else								/* 小于1mS */
		tempDelay = delayMs * (1000u / TICK_CYCLICITY_US);	
	#endif
	
	/* 检查延时周期是否为1,如果为1,再加1,防止运行时间严重变短,程序异常 */
	if(tempDelay == 1)
		tempDelay++;
	
	if(NULL == pFunction)				/* 空指针返回错误  */
		return false;
	
	/* 第一次创建事件,初始化事件相关标识  */	
	if(!gFirstCreate)	
	{
		/* 初始化任务相关标识 */
		gFirstCreate = 1;
		memset((uint8_t *)&gtEvent,0x00,sizeof(gtEvent));
	}
	
	/* 创建新的事件 */
	for(i=0; i < EVNT_CONCURRENT; i++)
	{
		if(gtEvent.pfFunc[i] == NULL)	/* 此位置的事件是我们要找的事件 */
		{
			OS_INFO_PRINTF("\r\ncreateEventSuccess,Id=%d,pFunction=%x\r\n",i,(uint32_t)pFunction);
			gtEvent.id[i] = (gTimes<<12) | i;		
			gtEvent.delayTime[i] = tempDelay;	
			gtEvent.pfFunc[i] = pFunction;
			gtEvent.param[i] = param;
			return gtEvent.id[i];
		}
	}	
	OS_ERR_PRINTF("\r\n\r\ncreateEventFalse,pleaseCheck \"EVNT_CONCURRENT\"=%d\r\n\r\n",EVNT_CONCURRENT);
	return 0;
}

/**
*	function:	bSearchEventForId
*	作用:	查找事件 
*	uint16_t eventId
*	参数1: 事件ID
*	return : true or false
*	返回:	
**/
bool search_event_for_id(uint32_t eventId)
{
	uint16_t id;
	
	id = eventId&0xfff;
	
	if(id >= EVNT_CONCURRENT)
		return false;
	
	if(gtEvent.id[id] != eventId)	/* 无事件 */
		return false;			
	
	if(gtEvent.pfFunc[id] != NULL)		/* 事件已经被创建 */
		return true;			
	
	return false;
}

/**
*	function:	bRstEventForId 
*	作用:	重置事件 
*	uint16_t eventId
*	参数1: 事件ID
*	return : true or false
*	返回:	
**/
bool rst_event_for_id(uint32_t eventId, uint32_t delayMs)
{
	uint16_t id;
	
	id = eventId&0xfff;
	
	if(id >= EVNT_CONCURRENT)
		return false;
	
	if(gtEvent.id[id] != eventId)	/* 无事件 */
		return false;			
	
	if(gtEvent.pfFunc[id] != NULL)	/* 事件已经被创建 */
	{		
		gtEvent.delayTime[id] = delayMs;	
		return true;			
	}
	return false;
}

/**
*	function:	clean Event 
*	作用:	清除事件 
*	uint16_t eventId
*	参数1: 事件ID
*	return : true or false
*	返回:	
**/
bool clean_event_for_id(uint32_t eventId)
{
	uint16_t id;
	
	id = eventId&0xfff;
	
	if(id >= EVNT_CONCURRENT)
		return false;
	
	if(gtEvent.id[id] != eventId)	/* 无事件 */
		return false;	
	
	if(gtEvent.pfFunc[id] != NULL)	/* 事件已经被创建 */
	{	
		gtEvent.pfFunc[id] = NULL;
		gtEvent.id[id] = 0;
		gtEvent.delayTime[id] = 0;
		gtEvent.param[id] = 0;
		return true;			
	}
	return false;
}
/**
*	function:	evnt Scheduler
*	作用:	事件调度 
*	parame :	void
*	参数:	空
*	return :	not return
*	返回:	无 
**/
void event_scheduler(void)
{
	uint16_t i;	
	tpEventFunc pfFunc;
	uint32_t id,param;
	
	for(i=0; i<EVNT_CONCURRENT; i++)
	{
		if(gtEvent.pfFunc[i] != NULL)		/* 查找到事件 */
		{
			if(!gtEvent.delayTime[i])		/* 延时时间到 */
			{
				pfFunc  = gtEvent.pfFunc[i];	/* 取出事件 */
				id    = gtEvent.id[i];
				param = gtEvent.param[i];
				
				gtEvent.pfFunc[i]  = NULL;	/* 删除当前事件 */
				gtEvent.id[i]    = 0;
				gtEvent.param[i] = 0;
				
				pfFunc(id,param);		/* 运行已经就绪的事件 */
				
				#if (EN_IDLE_TASK == 1) && (DEFAULT_IDLE_TASK == 1) && \
					(OS_DEBUG_LEVEL != CLOSE) && (OS_DEBUG_LEVEL != ERROR)
					if(gCpuRunBase < 0xffffffff)
						gCpuRunBase++;
				#endif						
			}
		}
	}
}


/* 系统移植时请将此函数放置于TICK定时器器 */
void os_event_timer_isr(void)
{
	uint16_t i;	
	#if (EN_IDLE_TASK == 1) && (DEFAULT_IDLE_TASK == 1) && \
		(OS_DEBUG_LEVEL != CLOSE) && (OS_DEBUG_LEVEL != ERROR)
		uint16_t usNum = 0;
	#endif
	gTimes ++;
	for(i=0; i<EVNT_CONCURRENT; i++)
	{
		if(gtEvent.pfFunc[i] != NULL)	/* 查找到事件 */
		{
			#if (EN_IDLE_TASK == 1) && (DEFAULT_IDLE_TASK == 1) && \
				(OS_DEBUG_LEVEL != CLOSE) && (OS_DEBUG_LEVEL != ERROR)
				usNum++;
				if(gtEvent.maxEvent < usNum)
				{
					gtEvent.maxEvent = usNum;
					OS_DEBUG_PRINTF("MaxEvent=%d,UsePercentage=%d%%\r\n\r\n",gtEvent.maxEvent,(gtEvent.maxEvent*100)/EVNT_CONCURRENT);						
				}					
			#endif				
			if(gtEvent.delayTime[i])
			{
				gtEvent.delayTime[i]--;
			}
		}
	}
}