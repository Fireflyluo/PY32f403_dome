/**
  ******************************************************************************
  * @file    lps22hb_reg.h
  * @author  Sensors Software Solution Team
  * @brief   ���ļ����� lps22hb_reg.c ������������к���ԭ�͡�
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

/* �����Է�ֹ�ݹ���� -------------------------------------*/
#ifndef LPS22HB_REGS_H
#define LPS22HB_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/** @addtogroup LPS22HB
  * @{
  *
  */

/** @defgroup  �ֽ�����
  * @{
  *
  */

#ifndef DRV_BYTE_ORDER
#ifndef __BYTE_ORDER__

#define DRV_LITTLE_ENDIAN 1234
#define DRV_BIG_ENDIAN    4321

/** ���δ���� _BYTE_ORDER����ͨ��ȡ��ע���ʺ���ƽ̨�ֽ���Ķ���
  * ��ѡ�����ܹ����ֽ���
  */
//#define DRV_BYTE_ORDER    DRV_BIG_ENDIAN
#define DRV_BYTE_ORDER    DRV_LITTLE_ENDIAN

#else /* defined __BYTE_ORDER__ */

#define DRV_LITTLE_ENDIAN  __ORDER_LITTLE_ENDIAN__
#define DRV_BIG_ENDIAN     __ORDER_BIG_ENDIAN__
#define DRV_BYTE_ORDER     __BYTE_ORDER__

#endif /* __BYTE_ORDER__*/
#endif /* DRV_BYTE_ORDER */

/**
  * @}
  *
  */

/** @defgroup STMicroelectronics ������ͨ������
  * @{
  *
  */

#ifndef MEMS_SHARED_TYPES
#define MEMS_SHARED_TYPES

/**
  * @brief  λ��ṹ��
  */
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t bit0       : 1;
  uint8_t bit1       : 1;
  uint8_t bit2       : 1;
  uint8_t bit3       : 1;
  uint8_t bit4       : 1;
  uint8_t bit5       : 1;
  uint8_t bit6       : 1;
  uint8_t bit7       : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t bit7       : 1;
  uint8_t bit6       : 1;
  uint8_t bit5       : 1;
  uint8_t bit4       : 1;
  uint8_t bit3       : 1;
  uint8_t bit2       : 1;
  uint8_t bit1       : 1;
  uint8_t bit0       : 1;
#endif /* DRV_BIG_ENDIAN */
} bitwise_t;

#define PROPERTY_DISABLE                (0U)
#define PROPERTY_ENABLE                 (1U)

/** @addtogroup  �ӿں���
  * @brief       �����ṩ��һ�����ڶ�д�豸ͨ�üĴ����ĺ�����
  *              ���룺���� 0 -> �޴���
  * @{
  *
  */

/**
  * @brief  д��Ĵ�������ָ������
  */
typedef int32_t (*stmdev_write_ptr)(void *, uint8_t, uint8_t *, uint16_t);

/**
  * @brief  ��ȡ�Ĵ�������ָ������
  */
typedef int32_t (*stmdev_read_ptr)(void *, uint8_t, uint8_t *, uint16_t);

/**
  * @brief  ��ʱ����ָ������
  */
typedef void (*stmdev_mdelay_ptr)(uint32_t millisec);

/**
  * @brief  �豸�����Ľṹ��
  */
typedef struct _stmdev_ctx_t
{
  /** ��������ֶ� **/
  stmdev_write_ptr  write_reg;
  stmdev_read_ptr   read_reg;
  /** �����ѡ�ֶ� **/
  stmdev_mdelay_ptr   mdelay;
  /** ���Զ���Ŀ�ѡָ�� **/
  void *handle;
} stmdev_ctx_t;

/**
  * @}
  *
  */

#endif /* MEMS_SHARED_TYPES */

#ifndef MEMS_UCF_SHARED_TYPES
#define MEMS_UCF_SHARED_TYPES

/** @defgroup    ͨ�õ�ַ-���ݽṹ����
  * @brief       �˽ṹ������ڼ��ش�������Ԥ�������á�
  *              �������Լ��������������ã���ʹ�� STMicroelectronics
  *              ��վ���ṩ�� Unico / Unicleo ���ߡ�
  *
  * @{
  *
  */

/**
  * @brief  UCF �нṹ��
  */
typedef struct
{
  uint8_t address;
  uint8_t data;
} ucf_line_t;

/**
  * @}
  *
  */

