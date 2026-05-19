 /*-----------------------------------------------File Info------------------------------------------------
** File Name:               osTypedef.h  
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
#ifndef _OS_TYPEDEF_H_
#define _OS_TYPEDEF_H_


/*************************************************************
* 	include 
* 	OS用到的标准C库文件	
*************************************************************/ 
#include <stdio.h>		
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* 
**MCU是否属于8051,8051的编译器与ARM的编译器缺少部分库,需要重定义,
**其它8位机如果没有<stdbool.h>和<stdint.h>也可当做8051单片机处理
*/
#define C8051		0

#if C8051
	#define bool 	bit
	#define true 	1
	#define false 	0
	/* exact-width signed integer types */
	typedef   signed          char int8_t;
	typedef   signed short     int int16_t;
	typedef   signed           int int32_t;

	/* exact-width unsigned integer types */
	typedef unsigned          char uint8_t;
	typedef unsigned short     int uint16_t;
	typedef unsigned           int uint32_t;	
#else
	#include <stdint.h> 
	#include <stdbool.h>
#endif
	

extern void disDebugTypedefPrintf(const char *s, ...);

#endif /* #ifndef _OS_TYPEDEF_H_ */

