/**
  ******************************************************************************
  * @file    lps22hb.h
  * @author  MEMS Software Solutions Team
  * @brief   LPS22HB 头文件驱动
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016-2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* 防止递归包含 -------------------------------------*/
#ifndef LPS22HB_DRIVER_H
#define LPS22HB_DRIVER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* 包含 ------------------------------------------------------------------*/
#include "lps22hb_reg.h"
#include <string.h>
#include <stdint.h>
/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component 组件
  * @{
  */

/** @addtogroup LPS22HB LPS22HB
  * @{
  */

/** @defgroup LPS22HB_Exported_Types LPS22HB 导出类型
  * @{
  */

/* 定义函数指针类型 */
typedef int32_t (*LPS22HB_Init_Func)(void);                 /*!< 初始化函数指针 */
typedef int32_t (*LPS22HB_DeInit_Func)(void);               /*!< 反初始化函数指针 */
typedef int32_t (*LPS22HB_GetTick_Func)(void);              /*!< 获取系统滴答函数指针 */
typedef void    (*LPS22HB_Delay_Func)(uint32_t);            /*!< 延时函数指针 */
typedef int32_t (*LPS22HB_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t); /*!< 写寄存器函数指针 */
typedef int32_t (*LPS22HB_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);  /*!< 读寄存器函数指针 */

/**
 * @brief 传感器IO接口结构体
 */
typedef struct
{
  LPS22HB_Init_Func          Init;           /*!< 初始化函数 */
  LPS22HB_DeInit_Func        DeInit;         /*!< 反初始化函数 */
  uint32_t                   BusType;        /*!< 总线类型：0-I2C, 1-SPI 4线, 2-SPI 3线 */
  uint8_t                    Address;        /*!< 设备地址 */
  LPS22HB_WriteReg_Func      WriteReg;       /*!< 写寄存器函数 */
  LPS22HB_ReadReg_Func       ReadReg;        /*!< 读寄存器函数 */
  LPS22HB_GetTick_Func       GetTick;        /*!< 获取系统滴答函数 */
  LPS22HB_Delay_Func         Delay;          /*!< 延时函数 */
} LPS22HB_IO_t;

/**
 * @brief LPS22HB对象结构体
 */
typedef struct
{
  LPS22HB_IO_t        IO;                    /*!< IO接口 */
  stmdev_ctx_t        Ctx;                   /*!< 设备上下文 */
  uint8_t             is_initialized;        /*!< 初始化状态 */
  uint8_t             press_is_enabled;      /*!< 压力传感器使能状态 */
  uint8_t             temp_is_enabled;       /*!< 温度传感器使能状态 */
  lps22hb_odr_t       last_odr;              /*!< 上次设置的输出数据速率 */
} LPS22HB_Object_t;

/**
 * @brief LPS22HB功能特性结构体
 */
typedef struct
{
  uint8_t Temperature;    /*!< 是否支持温度测量 */
  uint8_t Pressure;       /*!< 是否支持压力测量 */
  uint8_t Humidity;       /*!< 是否支持湿度测量 */
  uint8_t Gas;            /*!< 是否支持气体测量 */
  uint8_t LowPower;       /*!< 是否支持低功耗模式 */
  float   HumMaxOdr;      /*!< 湿度最大输出数据速率 */
  float   TempMaxOdr;     /*!< 温度最大输出数据速率 */
  float   PressMaxOdr;    /*!< 压力最大输出数据速率 */
  float   GasMaxOdr;      /*!< 气体最大输出数据速率 */
} LPS22HB_Capabilities_t;

/**
 * @brief 通用驱动函数结构体
 */
typedef struct
{
  int32_t (*Init)(LPS22HB_Object_t *);                            /*!< 初始化函数 */
  int32_t (*DeInit)(LPS22HB_Object_t *);                          /*!< 反初始化函数 */
  int32_t (*ReadID)(LPS22HB_Object_t *, uint8_t *);               /*!< 读取设备ID函数 */
  int32_t (*GetCapabilities)(LPS22HB_Object_t *, LPS22HB_Capabilities_t *); /*!< 获取设备功能函数 */
} LPS22HB_CommonDrv_t;

/**
 * @brief 温度传感器驱动函数结构体
 */
typedef struct
{
  int32_t (*Enable)(LPS22HB_Object_t *);                          /*!< 使能温度传感器 */
  int32_t (*Disable)(LPS22HB_Object_t *);                         /*!< 禁用温度传感器 */
  int32_t (*GetOutputDataRate)(LPS22HB_Object_t *, float *);      /*!< 获取输出数据速率 */
  int32_t (*SetOutputDataRate)(LPS22HB_Object_t *, float);        /*!< 设置输出数据速率 */
  int32_t (*GetTemperature)(LPS22HB_Object_t *, float *);         /*!< 获取温度值 */
} LPS22HB_TEMP_Drv_t;

/**
 * @brief 压力传感器驱动函数结构体
 */
typedef struct
{
  int32_t (*Enable)(LPS22HB_Object_t *);                          /*!< 使能压力传感器 */
  int32_t (*Disable)(LPS22HB_Object_t *);                         /*!< 禁用压力传感器 */
  int32_t (*GetOutputDataRate)(LPS22HB_Object_t *, float *);      /*!< 获取输出数据速率 */
  int32_t (*SetOutputDataRate)(LPS22HB_Object_t *, float);        /*!< 设置输出数据速率 */
  int32_t (*GetPressure)(LPS22HB_Object_t *, float *);            /*!< 获取压力值 */
} LPS22HB_PRESS_Drv_t;

/**
 * @brief FIFO模式枚举定义
 */
typedef enum
{
  LPS22HB_FIFO_BYPASS_MODE                    = (uint8_t)0x00,    /*!< FIFO旁路模式：FIFO禁用且为空，直接读取压力值 */
  LPS22HB_FIFO_FIFO_MODE                      = (uint8_t)0x20,    /*!< FIFO模式：FIFO满时停止收集数据 */
  LPS22HB_FIFO_STREAM_MODE                    = (uint8_t)0x40,    /*!< 流模式：FIFO中保持最新测量数据 */
  LPS22HB_FIFO_TRIGGER_STREAMTOFIFO_MODE      = (uint8_t)0x60,    /*!< 触发流到FIFO模式：触发未激活时为流模式，触发激活后变为FIFO模式 */
  LPS22HB_FIFO_TRIGGER_BYPASSTOSTREAM_MODE    = (uint8_t)0x80,    /*!< 触发旁路到流模式：触发未激活时为旁路模式，触发激活后变为流模式 */
  LPS22HB_FIFO_TRIGGER_BYPASSTOFIFO_MODE      = (uint8_t)0xE0     /*!< 触发旁路到FIFO模式：触发未激活时为旁路模式，触发激活后变为FIFO模式 */
} LPS22HB_FifoMode;

/* 数据联合体定义 */
typedef union
{
  int16_t i16bit[3];    /*!< 3个16位有符号整数 */
  uint8_t u8bit[6];     /*!< 6个8位无符号整数 */
} lps22hb_axis3bit16_t;

typedef union
{
  int16_t i16bit;       /*!< 1个16位有符号整数 */
  uint8_t u8bit[2];     /*!< 2个8位无符号整数 */
} lps22hb_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];    /*!< 3个32位有符号整数 */
  uint8_t u8bit[12];    /*!< 12个8位无符号整数 */
} lps22hb_axis3bit32_t;

typedef union
{
  int32_t i32bit;       /*!< 1个32位有符号整数 */
  uint8_t u8bit[4];     /*!< 4个8位无符号整数 */
} lps22hb_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LPS22HB_Exported_Constants LPS22HB 导出常量
  * @{
  */

#define LPS22HB_OK                0      /*!< 操作成功 */
#define LPS22HB_ERROR            -1      /*!< 操作失败 */

#define LPS22HB_I2C_BUS          0U      /*!< I2C总线 */
#define LPS22HB_SPI_4WIRES_BUS   1U      /*!< SPI 4线总线 */
#define LPS22HB_SPI_3WIRES_BUS   2U      /*!< SPI 3线总线 */

#define LPS22HB_FIFO_FULL        (uint8_t)0x20  /*!< FIFO满标志 */

/**
  * @}
  */

/** @addtogroup LPS22HB_Exported_Functions LPS22HB 导出函数
  * @{
  */

/* 传感器初始化和配置函数 */
int32_t LPS22HB_RegisterBusIO(LPS22HB_Object_t *pObj, LPS22HB_IO_t *pIO);
int32_t LPS22HB_Init(LPS22HB_Object_t *pObj);
int32_t LPS22HB_DeInit(LPS22HB_Object_t *pObj);
int32_t LPS22HB_ReadID(LPS22HB_Object_t *pObj, uint8_t *Id);
int32_t LPS22HB_GetCapabilities(LPS22HB_Object_t *pObj, LPS22HB_Capabilities_t *Capabilities);
int32_t LPS22HB_Get_Init_Status(LPS22HB_Object_t *pObj, uint8_t *Status);

/* 压力传感器相关函数 */
int32_t LPS22HB_PRESS_Enable(LPS22HB_Object_t *pObj);
int32_t LPS22HB_PRESS_Disable(LPS22HB_Object_t *pObj);
int32_t LPS22HB_PRESS_GetOutputDataRate(LPS22HB_Object_t *pObj, float *Odr);
int32_t LPS22HB_PRESS_SetOutputDataRate(LPS22HB_Object_t *pObj, float Odr);
int32_t LPS22HB_PRESS_GetPressure(LPS22HB_Object_t *pObj, float *Value);
int32_t LPS22HB_PRESS_Get_DRDY_Status(LPS22HB_Object_t *pObj, uint8_t *Status);
int32_t LPS22HB_PRESS_Get_FThStatus(LPS22HB_Object_t *pObj, uint8_t *Status);

/* 温度传感器相关函数 */
int32_t LPS22HB_TEMP_Enable(LPS22HB_Object_t *pObj);
int32_t LPS22HB_TEMP_Disable(LPS22HB_Object_t *pObj);
int32_t LPS22HB_TEMP_GetOutputDataRate(LPS22HB_Object_t *pObj, float *Odr);
int32_t LPS22HB_TEMP_SetOutputDataRate(LPS22HB_Object_t *pObj, float Odr);
int32_t LPS22HB_TEMP_GetTemperature(LPS22HB_Object_t *pObj, float *Value);
int32_t LPS22HB_TEMP_Get_DRDY_Status(LPS22HB_Object_t *pObj, uint8_t *Status);

/* FIFO相关函数 */
int32_t LPS22HB_FIFO_Get_Data(LPS22HB_Object_t *pObj, float *Press, float *Temp);
int32_t LPS22HB_FIFO_Get_FTh_Status(LPS22HB_Object_t *pObj, uint8_t *Status);
int32_t LPS22HB_FIFO_Get_Full_Status(LPS22HB_Object_t *pObj, uint8_t *Status);
int32_t LPS22HB_FIFO_Get_Level(LPS22HB_Object_t *pObj, uint8_t *Status);
int32_t LPS22HB_FIFO_Get_Ovr_Status(LPS22HB_Object_t *pObj, uint8_t *Status);
int32_t LPS22HB_FIFO_Reset_Interrupt(LPS22HB_Object_t *pObj, uint8_t interrupt);
int32_t LPS22HB_FIFO_Set_Interrupt(LPS22HB_Object_t *pObj, uint8_t interrupt);
int32_t LPS22HB_FIFO_Set_Mode(LPS22HB_Object_t *pObj, uint8_t Mode);
int32_t LPS22HB_FIFO_Set_Watermark_Level(LPS22HB_Object_t *pObj, uint8_t Watermark);
int32_t LPS22HB_FIFO_Usage(LPS22HB_Object_t *pObj, uint8_t Status);

/* 寄存器读写函数 */
int32_t LPS22HB_Read_Reg(LPS22HB_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LPS22HB_Write_Reg(LPS22HB_Object_t *pObj, uint8_t reg, uint8_t Data);

/* 获取压力和温度数据函数 */
int32_t LPS22HB_Get_Press(LPS22HB_Object_t *pObj, float *Data);
int32_t LPS22HB_Get_Temp(LPS22HB_Object_t *pObj, float *Data);

/* 单次测量模式函数 */
int32_t LPS22HB_Set_One_Shot(LPS22HB_Object_t *pObj);
int32_t LPS22HB_Get_One_Shot_Status(LPS22HB_Object_t *pObj, uint8_t *Status);

/**
  * @}
  */

/** @addtogroup LPS22HB_Exported_Variables LPS22HB 导出变量
  * @{
  */
extern LPS22HB_CommonDrv_t LPS22HB_COMMON_Driver;
extern LPS22HB_PRESS_Drv_t LPS22HB_PRESS_Driver;
extern LPS22HB_TEMP_Drv_t LPS22HB_TEMP_Driver;

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */