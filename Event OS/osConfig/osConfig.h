 /*-----------------------------------------------File Info------------------------------------------------
** File Name:               osConfig.h  
** Last modified date:      2016.09.8
** Last version:            V0.1
** Description:             OS相关配置     
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2016.09.8
** author:                  YLY
** Version:                 V0.1
** Descriptions:            
**--------------------------------------------------------------------------------------------------------*/
#ifndef _OS_CONFIG_H_
#define _OS_CONFIG_H_


/* 调用用户配置文件 */
//#include "bspConfig.h"

/*************************************************************
* 	定义系统打印所使用函数	
*************************************************************/ 
#ifndef	LOG_CLOSE
	#define LOG_CLOSE	0	/* 完全关闭-不建议使用,除非在不能打印的情况下才使用 */
#endif
#ifndef	LOG_ERROR
	#define LOG_ERROR	1	/* 错误等级-批量输出时使用 */
#endif
#ifndef	LOG_DEBUG
	#define LOG_DEBUG	2	/* 调试等级-用于软件测试发现故障 */
#endif
#ifndef	LOG_INFO
	#define LOG_INFO	3	/* 信息-开发时使用 */
#endif

#ifndef	OS_DEBUG_LEVEL
	#define OS_DEBUG_LEVEL	LOG_ERROR	/* 默认为错误等级 */	
#endif

/*************************************************************
* 	定义os相关配置参数, 用户根据实际情况修改	
*************************************************************/ 
/**  define Max Task Num
*	定义最大任务数 MAX<0xFFFF ,用户根据项目实际大小修改,
*	任务数越多,所占用的RAM就越多. 
**/
#define IDLE_TASK_NUM	1
#if IDLE_TASK_NUM != 1
	#err	"IDLE_TASK_NUM Must be 1"
#endif
#define MAX_TASK_NUM	(IDLE_TASK_NUM+10u)

/** define Max Task Message Buffer Num
* 定义最大任务消息缓冲数,
* 建议与MAX_TASK_NUM相等或者更大
**/
#define MAX_TASK_MSG_BUF_NUM 	(MAX_TASK_NUM*3)

/** define Max Task Message Buffer Num
* 是否使用空闲任务 1使能,0禁止 */
#ifndef	EN_IDLE_TASK
	#define EN_IDLE_TASK	1	
#endif
/* Whether to use the default idle task  
*	是否使用默认的空闲任务,1使能 ,0禁止 
*	void idleTask(uint16_t usTaskId,uint16_t usMsg);
**/
#ifndef	DEFAULT_IDLE_TASK
	#define DEFAULT_IDLE_TASK	1
#endif
/* TASK ID */
/* 定义空闲任务ID号  */
#define IDLE_TASK_ID	0	/* 空闲任务ID默认为0,无需修改  */

#if IDLE_TASK_ID != 0
	#err "IDLE_TASK_ID Must be 0 !!!"
#endif
/* Define event concurrency number  */
/* 定义事件并发数 */
#define EVNT_CONCURRENT		32u
	
/* Define event concurrency number  */
/* 定义定时器最大数量 */
#define MAX_TIMER_NUM		8u

/* 
**	系统延时并发数
**	加大此时,在任务中用到bOsDelayMs(),会增加栈的空间消耗,注意调整栈大小,并控制使用bOsDelayMs().
*/
#define OS_DELAY_MAX_CONCURRENT		1u

/* 系统定时器频率（Hz）最小为1Hz,最大不超过1000Hz,推荐频率100Hz~1000Hz*/

#define TICK_RATE_HZ   		1000u



/*************************************************************
* 	系统消息列表	
*************************************************************/ 
enum eUsrTaskMsg
{
	/* 用户自定义消息从0x01开始,避免与系统消息冲突 */
	USER_MSG=0x01,	
};

enum eSysTaskMsg
{
	/* 消息从0xf0开始*/
	TASK_INIT=0xf0,		/* 初始化状态 */
	OUTPUT_OS_INFO,		/* 打印输出OS关键信息 */
};



/*************************************************************
* 				错误配置警告	
*************************************************************/ 
#if MAX_TASK_MSG_BUF_NUM < MAX_TASK_NUM
	#err "MAX_TASK_MSG_BUF_NUM mast greater than or equal to MAX_TASK_NUM"
#endif


/*************************************************************
* 				动态内存管理
*************************************************************/ 
/* 定义堆大小 */
#define configTOTAL_HEAP_SIZE		(1024*2)	//2K
	
/* 内存对齐字节数 */
#define portBYTE_ALIGNMENT				4

#if portBYTE_ALIGNMENT == 32
	#define portBYTE_ALIGNMENT_MASK 	(0x001f)
#elif portBYTE_ALIGNMENT == 16
	#define portBYTE_ALIGNMENT_MASK 	(0x000f)
#elif portBYTE_ALIGNMENT == 8
	#define portBYTE_ALIGNMENT_MASK 	(0x0007)
#elif portBYTE_ALIGNMENT == 4
	#define portBYTE_ALIGNMENT_MASK		(0x0003)
