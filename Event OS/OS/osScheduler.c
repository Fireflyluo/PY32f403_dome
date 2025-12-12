/*-----------------------------------------------File Info------------------------------------------------
** File Name:               osScheduler.c  
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
/*任务创建/调度用户需要加载*/
#include "osTask.h"
/*事件创建/调度用户需要加载*/
#include "osEvent.h" 
/*定时器创建/调度用户需要加载*/
#include "osTimer.h"
/*系统调度和系统延时用户需要加载*/
#include "osScheduler.h"
/*OS硬件相关*/
#include "osHardware.h"

static volatile uint32_t gOsRelativeTime = 0;	/* OS相对时间 */
#if (EN_IDLE_TASK == 1) && (DEFAULT_IDLE_TASK == 1) && \
	(OS_DEBUG_LEVEL != CLOSE) && (OS_DEBUG_LEVEL != ERROR)
	uint32_t gCpuRunBase = 0;
#endif

void disDebugTypedefPrintf(const char *s, ...)
{
	;
}

/**
*	function:	
*	作用:	系统调度,由系统调度任务/事件/定时器,保证程序正常运行. 
*	parame :	void
*	参数:	空
*	return :	not return
*	返回:	无 
**/
void os_scheduler(void)
{
	sys_tick_init();	/* 初始化SysTick,调度开始 */
	wdt_init();
	while(1)
	{
		event_scheduler();
		while(task_scheduler())
		{
			OS_CLR_WDT();			
			#if (EN_IDLE_TASK == 1) && (DEFAULT_IDLE_TASK == 1) && \
				(OS_DEBUG_LEVEL != CLOSE) && (OS_DEBUG_LEVEL != ERROR)
				if(gCpuRunBase < 0xffffffff)
					gCpuRunBase++;
			#endif
			event_scheduler();
		}	
		#if (EN_IDLE_TASK == 1) && (DEFAULT_IDLE_TASK == 1) && \
			(OS_DEBUG_LEVEL != CLOSE) && (OS_DEBUG_LEVEL != ERROR)
			if(gCpuRunBase < 0xffffffff)
				gCpuRunBase++;
		#endif			
		#if EN_IDLE_TASK == 1	
			gtTask.pfFunc[0](0,0,NULL,0);	/* 执行空闲任务调度 */
		#endif	
		OS_CLR_WDT();				
	}	
}


/* 系统移植时请将此内联函数放置于TICK定时器器 */
void os_scheduler_timer_isr(void)
{
	gOsRelativeTime ++;
}

uint32_t get_os_time(void)
{
	return gOsRelativeTime;
}

/*
**  系统延时
**  最小单位1/2个TICK周期;延时时可调度其它任务和事件.
**  需要注意,延时中的函数可能会被其它任务或者事件调用,
**	要防止函数递归调用时全局变量或静态变量被修改,造成程序异常.
*/

/////////////////////////////////////////////////////////////////////////
/* 特别声明:由于vOsDelayMs时当前运行的函数会阻塞,
** 但为了做多任务,系统在函数阻塞时,调度其它任务和事件.
** 同时可能调用已经阻塞的任务,发生递归调用的情况;
** 这样压栈的数据就会大大增加,严重时发生栈溢出,导致程序崩溃.
** 为了解决此问题,当已经有任务在延时的,不允许有任务再延时,
** 这样可能会对某个任务造成异常,但保护了系统不会崩溃.
**  用户调用此延时程序时,需要判断返回值,如果返回值为fals,说明延时失败,
**  此时用户需要自行设法解决
*/	
////////////////////////////////////////////////////////////////////////
static uint8_t ucgDelayStatus = 0;
bool os_delay_ms(uint16_t delayMs)
{
	uint64_t endTime; 		// 结束时间
	uint64_t sumTime;		// 总TICK次数 
	uint32_t lastTime; 		// 上次一采样时间	
	uint32_t nowTime;		// 当前时间 

	if(ucgDelayStatus >= OS_DELAY_MAX_CONCURRENT)	/* 已经到达最大延时数 */
		return false;			/* 返回 */
	else
		ucgDelayStatus++;	/* 增加延时数量 */
	
	nowTime = gOsRelativeTime;	/* 获取OS相对时间 */	

	/* 将绝对延时时间转换为相对延时 */	
	#if(TICK_CYCLICITY_US == 1000u)		/* ==1mS */			
		endTime = delayMs;
	#elif(TICK_CYCLICITY_US > 1000u)	/* 大于1mS */
		endTime = delayMs / (TICK_CYCLICITY_US / 1000u);	
	#else								/* 小于1mS */
		endTime = delayMs * (1000u / TICK_CYCLICITY_US);	
	#endif

	lastTime = nowTime;
	sumTime = 0;
	do
	{
		event_scheduler();		
		while(task_scheduler())
		{			
			event_scheduler();
		}			
		OS_CLR_WDT();
		nowTime = gOsRelativeTime;
		if(nowTime > lastTime)	//未溢出
		{
			sumTime += nowTime - lastTime;
		} 
		else if(nowTime < lastTime)	//溢出 
		{
			sumTime += nowTime + ((uint32_t)~0 - lastTime);
		}
		lastTime = nowTime;		
	}while(endTime > sumTime);
	ucgDelayStatus --;
	return true;
}
		
