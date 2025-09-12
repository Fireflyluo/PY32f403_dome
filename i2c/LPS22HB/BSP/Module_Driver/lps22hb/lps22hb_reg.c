/**
  ******************************************************************************
  * @file    lps22hb_reg.c
  * @author  Sensors Software Solution Team
  * @brief   LPS22HB �����ļ�
  ******************************************************************************
  * @attention
  *
  * ��Ȩ���� (c) 2021 STMicroelectronics��
  * ��������Ȩ����
  *
  * ��������ݿ����� LICENSE �ļ����ҵ�����������ɣ�
  * ���ļ�λ�ڴ��������ĸ�Ŀ¼�¡�
  * ���û���������ṩ�� LICENSE �ļ�����ԭ���ṩ��
  *
  ******************************************************************************
  */

#include "lps22hb_reg.h"

/**
  * @defgroup    LPS22HB
  * @brief       ���ļ��ṩ��һ������������ѹ��ʽ����ѹ������������ĺ�����
  * @{
  *
  */

/**
  * @defgroup    LPS22HB_�ӿں���
  * @brief       �����ṩ��һ�����ڶ�д�豸ͨ�üĴ����ĺ�����
  * @{
  *
  */

/**
  * @brief  ��ȡͨ���豸�Ĵ���
  *
  * @param  ctx   ��/д�ӿڶ���ָ��
  * @param  reg   Ҫ��ȡ�ļĴ���
  * @param  data  ָ��洢��ȡ���ݵĻ�����ָ��
  * @param  len   Ҫ������ȡ�ļĴ�������
  * @retval       �ӿ�״̬ (���뷵�� 0 -> �޴���)
  *
  */
int32_t __weak lps22hb_read_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                                uint8_t *data,
                                uint16_t len)
{
  int32_t ret;

  // ���������ָ���Ƿ�Ϊ��
  if (ctx == NULL)
  {
    return -1;
  }

  // ���õײ��ȡ����
  ret = ctx->read_reg(ctx->handle, reg, data, len);

  return ret;
}

/**
  * @brief  д��ͨ���豸�Ĵ���
  *
  * @param  ctx   ��/д�ӿڶ���ָ��
  * @param  reg   Ҫд��ļĴ���
  * @param  data  ָ��Ҫд��Ĵ���������ָ��
  * @param  len   Ҫ����д��ļĴ�������
  * @retval       �ӿ�״̬ (���뷵�� 0 -> �޴���)
  *
  */
