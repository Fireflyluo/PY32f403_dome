/**
 ******************************************************************************
 * @file    lps22hb_reg.h
 * @brief   lps22hb气压计寄存器定义
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

/* ========================== 设备基本信息 ========================== */
/** I2C 设备地址 8 位格式: 如果 SA0=0 -> 0xB9 如果 SA0=1 -> 0xBB **/
#define LPS22HB_I2C_ADD_H     0xBBU
#define LPS22HB_I2C_ADD_L     0xB9U
/** 设备标识 (Who am I) **/
#define LPS22HB_ID            0xB1U
#define LPS22HB_WHO_AM_I      0x0FU    // 设备标识寄存器
/* ========================== 核心寄存器地址 ========================== */

#define LPS22HB_INTERRUPT_CFG  0x0BU   // 中断配置寄存器

/* 阈值寄存器 */
#define LPS22HB_THS_P_L        0x0CU    // 压力阈值寄存器低字节
#define LPS22HB_THS_P_H        0x0DU    // 压力阈值寄存器高字节


/* 控制寄存器组 */
#define LPS22HB_CTRL_REG1               0x10     // 控制寄存器1
#define LPS22HB_CTRL_REG2               0x11     // 控制寄存器1
#define LPS22HB_CTRL_REG3               0x12     // 控制寄存器1

#define LPS22HB_FIFO_CTRL               0x14     // FIFO 配置寄存器

#define LPS22HB_REF_P_XL                0x15     // 参考压力寄存器
#define LPS22HB_REF_P_L                 0x16
#define LPS22HB_REF_P_H                 0x17

#define LPS22HB_RPDS_L                  0x18     // 压力偏移寄存器
#define LPS22HB_RPDS_H                  0x19

#define LPS22HB_RES_CONF                0x1A     // 分辨率寄存器

/* 状态寄存器 */
#define LPS22HB_INT_SOURCE              0x25     // 中断寄存器
#define LPS22HB_FIFO_STATUS             0x26     // FIFO 状态寄存器
#define LPS22HB_STATUS                  0x27     // 状态寄存器

/* 输出寄存器 */
#define LPS22HB_PRESS_OUT_XL            0x28     // 压力输出寄存器
#define LPS22HB_PRESS_OUT_L             0x29     
#define LPS22HB_PRESS_OUT_H             0x2A     

#define LPS22HB_TEMP_OUT_L              0x2B     // 温度输出寄存器
#define LPS22HB_TEMP_OUT_H              0x2C

#define LPS22HB_LPFP_RES                0x33     //滤波器重置寄存器
/* ========================== 寄存器位域结构体 ========================== */
/* CTRL_REG1 (0x10): 控制寄存器1 */
typedef union{
    struct {
          uint8_t pe            : 2;     // 在压力高/低事件上启用中断生成。默认值：0
                                         // （0：禁用中断请求；1：在压力值高于/低于预设阈值时启用中断请求）
          uint8_t lir           : 1;     // 将中断请求锁存到 INT_SOURCE（25h）寄存器。默认值：0（0：未锁存；1：锁存）
          uint8_t diff_en       : 1;     // 启用中断生成。默认值：0（0：中断生成禁用；1：中断生成启用）
          uint8_t reset_az      : 1;     // 重置自动零位功能。默认值：0（0：正常模式；1：重置自动零位功能）
          uint8_t autozero      : 1;     // 启用自动零位功能。默认值：0（0：正常模式；1：启用自动零位功能）
          uint8_t reset_arp     : 1;     // 重置 AutoRifP 功能。默认值：0( 0：正常模式；1：启用 AutoRifP 功能）)
          uint8_t autorifp      : 1;     // 启用 AUTORIFP 功能。默认值：0（0：正常模式；1：启用 AutoRifP）
    }bit;
    uint8_t reg;
} lps22hb_interrupt_cfg_t;