#endif /* MEMS_UCF_SHARED_TYPES */

/**
  * @}
  *
  */


/** @defgroup LPS22HB_��Ϣ
  * @{
  *
  */

/** I2C �豸��ַ 8 λ��ʽ: ��� SA0=0 -> 0xB9 ��� SA0=1 -> 0xBB **/
#define LPS22HB_I2C_ADD_H   0xBBU
#define LPS22HB_I2C_ADD_L   0xB9U

/** �豸��ʶ (Who am I) **/
#define LPS22HB_ID            0xB1U

/**
  * @}
  *
  */

/**
  * @brief  �ж����üĴ��� (0x0B)
  */
#define LPS22HB_INTERRUPT_CFG  0x0BU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t pe               : 2; /* ple + phe -> pe */
  uint8_t lir              : 1;
  uint8_t diff_en          : 1;
  uint8_t reset_az         : 1;
  uint8_t autozero         : 1;
  uint8_t reset_arp        : 1;
  uint8_t autorifp         : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t autorifp         : 1;
  uint8_t reset_arp        : 1;
  uint8_t autozero         : 1;
  uint8_t reset_az         : 1;
  uint8_t diff_en          : 1;
  uint8_t lir              : 1;
  uint8_t pe               : 2; /* ple + phe -> pe */
#endif /* DRV_BIG_ENDIAN */
} lps22hb_interrupt_cfg_t;

/**
  * @brief  ѹ����ֵ�Ĵ������ֽ� (0x0C)
  */
#define LPS22HB_THS_P_L        0x0CU

/**
  * @brief  ѹ����ֵ�Ĵ������ֽ� (0x0D)
  */
#define LPS22HB_THS_P_H        0x0DU

/**
  * @brief  �豸��ʶ�Ĵ��� (0x0F)
  */
#define LPS22HB_WHO_AM_I       0x0FU

/**
  * @brief  ���ƼĴ��� 1 (0x10)
  */
#define LPS22HB_CTRL_REG1      0x10U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t sim              : 1;
  uint8_t bdu              : 1;
  uint8_t lpfp             : 2; /* en_lpfp + lpfp_cfg -> lpfp */
  uint8_t odr              : 3;
  uint8_t not_used_01      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01      : 1;
  uint8_t odr              : 3;
  uint8_t lpfp             : 2; /* en_lpfp + lpfp_cfg -> lpfp */
  uint8_t bdu              : 1;
  uint8_t sim              : 1;
#endif /* DRV_BIG_ENDIAN */
} lps22hb_ctrl_reg1_t;

/**
  * @brief  ���ƼĴ��� 2 (0x11)
  */
#define LPS22HB_CTRL_REG2      0x11U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t one_shot         : 1;
  uint8_t not_used_01      : 1;
  uint8_t swreset          : 1;
  uint8_t i2c_dis          : 1;
  uint8_t if_add_inc       : 1;
  uint8_t stop_on_fth      : 1;
  uint8_t fifo_en          : 1;
  uint8_t boot             : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t boot             : 1;
  uint8_t fifo_en          : 1;
  uint8_t stop_on_fth      : 1;
  uint8_t if_add_inc       : 1;
  uint8_t i2c_dis          : 1;
  uint8_t swreset          : 1;
  uint8_t not_used_01      : 1;
  uint8_t one_shot         : 1;
#endif /* DRV_BIG_ENDIAN */
} lps22hb_ctrl_reg2_t;

/**
  * @brief  ���ƼĴ��� 3 (0x12)
  */
#define LPS22HB_CTRL_REG3      0x12U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t int_s            : 2;
  uint8_t drdy             : 1;
  uint8_t f_ovr            : 1;
  uint8_t f_fth            : 1;
  uint8_t f_fss5           : 1;
  uint8_t pp_od            : 1;
  uint8_t int_h_l          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t int_h_l          : 1;
  uint8_t pp_od            : 1;
  uint8_t f_fss5           : 1;
  uint8_t f_fth            : 1;
  uint8_t f_ovr            : 1;
  uint8_t drdy             : 1;
  uint8_t int_s            : 2;
#endif /* DRV_BIG_ENDIAN */

} lps22hb_ctrl_reg3_t;


/**
  * @brief  FIFO ���ƼĴ��� (0x14)
  */
#define LPS22HB_FIFO_CTRL      0x14U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t wtm              : 5;
  uint8_t f_mode           : 3;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t f_mode           : 3;
  uint8_t wtm              : 5;
