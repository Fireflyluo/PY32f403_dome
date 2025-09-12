/**
  ******************************************************************************
  * @file    lps22hb_reg.h
  * @author  Sensors Software Solution Team
  * @brief   本文件包含 lps22hb_reg.c 驱动程序的所有函数原型。
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) 2021 STMicroelectronics。
  * 保留所有权利。
  *
  * 本软件根据可以在 LICENSE 文件中找到的条款获得许可，
  * 该文件位于此软件组件的根目录下。
  * 如果没有随此软件提供的 LICENSE 文件，则按原样提供。
  *
  ******************************************************************************
  */

/* 定义以防止递归包含 -------------------------------------*/
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

/** @defgroup  字节序定义
  * @{
  *
  */

#ifndef DRV_BYTE_ORDER
#ifndef __BYTE_ORDER__

#define DRV_LITTLE_ENDIAN 1234
#define DRV_BIG_ENDIAN    4321

/** 如果未定义 _BYTE_ORDER，请通过取消注释适合您平台字节序的定义
  * 来选择您架构的字节序
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

/** @defgroup STMicroelectronics 传感器通用类型
  * @{
  *
  */

#ifndef MEMS_SHARED_TYPES
#define MEMS_SHARED_TYPES

/**
  * @brief  位域结构体
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

/** @addtogroup  接口函数
  * @brief       本节提供了一组用于读写设备通用寄存器的函数。
  *              必须：返回 0 -> 无错误。
  * @{
  *
  */

/**
  * @brief  写入寄存器函数指针类型
  */
typedef int32_t (*stmdev_write_ptr)(void *, uint8_t, uint8_t *, uint16_t);

/**
  * @brief  读取寄存器函数指针类型
  */
typedef int32_t (*stmdev_read_ptr)(void *, uint8_t, uint8_t *, uint16_t);

/**
  * @brief  延时函数指针类型
  */
typedef void (*stmdev_mdelay_ptr)(uint32_t millisec);

/**
  * @brief  设备上下文结构体
  */
typedef struct _stmdev_ctx_t
{
  /** 组件必需字段 **/
  stmdev_write_ptr  write_reg;
  stmdev_read_ptr   read_reg;
  /** 组件可选字段 **/
  stmdev_mdelay_ptr   mdelay;
  /** 可自定义的可选指针 **/
  void *handle;
} stmdev_ctx_t;

/**
  * @}
  *
  */

#endif /* MEMS_SHARED_TYPES */

#ifndef MEMS_UCF_SHARED_TYPES
#define MEMS_UCF_SHARED_TYPES

/** @defgroup    通用地址-数据结构定义
  * @brief       此结构体可用于加载传感器的预定义配置。
  *              您可以自己创建传感器配置，或使用 STMicroelectronics
  *              网站上提供的 Unico / Unicleo 工具。
  *
  * @{
  *
  */

/**
  * @brief  UCF 行结构体
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


/** @defgroup LPS22HB_信息
  * @{
  *
  */

/** I2C 设备地址 8 位格式: 如果 SA0=0 -> 0xB9 如果 SA0=1 -> 0xBB **/
#define LPS22HB_I2C_ADD_H   0xBBU
#define LPS22HB_I2C_ADD_L   0xB9U

/** 设备标识 (Who am I) **/
#define LPS22HB_ID            0xB1U

/**
  * @}
  *
  */

/**
  * @brief  中断配置寄存器 (0x0B)
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
  * @brief  压力阈值寄存器低字节 (0x0C)
  */
#define LPS22HB_THS_P_L        0x0CU

/**
  * @brief  压力阈值寄存器高字节 (0x0D)
  */
#define LPS22HB_THS_P_H        0x0DU

/**
  * @brief  设备标识寄存器 (0x0F)
  */
#define LPS22HB_WHO_AM_I       0x0FU

/**
  * @brief  控制寄存器 1 (0x10)
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
  * @brief  控制寄存器 2 (0x11)
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
  * @brief  控制寄存器 3 (0x12)
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
  * @brief  FIFO 控制寄存器 (0x14)
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
  * @brief  参考压力寄存器 XL 字节 (0x15)
  */