/* CTRL_REG1 (0x10): 控制寄存器1 */
typedef union{
    struct {
        uint8_t sim   : 1;  // 启用三线/四线SPI接口 (0:四线, 1:三线)
        uint8_t bdu   : 1;  // 数据块更新  (0：连续更新；1：读取 MSB 和 LSB 之前不更新输出寄存器) 
        uint8_t lpfp  : 2;  // 低通配置寄存器 (0x:禁用 10： 11：)
        uint8_t odr   : 3;  // INT2引脚输启用 (000:启用掉电/单次测量模式 001...)    
        uint8_t not_used_01     : 1;  // 此位必须设置为‘0’以使设备正常工作。   
    }bit;
    uint8_t reg;
} lps22hb_ctrl1_t;

/* CTRL_REG2 (0x11): 控制寄存器2 */
typedef union{
    struct {
          uint8_t one_shot         : 1;     // 单次测量使能。默认值：0（0：正常模式；1：重启内存内容）。
          uint8_t not_used_01      : 1;     // 此位必须设置为‘0’以使设备正常工作。
          uint8_t swreset          : 1;     // 软件复位。默认值：0（0：正常模式；1：软件复位）。
          uint8_t i2c_dis          : 1;     // 禁用 I2C 接口。默认值：0（0：I2C 启用；1：I2C 禁用）
          uint8_t if_add_inc       : 1;     // 串行接口多字节访问时，寄存器地址自增。默认值：1（0：禁用；1：启用）
          uint8_t stop_on_fth      : 1;     // 启用 FIFO 水位线功能。默认值：0（0：禁用；1：启用）
          uint8_t fifo_en          : 1;     // FIFO 启用。默认值：0（0：禁用；1：启用）
          uint8_t boot             : 1;     // 重启内存内容。默认值：0（0：正常模式；1：重启内存内容）。
                                            // 当 BOOT 完成时，此位会自动清除。
    }bit;
    uint8_t reg;
} lps22hb_ctrl2_t;

/* CTRL_REG3 (0x12): 控制寄存器3 */
typedef union{
    struct {
            uint8_t int_s            : 2;     // INT_DRDY 引脚上的数据信号控制位。默认值：00 
            uint8_t drdy             : 1;     // INT_DRDY 引脚上的数据就绪信号。默认值：0（0：禁用；1：启用）
            uint8_t f_ovr            : 1;     // INT_DRDY 引脚上的 FIFO 溢出中断。默认值：0（0：禁用；1：启用）
            uint8_t f_fth            : 1;     // INT_DRDY 引脚上的 FIFO 水印状态。默认值：0（0：禁用；1：启用）
            uint8_t f_fss5           : 1;     // INT_DRDY 引脚上的 FIFO 满标志。默认值：0（0：禁用；1：启用）
            uint8_t pp_od            : 1;     // 中断引脚上的推挽/开漏选择。默认值：0（0：推挽；1：开漏）
            uint8_t int_h_l          : 1;     // 中断高电平/低电平激活。默认值：0（0：高电平激活；1：低电平激活）
    }bit;
    uint8_t reg;
} lps22hb_ctrl3_t;

/* FIFO 控制寄存器 (0x14) */
typedef union{
    struct {
            uint8_t wtm        : 5;     // FIFO 模式选择。
            uint8_t f_mode     : 3;     // FIFO 水印级别选择。
    }bit;
    uint8_t reg;
} lps22hb_fifo_ctrl_t;

/* 低功耗模式配置 (0x1A) */
typedef union{
    struct {
          uint8_t lc_en            : 1;   //低功耗模式启用。默认 00：正常模式（低噪声模式）；1：低功耗模式。
          uint8_t not_used_01      : 7;   // 此位必须设置为‘0’以使设备正常工作。
    }bit;
    uint8_t reg;
} lps22hb_res_conf_t;

/* 中断源寄存器 (0x25) */
typedef union{
    struct {
          uint8_t ph               : 1;     // 差压高。
          uint8_t pl               : 1;     // 差压低。
          uint8_t ia               : 1;     // 中断激活。（0：未产生中断；1：已产生一个或多个中断事件）。
          uint8_t not_used_01      : 4;     // 此位必须设置为‘0’以使设备正常工作。
          uint8_t boot_status      : 1;     // 如果 '1' 表示启动（重启）阶段正在运行。
    }bit;
    uint8_t reg;
} lps22hb_int_source_t;