#endif /* DRV_BIG_ENDIAN */

} lps22hb_fifo_ctrl_t;

/**
  * @brief  �ο�ѹ���Ĵ��� XL �ֽ� (0x15)
  */
#define LPS22HB_REF_P_XL       0x15U

/**
  * @brief  �ο�ѹ���Ĵ��� L �ֽ� (0x16)
  */
#define LPS22HB_REF_P_L        0x16U

/**
  * @brief  �ο�ѹ���Ĵ��� H �ֽ� (0x17)
  */
#define LPS22HB_REF_P_H        0x17U

/**
  * @brief  ѹ��ƫ�ƼĴ��� L �ֽ� (0x18)
  */
#define LPS22HB_RPDS_L         0x18U

/**
  * @brief  ѹ��ƫ�ƼĴ��� H �ֽ� (0x19)
  */
#define LPS22HB_RPDS_H         0x19U

/**
  * @brief  �ֱ������üĴ��� (0x1A)
  */
#define LPS22HB_RES_CONF       0x1AU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t lc_en            : 1;
  uint8_t not_used_01      : 7;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01      : 7;
  uint8_t lc_en            : 1;
#endif /* DRV_BIG_ENDIAN */

} lps22hb_res_conf_t;

/**
  * @brief  �ж�Դ�Ĵ��� (0x25)
  */
#define LPS22HB_INT_SOURCE     0x25U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t ph               : 1;
  uint8_t pl               : 1;
  uint8_t ia               : 1;
  uint8_t not_used_01      : 4;
  uint8_t boot_status      : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t boot_status      : 1;
  uint8_t not_used_01      : 4;
  uint8_t ia               : 1;
  uint8_t pl               : 1;
  uint8_t ph               : 1;
#endif /* DRV_BIG_ENDIAN */
} lps22hb_int_source_t;

/**
  * @brief  FIFO ״̬�Ĵ��� (0x26)
  */
#define LPS22HB_FIFO_STATUS    0x26U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t fss              : 6;
  uint8_t ovr              : 1;
  uint8_t fth_fifo         : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t fth_fifo         : 1;
  uint8_t ovr              : 1;
  uint8_t fss              : 6;
#endif /* DRV_BIG_ENDIAN */
} lps22hb_fifo_status_t;

/**
  * @brief  ״̬�Ĵ��� (0x27)
  */
#define LPS22HB_STATUS         0x27U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint8_t p_da             : 1;
  uint8_t t_da             : 1;
  uint8_t not_used_02      : 2;
  uint8_t p_or             : 1;
  uint8_t t_or             : 1;
  uint8_t not_used_01      : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint8_t not_used_01      : 2;
  uint8_t t_or             : 1;
  uint8_t p_or             : 1;
  uint8_t not_used_02      : 2;
  uint8_t t_da             : 1;
  uint8_t p_da             : 1;
#endif /* DRV_BIG_ENDIAN */
} lps22hb_status_t;

/**
  * @brief  ѹ������Ĵ��� XL �ֽ� (0x28)
  */
#define LPS22HB_PRESS_OUT_XL   0x28U

/**
  * @brief  ѹ������Ĵ��� L �ֽ� (0x29)
  */
#define LPS22HB_PRESS_OUT_L    0x29U

/**
  * @brief  ѹ������Ĵ��� H �ֽ� (0x2A)
  */
#define LPS22HB_PRESS_OUT_H    0x2AU

/**
  * @brief  �¶�����Ĵ��� L �ֽ� (0x2B)
  */
#define LPS22HB_TEMP_OUT_L     0x2BU

/**
  * @brief  �¶�����Ĵ��� H �ֽ� (0x2C)
  */
#define LPS22HB_TEMP_OUT_H     0x2CU

/**
  * @brief  ��ͨ�˲�����λ�Ĵ��� (0x33)
  */
#define LPS22HB_LPFP_RES       0x33U

/**
  * @defgroup LPS22HB_�Ĵ���������
  * @brief    �������彫���о���λ�������ļĴ������顣
  *           ������������ã����������򲢲���Ҫ��
  *
  *           �Ƴ�����������������ϣ�
  *           MISRA-C 2012 [Rule 19.2] -> " ������ʹ�������� "
  *
  * @{
  *
  */

/**
  * @brief  �Ĵ���������
  */