/*
**  系统指令延时,调用此函数,会使所有任务和事件阻塞,非必要情况下尽量少用
*/
void os_directive_delay(uint16_t delay)
{
	while(delay--)
	{
		OS_CLR_WDT();	
	}
}


/*
**  系统指令延时,调用此函数,会使所有任务和事件阻塞,非必要情况下尽量少用
*/
void os_directive_delay_us(uint16_t delayUs)
{
	float untPeriod;		// TICK单位 	
	uint64_t endTick; 		// 结束时间
	uint64_t sumTick;		// 总TICK次数 
	uint32_t lastTick; 		// 上次一采样时间	
	uint32_t nowTick;		// 当前时间 
	
	/* 获取系统定时器值 */
	nowTick = get_sys_tick_val();//定时器向下模式
	
	untPeriod = (float)1000000/SYS_CLOCK_HZ;	//tick时钟间隔uS	
	endTick = delayUs/untPeriod;	

	lastTick = nowTick;
	sumTick = 0;
	do
	{
		nowTick = get_sys_tick_val();
		if(lastTick > nowTick)	//未溢出
		{
			sumTick += lastTick - nowTick;
		} 
		else if(lastTick < nowTick)	//溢出 
		{
			sumTick += lastTick + (get_sys_tick_load() - nowTick);
		}
		lastTick = nowTick;
		OS_CLR_WDT();	
	}while(endTick > sumTick);	
}

/*
**  较精准延时,但最小延时不得小于TICK周期;
*/
void os_directive_delay_ms(uint16_t delayMs)
{	
	uint64_t endTime; 		// 结束时间
	uint64_t sumTime;		// 总TICK次数 
	uint32_t lastTime; 		// 上次一采样时间	
	uint32_t nowTime;		// 当前时间 
	
	nowTime = gOsRelativeTime;	/* 获取OS相对时间 */	
	printf("n=%d",nowTime);
	/* 将绝对延时时间转换为相对延时 */	
	#if(TICK_CYCLICITY_US == 1000u)		/* ==1mS */			
		endTime = delayMs;
	#elif(TICK_CYCLICITY_US > 1000u)	/* 大于1mS */
		endTime = delayMs / (TICK_CYCLICITY_US / 1000u);	
	#else								/* 小于1mS */
		endTime = delayMs * (1000u / TICK_CYCLICITY_US);	
	#endif

	lastTime = nowTime;
	sumTime = 0;
	do
	{
		nowTime = gOsRelativeTime;
		if(nowTime > lastTime)	//未溢出
		{
			sumTime += nowTime - lastTime;
		} 
		else if(nowTime < lastTime)	//溢出
		{
			sumTime += nowTime + ((uint32_t)~0 - lastTime);
		}
		lastTime = nowTime;
		OS_CLR_WDT();	
	}while(endTime > sumTime);		
}



/////////////////////////////////////////////////////////////////////////
/* 特别声明:由于bTaskDelayMs时所有任务会阻塞,
** 任务阻塞时,事件会被调度.
*/
bool task_delay_ms(uint16_t delayMs)
{
	uint64_t endTime; 		// 结束时间
	uint64_t sumTime;		// 总TICK次数 
	uint32_t lastTime; 		// 上次一采样时间	
	uint32_t nowTime;		// 当前时间 
	
	nowTime = gOsRelativeTime;	/* 获取OS相对时间 */	

	/* 将绝对延时时间转换为相对延时 */	
	#if(TICK_CYCLICITY_US == 1000u)		/* ==1mS */			
		endTime = delayMs;
	#elif(TICK_CYCLICITY_US > 1000u)	/* 大于1mS */
		endTime = delayMs / (TICK_CYCLICITY_US / 1000u);	
	#else								/* 小于1mS */
		endTime = delayMs * (1000u / TICK_CYCLICITY_US);	
	#endif

	lastTime = nowTime;
	sumTime = 0;
	do
	{
		event_scheduler();
		nowTime = gOsRelativeTime;
		if(nowTime > lastTime)	//未溢出
		{
			sumTime += nowTime - lastTime;
		} 
		else if(nowTime < lastTime)	//溢出 
		{
			sumTime += nowTime + ((uint32_t)~0 - lastTime);
		}
		lastTime = nowTime;
		OS_CLR_WDT();		
	}while(endTime > sumTime);
	return true;	
}

/* 特别声明:由于bTaskDelayUs时所有任务会阻塞,
** 任务阻塞时,事件会被调度.
*/
void task_delay_us(uint16_t delayUs)
{
	float untPeriod;		// TICK单位 	
	uint64_t endTick; 		// 结束时间
	uint64_t sumTick;		// 总TICK次数 
	uint32_t lastTick; 		// 上次一采样时间	
	uint32_t nowTick;		// 当前时间 
	
	/* 获取系统定时器值 */
	nowTick = get_sys_tick_val();//定时器向下模式
	
	untPeriod = (float)1000000/SYS_CLOCK_HZ;	//tick时钟间隔uS	
	endTick = delayUs/untPeriod;	

	lastTick = nowTick;
	sumTick = 0;
	do
	{
		event_scheduler();
		nowTick = get_sys_tick_val();
		if(lastTick > nowTick)	//未溢出
		{
			sumTick += lastTick - nowTick;
		} 
		else if(lastTick < nowTick)	//溢出 
		{
			sumTick += lastTick + (get_sys_tick_load() - nowTick);
		}
		lastTick = nowTick;
		OS_CLR_WDT();	
	}while(endTick > sumTick);	
}