/* FIFO 状态寄存器 (0x26) */
typedef union{
    struct {
          uint8_t fss              : 6;     // FIFO 存储数据级别。
          uint8_t ovr              : 1;     // FIFO 溢出状态。
          uint8_t fth_fifo         : 1;     // FIFO 水位状态。
    }bit;
    uint8_t reg;
} lps22hb_fifo_status_t;

/* 状态寄存器 (0x27) */
typedef union{
    struct {
          uint8_t p_da             : 1;     // 压力数据可用。
          uint8_t t_da             : 1;     // 温度数据可用。
          uint8_t not_used_02      : 2;     // 此位必须设置为‘0’以使设备正常工作。
          uint8_t p_or             : 1;     // 压力数据溢出。（0：未发生溢出；1：新的压力数据已覆盖旧数据）
          uint8_t t_or             : 1;     // 温度数据溢出。（0：未发生溢出；1：新的温度数据已覆盖旧数据）
          uint8_t not_used_01      : 2;     // 此位必须设置为‘0’以使设备正常工作。
    }bit;
    uint8_t reg;
} lps22hb_status_t;
/* ========================== 枚举类型定义 ========================== */
/* 低通滤波器模式枚举 */
typedef enum
{
  LPS22HB_LPF_ODR_DIV_2  = 0,
  LPS22HB_LPF_ODR_DIV_9  = 2,
  LPS22HB_LPF_ODR_DIV_20 = 3,
} lps22hb_lpfp_t;

/* 输出数据速率枚举 */
typedef enum {
          LPS22HB_POWER_DOWN  = 0,
          LPS22HB_ODR_1_Hz    = 1,
          LPS22HB_ODR_10_Hz   = 2,
          LPS22HB_ODR_25_Hz   = 3,
          LPS22HB_ODR_50_Hz   = 4,
          LPS22HB_ODR_75_Hz   = 5,
} lps22hb_odr_t;

/* 中断引脚模式枚举 */
typedef enum
{
  LPS22HB_DRDY_OR_FIFO_FLAGS = 0,
  LPS22HB_HIGH_PRES_INT      = 1,
  LPS22HB_LOW_PRES_INT       = 2,
  LPS22HB_EVERY_PRES_INT     = 3,
} lps22hb_int_s_t;

/* FIFO 模式枚举 */
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
  * @brief  压力阈值极性枚举
  */
typedef enum
{
  LPS22HB_NO_THRESHOLD = 0,
  LPS22HB_POSITIVE     = 1,
  LPS22HB_NEGATIVE     = 2,
  LPS22HB_BOTH         = 3,
} lps22hb_pe_t;
//中断锁存模式枚举
typedef enum
{
  LPS22HB_INT_PULSED  = 0,
  LPS22HB_INT_LATCHED = 1,
} lps22hb_lir_t;
/**
  * @brief  推挽/开漏模式枚举
  */
typedef enum
{
  LPS22HB_PUSH_PULL  = 0,
  LPS22HB_OPEN_DRAIN = 1,
} lps22hb_pp_od_t;
/**
  * @brief  中断极性枚举
  */
typedef enum
{
  LPS22HB_ACTIVE_HIGH = 0,
  LPS22HB_ACTIVE_LOW = 1,
} lps22hb_int_h_l_t;

/**
  * @brief  SPI 模式枚举
  */
typedef enum
{
  LPS22HB_SPI_4_WIRE = 0,
  LPS22HB_SPI_3_WIRE = 1,
} lps22hb_sim_t;
/**
  * @brief  I2C 接口模式枚举
  */
typedef enum
{
  LPS22HB_I2C_ENABLE = 0,
  LPS22HB_I2C_DISABLE = 1,
} lps22hb_i2c_dis_t;

/* ========================== 设备上下文结构 ========================== */


/* ========================== 函数声明 ========================== */

#ifdef __cplusplus
}
#endif

#endif /* LPS22HB_REGS_H */
