/*-----------------------------------------------File Info------------------------------------------------
** File Name:               osQueue.c  
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
/* 调用硬件的IRQ,使程序进入临界段 */
#include "osHardware.h"

/**
*	function:	
*	作用:	创建一个消息队列,队列可以是任意形式的 
*	tsQueue *psQueue:	Queue structure pointer 
*	参数1:	消息队列对象指针,实际是一个机构体变量,用于存放链表. 
*	void *pBuf:	Buffer pointer for storing messages 
*	参数2:	用于存放消息的缓冲区指针,
*			缓冲区可以是结构体类型BUF,可以是一个基本类型的BUF. 
*	uint8_t deep:	 sizeof(buf)/sizeof(buf[0])
*	参数3:	缓冲区的深度,sizeof(buf)/sizeof(width); 
*	uint8_t width:	Length of message data 
*	参数4:	消息数据宽度;sizeof(data)  
*	return : false of true	
*	返回:	成功 或 失败 
**/
bool create_queue(tsQueue *psQueue, void *pBuf, uint16_t deep, uint16_t width)
{
	if((NULL == psQueue) || (NULL == pBuf) || (0 == deep) || (0 == width))
		return false;
	
	psQueue->pHead = pBuf;	
	psQueue->pTail = pBuf;
	psQueue->pBufSta = pBuf;
	psQueue->pBufEnd = (void *)((size_t)pBuf + (deep - 1) * width); 
	psQueue->deep = deep;
	psQueue->width = width;	
	psQueue->num = 0;
	clean_queue(psQueue);
	return true;
}

/*
 * 清空队列 
 */
 void clean_queue(tsQueue *psQueue)
 {
	if(psQueue == NULL)
		return;
	
	psQueue->pHead = psQueue->pBufSta;
	psQueue->pTail = psQueue->pBufSta;	
	psQueue->num = 0;
 }
 
 /*
 * 获取当前队列数据个数 
 */
uint16_t get_queue_data_num(tsQueue *psQueue)
{
  	 return psQueue->num;
}

/***********************************************Function info*********************************************
** Function name:           PushQueue 
** Descriptions:            数据入队（正常入队） 
** Param[input]:            psQueue 队列指针 
** Param[input]:            vpBuf 数据指针 
** Param[input]:            num 消息的数量,注意:这里并非数据长度,而是装入消息的数量 
** Param[output]:           node 
** Returned value:          操作结果               
** Created Date:            2016.09.09
**--------------------------------------------------------------------------------------------------------
** Modified date:           node
** Descriptions:            node
** Descriptions:            node
*********************************************************************************************************/ 
bool push_queue(tsQueue *psQueue,void *pBuf, uint16_t num)
{
	uint16_t i,dataNum,add = 0;
	
	if((psQueue == NULL)||(pBuf == NULL))
		return false;	
	
	ENTER_CRITICAL();	/* 进入临界，避免被中断打断，造成数据异常 */
	dataNum = get_queue_data_num(psQueue);			/* 读数据数量 */ 
	if(num > (psQueue->deep - dataNum))	
	{
		EXIT_CRITICAL();		/* 退出临界，总是与进入临界成对出现 */
		return false;	/* 剩下缓冲区不够返回错误 */
	}
	for(i=0; i<num; i++)
	{
		memcpy(psQueue->pTail, (void *)((size_t)pBuf+add), psQueue->width);	/* 存储数据 */
		add += psQueue->width;
		psQueue->pTail = (void *)((size_t)psQueue->pTail - psQueue->width);
		if(psQueue->pTail < psQueue->pBufSta)
			psQueue->pTail = psQueue->pBufEnd;
	}	 
	psQueue->num += num;
	EXIT_CRITICAL();		/* 退出临界，总是与进入临界成对出现 */
	return true;
}

/***********************************************Function info*********************************************
** Function name:           PushQueueHead 
** Descriptions:            数据入队（插队）,此数据会优先出队 
** Param[input]:            psQueue 队列指针 
** Param[input]:            vpBuf 数据指针 
** Param[input]:            num 数据长度 
** Param[output]:           node 
** Returned value:          操作结果               
** Created Date:            2016.09.09
**--------------------------------------------------------------------------------------------------------
** Modified date:           node
** Descriptions:            node
** Descriptions:            node
*********************************************************************************************************/ 
bool push_queue_prior(tsQueue *psQueue,void *pbuf, uint16_t num)
{
	uint16_t i,dataNum,add = 0;
	
	if((psQueue == NULL)||(pbuf == NULL))
		return false;	
	
	ENTER_CRITICAL();	/* 进入临界，避免被中断打断，造成数据异常 */
	dataNum = get_queue_data_num(psQueue);
	if(num > (psQueue->deep - dataNum))
	{
		EXIT_CRITICAL();		/* 退出临界，总是与进入临界成对出现 */
		return false;
	}
	for(i = 0; i < num; i++)
	{
		psQueue->pHead =(void *)((size_t)psQueue->pHead + psQueue->width);
		if(psQueue->pHead > psQueue->pBufEnd)
			psQueue->pHead = psQueue->pBufSta;		
		memcpy(psQueue->pHead,(void *)((size_t)pbuf+add),psQueue->width);
		add += psQueue->width;
	
	}	 	 
	psQueue->num += num;
	EXIT_CRITICAL();		/* 退出临界，总是与进入临界成对出现 */	
	return true;
}

/***********************************************Function info*********************************************
** Function name:           PullQueue 
** Descriptions:            数据出队（队头出队队） 
** Param[input]:            psQueue 队列指针 
** Param[input]:            pbuf 数据指针 
** Param[input]:            num 消息的数量(或称为条数)
** Param[output]:           node 
** Returned value:          取出消息的条数.               
** Created Date:            2016.09.09
**--------------------------------------------------------------------------------------------------------
** Modified date:           node
** Descriptions:            node
** Descriptions:            node
*********************************************************************************************************/ 
uint16_t pull_queue(tsQueue *psQueue, void *pbuf, uint16_t num)
{
	uint16_t i,dataNum,tmpNum,add =0;
	
	if((psQueue == NULL)||(pbuf == NULL))
		return 0;	
	
	ENTER_CRITICAL();	/* 进入临界，避免被中断打断，造成数据异常 */	
	dataNum = get_queue_data_num(psQueue);
	if(!dataNum)	/* 没有数据返回 */
	{
		EXIT_CRITICAL();		/* 退出临界，总是与进入临界成对出现 */		
		return 0;
	}
	else if(dataNum >= num)
	{
	   tmpNum = num;
	}
	else
	{
       tmpNum = dataNum;
	}
	
	for(i=0; i < tmpNum; i++)
  	{ 
  		memcpy((void *)((size_t)pbuf+add),psQueue->pHead,psQueue->width);
  		add += psQueue->width;	
  		psQueue->pHead = (void *)((size_t)psQueue->pHead - psQueue->width);
		if(psQueue->pHead < psQueue->pBufSta)
			psQueue->pHead = psQueue->pBufEnd;				
  	}	
    psQueue->num -= tmpNum;	
	EXIT_CRITICAL();		/* 退出临界，总是与进入临界成对出现 */			
	return tmpNum;
}