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
** Descriptions:            软件定时器 
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
#include "osTimer.h"
/*OS硬件相关*/
#include "osHardware.h"
tsTimer gtTimer;


/**
*	function:	Create timer 
*	作用:	创建一个定时器,此定时器属于硬件定时器,避免定时器回调函数中有大量代码.
*           定时器一旦创建后,在删除前,就会周期性运行;如果需要单次定时器,请用延时事件来实现.
*	参数1:	定时时间,1mS为单位,0-65535
*	参数2:	回调函数指针,即任务函数指针,任务函数原型	void vTimerFunc(void *param);  			
*	返回:	false or true
*           任务创建失败时,请检查osConfig中任务最大数量"MAX_TASK_NUM",或者定时器已经被创建.
**/
bool create_timer(uint32_t timeMs, tpfTimerFunc pfFunction)
{
	static uint8_t firstCreateTimer = 0;
	uint16_t i;
	
	uint32_t tempTime;
	
	/* 将绝对延时时间转换为相对延时 */	
	#if(TICK_CYCLICITY_US == 1000u)		/* ==1mS */			
		tempTime = timeMs;
	#elif(TICK_CYCLICITY_US > 1000u)	/* 大于1mS */
		tempTime = timeMs / (TICK_CYCLICITY_US / 1000u);	
	#else								/* 小于1mS */
		tempTime = timeMs * (1000u / TICK_CYCLICITY_US);	
	#endif

	/* 第一次创建任务  */
	if(!firstCreateTimer)
	{
		/* 初始化任务相关标识 */
		firstCreateTimer = 1;
		memset((uint8_t *)&gtTimer,0x00,sizeof(gtTimer));/* 初始化定时器 */
	}
	
	/* 不允许创建空指针回调函数 */
	if(pfFunction == NULL)
		return false;	

	ENTER_CRITICAL();		/* 进入临界,屏蔽中断 */
	/* 定时器再次被创建时修改设置时间 */
	for(i=0; i<MAX_TIMER_NUM; i++)
	{
		if(gtTimer.pfFunc[i] == pfFunction)
		{
			gtTimer.setTime[i] = tempTime;
			gtTimer.runTime[i] = gtTimer.setTime[i];			
			OS_INFO_PRINTF("TimerTimeHasBeenModified\r\n");
			EXIT_CRITICAL();	/* 退出临界 */
			return true;
		}
	}
	/* 创建新的定时器 */
	for(i=0; i<MAX_TIMER_NUM; i++)
	{
		if(gtTimer.pfFunc[i] == NULL)		/* 找到位置 */
		{
			gtTimer.setTime[i] = tempTime;
			gtTimer.runTime[i] = gtTimer.setTime[i];
			gtTimer.pfFunc[i] = pfFunction;
			OS_INFO_PRINTF("createTimerSuccess,ID=%d,Function=%x\r\n",i,(uint32_t)pfFunction);
			EXIT_CRITICAL();	/* 退出临界 */		
			return true;
		}
	}	
	EXIT_CRITICAL();	/* 退出临界 */
	OS_ERR_PRINTF("createTimerFalse,pleaseCheck \"MAX_TIMER_NUM\"=%d\r\n",MAX_TIMER_NUM);
	return false;	
}

/**
*	function:	bDelTimer
*	作用:	删除定时器.
*	参数1:	回调函数指针,即任务函数指针;  			
*	返回:	false or ture
**/
bool del_timer(tpfTimerFunc pfFunction)
{
	uint16_t i;	
	/* 不允许创建空指针回调函数 */
	if(pfFunction == NULL)
		return false;
	ENTER_CRITICAL();		/* 进入临界,屏蔽中断 */	
	for(i=0; i<MAX_TIMER_NUM; i++)
	{
		if(gtTimer.pfFunc[i] == pfFunction)	/* 定时器已经被创建 */
		{
			gtTimer.setTime[i] = 0;
			gtTimer.runTime[i] = 0;
			gtTimer.pfFunc[i] = NULL;
			EXIT_CRITICAL();	/* 退出临界 */		
			return true;			
		}
	}
	EXIT_CRITICAL();	/* 退出临界 */		
	OS_DEBUG_PRINTF("TimerIsNotCreated\r\n");
	return false;		
}

/**
*	function:	
*	作用:	定时器调度,放将函数置系统TICK中断中调用. 			
*	返回:	无
**/
void timer_scheduler(void)
{
	uint16_t i;	
	#if (EN_IDLE_TASK == 1) && (DEFAULT_IDLE_TASK == 1) && \
		(OS_DEBUG_LEVEL != CLOSE) && (OS_DEBUG_LEVEL != ERROR)
		uint16_t num = 0;
	#endif	
	for(i=0; i<MAX_TIMER_NUM; i++)
	{
		if(gtTimer.pfFunc[i] != NULL)	/* 查找到定时器 */
		{
			#if (EN_IDLE_TASK == 1) && (DEFAULT_IDLE_TASK == 1) && \
				(OS_DEBUG_LEVEL != CLOSE) && (OS_DEBUG_LEVEL != ERROR)
				num++;
				if(gtTimer.maxTimer < num)
				{
					gtTimer.maxTimer = num;
					OS_DEBUG_PRINTF("MaxTimer=%d,UsePercentage=%d%%\r\n\r\n",gtTimer.maxTimer,(gtTimer.maxTimer*100)/MAX_TIMER_NUM);
				}
			#endif				
			gtTimer.runTime[i]--;		/* 定时器自减 */
			if(!gtTimer.runTime[i])	/* 判断设定时间是否到 */
			{
				gtTimer.runTime[i] = gtTimer.setTime[i];	/* 重置定时器 */
				gtTimer.pfFunc[i]();	/* 运行定时器回调函数 */
			}		
		}
	}
}