typedef union
{
  lps22hb_interrupt_cfg_t      interrupt_cfg;
  lps22hb_ctrl_reg1_t          ctrl_reg1;
  lps22hb_ctrl_reg2_t          ctrl_reg2;
  lps22hb_ctrl_reg3_t          ctrl_reg3;
  lps22hb_fifo_ctrl_t          fifo_ctrl;
  lps22hb_res_conf_t           res_conf;
  lps22hb_int_source_t         int_source;
  lps22hb_fifo_status_t        fifo_status;
  lps22hb_status_t             status;
  bitwise_t                    bitwise;
  uint8_t                      byte;
} lps22hb_reg_t;

/**
  * @}
  *
  */

#ifndef __weak
#define __weak __attribute__((weak))
#endif /* __weak */

/*
 * ��Щ�Ƕ�ȡ��д�����ӵ���׼���ߵ��豸�Ĵ����Ļ���ƽ̨��� I/O ���̡�
 * ��������ͨ�����ں���ָ��Ķ�/д�����ṩĬ��ʵ����ʵ���������ԡ�
 * __weak ָ����������Ӧ�ó���ʹ���Զ���ʵ�ָ������ǡ�
 */

/**
  * @brief  ��ȡͨ���豸�Ĵ���
  *
  * @param  ctx   ��/д�ӿڶ���ָ��
  * @param  reg   Ҫ��ȡ�ļĴ���
  * @param  data  ָ��洢��ȡ���ݵĻ�����ָ��
  * @param  len   Ҫ������ȡ�ļĴ�������
  * @retval       �ӿ�״̬ (���뷵�� 0 -> �޴���)
  */
int32_t lps22hb_read_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                         uint8_t *data,
                         uint16_t len);

/**
  * @brief  д��ͨ���豸�Ĵ���
  *
  * @param  ctx   ��/д�ӿڶ���ָ��
  * @param  reg   Ҫд��ļĴ���
  * @param  data  ָ��Ҫд��Ĵ���������ָ��
  * @param  len   Ҫ����д��ļĴ�������
  * @retval       �ӿ�״̬ (���뷵�� 0 -> �޴���)
  */
int32_t lps22hb_write_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                          uint8_t *data,
                          uint16_t len);

/**
  * @brief  ��ԭʼֵת��Ϊ���� (hPa)
  *
  * @param  lsb   ԭʼֵ
  * @retval       ת�����ѹ��ֵ (hPa)
  */
float_t lps22hb_from_lsb_to_hpa(int32_t lsb);

/**
  * @brief  ��ԭʼֵת��Ϊǧ�� (kPa)
  *
  * @param  lsb   ԭʼֵ
  * @retval       ת�����ѹ��ֵ (kPa)
  */
float_t lps22hb_from_lsb_to_kpa(int32_t lsb);

/**
  * @brief  ��ԭʼֵת��Ϊ��/ƽ��Ӣ�� (psi)
  *
  * @param  lsb   ԭʼֵ
  * @retval       ת�����ѹ��ֵ (psi)
  */
float_t lps22hb_from_lsb_to_psi(int32_t lsb);

/**
  * @brief  ��ԭʼֵת��Ϊ���θ߶� (��)
  *
  * @param  lsb   ԭʼֵ
  * @retval       ת����ĺ��θ߶� (��)
  */
float_t lps22hb_from_lsb_to_altitude(int32_t lsb);

/**
  * @brief  ��ԭʼֵת��Ϊ���϶� (��C)
  *
  * @param  lsb   ԭʼֵ
  * @retval       ת������¶�ֵ (��C)
  */
float_t lps22hb_from_lsb_to_degc(int16_t lsb);

/**
  * @brief  �����Զ����㹦�ܡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� reset_az ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_autozero_rst_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  �����Զ����㹦�ܡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� reset_az ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_autozero_rst_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  �����Զ����㹦�ܡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� autozero ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_autozero_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  �����Զ����㹦�ܡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� autozero ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_autozero_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  �����Զ��ο�ѹ�����ܡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� reset_arp ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_pressure_snap_rst_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  �����Զ��ο�ѹ�����ܡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� reset_arp ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_pressure_snap_rst_get(const stmdev_ctx_t *ctx,
                                      uint8_t *val);

/**
  * @brief  �����Զ��ο�ѹ�����ܡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� autorifp ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_pressure_snap_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  �����Զ��ο�ѹ�����ܡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� autorifp ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_pressure_snap_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  �����ݸ��¡�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG1 �Ĵ����� bdu ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_block_data_update_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  �����ݸ��¡�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG1 �Ĵ����� bdu ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_block_data_update_get(const stmdev_ctx_t *ctx,
                                      uint8_t *val);

/**
  * @brief  ��ͨ�˲���ģʽö��
  */
typedef enum
{
  LPS22HB_LPF_ODR_DIV_2  = 0,
  LPS22HB_LPF_ODR_DIV_9  = 2,
  LPS22HB_LPF_ODR_DIV_20 = 3,
} lps22hb_lpfp_t;

/**
  * @brief  ��ͨ����ѡ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG1 �Ĵ����� lpfp ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_low_pass_filter_mode_set(const stmdev_ctx_t *ctx,
                                         lps22hb_lpfp_t val);

/**
  * @brief  ��ͨ����ѡ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG1 �Ĵ����� lpfp ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_low_pass_filter_mode_get(const stmdev_ctx_t *ctx,
                                         lps22hb_lpfp_t *val);

/**
  * @brief  �����������ö��
  */
typedef enum
{
  LPS22HB_POWER_DOWN  = 0,
  LPS22HB_ODR_1_Hz    = 1,
  LPS22HB_ODR_10_Hz   = 2,
  LPS22HB_ODR_25_Hz   = 3,
  LPS22HB_ODR_50_Hz   = 4,
  LPS22HB_ODR_75_Hz   = 5,
} lps22hb_odr_t;

/**
  * @brief  �����������ѡ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG1 �Ĵ����� odr ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_data_rate_set(const stmdev_ctx_t *ctx, lps22hb_odr_t val);

/**
  * @brief  �����������ѡ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG1 �Ĵ����� odr ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_data_rate_get(const stmdev_ctx_t *ctx, lps22hb_odr_t *val);

/**
  * @brief  ���δ���ģʽ���豸ִ��һ�β�����[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� one_shot ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_one_shoot_trigger_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  ���δ���ģʽ���豸ִ��һ�β�����[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� one_shot ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_one_shoot_trigger_get(const stmdev_ctx_t *ctx,
                                      uint8_t *val);

/**
  * @brief  pressure_ref: �ο�ѹ��ֵ��һ�� 24 λ���ݣ�
  *         �Զ����Ʋ�����ʽ��ʾ�������� AUTOZERO �� AUTORIFP ����ʱʹ�ø�ֵ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    Ҫд�������
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_pressure_ref_set(const stmdev_ctx_t *ctx, int32_t val);

/**
  * @brief  pressure_ref: �ο�ѹ��ֵ��һ�� 24 λ���ݣ�
  *         �Զ����Ʋ�����ʽ��ʾ�������� AUTOZERO �� AUTORIFP ����ʱʹ�ø�ֵ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_pressure_ref_get(const stmdev_ctx_t *ctx, int32_t *val);

/**
  * @brief  ѹ��ƫ��ֵ�� 16 λ���ݣ��������ں��Ӻ�ʵ�ֵ���У׼ (OPC)��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    Ҫд�������
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_pressure_offset_set(const stmdev_ctx_t *ctx, int16_t val);

/**
  * @brief  ѹ��ƫ��ֵ�� 16 λ���ݣ��������ں��Ӻ�ʵ�ֵ���У׼ (OPC)��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_pressure_offset_get(const stmdev_ctx_t *ctx, int16_t *val);

/**
  * @brief  ѹ�����ݿ��á�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� STATUS �Ĵ����� p_da ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_press_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  �¶����ݿ��á�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� STATUS �Ĵ����� t_da ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_temp_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  ѹ�����¶����ݿ��á�[��ȡ]
  *
  * @param  ctx          ��/д�ӿڶ���
  * @param  press_val    �ı� STATUS �Ĵ����� p_da ��ֵ
  * @param  temp_val     �ı� STATUS �Ĵ����� t_da ��ֵ
  * @retval              �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *press_val, uint8_t *temp_val);

/**
  * @brief  ѹ�����������[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� STATUS �Ĵ����� p_or ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_press_data_ovr_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  �¶����������[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� STATUS �Ĵ����� t_or ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_temp_data_ovr_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  ѹ�����ֵ[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  buff   �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_pressure_raw_get(const stmdev_ctx_t *ctx, uint32_t *buff);

/**
  * @brief  temperature_raw: �¶����ֵ[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  buff   �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_temperature_raw_get(const stmdev_ctx_t *ctx, int16_t *buff);

/**
  * @brief  FIFO ������ݽṹ��
  */
typedef struct __attribute__((packed)) _lps22hb_fifo_output_data_t
{
  uint8_t bytes[5];
} lps22hb_fifo_output_data_t;

/**
  * @brief  �� FIFO �������ת��Ϊԭʼѹ��ֵ
  *
  * @param  val    FIFO �������
  * @retval        ԭʼѹ��ֵ
  */
int32_t lps22hb_fifo_output_data_to_raw_pressure(lps22hb_fifo_output_data_t *val);

/**
  * @brief  �� FIFO �������ת��Ϊԭʼ�¶�ֵ
  *
  * @param  val    FIFO �������
  * @retval        ԭʼ�¶�ֵ
  */
int16_t lps22hb_fifo_output_data_to_raw_temperature(lps22hb_fifo_output_data_t *val);

/**
  * @brief  ͻ����ȡ FIFO ������ݡ�
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  buff   �洢��ȡ���ݵĻ�����
  * @param  len    Ҫ�� FIFO ��ȡ��ѹ��-�¶ȶԵ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_output_data_burst_get(const stmdev_ctx_t *ctx,
                                           lps22hb_fifo_output_data_t *buff, uint8_t len);

/**
  * @brief  ��ͨ�˲�����λ�Ĵ�������� LPFP ���ڻ״̬��
  *         Ϊ�˱���˲̬�׶Σ�����ͨ��������ѹ������֮ǰ��ȡ�˼Ĵ�������λ�˲�����[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  buff   �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_low_pass_rst_get(const stmdev_ctx_t *ctx, uint8_t *buff);

/**
  * @brief  �豸��ʶ[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  buff   �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_device_id_get(const stmdev_ctx_t *ctx, uint8_t *buff);

/**
  * @brief  �����λ���ָ��û��Ĵ����е�Ĭ��ֵ[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� swreset ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_reset_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  �����λ���ָ��û��Ĵ����е�Ĭ��ֵ[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� swreset ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_reset_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  �����ڴ����ݡ����¼���У׼������[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� boot ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_boot_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  �����ڴ����ݡ����¼���У׼������[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� boot ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_boot_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  �͵���ģʽ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� RES_CONF �Ĵ����� lc_en ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_low_power_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  �͵���ģʽ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� RES_CONF �Ĵ����� lc_en ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_low_power_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  ���Ϊ '1' ��ʾ������������ (����) �׶Ρ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INT_SOURCE �Ĵ����� boot_status ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_boot_status_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  �豸״̬�ṹ��
  */
typedef struct
{
  lps22hb_fifo_status_t  fifo_status;
  lps22hb_status_t       status;
} lps22hb_dev_stat_t;

/**
  * @brief  ����״̬λ��FIFO ����������[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �� FIFO_STATUS �� STATUS �Ĵ����Ľṹ��
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_dev_status_get(const stmdev_ctx_t *ctx,
                               lps22hb_dev_stat_t *val);

/**
  * @brief  ѹ����ֵ����ö��
  */
typedef enum
{
  LPS22HB_NO_THRESHOLD = 0,
  LPS22HB_POSITIVE     = 1,
  LPS22HB_NEGATIVE     = 2,
  LPS22HB_BOTH         = 3,
} lps22hb_pe_t;

/**
  * @brief  ����ѹ����ֵ�ж����ɡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� pe ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_sign_of_int_threshold_set(const stmdev_ctx_t *ctx,
                                          lps22hb_pe_t val);

/**
  * @brief  ����ѹ����ֵ�ж����ɡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ INTERRUPT_CFG �Ĵ����� pe ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_sign_of_int_threshold_get(const stmdev_ctx_t *ctx,
                                          lps22hb_pe_t *val);

/**
  * @brief  �ж�����ģʽö��
  */
typedef enum
{
  LPS22HB_INT_PULSED  = 0,
  LPS22HB_INT_LATCHED = 1,
} lps22hb_lir_t;

/**
  * @brief  �ж����� INT_SOURCE (25h) �Ĵ���
  *         ģʽ (���� / ����) [����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� lir ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_int_notification_mode_set(const stmdev_ctx_t *ctx,
                                          lps22hb_lir_t val);

/**
  * @brief  �ж����� INT_SOURCE (25h) �Ĵ���
  *         ģʽ (���� / ����) [��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ INTERRUPT_CFG �Ĵ����� lir ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_int_notification_mode_get(const stmdev_ctx_t *ctx,
                                          lps22hb_lir_t *val);

/**
  * @brief  �����ж����ɡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� diff_en ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_int_generation_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  �����ж����ɡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INTERRUPT_CFG �Ĵ����� diff_en ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_int_generation_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  �û������ѹ���ж��¼���ֵֵ[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    Ҫд�������
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_int_threshold_set(const stmdev_ctx_t *ctx, uint16_t val);

/**
  * @brief  �û������ѹ���ж��¼���ֵֵ[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �洢��ȡ���ݵĻ�����
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_int_threshold_get(const stmdev_ctx_t *ctx, uint16_t *val);

/**
  * @brief  �ж�����ģʽö��
  */
typedef enum
{
  LPS22HB_DRDY_OR_FIFO_FLAGS = 0,
  LPS22HB_HIGH_PRES_INT      = 1,
  LPS22HB_LOW_PRES_INT       = 2,
  LPS22HB_EVERY_PRES_INT     = 3,
} lps22hb_int_s_t;

/**
  * @brief  INT_DRDY �����ϵ������źſ���λ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� int_s ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_int_pin_mode_set(const stmdev_ctx_t *ctx,
                                 lps22hb_int_s_t val);

/**
  * @brief  INT_DRDY �����ϵ������źſ���λ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG3 �Ĵ����� int_s ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_int_pin_mode_get(const stmdev_ctx_t *ctx,
                                 lps22hb_int_s_t *val);

/**
  * @brief  INT_DRDY �����ϵ����ݾ����źš�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� drdy ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_drdy_on_int_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  INT_DRDY �����ϵ����ݾ����źš�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� drdy ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_drdy_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  FIFO ����ж��� INT_DRDY �����ϡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� f_ovr ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_ovr_on_int_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  FIFO ����ж��� INT_DRDY �����ϡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� f_ovr ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_ovr_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  FIFO ˮλ״̬�� INT_DRDY �����ϡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� f_fth ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_threshold_on_int_set(const stmdev_ctx_t *ctx,
                                          uint8_t val);

/**
  * @brief  FIFO ˮλ״̬�� INT_DRDY �����ϡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� f_fth ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_threshold_on_int_get(const stmdev_ctx_t *ctx,
                                          uint8_t *val);

/**
  * @brief  FIFO ����־�� INT_DRDY �����ϡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� f_fss5 ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_full_on_int_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  FIFO ����־�� INT_DRDY �����ϡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� f_fss5 ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_full_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  ����/��©ģʽö��
  */
typedef enum
{
  LPS22HB_PUSH_PULL  = 0,
  LPS22HB_OPEN_DRAIN = 1,
} lps22hb_pp_od_t;

/**
  * @brief  �ж������ϵ�����/��©ѡ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� pp_od ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_pin_mode_set(const stmdev_ctx_t *ctx, lps22hb_pp_od_t val);

/**
  * @brief  �ж������ϵ�����/��©ѡ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG3 �Ĵ����� pp_od ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_pin_mode_get(const stmdev_ctx_t *ctx, lps22hb_pp_od_t *val);

/**
  * @brief  �жϼ���ö��
  */
typedef enum
{
  LPS22HB_ACTIVE_HIGH = 0,
  LPS22HB_ACTIVE_LOW = 1,
} lps22hb_int_h_l_t;

/**
  * @brief  �жϸߵ�ƽ/�͵�ƽ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG3 �Ĵ����� int_h_l ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_int_polarity_set(const stmdev_ctx_t *ctx,
                                 lps22hb_int_h_l_t val);

/**
  * @brief  �жϸߵ�ƽ/�͵�ƽ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG3 �Ĵ����� int_h_l ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_int_polarity_get(const stmdev_ctx_t *ctx,
                                 lps22hb_int_h_l_t *val);

/**
  * @brief  �ж�Դ�Ĵ���[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    INT_SOURCE �Ĵ���
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_int_source_get(const stmdev_ctx_t *ctx,
                               lps22hb_int_source_t *val);

/**
  * @brief  ѹ�����жϱ�־��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INT_SOURCE �Ĵ����� ph ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_int_on_press_high_get(const stmdev_ctx_t *ctx,
                                      uint8_t *val);

/**
  * @brief  ѹ�����жϱ�־��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INT_SOURCE �Ĵ����� pl ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_int_on_press_low_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  �жϻ��־��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� INT_SOURCE �Ĵ����� ia ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_interrupt_event_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  �� FIFO ˮλ��ֹͣ������ FIFO ˮλ����ʹ�á�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� stop_on_fth ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_stop_on_fifo_threshold_set(const stmdev_ctx_t *ctx,
                                           uint8_t val);

/**
  * @brief  �� FIFO ˮλ��ֹͣ������ FIFO ˮλ����ʹ�á�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� stop_on_fth ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_stop_on_fifo_threshold_get(const stmdev_ctx_t *ctx,
                                           uint8_t *val);

/**
  * @brief  FIFO ʹ�ܡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� fifo_en ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  FIFO ʹ�ܡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� fifo_en ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  FIFO ˮλ����ѡ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� FIFO_CTRL �Ĵ����� wtm ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_watermark_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  FIFO ˮλ����ѡ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� FIFO_CTRL �Ĵ����� wtm ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_watermark_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  FIFO ģʽö��
  */