#define LPS22HB_REF_P_XL       0x15U

/**
  * @brief  参考压力寄存器 L 字节 (0x16)
  */
#define LPS22HB_REF_P_L        0x16U

/**
  * @brief  参考压力寄存器 H 字节 (0x17)
  */
#define LPS22HB_REF_P_H        0x17U

/**
  * @brief  压力偏移寄存器 L 字节 (0x18)
  */
#define LPS22HB_RPDS_L         0x18U

/**
  * @brief  压力偏移寄存器 H 字节 (0x19)
  */
#define LPS22HB_RPDS_H         0x19U

/**
  * @brief  分辨率配置寄存器 (0x1A)
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
  * @brief  中断源寄存器 (0x25)
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
  * @brief  FIFO 状态寄存器 (0x26)
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
  * @brief  状态寄存器 (0x27)
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
  * @brief  压力输出寄存器 XL 字节 (0x28)
  */
#define LPS22HB_PRESS_OUT_XL   0x28U

/**
  * @brief  压力输出寄存器 L 字节 (0x29)
  */
#define LPS22HB_PRESS_OUT_L    0x29U

/**
  * @brief  压力输出寄存器 H 字节 (0x2A)
  */
#define LPS22HB_PRESS_OUT_H    0x2AU

/**
  * @brief  温度输出寄存器 L 字节 (0x2B)
  */
#define LPS22HB_TEMP_OUT_L     0x2BU

/**
  * @brief  温度输出寄存器 H 字节 (0x2C)
  */
#define LPS22HB_TEMP_OUT_H     0x2CU

/**
  * @brief  低通滤波器复位寄存器 (0x33)
  */
#define LPS22HB_LPFP_RES       0x33U

/**
  * @defgroup LPS22HB_寄存器联合体
  * @brief    此联合体将所有具有位域描述的寄存器分组。
  *           此联合体很有用，但驱动程序并不需要。
  *
  *           移除此联合体后，您将符合：
  *           MISRA-C 2012 [Rule 19.2] -> " 不允许使用联合体 "
  *
  * @{
  *
  */

/**
  * @brief  寄存器联合体
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
 * 这些是读取和写入连接到标准总线的设备寄存器的基本平台相关 I/O 例程。
 * 驱动程序通过基于函数指针的读/写例程提供默认实现以实现向后兼容性。
 * __weak 指令允许最终应用程序使用自定义实现覆盖它们。
 */

/**
  * @brief  读取通用设备寄存器
  *
  * @param  ctx   读/写接口定义指针
  * @param  reg   要读取的寄存器
  * @param  data  指向存储读取数据的缓冲区指针
  * @param  len   要连续读取的寄存器数量
  * @retval       接口状态 (必须返回 0 -> 无错误)
  */
int32_t lps22hb_read_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                         uint8_t *data,
                         uint16_t len);

/**
  * @brief  写入通用设备寄存器
  *
  * @param  ctx   读/写接口定义指针
  * @param  reg   要写入的寄存器
  * @param  data  指向要写入寄存器的数据指针
  * @param  len   要连续写入的寄存器数量
  * @retval       接口状态 (必须返回 0 -> 无错误)
  */
int32_t lps22hb_write_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                          uint8_t *data,
                          uint16_t len);

/**
  * @brief  将原始值转换为百帕 (hPa)
  *
  * @param  lsb   原始值
  * @retval       转换后的压力值 (hPa)
  */
float_t lps22hb_from_lsb_to_hpa(int32_t lsb);

/**
  * @brief  将原始值转换为千帕 (kPa)
  *
  * @param  lsb   原始值
  * @retval       转换后的压力值 (kPa)
  */
float_t lps22hb_from_lsb_to_kpa(int32_t lsb);

/**
  * @brief  将原始值转换为磅/平方英寸 (psi)
  *
  * @param  lsb   原始值
  * @retval       转换后的压力值 (psi)
  */
float_t lps22hb_from_lsb_to_psi(int32_t lsb);

/**
  * @brief  将原始值转换为海拔高度 (米)
  *
  * @param  lsb   原始值
  * @retval       转换后的海拔高度 (米)
  */
float_t lps22hb_from_lsb_to_altitude(int32_t lsb);

/**
  * @brief  将原始值转换为摄氏度 (°C)
  *
  * @param  lsb   原始值
  * @retval       转换后的温度值 (°C)
  */
float_t lps22hb_from_lsb_to_degc(int16_t lsb);

/**
  * @brief  重置自动归零功能。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 reset_az 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_autozero_rst_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  重置自动归零功能。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 reset_az 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_autozero_rst_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  启用自动归零功能。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 autozero 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_autozero_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  启用自动归零功能。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 autozero 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_autozero_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  重置自动参考压力功能。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 reset_arp 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_pressure_snap_rst_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  重置自动参考压力功能。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 reset_arp 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_pressure_snap_rst_get(const stmdev_ctx_t *ctx,
                                      uint8_t *val);

/**
  * @brief  启用自动参考压力功能。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 autorifp 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_pressure_snap_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  启用自动参考压力功能。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 autorifp 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_pressure_snap_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  块数据更新。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG1 寄存器中 bdu 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_block_data_update_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  块数据更新。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG1 寄存器中 bdu 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_block_data_update_get(const stmdev_ctx_t *ctx,
                                      uint8_t *val);

/**
  * @brief  低通滤波器模式枚举
  */
typedef enum
{
  LPS22HB_LPF_ODR_DIV_2  = 0,
  LPS22HB_LPF_ODR_DIV_9  = 2,
  LPS22HB_LPF_ODR_DIV_20 = 3,
} lps22hb_lpfp_t;

/**
  * @brief  低通带宽选择。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG1 寄存器中 lpfp 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_low_pass_filter_mode_set(const stmdev_ctx_t *ctx,
                                         lps22hb_lpfp_t val);

/**
  * @brief  低通带宽选择。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG1 寄存器中 lpfp 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_low_pass_filter_mode_get(const stmdev_ctx_t *ctx,
                                         lps22hb_lpfp_t *val);

/**
  * @brief  输出数据速率枚举
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
  * @brief  输出数据速率选择。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG1 寄存器中 odr 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_data_rate_set(const stmdev_ctx_t *ctx, lps22hb_odr_t val);

/**
  * @brief  输出数据速率选择。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG1 寄存器中 odr 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_data_rate_get(const stmdev_ctx_t *ctx, lps22hb_odr_t *val);

/**
  * @brief  单次触发模式。设备执行一次测量。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 one_shot 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_one_shoot_trigger_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  单次触发模式。设备执行一次测量。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 one_shot 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_one_shoot_trigger_get(const stmdev_ctx_t *ctx,
                                      uint8_t *val);

/**
  * @brief  pressure_ref: 参考压力值是一个 24 位数据，
  *         以二进制补码形式表示。当启用 AUTOZERO 或 AUTORIFP 功能时使用该值。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    要写入的数据
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_pressure_ref_set(const stmdev_ctx_t *ctx, int32_t val);

/**
  * @brief  pressure_ref: 参考压力值是一个 24 位数据，
  *         以二进制补码形式表示。当启用 AUTOZERO 或 AUTORIFP 功能时使用该值。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_pressure_ref_get(const stmdev_ctx_t *ctx, int32_t *val);

/**
  * @brief  压力偏移值是 16 位数据，可用于在焊接后实现单点校准 (OPC)。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    要写入的数据
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_pressure_offset_set(const stmdev_ctx_t *ctx, int16_t val);

/**
  * @brief  压力偏移值是 16 位数据，可用于在焊接后实现单点校准 (OPC)。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_pressure_offset_get(const stmdev_ctx_t *ctx, int16_t *val);

/**
  * @brief  压力数据可用。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 STATUS 寄存器中 p_da 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_press_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  温度数据可用。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 STATUS 寄存器中 t_da 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_temp_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  压力和温度数据可用。[获取]
  *
  * @param  ctx          读/写接口定义
  * @param  press_val    改变 STATUS 寄存器中 p_da 的值
  * @param  temp_val     改变 STATUS 寄存器中 t_da 的值
  * @retval              接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *press_val, uint8_t *temp_val);

/**
  * @brief  压力数据溢出。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 STATUS 寄存器中 p_or 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_press_data_ovr_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  温度数据溢出。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 STATUS 寄存器中 t_or 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_temp_data_ovr_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  压力输出值[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  buff   存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_pressure_raw_get(const stmdev_ctx_t *ctx, uint32_t *buff);

/**
  * @brief  temperature_raw: 温度输出值[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  buff   存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_temperature_raw_get(const stmdev_ctx_t *ctx, int16_t *buff);

/**
  * @brief  FIFO 输出数据结构体
  */
typedef struct __attribute__((packed)) _lps22hb_fifo_output_data_t
{
  uint8_t bytes[5];
} lps22hb_fifo_output_data_t;

/**
  * @brief  将 FIFO 输出数据转换为原始压力值
  *
  * @param  val    FIFO 输出数据
  * @retval        原始压力值
  */
int32_t lps22hb_fifo_output_data_to_raw_pressure(lps22hb_fifo_output_data_t *val);

/**
  * @brief  将 FIFO 输出数据转换为原始温度值
  *
  * @param  val    FIFO 输出数据
  * @retval        原始温度值
  */
int16_t lps22hb_fifo_output_data_to_raw_temperature(lps22hb_fifo_output_data_t *val);

/**
  * @brief  突发读取 FIFO 输出数据。
  *
  * @param  ctx    读/写接口定义
  * @param  buff   存储读取数据的缓冲区
  * @param  len    要从 FIFO 读取的压力-温度对的数量
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_output_data_burst_get(const stmdev_ctx_t *ctx,
                                           lps22hb_fifo_output_data_t *buff, uint8_t len);

/**
  * @brief  低通滤波器复位寄存器。如果 LPFP 处于活动状态，
  *         为了避免瞬态阶段，可以通过在生成压力测量之前读取此寄存器来复位滤波器。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  buff   存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_low_pass_rst_get(const stmdev_ctx_t *ctx, uint8_t *buff);

/**
  * @brief  设备标识[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  buff   存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_device_id_get(const stmdev_ctx_t *ctx, uint8_t *buff);

/**
  * @brief  软件复位。恢复用户寄存器中的默认值[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 swreset 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_reset_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  软件复位。恢复用户寄存器中的默认值[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 swreset 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_reset_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  重启内存内容。重新加载校准参数。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 boot 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_boot_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  重启内存内容。重新加载校准参数。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 boot 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_boot_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  低电流模式。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 RES_CONF 寄存器中 lc_en 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_low_power_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  低电流模式。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 RES_CONF 寄存器中 lc_en 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_low_power_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  如果为 '1' 表示正在运行启动 (重启) 阶段。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INT_SOURCE 寄存器中 boot_status 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_boot_status_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  设备状态结构体
  */
typedef struct
{
  lps22hb_fifo_status_t  fifo_status;
  lps22hb_status_t       status;
} lps22hb_dev_stat_t;

/**
  * @brief  所有状态位，FIFO 和数据生成[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    从 FIFO_STATUS 到 STATUS 寄存器的结构体
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_dev_status_get(const stmdev_ctx_t *ctx,
                               lps22hb_dev_stat_t *val);

/**
  * @brief  压力阈值极性枚举
  */
typedef enum
{
  LPS22HB_NO_THRESHOLD = 0,
  LPS22HB_POSITIVE     = 1,
  LPS22HB_NEGATIVE     = 2,
  LPS22HB_BOTH         = 3,
} lps22hb_pe_t;

/**
  * @brief  启用压力阈值中断生成。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 pe 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_sign_of_int_threshold_set(const stmdev_ctx_t *ctx,
                                          lps22hb_pe_t val);

/**
  * @brief  启用压力阈值中断生成。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 INTERRUPT_CFG 寄存器中 pe 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_sign_of_int_threshold_get(const stmdev_ctx_t *ctx,
                                          lps22hb_pe_t *val);

/**
  * @brief  中断锁存模式枚举
  */
