/**
  ******************************************************************************
  * @file    lps22hb.c
  * @author  MEMS Software Solutions Team
  * @brief   LPS22HB�����ļ�
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

/* ���� ------------------------------------------------------------------*/
#include "lps22hb_driver.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component ���
  * @{
  */

/** @defgroup LPS22HB LPS22HB
  * @{
  */

/** @defgroup LPS22HB_Exported_Variables LPS22HB��������
  * @{
  */

LPS22HB_CommonDrv_t LPS22HB_COMMON_Driver =
{
  LPS22HB_Init,
  LPS22HB_DeInit,
  LPS22HB_ReadID,
  LPS22HB_GetCapabilities,
};

LPS22HB_PRESS_Drv_t LPS22HB_PRESS_Driver =
{
  LPS22HB_PRESS_Enable,
  LPS22HB_PRESS_Disable,
  LPS22HB_PRESS_GetOutputDataRate,
  LPS22HB_PRESS_SetOutputDataRate,
  LPS22HB_PRESS_GetPressure,
};

LPS22HB_TEMP_Drv_t LPS22HB_TEMP_Driver =
{
  LPS22HB_TEMP_Enable,
  LPS22HB_TEMP_Disable,
  LPS22HB_TEMP_GetOutputDataRate,
  LPS22HB_TEMP_SetOutputDataRate,
  LPS22HB_TEMP_GetTemperature,
};

/**
  * @}
  */

/** @defgroup LPS22HB_Private_Function_Prototypes LPS22HB˽�к���ԭ��
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t LPS22HB_GetOutputDataRate(LPS22HB_Object_t *pObj, float *Odr);
static int32_t LPS22HB_SetOutputDataRate_When_Enabled(LPS22HB_Object_t *pObj, float Odr);
static int32_t LPS22HB_SetOutputDataRate_When_Disabled(LPS22HB_Object_t *pObj, float Odr);
static int32_t LPS22HB_Initialize(LPS22HB_Object_t *pObj);

/**
  * @}
  */

/** @defgroup LPS22HB_Exported_Functions LPS22HB��������
  * @{
  */

/**
  * @brief  ע���������IO����
  * @param  pObj �豸����
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_RegisterBusIO(LPS22HB_Object_t *pObj, LPS22HB_IO_t *pIO)
{
  int32_t ret = LPS22HB_OK;

  if (pObj == NULL)
  {
    ret = LPS22HB_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.BusType   = pIO->BusType;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;

    pObj->Ctx.read_reg  = ReadRegWrap;
    pObj->Ctx.write_reg = WriteRegWrap;
    pObj->Ctx.mdelay    = pIO->Delay;
    pObj->Ctx.handle   = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = LPS22HB_ERROR;
    }
    else if (pObj->IO.Init() != LPS22HB_OK)
    {
      ret = LPS22HB_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == LPS22HB_SPI_3WIRES_BUS) /* SPI 3���� */
      {
        /* ���ڵ�һ��ʱ����SPI 3����֧�� */
        if (pObj->is_initialized == 0U)
        {
          /* �����������SPI 3���� */
          uint8_t data = 0x01;

          if (LPS22HB_Write_Reg(pObj, LPS22HB_CTRL_REG1, data) != LPS22HB_OK)
          {
            ret = LPS22HB_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  ��ʼ��LPS22HB������
  * @param  pObj �豸����
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_Init(LPS22HB_Object_t *pObj)
{
  if (pObj->is_initialized == 0U)
  {
    if (LPS22HB_Initialize(pObj) != LPS22HB_OK)
    {
      return LPS22HB_ERROR;
    }
  }

  pObj->is_initialized = 1U;

  return LPS22HB_OK;
}

/**
  * @brief  ����ʼ��LPS22HB������
  * @param  pObj �豸����
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_DeInit(LPS22HB_Object_t *pObj)
{
  if (pObj->is_initialized == 1U)
  {
    if (LPS22HB_PRESS_Disable(pObj) != LPS22HB_OK)
    {
      return LPS22HB_ERROR;
    }

    if (LPS22HB_TEMP_Disable(pObj) != LPS22HB_OK)
    {
      return LPS22HB_ERROR;
    }
  }

  pObj->is_initialized = 0;

  return LPS22HB_OK;
}

/**
  * @brief  ��ȡWHO_AM_Iֵ
  * @param  pObj �豸����
  * @param  Id WHO_AM_Iֵ
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_ReadID(LPS22HB_Object_t *pObj, uint8_t *Id)
{
  if (lps22hb_device_id_get(&(pObj->Ctx), Id) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ��ȡLPS22HB����������
  * @param  pObj �������ָ��
  * @param  Capabilities ָ��LPS22HB���������ܵ�ָ��
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_GetCapabilities(LPS22HB_Object_t *pObj, LPS22HB_Capabilities_t *Capabilities)
{
  /* ��ֹδʹ�ò������뾯�� */
  (void)(pObj);

  Capabilities->Humidity    = 0;
  Capabilities->Pressure    = 1;
  Capabilities->Temperature = 1;
  Capabilities->Gas         = 0;
  Capabilities->LowPower    = 0;
  Capabilities->HumMaxOdr   = 0.0f;
  Capabilities->TempMaxOdr  = 75.0f;
  Capabilities->PressMaxOdr = 75.0f;
  Capabilities->GasMaxOdr   = 0.0f;
  return LPS22HB_OK;
}

/**
  * @brief  ��ȡLPS22HB��ʼ��״̬
  * @param  pObj �豸����
  * @param  Status 1��ʾ�ѳ�ʼ����0��ʾδ��ʼ��
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_Get_Init_Status(LPS22HB_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return LPS22HB_ERROR;
  }

  *Status = pObj->is_initialized;

  return LPS22HB_OK;
}

/**
  * @brief  ����LPS22HBѹ��������
  * @param  pObj �豸����
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_PRESS_Enable(LPS22HB_Object_t *pObj)
{
  /* �������Ƿ������� */
  if (pObj->press_is_enabled == 1U)
  {
    return LPS22HB_OK;
  }

  /* ѡ������������� */
  if (lps22hb_data_rate_set(&(pObj->Ctx), pObj->last_odr) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  pObj->press_is_enabled = 1;

  return LPS22HB_OK;
}

/**
  * @brief  ����LPS22HBѹ��������
  * @param  pObj �豸����
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_PRESS_Disable(LPS22HB_Object_t *pObj)
{
  /* �������Ƿ��ѽ��� */
  if (pObj->press_is_enabled == 0U)
  {
    return LPS22HB_OK;
  }

  /* ���LPS22HB�¶ȴ������Ƿ������� */
  /* ����ǣ����������ú�����������ý��ú��� */
  if (pObj->temp_is_enabled == 0U)
  {
    /* ��ȡ��ǰ����������� */
    if (lps22hb_data_rate_get(&(pObj->Ctx), &pObj->last_odr) != LPS22HB_OK)
    {
      return LPS22HB_ERROR;
    }

    /* ѡ������������� - �ػ�ģʽ */
    if (lps22hb_data_rate_set(&(pObj->Ctx), LPS22HB_POWER_DOWN) != LPS22HB_OK)
    {
      return LPS22HB_ERROR;
    }
  }

  pObj->press_is_enabled = 0;

  return LPS22HB_OK;
}

/**
  * @brief  ��ȡLPS22HBѹ�������������������
  * @param  pObj �豸����
  * @param  Odr ָ��д������������ʵ�ָ��
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_PRESS_GetOutputDataRate(LPS22HB_Object_t *pObj, float *Odr)
{
  return LPS22HB_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  ����LPS22HBѹ�������������������
  * @param  pObj �豸����
  * @param  Odr Ҫ���õ������������ֵ
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_PRESS_SetOutputDataRate(LPS22HB_Object_t *pObj, float Odr)
{
  /* �������Ƿ������� */
  if (pObj->press_is_enabled == 1U)
  {
    return LPS22HB_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LPS22HB_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  ��ȡLPS22HBѹ��ֵ
  * @param  pObj �豸����
  * @param  Value ָ��д��ѹ��ֵ��ָ��
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_PRESS_GetPressure(LPS22HB_Object_t *pObj, float *Value)
{
  lps22hb_axis1bit32_t data_raw_pressure;

  (void)memset(data_raw_pressure.u8bit, 0x00, sizeof(int32_t));
  if (lps22hb_pressure_raw_get(&(pObj->Ctx), (uint32_t *)&data_raw_pressure.i32bit) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  *Value = lps22hb_from_lsb_to_hpa(data_raw_pressure.i32bit);
//    *Value =lps22hb_from_lsb_to_altitude(data_raw_pressure.i32bit);
  return LPS22HB_OK;
}

/**
  * @brief  ��ȡLPS22HBѹ������׼������λֵ
  * @param  pObj �豸����
  * @param  Status ����׼������λ��״̬
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_PRESS_Get_DRDY_Status(LPS22HB_Object_t *pObj, uint8_t *Status)
{
  if (lps22hb_press_data_ready_get(&(pObj->Ctx), Status) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ����LPS22HB�¶ȴ�����
  * @param  pObj �豸����
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_TEMP_Enable(LPS22HB_Object_t *pObj)
{
  /* �������Ƿ������� */
  if (pObj->temp_is_enabled == 1U)
  {
    return LPS22HB_OK;
  }

  /* ѡ������������� */
  if (lps22hb_data_rate_set(&(pObj->Ctx), pObj->last_odr) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  pObj->temp_is_enabled = 1;

  return LPS22HB_OK;
}

/**
  * @brief  ����LPS22HB�¶ȴ�����
  * @param  pObj �豸����
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_TEMP_Disable(LPS22HB_Object_t *pObj)
{
  /* �������Ƿ��ѽ��� */
  if (pObj->temp_is_enabled == 0U)
  {
    return LPS22HB_OK;
  }

  /* ���LPS22HBѹ���������Ƿ������� */
  /* ����ǣ����������ú�����������ý��ú��� */
  if (pObj->press_is_enabled == 0U)
  {
    /* ��ȡ��ǰ����������� */
    if (lps22hb_data_rate_get(&(pObj->Ctx), &pObj->last_odr) != LPS22HB_OK)
    {
      return LPS22HB_ERROR;
    }

    /* ѡ������������� - �ػ�ģʽ */
    if (lps22hb_data_rate_set(&(pObj->Ctx), LPS22HB_POWER_DOWN) != LPS22HB_OK)
    {
      return LPS22HB_ERROR;
    }
  }

  pObj->temp_is_enabled = 0;

  return LPS22HB_OK;
}

/**
  * @brief  ��ȡLPS22HB�¶ȴ����������������
  * @param  pObj �豸����
  * @param  Odr ָ��д������������ʵ�ָ��
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_TEMP_GetOutputDataRate(LPS22HB_Object_t *pObj, float *Odr)
{
  return LPS22HB_GetOutputDataRate(pObj, Odr);
}

/**
  * @brief  ����LPS22HB�¶ȴ����������������
  * @param  pObj �豸����
  * @param  Odr Ҫ���õ������������ֵ
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_TEMP_SetOutputDataRate(LPS22HB_Object_t *pObj, float Odr)
{
  /* �������Ƿ������� */
  if (pObj->temp_is_enabled == 1U)
  {
    return LPS22HB_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return LPS22HB_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
  * @brief  ��ȡLPS22HB�¶�ֵ
  * @param  pObj �豸����
  * @param  Value ָ��д���¶�ֵ��ָ��
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_TEMP_GetTemperature(LPS22HB_Object_t *pObj, float *Value)
{
  lps22hb_axis1bit16_t data_raw_temperature;

  (void)memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
  if (lps22hb_temperature_raw_get(&(pObj->Ctx), &data_raw_temperature.i16bit) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  *Value = lps22hb_from_lsb_to_degc(data_raw_temperature.i16bit);

  return LPS22HB_OK;
}

/**
  * @brief  ��ȡLPS22HB�¶�����׼������λֵ
  * @param  pObj �豸����
  * @param  Status ����׼������λ��״̬
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_TEMP_Get_DRDY_Status(LPS22HB_Object_t *pObj, uint8_t *Status)
{
  if (lps22hb_temp_data_ready_get(&(pObj->Ctx), Status) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ��ȡLPS22HB FIFO���ݼ���
  * @param  pObj �豸����
  * @param  Status ����׼������λ��״̬
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_FIFO_Get_Data(LPS22HB_Object_t *pObj, float *Press, float *Temp)
{
  if (LPS22HB_PRESS_GetPressure(pObj, Press) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  if (LPS22HB_Get_Temp(pObj, Temp) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ��ȡLPS22HB FIFO��ֵ
  * @param  pObj �豸����
  * @param  Status ����׼������λ��״̬
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_FIFO_Get_FTh_Status(LPS22HB_Object_t *pObj, uint8_t *Status)
{
  if (lps22hb_fifo_fth_flag_get(&(pObj->Ctx), Status) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ��ȡLPS22HB FIFO��״̬
  * @param  pObj �豸����
  * @param  Status ����׼������λ��״̬
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_FIFO_Get_Full_Status(LPS22HB_Object_t *pObj, uint8_t *Status)
{
  if (lps22hb_fifo_data_level_get(&(pObj->Ctx), Status) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  if (*Status == LPS22HB_FIFO_FULL)
  {
    *Status = (uint8_t) 1;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ��ȡLPS22HB FIFO���ݼ���
  * @param  pObj �豸����
  * @param  Status ����׼������λ��״̬
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_FIFO_Get_Level(LPS22HB_Object_t *pObj, uint8_t *Status)
{
  if (lps22hb_fifo_data_level_get(&(pObj->Ctx), Status) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ��ȡLPS22HB FIFO���״̬
  * @param  pObj �豸����
  * @param  Status ����׼������λ��״̬
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_FIFO_Get_Ovr_Status(LPS22HB_Object_t *pObj, uint8_t *Status)
{
  if (lps22hb_fifo_ovr_flag_get(&(pObj->Ctx), Status) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ��λFIFO�ж�
  * @param  pObj �豸����
  * @param  interrupt Ҫ��λ��FIFO�жϣ�ֵ��0 = FTH��1 = FULL��2 = OVR
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_FIFO_Reset_Interrupt(LPS22HB_Object_t *pObj, uint8_t interrupt)
{
  switch (interrupt)
  {
    case 0:
      if (lps22hb_fifo_threshold_on_int_set(&(pObj->Ctx), PROPERTY_DISABLE) != LPS22HB_OK)
      {
        return LPS22HB_ERROR;
      }
      break;
    case 1:
      if (lps22hb_fifo_full_on_int_set(&(pObj->Ctx), PROPERTY_DISABLE) != LPS22HB_OK)
      {
        return LPS22HB_ERROR;
      }
      break;
    case 2:
      if (lps22hb_fifo_ovr_on_int_set(&(pObj->Ctx), PROPERTY_DISABLE) != LPS22HB_OK)
      {
        return LPS22HB_ERROR;
      }
      break;
    default:
      return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ����FIFO�ж�
  * @param  pObj �豸����
  * @param  interrupt Ҫ��λ��FIFO�жϣ�ֵ��0 = FTH��1 = FULL��2 = OVR
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_FIFO_Set_Interrupt(LPS22HB_Object_t *pObj, uint8_t interrupt)
{
  switch (interrupt)
  {
    case 0:
      if (lps22hb_fifo_threshold_on_int_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS22HB_OK)
      {
        return LPS22HB_ERROR;
      }
      break;
    case 1:
      if (lps22hb_fifo_full_on_int_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS22HB_OK)
      {
        return LPS22HB_ERROR;
      }
      break;
    case 2:
      if (lps22hb_fifo_ovr_on_int_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS22HB_OK)
      {
        return LPS22HB_ERROR;
      }
      break;
    default:
      return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ����FIFOģʽ
  * @param  pObj �豸����
  * @param  Mode Ҫ���õ�FIFOģʽ
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_FIFO_Set_Mode(LPS22HB_Object_t *pObj, uint8_t Mode)
{

  /* ��֤���ݵĲ���������Чֵ֮һ */
  switch ((lps22hb_f_mode_t)Mode)
  {
    case LPS22HB_BYPASS_MODE:
    case LPS22HB_FIFO_MODE:
    case LPS22HB_STREAM_MODE:
    case LPS22HB_STREAM_TO_FIFO_MODE:
    case LPS22HB_BYPASS_TO_STREAM_MODE:
    case LPS22HB_BYPASS_TO_FIFO_MODE:
      break;
    default:
      return LPS22HB_ERROR;
  }

  if (lps22hb_fifo_mode_set(&(pObj->Ctx), (lps22hb_f_mode_t)Mode) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ����LPS22HB FIFO���ݼ���
  * @param  pObj �豸����
  * @param  Status ����׼������λ��״̬
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_FIFO_Set_Watermark_Level(LPS22HB_Object_t *pObj, uint8_t Watermark)
{
  if (lps22hb_fifo_watermark_set(&(pObj->Ctx), Watermark) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ����FIFOʹ��
  * @param  pObj �豸����
  * @param  Mode Ҫ���õ�FIFOģʽ
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_FIFO_Usage(LPS22HB_Object_t *pObj, uint8_t Status)
{

  if (lps22hb_fifo_set(&(pObj->Ctx), Status) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ��ȡLPS22HB�Ĵ���ֵ
  * @param  pObj �豸����
  * @param  Reg Ҫд��ĵ�ַ
  * @param  Data Ҫд���ֵ
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_Read_Reg(LPS22HB_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (lps22hb_read_reg(&(pObj->Ctx), Reg, Data, 1) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ����LPS22HB�Ĵ���ֵ
  * @param  pObj �豸����
  * @param  Reg Ҫд��ĵ�ַ
  * @param  Data Ҫд���ֵ
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_Write_Reg(LPS22HB_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (lps22hb_write_reg(&(pObj->Ctx), Reg, &Data, 1) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @}
  */

/** @defgroup LPS22HB_Private_Functions LPS22HB˽�к���
  * @{
  */

/**
  * @brief  ��ȡLPS22HB FIFO�¶�����
  * @param  pObj �豸����
  * @param  Status ����׼������λ��״̬
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_Get_Temp(LPS22HB_Object_t *pObj, float *Data)
{
  lps22hb_axis1bit16_t data_raw_temperature;

  (void)memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
  if (lps22hb_temperature_raw_get(&(pObj->Ctx), &data_raw_temperature.i16bit) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  *Data = lps22hb_from_lsb_to_degc(data_raw_temperature.i16bit);

  return LPS22HB_OK;
}

/**
  * @brief  ��ȡ�����������
  * @param  pObj �豸����
  * @param  Odr �����������ֵ
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
static int32_t LPS22HB_GetOutputDataRate(LPS22HB_Object_t *pObj, float *Odr)
{
  int32_t ret = LPS22HB_OK;
  lps22hb_odr_t odr_low_level;

  if (lps22hb_data_rate_get(&(pObj->Ctx), &odr_low_level) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  switch (odr_low_level)
  {
    case LPS22HB_POWER_DOWN:
      *Odr = 0.0f;
      break;

    case LPS22HB_ODR_1_Hz:
      *Odr = 1.0f;
      break;

    case LPS22HB_ODR_10_Hz:
      *Odr = 10.0f;
      break;

    case LPS22HB_ODR_25_Hz:
      *Odr = 25.0f;
      break;

    case LPS22HB_ODR_50_Hz:
      *Odr = 50.0f;
      break;

    case LPS22HB_ODR_75_Hz:
      *Odr = 75.0f;
      break;

    default:
      ret = LPS22HB_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  ���������������
  * @param  pObj �豸����
  * @param  Odr Ҫ���õ������������ֵ
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
static int32_t LPS22HB_SetOutputDataRate_When_Enabled(LPS22HB_Object_t *pObj, float Odr)
{
  lps22hb_odr_t new_odr;

  new_odr = (Odr <=  1.0f) ? LPS22HB_ODR_1_Hz
            : (Odr <= 10.0f) ? LPS22HB_ODR_10_Hz
            : (Odr <= 25.0f) ? LPS22HB_ODR_25_Hz
            : (Odr <= 50.0f) ? LPS22HB_ODR_50_Hz
            :                  LPS22HB_ODR_75_Hz;

  if (lps22hb_data_rate_set(&(pObj->Ctx), new_odr) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  if (lps22hb_data_rate_get(&(pObj->Ctx), &pObj->last_odr) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ����ʱ���������������
  * @param  pObj �豸����
  * @param  Odr Ҫ���õ������������ֵ
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
static int32_t LPS22HB_SetOutputDataRate_When_Disabled(LPS22HB_Object_t *pObj, float Odr)
{
  pObj->last_odr = (Odr <=  1.0f) ? LPS22HB_ODR_1_Hz
                   : (Odr <= 10.0f) ? LPS22HB_ODR_10_Hz
                   : (Odr <= 25.0f) ? LPS22HB_ODR_25_Hz
                   : (Odr <= 50.0f) ? LPS22HB_ODR_50_Hz
                   :                  LPS22HB_ODR_75_Hz;

  return LPS22HB_OK;
}

/**
  * @brief  ��ʼ��LPS22HB������
  * @param  pObj �豸����
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
static int32_t LPS22HB_Initialize(LPS22HB_Object_t *pObj)
{
  /* ���õ�Դģʽ */
  if (lps22hb_low_power_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  /* �ر��豸 */
  if (lps22hb_data_rate_set(&(pObj->Ctx), LPS22HB_POWER_DOWN) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  /* ��LPS22HBѹ�������Ͻ��õ�ͨ�˲��� */
  if (lps22hb_low_pass_filter_mode_set(&(pObj->Ctx), LPS22HB_LPF_ODR_DIV_20) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  if (lps22hb_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  if (pObj->IO.BusType == LPS22HB_I2C_BUS) /* I2C */
  {
    if (lps22hb_auto_add_inc_set(&(pObj->Ctx), PROPERTY_DISABLE) != LPS22HB_OK)
    {
      return LPS22HB_ERROR;
    }
  }
  else /* SPI 4���ƻ�SPI 3���� */
  {
    if (lps22hb_auto_add_inc_set(&(pObj->Ctx), PROPERTY_ENABLE) != LPS22HB_OK)
    {
      return LPS22HB_ERROR;
    }
  }

  pObj->last_odr = LPS22HB_ODR_25_Hz;

  return LPS22HB_OK;
}

/**
  * @brief  ����LPS22HB���β���ģʽ
  * @param  pObj �豸����
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
int32_t LPS22HB_Set_One_Shot(LPS22HB_Object_t *pObj)
{
  /* ����ODR */
  if (lps22hb_data_rate_set(&(pObj->Ctx), LPS22HB_POWER_DOWN) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  /* �������β��� */
  if (lps22hb_one_shoot_trigger_set(&(pObj->Ctx), 1) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  return LPS22HB_OK;
}

/**
  * @brief  ��ȡ LPS22HB ���δ���״̬
  * @param  pObj �豸����
  * @param  Status ָ�򵥴δ���״̬��ָ�루1 ��ʾ�������ã�0 ��ʾ�����в����ã�
  * @retval 0 ��ʾ�ɹ�������Ϊ�������
  */
int32_t LPS22HB_Get_One_Shot_Status(LPS22HB_Object_t *pObj, uint8_t *Status)
{
  uint8_t p_da;
  uint8_t t_da;

  /* ��ȡѹ�����ݾ���״̬ */
  if (lps22hb_press_data_ready_get(&(pObj->Ctx), &p_da) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  /* ��ȡ�¶����ݾ���״̬ */
  if (lps22hb_temp_data_ready_get(&(pObj->Ctx), &t_da) != LPS22HB_OK)
  {
    return LPS22HB_ERROR;
  }

  if (p_da && t_da)
  {
    *Status = 1;
  }
  else
  {
    *Status = 0;
  }

  return LPS22HB_OK;
}
/**
  * @brief  ����ȡ�Ĵ������������װΪ���� IO ����
  * @param  Handle �豸���
  * @param  Reg �Ĵ�����ַ
  * @param  pData �洢����ָ��
  * @param  Length ����
  * @retval 0 ��ʾ�ɹ�������Ϊ�������
  */
static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  uint16_t i;
  int32_t ret = LPS22HB_OK;
  LPS22HB_Object_t *pObj = (LPS22HB_Object_t *)Handle;

  if (pObj->IO.BusType == LPS22HB_I2C_BUS) /* I2C */
  {
    for (i = 0; i < Length; i++)
    {
      ret = pObj->IO.ReadReg(pObj->IO.Address, (Reg + i), &pData[i], 1);
      if (ret != LPS22HB_OK)
      {
        return LPS22HB_ERROR;
      }
    }

    return ret;
  }
  else /* SPI 4���ƻ�SPI 3���� */
  {
    return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
  }
}

/**
  * @brief  ��д�Ĵ������������װΪ����IO����
  * @param  Handle �豸���
  * @param  Reg �Ĵ�����ַ
  * @param  pData �洢����ָ��
  * @param  Length ����
  * @retval 0��ʾ�ɹ������򷵻ش������
  */
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  uint16_t i;
  int32_t ret = LPS22HB_OK;
  LPS22HB_Object_t *pObj = (LPS22HB_Object_t *)Handle;

  if (pObj->IO.BusType == LPS22HB_I2C_BUS) /* I2C */
  {
    for (i = 0; i < Length; i++)
    {
      ret = pObj->IO.WriteReg(pObj->IO.Address, (Reg + i), &pData[i], 1);
      if (ret != LPS22HB_OK)
      {
        return LPS22HB_ERROR;
      }
    }

    return ret;
  }
  else /* SPI 4���ƻ�SPI 3���� */
  {
    return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */