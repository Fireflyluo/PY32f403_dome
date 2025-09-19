/**
 ******************************************************************************
 * @file    lps22hb_reg.h
 * @brief   lps22hb��ѹ�ƼĴ�������
 ******************************************************************************
 * @note    
 ******************************************************************************
 */
#ifndef LPS22HB_H
#define LPS22HB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ========================== �豸������Ϣ ========================== */
/** I2C �豸��ַ 8 λ��ʽ: ��� SA0=0 -> 0xB9 ��� SA0=1 -> 0xBB **/
#define LPS22HB_I2C_ADD_H     0xBBU
#define LPS22HB_I2C_ADD_L     0xB9U
/** �豸��ʶ (Who am I) **/
#define LPS22HB_ID            0xB1U
#define LPS22HB_WHO_AM_I      0x0FU    // �豸��ʶ�Ĵ���
/* ========================== ���ļĴ�����ַ ========================== */

#define LPS22HB_INTERRUPT_CFG  0x0BU   // �ж����üĴ���

/* ��ֵ�Ĵ��� */
#define LPS22HB_THS_P_L        0x0CU    // ѹ����ֵ�Ĵ������ֽ�
#define LPS22HB_THS_P_H        0x0DU    // ѹ����ֵ�Ĵ������ֽ�


/* ���ƼĴ����� */
#define LPS22HB_CTRL_REG1               0x10     // ���ƼĴ���1
#define LPS22HB_CTRL_REG2               0x11     // ���ƼĴ���1
#define LPS22HB_CTRL_REG3               0x12     // ���ƼĴ���1

#define LPS22HB_FIFO_CTRL               0x14     // FIFO ���üĴ���

#define LPS22HB_REF_P_XL                0x15     // �ο�ѹ���Ĵ���
#define LPS22HB_REF_P_L                 0x16
#define LPS22HB_REF_P_H                 0x17

#define LPS22HB_RPDS_L                  0x18     // ѹ��ƫ�ƼĴ���
#define LPS22HB_RPDS_H                  0x19

#define LPS22HB_RES_CONF                0x1A     // �ֱ��ʼĴ���

/* ״̬�Ĵ��� */
#define LPS22HB_INT_SOURCE              0x25     // �жϼĴ���
#define LPS22HB_FIFO_STATUS             0x26     // FIFO ״̬�Ĵ���
#define LPS22HB_STATUS                  0x27     // ״̬�Ĵ���

/* ����Ĵ��� */
#define LPS22HB_PRESS_OUT_XL            0x28     // ѹ������Ĵ���
#define LPS22HB_PRESS_OUT_L             0x29     
#define LPS22HB_PRESS_OUT_H             0x2A     

#define LPS22HB_TEMP_OUT_L              0x2B     // �¶�����Ĵ���
#define LPS22HB_TEMP_OUT_H              0x2C

#define LPS22HB_LPFP_RES                0x33     //�˲������üĴ���
/* ========================== �Ĵ���λ��ṹ�� ========================== */
/* CTRL_REG1 (0x10): ���ƼĴ���1 */
typedef union{
    struct {
          uint8_t pe            : 2;     // ��ѹ����/���¼��������ж����ɡ�Ĭ��ֵ��0
                                         // ��0�������ж�����1����ѹ��ֵ����/����Ԥ����ֵʱ�����ж�����
          uint8_t lir           : 1;     // ���ж��������浽 INT_SOURCE��25h���Ĵ�����Ĭ��ֵ��0��0��δ���棻1�����棩
          uint8_t diff_en       : 1;     // �����ж����ɡ�Ĭ��ֵ��0��0���ж����ɽ��ã�1���ж��������ã�
          uint8_t reset_az      : 1;     // �����Զ���λ���ܡ�Ĭ��ֵ��0��0������ģʽ��1�������Զ���λ���ܣ�
          uint8_t autozero      : 1;     // �����Զ���λ���ܡ�Ĭ��ֵ��0��0������ģʽ��1�������Զ���λ���ܣ�
          uint8_t reset_arp     : 1;     // ���� AutoRifP ���ܡ�Ĭ��ֵ��0( 0������ģʽ��1������ AutoRifP ���ܣ�)
          uint8_t autorifp      : 1;     // ���� AUTORIFP ���ܡ�Ĭ��ֵ��0��0������ģʽ��1������ AutoRifP��
    }bit;
    uint8_t reg;
} lps22hb_interrupt_cfg_t;

/* CTRL_REG1 (0x10): ���ƼĴ���1 */
typedef union{
    struct {
        uint8_t sim   : 1;  // ��������/����SPI�ӿ� (0:����, 1:����)
        uint8_t bdu   : 1;  // ���ݿ����  (0���������£�1����ȡ MSB �� LSB ֮ǰ����������Ĵ���) 
        uint8_t lpfp  : 2;  // ��ͨ���üĴ��� (0x:���� 10�� 11��)
        uint8_t odr   : 3;  // INT2���������� (000:���õ���/���β���ģʽ 001...)    
        uint8_t not_used_01     : 1;  // ��λ��������Ϊ��0����ʹ�豸����������   
    }bit;
    uint8_t reg;
} lps22hb_ctrl1_t;

/* CTRL_REG2 (0x11): ���ƼĴ���2 */
typedef union{
    struct {
          uint8_t one_shot         : 1;     // ���β���ʹ�ܡ�Ĭ��ֵ��0��0������ģʽ��1�������ڴ����ݣ���
          uint8_t not_used_01      : 1;     // ��λ��������Ϊ��0����ʹ�豸����������
          uint8_t swreset          : 1;     // �����λ��Ĭ��ֵ��0��0������ģʽ��1�������λ����
          uint8_t i2c_dis          : 1;     // ���� I2C �ӿڡ�Ĭ��ֵ��0��0��I2C ���ã�1��I2C ���ã�
          uint8_t if_add_inc       : 1;     // ���нӿڶ��ֽڷ���ʱ���Ĵ�����ַ������Ĭ��ֵ��1��0�����ã�1�����ã�
          uint8_t stop_on_fth      : 1;     // ���� FIFO ˮλ�߹��ܡ�Ĭ��ֵ��0��0�����ã�1�����ã�
          uint8_t fifo_en          : 1;     // FIFO ���á�Ĭ��ֵ��0��0�����ã�1�����ã�
          uint8_t boot             : 1;     // �����ڴ����ݡ�Ĭ��ֵ��0��0������ģʽ��1�������ڴ����ݣ���
                                            // �� BOOT ���ʱ����λ���Զ������
    }bit;
    uint8_t reg;
} lps22hb_ctrl2_t;

/* CTRL_REG3 (0x12): ���ƼĴ���3 */
typedef union{
    struct {
            uint8_t int_s            : 2;     // INT_DRDY �����ϵ������źſ���λ��Ĭ��ֵ��00 
            uint8_t drdy             : 1;     // INT_DRDY �����ϵ����ݾ����źš�Ĭ��ֵ��0��0�����ã�1�����ã�
            uint8_t f_ovr            : 1;     // INT_DRDY �����ϵ� FIFO ����жϡ�Ĭ��ֵ��0��0�����ã�1�����ã�
            uint8_t f_fth            : 1;     // INT_DRDY �����ϵ� FIFO ˮӡ״̬��Ĭ��ֵ��0��0�����ã�1�����ã�
            uint8_t f_fss5           : 1;     // INT_DRDY �����ϵ� FIFO ����־��Ĭ��ֵ��0��0�����ã�1�����ã�
            uint8_t pp_od            : 1;     // �ж������ϵ�����/��©ѡ��Ĭ��ֵ��0��0�����죻1����©��
            uint8_t int_h_l          : 1;     // �жϸߵ�ƽ/�͵�ƽ���Ĭ��ֵ��0��0���ߵ�ƽ���1���͵�ƽ���
    }bit;
    uint8_t reg;
} lps22hb_ctrl3_t;

/* FIFO ���ƼĴ��� (0x14) */
typedef union{
    struct {
            uint8_t wtm        : 5;     // FIFO ģʽѡ��
            uint8_t f_mode     : 3;     // FIFO ˮӡ����ѡ��
    }bit;
    uint8_t reg;
} lps22hb_fifo_ctrl_t;

/* �͹���ģʽ���� (0x1A) */
typedef union{
    struct {
          uint8_t lc_en            : 1;   //�͹���ģʽ���á�Ĭ�� 00������ģʽ��������ģʽ����1���͹���ģʽ��
          uint8_t not_used_01      : 7;   // ��λ��������Ϊ��0����ʹ�豸����������
    }bit;
    uint8_t reg;
} lps22hb_res_conf_t;

/* �ж�Դ�Ĵ��� (0x25) */
typedef union{
    struct {
          uint8_t ph               : 1;     // ��ѹ�ߡ�
          uint8_t pl               : 1;     // ��ѹ�͡�
          uint8_t ia               : 1;     // �жϼ����0��δ�����жϣ�1���Ѳ���һ�������ж��¼�����
          uint8_t not_used_01      : 4;     // ��λ��������Ϊ��0����ʹ�豸����������
          uint8_t boot_status      : 1;     // ��� '1' ��ʾ�������������׶��������С�
    }bit;
    uint8_t reg;
} lps22hb_int_source_t;

/* FIFO ״̬�Ĵ��� (0x26) */
typedef union{
    struct {
          uint8_t fss              : 6;     // FIFO �洢���ݼ���
          uint8_t ovr              : 1;     // FIFO ���״̬��
          uint8_t fth_fifo         : 1;     // FIFO ˮλ״̬��
    }bit;
    uint8_t reg;
} lps22hb_fifo_status_t;

/* ״̬�Ĵ��� (0x27) */
typedef union{
    struct {
          uint8_t p_da             : 1;     // ѹ�����ݿ��á�
          uint8_t t_da             : 1;     // �¶����ݿ��á�
          uint8_t not_used_02      : 2;     // ��λ��������Ϊ��0����ʹ�豸����������
          uint8_t p_or             : 1;     // ѹ�������������0��δ���������1���µ�ѹ�������Ѹ��Ǿ����ݣ�
          uint8_t t_or             : 1;     // �¶������������0��δ���������1���µ��¶������Ѹ��Ǿ����ݣ�
          uint8_t not_used_01      : 2;     // ��λ��������Ϊ��0����ʹ�豸����������
    }bit;
    uint8_t reg;
} lps22hb_status_t;
/* ========================== ö�����Ͷ��� ========================== */
/* ��ͨ�˲���ģʽö�� */
typedef enum
{
  LPS22HB_LPF_ODR_DIV_2  = 0,
  LPS22HB_LPF_ODR_DIV_9  = 2,
  LPS22HB_LPF_ODR_DIV_20 = 3,
} lps22hb_lpfp_t;

/* �����������ö�� */
typedef enum {
          LPS22HB_POWER_DOWN  = 0,
          LPS22HB_ODR_1_Hz    = 1,
          LPS22HB_ODR_10_Hz   = 2,
          LPS22HB_ODR_25_Hz   = 3,
          LPS22HB_ODR_50_Hz   = 4,
          LPS22HB_ODR_75_Hz   = 5,
} lps22hb_odr_t;

/* �ж�����ģʽö�� */
typedef enum
{
  LPS22HB_DRDY_OR_FIFO_FLAGS = 0,
  LPS22HB_HIGH_PRES_INT      = 1,
  LPS22HB_LOW_PRES_INT       = 2,
  LPS22HB_EVERY_PRES_INT     = 3,
} lps22hb_int_s_t;

/* FIFO ģʽö�� */
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
  * @brief  ѹ����ֵ����ö��
  */
typedef enum
{
  LPS22HB_NO_THRESHOLD = 0,
  LPS22HB_POSITIVE     = 1,
  LPS22HB_NEGATIVE     = 2,
  LPS22HB_BOTH         = 3,
} lps22hb_pe_t;
//�ж�����ģʽö��
typedef enum
{
  LPS22HB_INT_PULSED  = 0,
  LPS22HB_INT_LATCHED = 1,
} lps22hb_lir_t;
/**
  * @brief  ����/��©ģʽö��
  */
typedef enum
{
  LPS22HB_PUSH_PULL  = 0,
  LPS22HB_OPEN_DRAIN = 1,
} lps22hb_pp_od_t;
/**
  * @brief  �жϼ���ö��
  */
typedef enum
{
  LPS22HB_ACTIVE_HIGH = 0,
  LPS22HB_ACTIVE_LOW = 1,
} lps22hb_int_h_l_t;

/**
  * @brief  SPI ģʽö��
  */
typedef enum
{
  LPS22HB_SPI_4_WIRE = 0,
  LPS22HB_SPI_3_WIRE = 1,
} lps22hb_sim_t;
/**
  * @brief  I2C �ӿ�ģʽö��
  */
typedef enum
{
  LPS22HB_I2C_ENABLE = 0,
  LPS22HB_I2C_DISABLE = 1,
} lps22hb_i2c_dis_t;

/* ========================== �豸�����Ľṹ ========================== */


/* ========================== �������� ========================== */

#ifdef __cplusplus
}
#endif

#endif /* LPS22HB_REGS_H */