typedef enum
{
  LPS22HB_INT_PULSED  = 0,
  LPS22HB_INT_LATCHED = 1,
} lps22hb_lir_t;

/**
  * @brief  中断请求到 INT_SOURCE (25h) 寄存器
  *         模式 (脉冲 / 锁存) [设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 lir 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_int_notification_mode_set(const stmdev_ctx_t *ctx,
                                          lps22hb_lir_t val);

/**
  * @brief  中断请求到 INT_SOURCE (25h) 寄存器
  *         模式 (脉冲 / 锁存) [获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 INTERRUPT_CFG 寄存器中 lir 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_int_notification_mode_get(const stmdev_ctx_t *ctx,
                                          lps22hb_lir_t *val);

/**
  * @brief  启用中断生成。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 diff_en 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_int_generation_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  启用中断生成。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 diff_en 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_int_generation_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  用户定义的压力中断事件阈值值[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    要写入的数据
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_int_threshold_set(const stmdev_ctx_t *ctx, uint16_t val);

/**
  * @brief  用户定义的压力中断事件阈值值[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_int_threshold_get(const stmdev_ctx_t *ctx, uint16_t *val);

/**
  * @brief  中断引脚模式枚举
  */
typedef enum
{
  LPS22HB_DRDY_OR_FIFO_FLAGS = 0,
  LPS22HB_HIGH_PRES_INT      = 1,
  LPS22HB_LOW_PRES_INT       = 2,
  LPS22HB_EVERY_PRES_INT     = 3,
} lps22hb_int_s_t;

/**
  * @brief  INT_DRDY 引脚上的数据信号控制位。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 int_s 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_int_pin_mode_set(const stmdev_ctx_t *ctx,
                                 lps22hb_int_s_t val);

/**
  * @brief  INT_DRDY 引脚上的数据信号控制位。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG3 寄存器中 int_s 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_int_pin_mode_get(const stmdev_ctx_t *ctx,
                                 lps22hb_int_s_t *val);

/**
  * @brief  INT_DRDY 引脚上的数据就绪信号。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 drdy 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_drdy_on_int_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  INT_DRDY 引脚上的数据就绪信号。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 drdy 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_drdy_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  FIFO 溢出中断在 INT_DRDY 引脚上。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 f_ovr 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_ovr_on_int_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  FIFO 溢出中断在 INT_DRDY 引脚上。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 f_ovr 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_ovr_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  FIFO 水位状态在 INT_DRDY 引脚上。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 f_fth 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_threshold_on_int_set(const stmdev_ctx_t *ctx,
                                          uint8_t val);

/**
  * @brief  FIFO 水位状态在 INT_DRDY 引脚上。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 f_fth 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_threshold_on_int_get(const stmdev_ctx_t *ctx,
                                          uint8_t *val);

/**
  * @brief  FIFO 满标志在 INT_DRDY 引脚上。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 f_fss5 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_full_on_int_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  FIFO 满标志在 INT_DRDY 引脚上。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 f_fss5 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_full_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  推挽/开漏模式枚举
  */
typedef enum
{
  LPS22HB_PUSH_PULL  = 0,
  LPS22HB_OPEN_DRAIN = 1,
} lps22hb_pp_od_t;

/**
  * @brief  中断引脚上的推挽/开漏选择。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 pp_od 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_pin_mode_set(const stmdev_ctx_t *ctx, lps22hb_pp_od_t val);

/**
  * @brief  中断引脚上的推挽/开漏选择。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG3 寄存器中 pp_od 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_pin_mode_get(const stmdev_ctx_t *ctx, lps22hb_pp_od_t *val);

/**
  * @brief  中断极性枚举
  */
typedef enum
{
  LPS22HB_ACTIVE_HIGH = 0,
  LPS22HB_ACTIVE_LOW = 1,
} lps22hb_int_h_l_t;

