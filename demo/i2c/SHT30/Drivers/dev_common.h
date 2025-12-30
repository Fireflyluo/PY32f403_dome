/*-----------------------------------------------File Info------------------------------------------------
** File Name:               dev_common.h  
** Last modified date:      2025.7.1
** Last version:            V0.1
** Description:             公共类型定义与枚举
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            定义通用设备状态、上下拉、输出类型等枚举；
**                          
**                          引入标准库与dev相关头文件。
**--------------------------------------------------------------------------------------------------------*/
#ifndef __DEV_COMMON_H
#define __DEV_COMMON_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "bsp_core.h"


// dev 错误码
typedef enum {
    DEV_OK = 0,          // 操作成功
    DEV_ERR_INIT,        // 初始化失败
    DEV_ERR_PARAM,       // 参数错误
    DEV_ERR_STATE,       // 状态错误
    DEV_ERR_IO,          // IO 操作失败
    DEV_ERR_NOT_SUPPORT, // 不支持的操作
    DEV_ERR_TIMEOUT,     // 操作超时
    DEV_ERR_UNKNOWN      // 未知错误
} dev_error_t;






#endif
