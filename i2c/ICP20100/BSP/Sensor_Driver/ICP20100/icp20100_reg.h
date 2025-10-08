/**
 * @file icp20100_reg.h
 * @brief ICP-20100气压温度传感器寄存器定义和驱动接口
 * @version 1.0
 * @date 2023
 * @license MIT
 */

#ifndef ICP20100_REG_H
#define ICP20100_REG_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 错误代码定义 */
typedef enum {
    ICP20100_OK = 0,
    ICP20100_ERR_INVALID_PARAM,
    ICP20100_ERR_COMM,
    ICP20100_ERR_NOT_READY,
    ICP20100_ERR_FIFO_EMPTY,
    ICP20100_ERR_FIFO_FULL,
    ICP20100_ERR_TIMEOUT
} icp20100_err_t;

/* 测量模式枚举 */
typedef enum {
    ICP20100_MODE_STANDBY = 0,
    ICP20100_MODE_PRESSURE_ONLY,
    ICP20100_MODE_TEMP_ONLY,
    ICP20100_MODE_PRESSURE_TEMP,
    ICP20100_MODE_USER_CONFIG
} icp20100_mode_t;

/* 滤波器类型枚举 */
typedef enum {
    ICP20100_FILTER_DISABLED = 0,
    ICP20100_FILTER_FIR,
    ICP20100_FILTER_IIR
} icp20100_filter_t;

/* 传感器数据结构 */
typedef struct {
    int32_t pressure_pa;    /* 压力值(Pa) */
    int32_t temperature_c; /* 温度值(0.01°C) */
    uint32_t timestamp;    /* 时间戳 */
} icp20100_data_t;

/* I2C读写函数指针类型 */
typedef icp20100_err_t (*i2c_write_fn)(uint8_t dev_addr, uint8_t reg_addr, 
                                      const uint8_t *data, uint16_t len);
typedef icp20100_err_t (*i2c_read_fn)(uint8_t dev_addr, uint8_t reg_addr, 
                                     uint8_t *data, uint16_t len);

/* 设备配置结构 */
typedef struct {
    uint8_t i2c_addr;      /* I2C设备地址 */
    i2c_write_fn i2c_write; /* I2C写函数指针 */
    i2c_read_fn i2c_read;   /* I2C读函数指针 */
    bool use_filter;        /* 是否使用滤波器 */
    icp20100_filter_t filter_type; /* 滤波器类型 */
} icp20100_cfg_t;

/* 设备句柄 */
typedef struct {
    icp20100_cfg_t cfg;     /* 设备配置 */
    bool initialized;       /* 初始化标志 */
    uint8_t device_id;      /* 设备ID */
    uint8_t firmware_ver;   /* 固件版本 */
} icp20100_dev_t;

/* 寄存器地址宏定义 */
#define ICP20100_REG_DEVICE_ID          0x00
#define ICP20100_REG_IO_DRIVE_STRENGTH  0x01
#define ICP20100_REG_MODE_SELECT        0x02
#define ICP20100_REG_INTERRUPT_STATUS   0x03
#define ICP20100_REG_INTERRUPT_MASK     0x04
#define ICP20100_REG_FIFO_CONFIG        0x05
#define ICP20100_REG_FIFO_FILL          0x06
#define ICP20100_REG_PRESS_ABS_LSB      0x07
#define ICP20100_REG_PRESS_ABS_MSB      0x08
#define ICP20100_REG_PRESS_DELTA_LSB    0x09
#define ICP20100_REG_PRESS_DELTA_MSB    0x0A
#define ICP20100_REG_DEVICE_STATUS      0x0B
#define ICP20100_REG_PRESS_DATA_0       0x0C
#define ICP20100_REG_PRESS_DATA_1       0x0D
#define ICP20100_REG_PRESS_DATA_2       0x0E
#define ICP20100_REG_TEMP_DATA_0        0x0F
#define ICP20100_REG_TEMP_DATA_1        0x10
#define ICP20100_REG_TEMP_DATA_2        0x11
#define ICP20100_REG_I3C_INFO           0x12
#define ICP20100_REG_VERSION           0x20

/* 寄存器位域结构体定义 */

/* IO_DRIVE_STRENGTH寄存器(0x01) */
typedef union {
    struct {
        uint8_t io_ds : 2;      /* 驱动强度配置 */
        uint8_t reserved : 6;   /* 保留位 */
    } bit;
    uint8_t reg;
} icp20100_io_drive_strength_t;

/* MODE_SELECT寄存器(0x02) */
typedef union {
    struct {
        uint8_t meas_config : 4; /* 测量模式配置 */
        uint8_t reserved : 3;    /* 保留位 */
        uint8_t power_mode : 1;  /* 电源模式 */
    } bit;
    uint8_t reg;
} icp20100_mode_select_t;

/* INTERRUPT_STATUS寄存器(0x03) */
typedef union {
    struct {
        uint8_t reserved : 1;     /* 保留位 */
        uint8_t delta_ovr : 1;    /* 压差超限标志 */
        uint8_t press_udr : 1;    /* 压力欠限标志 */
        uint8_t press_ovr : 1;    /* 压力超限标志 */
        uint8_t wmk_high : 1;     /* 高水位标志 */
        uint8_t wmk_low : 1;      /* 低水位标志 */
        uint8_t fifo_udf : 1;     /* FIFO下溢标志 */
        uint8_t fifo_ovf : 1;     /* FIFO溢出标志 */
    } bit;
    uint8_t reg;
} icp20100_interrupt_status_t;

/* INTERRUPT_MASK寄存器(0x04) */
typedef union {
    struct {
        uint8_t reserved : 1;         /* 保留位 */
        uint8_t delta_ovr_mask : 1;   /* 压差超限中断屏蔽 */
        uint8_t press_udr_mask : 1;   /* 压力欠限中断屏蔽 */
        uint8_t press_ovr_mask : 1;   /* 压力超限中断屏蔽 */
        uint8_t wmk_high_mask : 1;    /* 高水位中断屏蔽 */
        uint8_t wmk_low_mask : 1;     /* 低水位中断屏蔽 */
        uint8_t fifo_udf_mask : 1;    /* FIFO下溢中断屏蔽 */
        uint8_t fifo_ovf_mask : 1;    /* FIFO溢出中断屏蔽 */
    } bit;
    uint8_t reg;
} icp20100_interrupt_mask_t;

/* FIFO_CONFIG寄存器(0x05) */
typedef union {
    struct {
        uint8_t reserved : 2;         /* 保留位 */
        uint8_t fifo_wmk_low : 2;     /* 低水位阈值 */
        uint8_t fifo_wmk_high : 2;    /* 高水位阈值 */
        uint8_t fifo_mode : 2;         /* FIFO工作模式 */
    } bit;
    uint8_t reg;
} icp20100_fifo_config_t;

/* FIFO_FILL寄存器(0x06) */
typedef union {
    struct {
        uint8_t fifo_level : 6;   /* FIFO水位 */
        uint8_t reserved : 1;      /* 保留位 */
        uint8_t flush : 1;         /* FIFO清空控制 */
    } bit;
    uint8_t reg;
} icp20100_fifo_fill_t;

/* DEVICE_STATUS寄存器(0x0B) */
typedef union {
    struct {
        uint8_t delta_ovr : 1;     /* 压差超限状态 */
        uint8_t press_ovr : 1;     /* 压力超限状态 */
        uint8_t wmk_high : 1;      /* 高水位状态 */
        uint8_t wmk_low : 1;       /* 低水位状态 */
        uint8_t fifo_udf : 1;       /* FIFO下溢状态 */
        uint8_t fifo_ovf : 1;       /* FIFO溢出状态 */
        uint8_t reserved : 1;      /* 保留位 */
        uint8_t mode_sync_status : 1; /* 模式同步状态 */
    } bit;
    uint8_t reg;
} icp20100_device_status_t;

/* 驱动API函数声明 */
icp20100_err_t icp20100_init(icp20100_dev_t *dev, const icp20100_cfg_t *cfg);
icp20100_err_t icp20100_deinit(icp20100_dev_t *dev);
icp20100_err_t icp20100_set_mode(icp20100_dev_t *dev, icp20100_mode_t mode);
icp20100_err_t icp20100_read_data(icp20100_dev_t *dev, icp20100_data_t *data);
icp20100_err_t icp20100_read_fifo(icp20100_dev_t *dev, icp20100_data_t *data, 
                                 uint8_t *count);
icp20100_err_t icp20100_set_interrupt_mask(icp20100_dev_t *dev, 
                                         icp20100_interrupt_mask_t mask);
icp20100_err_t icp20100_get_interrupt_status(icp20100_dev_t *dev, 
                                            icp20100_interrupt_status_t *status);
icp20100_err_t icp20100_clear_interrupt(icp20100_dev_t *dev, 
                                       icp20100_interrupt_status_t status);
icp20100_err_t icp20100_configure_fifo(icp20100_dev_t *dev, 
                                       icp20100_fifo_config_t config);
icp20100_err_t icp20100_flush_fifo(icp20100_dev_t *dev);
icp20100_err_t icp20100_get_device_status(icp20100_dev_t *dev, 
                                          icp20100_device_status_t *status);
icp20100_err_t icp20100_software_reset(icp20100_dev_t *dev);
icp20100_err_t icp20100_read_raw_pressure(icp20100_dev_t *dev, int32_t *raw_pressure);
icp20100_err_t icp20100_read_raw_temperature(icp20100_dev_t *dev, int32_t *raw_temp);

#ifdef __cplusplus
}
#endif

#endif /* ICP20100_REG_H */
