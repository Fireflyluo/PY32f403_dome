 /*-----------------------------------------------File Info------------------------------------------------
** File Name:               osInclude.h  
** Last modified date:      2016.09.8
** Last version:            V0.1
** Description:             OS????     
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2016.09.8
** author:                  YLY
** Version:                 V0.1
** Descriptions:            
**--------------------------------------------------------------------------------------------------------*/
#ifndef _OS_INCLUDE_H_
#define _OS_INCLUDE_H_

/*
**OS相关头文件
*/
/* 重定义相关头文件 */
#include "osTypedef.h"
/*硬件相关*/
#include "osHardware.h"
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

#endif	/*_OS_INCLUDE_H_ */