/**
  * @brief  中断高电平/低电平。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 int_h_l 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_int_polarity_set(const stmdev_ctx_t *ctx,
                                 lps22hb_int_h_l_t val);

/**
  * @brief  中断高电平/低电平。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG3 寄存器中 int_h_l 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_int_polarity_get(const stmdev_ctx_t *ctx,
                                 lps22hb_int_h_l_t *val);

/**
  * @brief  中断源寄存器[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    INT_SOURCE 寄存器
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_int_source_get(const stmdev_ctx_t *ctx,
                               lps22hb_int_source_t *val);

/**
  * @brief  压力高中断标志。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INT_SOURCE 寄存器中 ph 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_int_on_press_high_get(const stmdev_ctx_t *ctx,
                                      uint8_t *val);

/**
  * @brief  压力低中断标志。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INT_SOURCE 寄存器中 pl 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_int_on_press_low_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  中断活动标志。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INT_SOURCE 寄存器中 ia 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_interrupt_event_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  在 FIFO 水位处停止。启用 FIFO 水位级别使用。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 stop_on_fth 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_stop_on_fifo_threshold_set(const stmdev_ctx_t *ctx,
                                           uint8_t val);

/**
  * @brief  在 FIFO 水位处停止。启用 FIFO 水位级别使用。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 stop_on_fth 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_stop_on_fifo_threshold_get(const stmdev_ctx_t *ctx,
                                           uint8_t *val);

/**
  * @brief  FIFO 使能。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 fifo_en 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  FIFO 使能。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 fifo_en 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  FIFO 水位级别选择。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 FIFO_CTRL 寄存器中 wtm 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_watermark_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  FIFO 水位级别选择。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 FIFO_CTRL 寄存器中 wtm 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_watermark_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  FIFO 模式枚举
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
  * @brief  FIFO 模式选择。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 FIFO_CTRL 寄存器中 f_mode 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_mode_set(const stmdev_ctx_t *ctx,
                              lps22hb_f_mode_t val);

/**
  * @brief  FIFO 模式选择。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 FIFO_CTRL 寄存器中 f_mode 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_mode_get(const stmdev_ctx_t *ctx,
                              lps22hb_f_mode_t *val);

/**
  * @brief  FIFO 存储的数据级别。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 FIFO_STATUS 寄存器中 fss 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_data_level_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  FIFO 溢出状态。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 FIFO_STATUS 寄存器中 ovr 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_ovr_flag_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  FIFO 水位状态。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 FIFO_STATUS 寄存器中 fth_fifo 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_fifo_fth_flag_get(const stmdev_ctx_t *ctx, uint8_t *val);

/**
  * @brief  SPI 模式枚举
  */
typedef enum
{
  LPS22HB_SPI_4_WIRE = 0,
  LPS22HB_SPI_3_WIRE = 1,
} lps22hb_sim_t;

/**
  * @brief  SPI 串行接口模式选择。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG1 寄存器中 sim 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_spi_mode_set(const stmdev_ctx_t *ctx, lps22hb_sim_t val);

/**
  * @brief  SPI 串行接口模式选择。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG1 寄存器中 sim 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_spi_mode_get(const stmdev_ctx_t *ctx, lps22hb_sim_t *val);

/**
  * @brief  I2C 接口模式枚举
  */
typedef enum
{
  LPS22HB_I2C_ENABLE = 0,
  LPS22HB_I2C_DISABLE = 1,
} lps22hb_i2c_dis_t;

/**
  * @brief  禁用 I2C 接口。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 i2c_dis 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_i2c_interface_set(const stmdev_ctx_t *ctx,
                                  lps22hb_i2c_dis_t val);

/**
  * @brief  禁用 I2C 接口。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG2 寄存器中 i2c_dis 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_i2c_interface_get(const stmdev_ctx_t *ctx,
                                  lps22hb_i2c_dis_t *val);

/**
  * @brief  在使用串行接口 (I2C 或 SPI) 进行多字节访问期间，
  *         寄存器地址自动递增。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 if_add_inc 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  */
int32_t lps22hb_auto_add_inc_set(const stmdev_ctx_t *ctx, uint8_t val);

/**
  * @brief  在使用串行接口 (I2C 或 SPI) 进行多字节访问期间，
  *         寄存器地址自动递增。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 if_add_inc 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
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
