/*-----------------------------------------------File Info------------------------------------------------
** File Name:               osQueue.h  
** Last modified date:      2016.09.09
** Last version:            V0.1
** Description:             循环队列 
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2016.09.09
** author:                  lth
** Version:                 V0.1
** Descriptions:            用于消息的缓存 
**--------------------------------------------------------------------------------------------------------
** author:                  
** Modified date:           
** Version:                 
** Descriptions:            
**-------------------------------------------------------------------------------------------------------*/
#ifndef _MSG_QUEUE_H_
#define _MSG_QUEUE_H_

/*************************************************************
* 	include 
* 	头文件	
*************************************************************/ 
#include "osTypedef.h"


/*  队列管理器   */
typedef struct strQueue 
{
	void *pHead;
	void *pTail;
	void *pBufSta;
	void *pBufEnd;
	uint16_t deep;
	uint16_t width;
	uint16_t num;	
}tsQueue;

/**--------------以下API操作较为复杂,不推荐用户使用,用户可以使用后面的API----------------------**/
extern bool create_queue(tsQueue *psQueue, void *pBuf, uint16_t deep, uint16_t width);
extern void clean_queue(tsQueue *psQueue);									/* 清除队列  */
extern uint16_t get_queue_data_num(tsQueue *psQueue);							/* 获取当前队列数据个数  */
extern bool push_queue(tsQueue *psQueue,void *pBuf,uint16_t len);				/*  正常装入消息 */
extern bool push_queue_prior(tsQueue *psQueue,void *pbuf,uint16_t len);		/*  插队方式入消息 */
extern uint16_t pull_queue(tsQueue *psQueue,void *pbuf,uint16_t len);			/*  取消息 */




/**-----------------------推荐用户使用以下宏定义的API,操作更方便-------------------------------**/

/*  
** 队列初始化 
** S_QUEUE  ->队列名称,即使用tsQueue 创建的变量
** P_BUF	->用来存放消息的缓冲区指针
** 返回:成功 失败.
*/
#define create_qu(S_QUEUE, P_BUF) create_queue(&S_QUEUE, (void *)P_BUF, sizeof(P_BUF)/sizeof(P_BUF[0]), sizeof(P_BUF[0]))

/*  
** 清除队列中消息 
** S_QUEUE  ->队列名称,即使用tsQueue 创建的变量
*/
#define clean_qu(S_QUEUE) clean_queue(&S_QUEUE)

/*  
** 获取队列中数据数量 
** S_QUEUE  ->队列名称,即使用tsQueue 创建的变量
** 返回队列中的消息数量
*/
#define get_qu_data_num(S_QUEUE) get_queue_data_num(&S_QUEUE)


/*  
** 装入多个消息 
** S_QUEUE  ->队列名称,即使用tsQueue 创建的变量
** P_BUF	->用来存放消息的缓冲区指针
** LEN   ->装消息的长度
** 返回:成功 失败.
*/
#define push_qu_buf(S_QUEUE, P_BUF, NUM)  push_queue(&S_QUEUE, (void *)P_BUF, NUM)
/*  
** 装入1个消息 
** S_QUEUE  ->队列名称,即使用tsQueue 创建的变量
** S_BUF	->用来存放消息的变量,通常是一个结构体类型,也可以是一个基本数据类型,但需要与创建时的类型保持一致
** 返回:成功 失败.
*/	
#define push_qu_one(S_QUEUE, S_BUF)  push_queue(&S_QUEUE, (void *)&S_BUF, 1)
	
/*  
** 插入多个消息 
** S_QUEUE  ->队列名称,即使用tsQueue 创建的变量
** P_BUF	->用来存放消息的缓冲区指针
** LEN   ->装消息的长度
** 返回:成功 失败.
*/
#define push_qu_prior(S_QUEUE, P_BUF, NUM)  push_queue_prior(&S_QUEUE, (void *)P_BUF, NUM)
/*  
** 插入1个消息 
** S_QUEUE  ->队列名称,即使用tsQueue 创建的变量
** S_BUF	->用来存放消息的变量,通常是一个结构体类型,也可以是一个基本数据类型,但需要与创建时的类型保持一致
** 返回:成功 失败.
*/
#define push_qu_prior_one(S_QUEUE, S_BUF)  push_queue_prior(&S_QUEUE, (void *)&S_BUF, 1)
	
/* 取消息 */
/*  
** 取消息 
** S_QUEUE  ->队列名称,即使用tsQueue 创建的变量
** P_BUF	->用来存放消息的缓冲区指针
** LEN   	->取出消息的长度
** 返回:	实际取出的消息长度
*/
#define pull_qu(S_QUEUE, P_BUF, NUM)  pull_queue(&S_QUEUE, (void *)P_BUF, NUM)

/* 取一个消息 */
/*  
** 取消息 
** S_QUEUE  ->队列名称,即使用tsQueue 创建的变量
** P_BUF	->用来存放消息的缓冲区指针
** LEN   	->取出消息的长度
** 返回:	实际取出的消息长度
*/
#define pull_qu_one(S_QUEUE, P_BUF)  pull_queue(&S_QUEUE, (void *)P_BUF, 1)
#endif //_MSG_QUEUE_H_