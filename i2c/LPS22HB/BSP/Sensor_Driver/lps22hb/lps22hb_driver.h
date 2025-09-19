/**
  ******************************************************************************
  * @file    lps22hb.h
  * @author  MEMS Software Solutions Team
  * @brief   LPS22HB ͷ�ļ�����
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

/* ��ֹ�ݹ���� -------------------------------------*/
#ifndef LPS22HB_DRIVER_H
#define LPS22HB_DRIVER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* ���� ------------------------------------------------------------------*/
#include "lps22hb_reg.h"
#include <string.h>
#include <stdint.h>
/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component ���
  * @{
  */

/** @addtogroup LPS22HB LPS22HB
  * @{
  */

/** @defgroup LPS22HB_Exported_Types LPS22HB ��������
  * @{
  */

/* ���庯��ָ������ */
typedef int32_t (*LPS22HB_Init_Func)(void);                 /*!< ��ʼ������ָ�� */
typedef int32_t (*LPS22HB_DeInit_Func)(void);               /*!< ����ʼ������ָ�� */
typedef int32_t (*LPS22HB_GetTick_Func)(void);              /*!< ��ȡϵͳ�δ���ָ�� */
typedef void    (*LPS22HB_Delay_Func)(uint32_t);            /*!< ��ʱ����ָ�� */
typedef int32_t (*LPS22HB_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t); /*!< д�Ĵ�������ָ�� */
typedef int32_t (*LPS22HB_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);  /*!< ���Ĵ�������ָ�� */

/**
 * @brief ������IO�ӿڽṹ��
 */
typedef struct
{
  LPS22HB_Init_Func          Init;           /*!< ��ʼ������ */
  LPS22HB_DeInit_Func        DeInit;         /*!< ����ʼ������ */
  uint32_t                   BusType;        /*!< �������ͣ�0-I2C, 1-SPI 4��, 2-SPI 3�� */
  uint8_t                    Address;        /*!< �豸��ַ */
  LPS22HB_WriteReg_Func      WriteReg;       /*!< д�Ĵ������� */
  LPS22HB_ReadReg_Func       ReadReg;        /*!< ���Ĵ������� */
  LPS22HB_GetTick_Func       GetTick;        /*!< ��ȡϵͳ�δ��� */
  LPS22HB_Delay_Func         Delay;          /*!< ��ʱ���� */
} LPS22HB_IO_t;

/**
 * @brief LPS22HB����ṹ��
 */
typedef struct
{
  LPS22HB_IO_t        IO;                    /*!< IO�ӿ� */
  stmdev_ctx_t        Ctx;                   /*!< �豸������ */
  uint8_t             is_initialized;        /*!< ��ʼ��״̬ */
  uint8_t             press_is_enabled;      /*!< ѹ��������ʹ��״̬ */
  uint8_t             temp_is_enabled;       /*!< �¶ȴ�����ʹ��״̬ */
  lps22hb_odr_t       last_odr;              /*!< �ϴ����õ������������ */
} LPS22HB_Object_t;

/**
 * @brief LPS22HB�������Խṹ��
 */
typedef struct
{
  uint8_t Temperature;    /*!< �Ƿ�֧���¶Ȳ��� */
  uint8_t Pressure;       /*!< �Ƿ�֧��ѹ������ */
  uint8_t Humidity;       /*!< �Ƿ�֧��ʪ�Ȳ��� */
  uint8_t Gas;            /*!< �Ƿ�֧��������� */
  uint8_t LowPower;       /*!< �Ƿ�֧�ֵ͹���ģʽ */
  float   HumMaxOdr;      /*!< ʪ���������������� */
  float   TempMaxOdr;     /*!< �¶��������������� */
  float   PressMaxOdr;    /*!< ѹ���������������� */
  float   GasMaxOdr;      /*!< ������������������ */
} LPS22HB_Capabilities_t;

/**
 * @brief ͨ�����������ṹ��
 */
typedef struct
{
  int32_t (*Init)(LPS22HB_Object_t *);                            /*!< ��ʼ������ */
  int32_t (*DeInit)(LPS22HB_Object_t *);                          /*!< ����ʼ������ */
  int32_t (*ReadID)(LPS22HB_Object_t *, uint8_t *);               /*!< ��ȡ�豸ID���� */
  int32_t (*GetCapabilities)(LPS22HB_Object_t *, LPS22HB_Capabilities_t *); /*!< ��ȡ�豸���ܺ��� */
} LPS22HB_CommonDrv_t;

/**
 * @brief �¶ȴ��������������ṹ��
 */
typedef struct
{
  int32_t (*Enable)(LPS22HB_Object_t *);                          /*!< ʹ���¶ȴ����� */
  int32_t (*Disable)(LPS22HB_Object_t *);                         /*!< �����¶ȴ����� */
  int32_t (*GetOutputDataRate)(LPS22HB_Object_t *, float *);      /*!< ��ȡ����������� */
  int32_t (*SetOutputDataRate)(LPS22HB_Object_t *, float);        /*!< ��������������� */
  int32_t (*GetTemperature)(LPS22HB_Object_t *, float *);         /*!< ��ȡ�¶�ֵ */
} LPS22HB_TEMP_Drv_t;

/**
 * @brief ѹ�����������������ṹ��
 */
typedef struct
{
  int32_t (*Enable)(LPS22HB_Object_t *);                          /*!< ʹ��ѹ�������� */
  int32_t (*Disable)(LPS22HB_Object_t *);                         /*!< ����ѹ�������� */
  int32_t (*GetOutputDataRate)(LPS22HB_Object_t *, float *);      /*!< ��ȡ����������� */
  int32_t (*SetOutputDataRate)(LPS22HB_Object_t *, float);        /*!< ��������������� */
  int32_t (*GetPressure)(LPS22HB_Object_t *, float *);            /*!< ��ȡѹ��ֵ */
} LPS22HB_PRESS_Drv_t;

/**
 * @brief FIFOģʽö�ٶ���
 */
typedef enum
{
  LPS22HB_FIFO_BYPASS_MODE                    = (uint8_t)0x00,    /*!< FIFO��·ģʽ��FIFO������Ϊ�գ�ֱ�Ӷ�ȡѹ��ֵ */
  LPS22HB_FIFO_FIFO_MODE                      = (uint8_t)0x20,    /*!< FIFOģʽ��FIFO��ʱֹͣ�ռ����� */
  LPS22HB_FIFO_STREAM_MODE                    = (uint8_t)0x40,    /*!< ��ģʽ��FIFO�б������²������� */
  LPS22HB_FIFO_TRIGGER_STREAMTOFIFO_MODE      = (uint8_t)0x60,    /*!< ��������FIFOģʽ������δ����ʱΪ��ģʽ������������ΪFIFOģʽ */
  LPS22HB_FIFO_TRIGGER_BYPASSTOSTREAM_MODE    = (uint8_t)0x80,    /*!< ������·����ģʽ������δ����ʱΪ��·ģʽ������������Ϊ��ģʽ */
  LPS22HB_FIFO_TRIGGER_BYPASSTOFIFO_MODE      = (uint8_t)0xE0     /*!< ������·��FIFOģʽ������δ����ʱΪ��·ģʽ������������ΪFIFOģʽ */
} LPS22HB_FifoMode;

/* ���������嶨�� */
typedef union
{
  int16_t i16bit[3];    /*!< 3��16λ�з������� */
  uint8_t u8bit[6];     /*!< 6��8λ�޷������� */
} lps22hb_axis3bit16_t;

typedef union
{
  int16_t i16bit;       /*!< 1��16λ�з������� */
  uint8_t u8bit[2];     /*!< 2��8λ�޷������� */
} lps22hb_axis1bit16_t;

typedef union
{
  int32_t i32bit[3];    /*!< 3��32λ�з������� */
  uint8_t u8bit[12];    /*!< 12��8λ�޷������� */
} lps22hb_axis3bit32_t;

typedef union
{
  int32_t i32bit;       /*!< 1��32λ�з������� */
  uint8_t u8bit[4];     /*!< 4��8λ�޷������� */
} lps22hb_axis1bit32_t;

/**
  * @}
  */

/** @defgroup LPS22HB_Exported_Constants LPS22HB ��������
  * @{
  */

#define LPS22HB_OK                0      /*!< �����ɹ� */
#define LPS22HB_ERROR            -1      /*!< ����ʧ�� */

#define LPS22HB_I2C_BUS          0U      /*!< I2C���� */
#define LPS22HB_SPI_4WIRES_BUS   1U      /*!< SPI 4������ */
#define LPS22HB_SPI_3WIRES_BUS   2U      /*!< SPI 3������ */

#define LPS22HB_FIFO_FULL        (uint8_t)0x20  /*!< FIFO����־ */

/**
  * @}
  */

/** @addtogroup LPS22HB_Exported_Functions LPS22HB ��������
  * @{
  */

/* ��������ʼ�������ú��� */
int32_t LPS22HB_RegisterBusIO(LPS22HB_Object_t *pObj, LPS22HB_IO_t *pIO);
int32_t LPS22HB_Init(LPS22HB_Object_t *pObj);
int32_t LPS22HB_DeInit(LPS22HB_Object_t *pObj);
int32_t LPS22HB_ReadID(LPS22HB_Object_t *pObj, uint8_t *Id);
int32_t LPS22HB_GetCapabilities(LPS22HB_Object_t *pObj, LPS22HB_Capabilities_t *Capabilities);
int32_t LPS22HB_Get_Init_Status(LPS22HB_Object_t *pObj, uint8_t *Status);

/* ѹ����������غ��� */
int32_t LPS22HB_PRESS_Enable(LPS22HB_Object_t *pObj);
int32_t LPS22HB_PRESS_Disable(LPS22HB_Object_t *pObj);
int32_t LPS22HB_PRESS_GetOutputDataRate(LPS22HB_Object_t *pObj, float *Odr);
int32_t LPS22HB_PRESS_SetOutputDataRate(LPS22HB_Object_t *pObj, float Odr);
int32_t LPS22HB_PRESS_GetPressure(LPS22HB_Object_t *pObj, float *Value);
int32_t LPS22HB_PRESS_Get_DRDY_Status(LPS22HB_Object_t *pObj, uint8_t *Status);
int32_t LPS22HB_PRESS_Get_FThStatus(LPS22HB_Object_t *pObj, uint8_t *Status);

/* �¶ȴ�������غ��� */
int32_t LPS22HB_TEMP_Enable(LPS22HB_Object_t *pObj);
int32_t LPS22HB_TEMP_Disable(LPS22HB_Object_t *pObj);
int32_t LPS22HB_TEMP_GetOutputDataRate(LPS22HB_Object_t *pObj, float *Odr);
int32_t LPS22HB_TEMP_SetOutputDataRate(LPS22HB_Object_t *pObj, float Odr);
int32_t LPS22HB_TEMP_GetTemperature(LPS22HB_Object_t *pObj, float *Value);
int32_t LPS22HB_TEMP_Get_DRDY_Status(LPS22HB_Object_t *pObj, uint8_t *Status);

/* FIFO��غ��� */
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

/* �Ĵ�����д���� */
int32_t LPS22HB_Read_Reg(LPS22HB_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t LPS22HB_Write_Reg(LPS22HB_Object_t *pObj, uint8_t reg, uint8_t Data);

/* ��ȡѹ�����¶����ݺ��� */
int32_t LPS22HB_Get_Press(LPS22HB_Object_t *pObj, float *Data);
int32_t LPS22HB_Get_Temp(LPS22HB_Object_t *pObj, float *Data);

/* ���β���ģʽ���� */
int32_t LPS22HB_Set_One_Shot(LPS22HB_Object_t *pObj);
int32_t LPS22HB_Get_One_Shot_Status(LPS22HB_Object_t *pObj, uint8_t *Status);

/**
  * @}
  */

/** @addtogroup LPS22HB_Exported_Variables LPS22HB ��������
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