#elif portBYTE_ALIGNMENT == 2
	#define portBYTE_ALIGNMENT_MASK		(0x0001)
#elif portBYTE_ALIGNMENT == 1
	#define portBYTE_ALIGNMENT_MASK		(0x0000)
#else
	#err portBYTE_ALIGNMENT err
#endif

/* 是否使用钩子函数 */
#define configUSE_MALLOC_FAILED_HOOK 	0
#define mtCOVERAGE_TEST_MARKER()		
#define configASSERT( x )
#define traceMALLOC( pvAddress, uiSize )	HEAP_PRINTF("\nMalloc:%x,size:%d",(uint32_t )pvAddress,uiSize)
#define traceFREE( pvAddress, uiSize )		HEAP_PRINTF("\nFree:%x,size:%d",(uint32_t )pvAddress,uiSize)


#if OS_DEBUG_LEVEL ==  LOG_CLOSE
	#ifdef	OS_DEBUG_PRINTF		/* 调试信息 */
		#undef  OS_DEBUG_PRINTF
	#endif
		#define OS_DEBUG_PRINTF  	disDebugTypedefPrintf

	#ifdef	OS_INFO_PRINTF		/* 开发消息 */
		#undef  OS_INFO_PRINTF
	#endif
		#define OS_INFO_PRINTF  	disDebugTypedefPrintf
		
	#ifdef	HEAP_PRINTF			/* 动态内存信息 */
		#undef  HEAP_PRINTF
	#endif
		#define HEAP_PRINTF 	disDebugTypedefPrintf

	#ifdef	OS_ERR_PRINTF		/* 系统错误信息 */	
		#undef  OS_ERR_PRINTF
	#endif
		#define OS_ERR_PRINTF 	printf
	
#elif OS_DEBUG_LEVEL ==  LOG_ERROR
	#ifdef	OS_DEBUG_PRINTF		/* 调试信息 */
		#undef  OS_DEBUG_PRINTF
	#endif
		#define OS_DEBUG_PRINTF  	disDebugTypedefPrintf

	#ifdef	OS_INFO_PRINTF		/* 开发消息 */
		#undef  OS_INFO_PRINTF
	#endif
		#define OS_INFO_PRINTF  	disDebugTypedefPrintf
		
	#ifdef	HEAP_PRINTF			/* 动态内存信息 */
		#undef  HEAP_PRINTF
	#endif
		#define HEAP_PRINTF 	disDebugTypedefPrintf

	#ifdef	OS_ERR_PRINTF		/* 系统错误信息 */	
		#undef  OS_ERR_PRINTF
	#endif
		#define OS_ERR_PRINTF 	disDebugTypedefPrintf

#elif OS_DEBUG_LEVEL ==  LOG_DEBUG
	#ifdef	OS_DEBUG_PRINTF		/* 调试信息 */
		#undef  OS_DEBUG_PRINTF
	#endif
		#define OS_DEBUG_PRINTF  	printf

	#ifdef	OS_INFO_PRINTF		/* 开发消息 */
		#undef  OS_INFO_PRINTF
	#endif
		#define OS_INFO_PRINTF  	disDebugTypedefPrintf
		
	#ifdef	HEAP_PRINTF			/* 动态内存信息 */
		#undef  HEAP_PRINTF
	#endif
		#define HEAP_PRINTF 	printf

	#ifdef	OS_ERR_PRINTF		/* 系统错误信息 */	
		#undef  OS_ERR_PRINTF
	#endif
		#define OS_ERR_PRINTF 	printf
		
	#undef  EN_IDLE_TASK	
	#define	EN_IDLE_TASK		1

	#undef  DEFAULT_IDLE_TASK	
	#define	DEFAULT_IDLE_TASK		1
	
#elif OS_DEBUG_LEVEL ==  LOG_INFO
	#ifdef	OS_DEBUG_PRINTF		/* 调试信息 */
		#undef  OS_DEBUG_PRINTF
	#endif
		#define OS_DEBUG_PRINTF  	printf

	#ifdef	OS_INFO_PRINTF		/* 开发消息 */
		#undef  OS_INFO_PRINTF
	#endif
		#define OS_INFO_PRINTF  	printf
		
	#ifdef	HEAP_PRINTF			/* 动态内存信息 */
		#undef  HEAP_PRINTF
	#endif
		#define HEAP_PRINTF 	printf

	#ifdef	OS_ERR_PRINTF		/* 系统错误信息 */	
		#undef  OS_ERR_PRINTF
	#endif
		#define OS_ERR_PRINTF 	printf
		
	#undef  EN_IDLE_TASK	
	#define	EN_IDLE_TASK		1


	#undef  DEFAULT_IDLE_TASK	
	#define	DEFAULT_IDLE_TASK		1

#else 
	#err "please define OS_DEBUG_LEVEL"
#endif



#define OUTPUT_OS_INFO_TIME			60	/* 系统信息输出时间,1~65535 S */







#endif /* #ifndef _OS_CONFIG_H_ */