int32_t __weak lps22hb_write_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                                 uint8_t *data,
                                 uint16_t len)
{
  int32_t ret;

  // ���������ָ���Ƿ�Ϊ��
  if (ctx == NULL)
  {
    return -1;
  }

  // ���õײ�д�뺯��
  ret = ctx->write_reg(ctx->handle, reg, data, len);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS22HB_������
  * @brief       ��Щ������ԭʼ����ת��Ϊ���̵�λ��
  * @{
  *
  */

/**
  * @brief  ��ԭʼֵת��Ϊ���� (hPa)
  *
  * @param  lsb   ԭʼֵ
  * @retval       ת�����ѹ��ֵ (hPa)
  */
float_t lps22hb_from_lsb_to_hpa(int32_t lsb)
{
  return ((float_t)lsb / 4096.0f);
}

/**
  * @brief  ��ԭʼֵת��Ϊǧ�� (kPa)
  *
  * @param  lsb   ԭʼֵ
  * @retval       ת�����ѹ��ֵ (kPa)
  */
float_t lps22hb_from_lsb_to_kpa(int32_t lsb)
{
  return lps22hb_from_lsb_to_hpa(lsb) / 10.0f;
}

/**
  * @brief  ��ԭʼֵת��Ϊ��/ƽ��Ӣ�� (psi)
  *
  * @param  lsb   ԭʼֵ
  * @retval       ת�����ѹ��ֵ (psi)
  */
float_t lps22hb_from_lsb_to_psi(int32_t lsb)
{
  return lps22hb_from_lsb_to_hpa(lsb) * 0.0145038f;
}

/**
  * @brief  ��ԭʼֵת��Ϊ���θ߶� (��)
  *
  * @param  lsb   ԭʼֵ
  * @retval       ת����ĺ��θ߶� (��)
  */
float_t lps22hb_from_lsb_to_altitude(int32_t lsb)
{
  float_t atmospheric = lps22hb_from_lsb_to_hpa(lsb);
  // ���θ߶ȿ���ͨ��������ѹ��ʽ���㡣
  // ƽ����ƽ��ѹ��Ϊ 1013.25 hPa��
  return 44330.0 * (1.0 - pow(atmospheric / 1013.25f, (1.0 / 5.255)));
}

/**
  * @brief  ��ԭʼֵת��Ϊ���϶� (��C)
  *
  * @param  lsb   ԭʼֵ
  * @retval       ת������¶�ֵ (��C)
  */
float_t lps22hb_from_lsb_to_degc(int16_t lsb)
{
  return ((float_t)lsb / 100.0f);
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS22HB_��������_c
  * @brief       ���ڽ�����������������صĺ�������
  * @{
  *
  */


/**
  * @brief  �����Զ����㹦�ܡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� reset_az ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_autozero_rst_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.reset_az = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  �����Զ����㹦�ܡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� reset_az ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_autozero_rst_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.reset_az;

  return ret;
}

/**
  * @brief  �����Զ����㹦�ܡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� autozero ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_autozero_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.autozero = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  �����Զ����㹦�ܡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� autozero ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_autozero_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.autozero;

  return ret;
}

/**
  * @brief  �����Զ��ο�ѹ�����ܡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� reset_arp ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_pressure_snap_rst_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.reset_arp = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  �����Զ��ο�ѹ�����ܡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� reset_arp ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_pressure_snap_rst_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.reset_arp;

  return ret;
}

/**
  * @brief  �����Զ��ο�ѹ�����ܡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� autorifp ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_pressure_snap_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.autorifp = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  �����Զ��ο�ѹ�����ܡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� autorifp ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_pressure_snap_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.autorifp;

  return ret;
}

/**
  * @brief  �����ݸ��¡�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG1 �Ĵ����� bdu ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_block_data_update_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.bdu = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief  �����ݸ��¡�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG1 �Ĵ����� bdu ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_block_data_update_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  *val = ctrl_reg1.bdu;

  return ret;
}

/**
  * @brief  ��ͨ����ѡ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG1 �Ĵ����� lpfp ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_low_pass_filter_mode_set(const stmdev_ctx_t *ctx,
                                         lps22hb_lpfp_t val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.lpfp = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief  ��ͨ����ѡ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG1 �Ĵ����� lpfp ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_low_pass_filter_mode_get(const stmdev_ctx_t *ctx,
                                         lps22hb_lpfp_t *val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  switch (ctrl_reg1.lpfp)
  {
    case LPS22HB_LPF_ODR_DIV_2:
      *val = LPS22HB_LPF_ODR_DIV_2;
      break;

    case LPS22HB_LPF_ODR_DIV_9:
      *val = LPS22HB_LPF_ODR_DIV_9;
      break;

    case LPS22HB_LPF_ODR_DIV_20:
      *val = LPS22HB_LPF_ODR_DIV_20;
      break;

    default:
      *val = LPS22HB_LPF_ODR_DIV_2;
      break;
  }

  return ret;
}

/**
  * @brief  �����������ѡ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG1 �Ĵ����� odr ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_data_rate_set(const stmdev_ctx_t *ctx, lps22hb_odr_t val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.odr = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief  �����������ѡ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG1 �Ĵ����� odr ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_data_rate_get(const stmdev_ctx_t *ctx, lps22hb_odr_t *val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  switch (ctrl_reg1.odr)
  {
    case LPS22HB_POWER_DOWN:
      *val = LPS22HB_POWER_DOWN;
      break;

    case LPS22HB_ODR_1_Hz:
      *val = LPS22HB_ODR_1_Hz;
      break;

    case LPS22HB_ODR_10_Hz:
      *val = LPS22HB_ODR_10_Hz;
      break;

    case LPS22HB_ODR_25_Hz:
      *val = LPS22HB_ODR_25_Hz;
      break;

    case LPS22HB_ODR_50_Hz:
      *val = LPS22HB_ODR_50_Hz;
      break;

    case LPS22HB_ODR_75_Hz:
      *val = LPS22HB_ODR_75_Hz;
      break;

    default:
      *val = LPS22HB_ODR_1_Hz;
      break;
  }

  return ret;
}

/**
  * @brief  ���δ���ģʽ���豸ִ��һ�β�����[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� one_shot ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_one_shoot_trigger_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.one_shot = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  ���δ���ģʽ���豸ִ��һ�β�����[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� one_shot ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_one_shoot_trigger_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.one_shot;

  return ret;
}

/**
  * @brief  pressure_ref: �ο�ѹ��ֵ��һ�� 24 λ���ݣ�
  *         �Զ����Ʋ�����ʽ��ʾ�������� AUTOZERO �� AUTORIFP ����ʱʹ�ø�ֵ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    Ҫд�������
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_pressure_ref_set(const stmdev_ctx_t *ctx, int32_t val)
{
  uint8_t buff[3];
  int32_t ret;

  buff[2] = (uint8_t)((uint32_t)val / 65536U);
  buff[1] = (uint8_t)((uint32_t)val - (buff[2] * 65536U)) / 256U;
  buff[0] = (uint8_t)((uint32_t)val - (buff[2] * 65536U) -
                      (buff[1] * 256U));
  ret =  lps22hb_write_reg(ctx, LPS22HB_REF_P_XL, buff, 3);

  return ret;
}

/**
  * @brief  pressure_ref: �ο�ѹ��ֵ��һ�� 24 λ���ݣ�
  *         �Զ����Ʋ�����ʽ��ʾ�������� AUTOZERO �� AUTORIFP ����ʱʹ�ø�ֵ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_pressure_ref_get(const stmdev_ctx_t *ctx, int32_t *val)
{
  uint8_t buff[3];
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_REF_P_XL, buff, 3);
  *val = (int32_t)buff[2];
  *val = (*val * 256) + (int32_t)buff[1];
  *val = (*val * 256) + (int32_t)buff[0];

  return ret;
}

/**
  * @brief  ѹ��ƫ��ֵ�� 16 λ���ݣ��������ں��Ӻ�ʵ�ֵ���У׼ (OPC)��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    Ҫд�������
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_pressure_offset_set(const stmdev_ctx_t *ctx, int16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)((uint16_t)val / 256U);
  buff[0] = (uint8_t)((uint16_t)val - (buff[1] * 256U));
  ret =  lps22hb_write_reg(ctx, LPS22HB_RPDS_L, buff, 2);

  return ret;
}

/**
  * @brief  ѹ��ƫ��ֵ�� 16 λ���ݣ��������ں��Ӻ�ʵ�ֵ���У׼ (OPC)��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_pressure_offset_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_RPDS_L, buff, 2);
  *val = (int16_t)buff[1];
  *val = (*val * 256) + (int16_t)buff[0];

  return ret;
}

/**
  * @brief  ѹ�����ݿ��á�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� STATUS �Ĵ����� p_da ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_press_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_status_t status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_STATUS, (uint8_t *)&status, 1);
  *val = status.p_da;

  return ret;
}

/**
  * @brief  �¶����ݿ��á�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� STATUS �Ĵ����� t_da ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_temp_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_status_t status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_STATUS, (uint8_t *)&status, 1);
  *val = status.t_da;

  return ret;
}

/**
  * @brief  ѹ�����¶����ݿ��á�[��ȡ]
  *
  * @param  ctx          ��/д�ӿڶ���
  * @param  press_val    �ı� STATUS �Ĵ����� p_da ��ֵ
  * @param  temp_val     �ı� STATUS �Ĵ����� t_da ��ֵ
  * @retval              �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *press_val, uint8_t *temp_val)
{
  lps22hb_status_t status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_STATUS, (uint8_t *)&status, 1);
  *press_val = status.p_da;
  *temp_val = status.t_da;

  return ret;
}

/**
  * @brief  ѹ�����������[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� STATUS �Ĵ����� p_or ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_press_data_ovr_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_status_t status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_STATUS, (uint8_t *)&status, 1);
  *val = status.p_or;

  return ret;
}

/**
  * @brief  �¶����������[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� STATUS �Ĵ����� t_or ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_temp_data_ovr_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_status_t status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_STATUS, (uint8_t *)&status, 1);
  *val = status.t_or;

  return ret;
}

/**
  * @brief  ѹ�����ֵ[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  buff   �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_pressure_raw_get(const stmdev_ctx_t *ctx, uint32_t *buff)
{
  uint8_t reg[3];
  int32_t ret,raw_pressure;

  ret =  lps22hb_read_reg(ctx, LPS22HB_PRESS_OUT_XL, reg, 3);
  
// ��ȷ���24λ���� (С�˸�ʽ)
    // reg[2] = PRESS_OUT_H (MSB)
    // reg[1] = PRESS_OUT_L
    // reg[0] = PRESS_OUT_XL (LSB)
    raw_pressure = (int32_t)reg[2];       // ��ȡ����ֽ�
    raw_pressure = raw_pressure << 8;     // ����8λ
    raw_pressure |= (int32_t)reg[1];       // �����м��ֽ�
    raw_pressure = raw_pressure << 8;     // ������8λ
    raw_pressure |= (int32_t)reg[0];       // ��������ֽ�

    // ����24λ�з�������������չ��
    if (raw_pressure & 0x00800000) {
        raw_pressure |= 0xFF000000;  // ���ø�8λΪ1��������
    }

    *buff = (uint32_t)raw_pressure;  // ����Ϊ32λֵ
//  *buff = reg[2];
//  *buff = (*buff * 256) + reg[1];
//  *buff = (*buff * 256) + reg[0];
//  *buff *= 256;

  return ret;
}

/**
  * @brief  temperature_raw: �¶����ֵ[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  buff   �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_temperature_raw_get(const stmdev_ctx_t *ctx, int16_t *buff)
{
  uint8_t reg[2];
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_TEMP_OUT_L, (uint8_t *) reg, 2);
  *buff = reg[1];
  *buff = (*buff * 256) + reg[0];

  return ret;
}

/**
  * @defgroup    LPS22HB_FIFO_�������
  * @brief       ��Щ������ FIFO �������ת��Ϊ������
  *              (ѹ��, �¶�) �ֶΡ�
  * @{
  *
  */

/**
  * @brief  �� FIFO �������ת��Ϊԭʼѹ��ֵ
  *
  * @param  val    FIFO �������
  * @retval        ԭʼѹ��ֵ
  */
int32_t lps22hb_fifo_output_data_to_raw_pressure(lps22hb_fifo_output_data_t *val)
{
  int32_t pressure = val->bytes[2];
  pressure = (pressure * 256) + val->bytes[1];
  return (pressure * 256) + val->bytes[0];
}

/**
  * @brief  �� FIFO �������ת��Ϊԭʼ�¶�ֵ
  *
  * @param  val    FIFO �������
  * @retval        ԭʼ�¶�ֵ
  */
int16_t lps22hb_fifo_output_data_to_raw_temperature(lps22hb_fifo_output_data_t *val)
{
  int16_t temperature = val->bytes[4];
  return (temperature * 256) + val->bytes[3];
}

/**
  * @}
  *
  */

/**
  * @brief  ͻ����ȡ FIFO ������ݡ�
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  buff   �洢��ȡ���ݵĻ�����
  * @param  len    Ҫ�� FIFO ��ȡ��ѹ��-�¶ȶԵ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_output_data_burst_get(const stmdev_ctx_t *ctx,
                                           lps22hb_fifo_output_data_t *buff, uint8_t len)
{
  if (len > 32)
  {
    len = 32;
  }
  return lps22hb_read_reg(ctx, LPS22HB_PRESS_OUT_XL, (uint8_t *)&buff[0],
                          len * sizeof(lps22hb_fifo_output_data_t));
}

/**
  * @brief  ��ͨ�˲�����λ�Ĵ�������� LPFP ���ڻ״̬��
  *         Ϊ�˱���˲̬�׶Σ�����ͨ��������ѹ������֮ǰ��ȡ�˼Ĵ�������λ�˲�����[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  buff   �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_low_pass_rst_get(const stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_LPFP_RES, (uint8_t *) buff, 1);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS22HB_ͨ��
  * @brief       ���ڽ�ͨ�õ����ú�������
  * @{
  *
  */

/**
  * @brief  �豸��ʶ[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  buff   �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_device_id_get(const stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_WHO_AM_I, (uint8_t *) buff, 1);

  return ret;
}

/**
  * @brief  �����λ���ָ��û��Ĵ����е�Ĭ��ֵ[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� swreset ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_reset_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.swreset = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  �����λ���ָ��û��Ĵ����е�Ĭ��ֵ[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� swreset ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_reset_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.swreset;

  return ret;
}

/**
  * @brief  �����ڴ����ݡ����¼���У׼������[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� boot ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_boot_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.boot = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  �����ڴ����ݡ����¼���У׼������[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� boot ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_boot_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.boot;

  return ret;
}

/**
  * @brief  �͵���ģʽ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� RES_CONF �Ĵ����� lc_en ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_low_power_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_res_conf_t res_conf;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_RES_CONF, (uint8_t *)&res_conf, 1);

  if (ret == 0)
  {
    res_conf.lc_en = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_RES_CONF, (uint8_t *)&res_conf, 1);
  }

  return ret;
}

/**
  * @brief  �͵���ģʽ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� RES_CONF �Ĵ����� lc_en ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_low_power_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_res_conf_t res_conf;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_RES_CONF, (uint8_t *)&res_conf, 1);
  *val = res_conf.lc_en;

  return ret;
}

/**
  * @brief  ���Ϊ '1' ��ʾ������������ (����) �׶Ρ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INT_SOURCE �Ĵ����� boot_status ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_boot_status_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_int_source_t int_source;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INT_SOURCE,
                         (uint8_t *)&int_source, 1);
  *val = int_source.boot_status;

  return ret;
}

/**
  * @brief  ����״̬λ��FIFO ����������[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �� FIFO_STATUS �� STATUS �Ĵ����Ľṹ��
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_dev_status_get(const stmdev_ctx_t *ctx,
                               lps22hb_dev_stat_t *val)
{
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_FIFO_STATUS, (uint8_t *) val, 2);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS22HB_�ж�
  * @brief       ���ڽ������жϵ����к�������
  * @{
  *
  */

/**
  * @brief  ��ѹ����/���¼��������ж����ɡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� pe ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_sign_of_int_threshold_set(const stmdev_ctx_t *ctx,
                                          lps22hb_pe_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.pe = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  ��ѹ����/���¼��������ж����ɡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ INTERRUPT_CFG �Ĵ����� pe ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_sign_of_int_threshold_get(const stmdev_ctx_t *ctx,
                                          lps22hb_pe_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  switch (interrupt_cfg.pe)
  {
    case LPS22HB_NO_THRESHOLD:
      *val = LPS22HB_NO_THRESHOLD;
      break;

    case LPS22HB_POSITIVE:
      *val = LPS22HB_POSITIVE;
      break;

    case LPS22HB_NEGATIVE:
      *val = LPS22HB_NEGATIVE;
      break;

    case LPS22HB_BOTH:
      *val = LPS22HB_BOTH;
      break;

    default:
      *val = LPS22HB_NO_THRESHOLD;
      break;
  }

  return ret;
}

/**
  * @brief  �� INT_SOURCE (25h) �Ĵ��������ж�
  *         ģʽ (���� / ����) [����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� lir ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_int_notification_mode_set(const stmdev_ctx_t *ctx,
                                          lps22hb_lir_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.lir = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  �� INT_SOURCE (25h) �Ĵ��������ж�
  *         ģʽ (���� / ����) [��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ INTERRUPT_CFG �Ĵ����� lir ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_int_notification_mode_get(const stmdev_ctx_t *ctx,
                                          lps22hb_lir_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  switch (interrupt_cfg.lir)
  {
    case LPS22HB_INT_PULSED:
      *val = LPS22HB_INT_PULSED;
      break;

    case LPS22HB_INT_LATCHED:
      *val = LPS22HB_INT_LATCHED;
      break;

    default:
      *val = LPS22HB_INT_PULSED;
      break;
  }

  return ret;
}

/**
  * @brief  �����ж����ɡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� diff_en ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_int_generation_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.diff_en = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  �����ж����ɡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� diff_en ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_int_generation_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.diff_en;

  return ret;
}

/**
  * @brief  �û������ѹ���ж��¼���ֵֵ[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    Ҫд�������
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_int_threshold_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));
  ret =  lps22hb_write_reg(ctx, LPS22HB_THS_P_L, (uint8_t *) buff, 2);

  return ret;
}

/**
  * @brief  �û������ѹ���ж��¼���ֵֵ[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_int_threshold_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_THS_P_L, (uint8_t *) buff, 2);
  *val = buff[1];
  *val = (*val * 256) + buff[0];

  return ret;
}

/**
  * @brief  INT_DRDY �����ϵ������źſ���λ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� int_s ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_int_pin_mode_set(const stmdev_ctx_t *ctx,
                                 lps22hb_int_s_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.int_s = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  INT_DRDY �����ϵ������źſ���λ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG3 �Ĵ����� int_s ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_int_pin_mode_get(const stmdev_ctx_t *ctx,
                                 lps22hb_int_s_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  switch (ctrl_reg3.int_s)
  {
    case LPS22HB_DRDY_OR_FIFO_FLAGS:
      *val = LPS22HB_DRDY_OR_FIFO_FLAGS;
      break;

    case LPS22HB_HIGH_PRES_INT:
      *val = LPS22HB_HIGH_PRES_INT;
      break;

    case LPS22HB_LOW_PRES_INT:
      *val = LPS22HB_LOW_PRES_INT;
      break;

    case LPS22HB_EVERY_PRES_INT:
      *val = LPS22HB_EVERY_PRES_INT;
      break;

    default:
      *val = LPS22HB_DRDY_OR_FIFO_FLAGS;
      break;
  }

  return ret;
}

/**
  * @brief  INT_DRDY �����ϵ����ݾ����źš�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� drdy ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_drdy_on_int_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.drdy = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  INT_DRDY �����ϵ����ݾ����źš�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� drdy ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_drdy_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  *val = ctrl_reg3.drdy;

  return ret;
}

/**
  * @brief  FIFO ����ж��� INT_DRDY �����ϡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� f_ovr ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_ovr_on_int_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.f_ovr = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  FIFO ����ж��� INT_DRDY �����ϡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� f_ovr ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_ovr_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  *val = ctrl_reg3.f_ovr;

  return ret;
}

/**
  * @brief  FIFO ˮλ״̬�� INT_DRDY �����ϡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� f_fth ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_threshold_on_int_set(const stmdev_ctx_t *ctx,
                                          uint8_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.f_fth = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  FIFO ˮλ״̬�� INT_DRDY �����ϡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� f_fth ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_threshold_on_int_get(const stmdev_ctx_t *ctx,
                                          uint8_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  *val = ctrl_reg3.f_fth;

  return ret;
}

/**
  * @brief  FIFO ����־�� INT_DRDY �����ϡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� f_fss5 ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_full_on_int_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.f_fss5 = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  FIFO ����־�� INT_DRDY �����ϡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� f_fss5 ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_full_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  *val = ctrl_reg3.f_fss5;

  return ret;
}

/**
  * @brief  �ж������ϵ�����/��©ѡ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� pp_od ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_pin_mode_set(const stmdev_ctx_t *ctx, lps22hb_pp_od_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.pp_od = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  �ж������ϵ�����/��©ѡ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG3 �Ĵ����� pp_od ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_pin_mode_get(const stmdev_ctx_t *ctx, lps22hb_pp_od_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  switch (ctrl_reg3.pp_od)
  {
    case LPS22HB_PUSH_PULL:
      *val = LPS22HB_PUSH_PULL;
      break;

    case LPS22HB_OPEN_DRAIN:
      *val = LPS22HB_OPEN_DRAIN;
      break;

    default:
      *val = LPS22HB_PUSH_PULL;
      break;
  }

  return ret;
}

/**
  * @brief  �жϸߵ�ƽ/�͵�ƽ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� int_h_l ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_int_polarity_set(const stmdev_ctx_t *ctx,
                                 lps22hb_int_h_l_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.int_h_l = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  �жϸߵ�ƽ/�͵�ƽ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG3 �Ĵ����� int_h_l ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_int_polarity_get(const stmdev_ctx_t *ctx,
                                 lps22hb_int_h_l_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  switch (ctrl_reg3.int_h_l)
  {
    case LPS22HB_ACTIVE_HIGH:
      *val = LPS22HB_ACTIVE_HIGH;
      break;

    case LPS22HB_ACTIVE_LOW:
      *val = LPS22HB_ACTIVE_LOW;
      break;

    default:
      *val = LPS22HB_ACTIVE_HIGH;
      break;
  }

  return ret;
}

/**
  * @brief  �ж�Դ�Ĵ���[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    INT_SOURCE �Ĵ���
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_int_source_get(const stmdev_ctx_t *ctx,
                               lps22hb_int_source_t *val)
{
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_INT_SOURCE, (uint8_t *) val, 1);

  return ret;
}

/**
  * @brief  ѹ�����жϱ�־��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INT_SOURCE �Ĵ����� ph ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_int_on_press_high_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_int_source_t int_source;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INT_SOURCE,
                         (uint8_t *)&int_source, 1);
  *val = int_source.ph;

  return ret;
}

/**
  * @brief  ѹ�����жϱ�־��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INT_SOURCE �Ĵ����� pl ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_int_on_press_low_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_int_source_t int_source;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INT_SOURCE,
                         (uint8_t *)&int_source, 1);
  *val = int_source.pl;

  return ret;
}

/**
  * @brief  �жϻ��־��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INT_SOURCE �Ĵ����� ia ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_interrupt_event_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_int_source_t int_source;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INT_SOURCE,
                         (uint8_t *)&int_source, 1);
  *val = int_source.ia;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS22HB_FIFO
  * @brief       ���ڽ������� FIFO ʹ����صĺ�������
  * @{
  *
  */

/**
  * @brief  �� FIFO ˮλ��ֹͣ������ FIFO ˮλ����ʹ�á�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� stop_on_fth ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_stop_on_fifo_threshold_set(const stmdev_ctx_t *ctx,
                                           uint8_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.stop_on_fth = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  �� FIFO ˮλ��ֹͣ������ FIFO ˮλ����ʹ�á�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� stop_on_fth ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_stop_on_fifo_threshold_get(const stmdev_ctx_t *ctx,
                                           uint8_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.stop_on_fth;

  return ret;
}

/**
  * @brief  FIFO ʹ�ܡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� fifo_en ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.fifo_en = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  FIFO ʹ�ܡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� fifo_en ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.fifo_en;

  return ret;
}

/**
  * @brief  FIFO ˮλ����ѡ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� FIFO_CTRL �Ĵ����� wtm ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_watermark_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_fifo_ctrl_t fifo_ctrl;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);

  if (ret == 0)
  {
    fifo_ctrl.wtm = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  FIFO ˮλ����ѡ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� FIFO_CTRL �Ĵ����� wtm ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_watermark_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_fifo_ctrl_t fifo_ctrl;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
  *val = fifo_ctrl.wtm;

  return ret;
}

/**
  * @brief  FIFO ģʽѡ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� FIFO_CTRL �Ĵ����� f_mode ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_mode_set(const stmdev_ctx_t *ctx, lps22hb_f_mode_t val)
{
  lps22hb_fifo_ctrl_t fifo_ctrl;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);

  if (ret == 0)
  {
    fifo_ctrl.f_mode = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  FIFO ģʽѡ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ FIFO_CTRL �Ĵ����� f_mode ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_mode_get(const stmdev_ctx_t *ctx,
                              lps22hb_f_mode_t *val)
{
  lps22hb_fifo_ctrl_t fifo_ctrl;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);

  switch (fifo_ctrl.f_mode)
  {
    case LPS22HB_BYPASS_MODE:
      *val = LPS22HB_BYPASS_MODE;
      break;

    case LPS22HB_FIFO_MODE:
      *val = LPS22HB_FIFO_MODE;
      break;

    case LPS22HB_STREAM_MODE:
      *val = LPS22HB_STREAM_MODE;
      break;

    case LPS22HB_STREAM_TO_FIFO_MODE:
      *val = LPS22HB_STREAM_TO_FIFO_MODE;
      break;

    case LPS22HB_BYPASS_TO_STREAM_MODE:
      *val = LPS22HB_BYPASS_TO_STREAM_MODE;
      break;

    case LPS22HB_DYNAMIC_STREAM_MODE:
      *val = LPS22HB_DYNAMIC_STREAM_MODE;
      break;

    case LPS22HB_BYPASS_TO_FIFO_MODE:
      *val = LPS22HB_BYPASS_TO_FIFO_MODE;
      break;

    default:
      *val = LPS22HB_BYPASS_MODE;
      break;
  }

  return ret;
}

/**
  * @brief  FIFO �洢�����ݼ���[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� FIFO_STATUS �Ĵ����� fss ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_data_level_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_fifo_status_t fifo_status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_STATUS,
                         (uint8_t *)&fifo_status, 1);
  *val = fifo_status.fss;

  return ret;
}

/**
  * @brief  FIFO ���״̬��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� FIFO_STATUS �Ĵ����� ovr ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_ovr_flag_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_fifo_status_t fifo_status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_STATUS,
                         (uint8_t *)&fifo_status, 1);
  *val = fifo_status.ovr;

  return ret;
}

/**
  * @brief  FIFO ˮλ״̬��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� FIFO_STATUS �Ĵ����� fth_fifo ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_fifo_fth_flag_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_fifo_status_t fifo_status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_STATUS,
                         (uint8_t *)&fifo_status, 1);
  *val = fifo_status.fth_fifo;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS22HB_���нӿ�
  * @brief       ���ڽ������봮�нӿڹ�����صĺ�������
  * @{
  *
  */

/**
  * @brief  SPI ���нӿ�ģʽѡ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG1 �Ĵ����� sim ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_spi_mode_set(const stmdev_ctx_t *ctx, lps22hb_sim_t val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.sim = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief  SPI ���нӿ�ģʽѡ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG1 �Ĵ����� sim ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_spi_mode_get(const stmdev_ctx_t *ctx, lps22hb_sim_t *val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  switch (ctrl_reg1.sim)
  {
    case LPS22HB_SPI_4_WIRE:
      *val = LPS22HB_SPI_4_WIRE;
      break;

    case LPS22HB_SPI_3_WIRE:
      *val = LPS22HB_SPI_3_WIRE;
      break;

    default:
      *val = LPS22HB_SPI_4_WIRE;
      break;
  }

  return ret;
}

/**
  * @brief  ���� I2C �ӿڡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� i2c_dis ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_i2c_interface_set(const stmdev_ctx_t *ctx,
                                  lps22hb_i2c_dis_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.i2c_dis = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  ���� I2C �ӿڡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG2 �Ĵ����� i2c_dis ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_i2c_interface_get(const stmdev_ctx_t *ctx,
                                  lps22hb_i2c_dis_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  switch (ctrl_reg2.i2c_dis)
  {
    case LPS22HB_I2C_ENABLE:
      *val = LPS22HB_I2C_ENABLE;
      break;

    case LPS22HB_I2C_DISABLE:
      *val = LPS22HB_I2C_DISABLE;
      break;

    default:
      *val = LPS22HB_I2C_ENABLE;
      break;
  }

  return ret;
}

/**
  * @brief  ��ʹ�ô��нӿ� (I2C �� SPI) ���ж��ֽڷ����ڼ䣬
  *         �Ĵ�����ַ�Զ�������[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� if_add_inc ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_auto_add_inc_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.if_add_inc = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  ��ʹ�ô��нӿ� (I2C �� SPI) ���ж��ֽڷ����ڼ䣬
  *         �Ĵ�����ַ�Զ�������[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� if_add_inc ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  *
  */
int32_t lps22hb_auto_add_inc_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.if_add_inc;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @}
  *
  */