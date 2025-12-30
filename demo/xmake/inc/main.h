/**
* 代码规范
* Tab Size == 4;
*
* 数据类型:	数据类型使用<stdint.h>定义的别名,不建议使用C标准全名->,unsigned
char-uint8_t;
*
* 变量命名总则:	基本数据类->
*
全局变量->以g开头，例uint8_t gSize, *
局部变量->无需前缀，按常规命名。
*
*           	  构造类->
*						      指针->
在基本数据类型命名规则前在加p. 例uint8_t *pPointer; *
结构->	结构体必需与typedef配合使用,不推荐单独使用 *
结构名->	以小写str开头,名称等一个字母大写. *
别名->	以小写ts开头,名称等一个字母大写. *
变量->	以小写t开头,结构名称首字母大写,也可以把机构体名称加入到变量第二个单词.
*
*								例struct strData
tTempTime; struct osTask tTempTime;

*						枚举名->
以小写em开头,结构名称首字母大写. 例enum DATE eTempTime; *
标识符->    标识符通常属于常量,全部字母大写,用_连接. *
枚举变量->  以小写e开头,结构名称首字母大写. 例enum DATE eTempTime;
*
*						共同体->
以小写n开头,结构名称首字母大写. 例union DATE nTempTime;
*
*
* 常量和宏定义:	全部字母大写,单词间用下横杠连接"_";
*
* 全局变量:
在类型修饰后,变量名前,加小写g(global)以区分,不允许以单个字母命名的变量,如i,j,k等等.
*			例uint32_t *pgKeyNum;	uint16_t *pgKeyNum;	uint8_t
gKeyNum;
*
* 局部变量:
需要保证与全局变量名称不同,只有用于确认循环次数的允许使用i,j,k....单个字的变量.
*
* 函数命名: 第1个单词首字母小写,第2个单词开始首字母大写;返回值是指针的以小p开头,
*			如 void mcuInit(void){xx}; uint8_t mcuInit(void){xx};
uint16_t * pMcuInit(void){xxx}; *
以下函数"任务Ta/事件Ev/定时Ti",第一个单词与子函数相同,第2个单词为函数的类型的首两个字母.
*			如 uint32 *pEvKeyScan(void){xx};
*			当函数不允许外部文件访问时,需要在函数名称前加static.
*
* 注释:	对代码进行解释除代码外的注释采用 "/""*"  "*""/"
的方式,临时屏蔽某行或者某段代码使用"//",
*
* 大括号:采用C标准,不建议使用C++或JAVA标准
*        如  	if	推荐		|	if{	禁止
*				  {               |  		xxxx;
*					        xxxx;		|    }
*				          }				|
*
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "py32f4xx_hal.h"
#include <stdint.h>
/* Private includes ----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

/* Exported variables prototypes ---------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void APP_ErrorHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