typedef enum
{
  LPS22HB_BYPASS_MODE           = 0,
  LPS22HB_FIFO_MODE             = 1,
  LPS22HB_STREAM_MODE           = 2,
  LPS22HB_STREAM_TO_FIFO_MODE   = 3,
  LPS22HB_BYPASS_TO_STREAM_MODE = 4,
  LPS22HB_DYNAMIC_STREAM_MODE   = 6,
  LPS22HB_BYPASS_TO_FIFO_MODE   = 7,
} lps22hb_f_mode_t;

/**
  * @brief  FIFO ģʽѡ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� FIFO_CTRL �Ĵ����� f_mode ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_mode_set(const stmdev_ctx_t *ctx,
                              lps22hb_f_mode_t val);

/**
  * @brief  FIFO ģʽѡ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ FIFO_CTRL �Ĵ����� f_mode ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_mode_get(const stmdev_ctx_t *ctx,
                              lps22hb_f_mode_t *val);

/**
  * @brief  FIFO �洢�����ݼ���[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� FIFO_STATUS �Ĵ����� fss ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_data_level_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  FIFO ���״̬��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� FIFO_STATUS �Ĵ����� ovr ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_ovr_flag_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  FIFO ˮλ״̬��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� FIFO_STATUS �Ĵ����� fth_fifo ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_fifo_fth_flag_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  SPI ģʽö��
  */
typedef enum
{
  LPS22HB_SPI_4_WIRE = 0,
  LPS22HB_SPI_3_WIRE = 1,
} lps22hb_sim_t;

/**
  * @brief  SPI ���нӿ�ģʽѡ��[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG1 �Ĵ����� sim ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_spi_mode_set(const stmdev_ctx_t *ctx, lps22hb_sim_t val);

/**
  * @brief  SPI ���нӿ�ģʽѡ��[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG1 �Ĵ����� sim ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_spi_mode_get(const stmdev_ctx_t *ctx, lps22hb_sim_t *val);

/**
  * @brief  I2C �ӿ�ģʽö��
  */
typedef enum
{
  LPS22HB_I2C_ENABLE = 0,
  LPS22HB_I2C_DISABLE = 1,
} lps22hb_i2c_dis_t;

/**
  * @brief  ���� I2C �ӿڡ�[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� i2c_dis ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_i2c_interface_set(const stmdev_ctx_t *ctx,
                                  lps22hb_i2c_dis_t val);

/**
  * @brief  ���� I2C �ӿڡ�[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    ��ȡ CTRL_REG2 �Ĵ����� i2c_dis ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_i2c_interface_get(const stmdev_ctx_t *ctx,
                                  lps22hb_i2c_dis_t *val);

/**
  * @brief  ��ʹ�ô��нӿ� (I2C �� SPI) ���ж��ֽڷ����ڼ䣬
  *         �Ĵ�����ַ�Զ�������[����]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� if_add_inc ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_auto_add_inc_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  ��ʹ�ô��нӿ� (I2C �� SPI) ���ж��ֽڷ����ڼ䣬
  *         �Ĵ�����ַ�Զ�������[��ȡ]
  *
  * @param  ctx    ��/д�ӿڶ���
  * @param  val    �ı� CTRL_REG2 �Ĵ����� if_add_inc ��ֵ
  * @retval        �ӿ�״̬ (���뷵�� 0 -> �޴���)��
  */
int32_t lps22hb_auto_add_inc_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  *@}
  *
  */

#ifdef __cplusplus
}
#endif

#endif /* LPS22HB_REGS_H */
