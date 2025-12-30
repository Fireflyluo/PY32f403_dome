/**
 * @file icp20100.h
 * @brief ICP-20100 高精度气压和温度传感器驱动程序
 * @version 1.0
 * @date 2025
 * 
 * 驱动程序遵循Bosch Sensortec标准，用于MCU无关的传感器库
 * 兼容ICP-20100数据手册版本1.4
 */

#ifndef ICP20100_H_
#define ICP20100_H_

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/**\name        头文件                                                      */
/****************************************************************************/
#include <stdint.h>
#include <stddef.h>

/****************************************************************************/
/**\name        常量定义                                                    */
/****************************************************************************/

/** 设备识别 */
#define ICP20100_CHIP_ID                    UINT8_C(0x63)  /* 芯片ID，用于识别ICP20100传感器 */
#define ICP20100_VERSION_A                  UINT8_C(0x00)  /* 版本A标识 */
#define ICP20100_VERSION_B                  UINT8_C(0xB2)  /* 版本B标识 */

/** 接口选择 */
#define ICP20100_I2C_ADDR_LOW               UINT8_C(0x63)  /* I2C地址低电平(AD0引脚接地) */
#define ICP20100_I2C_ADDR_HIGH              UINT8_C(0x64)  /* I2C地址高电平(AD0引脚接VDD) */
#define ICP20100_SPI_3_WIRE                 UINT8_C(0x01)  /* SPI 3线模式 */
#define ICP20100_SPI_4_WIRE                 UINT8_C(0x00)  /* SPI 4线模式 */

/** 返回码 */
#define ICP20100_OK                         INT8_C(0)      /* 操作成功 */
#define ICP20100_E_NULL_PTR                 INT8_C(-1)     /* 空指针错误 */
#define ICP20100_E_COMM_FAIL                INT8_C(-2)     /* 通信失败 */
#define ICP20100_E_DEV_NOT_FOUND            INT8_C(-3)     /* 设备未找到 */
#define ICP20100_E_INVALID_CONFIG           INT8_C(-4)     /* 无效配置 */
#define ICP20100_E_BOOT_FAILED              INT8_C(-5)     /* 启动序列失败 */
#define ICP20100_W_PARTIAL_READ             INT8_C(1)      /* 部分读取警告 */

/** 寄存器地址 */
#define ICP20100_REG_TRIM1_MSB              UINT8_C(0x05)  /* 修调寄存器1高字节 */
#define ICP20100_REG_TRIM2_LSB              UINT8_C(0x06)  /* 修调寄存器2低字节 */
#define ICP20100_REG_TRIM2_MSB              UINT8_C(0x07)  /* 修调寄存器2高字节 */
#define ICP20100_REG_DEVICE_ID              UINT8_C(0x0C)  /* 设备ID寄存器 */
#define ICP20100_REG_IO_DRIVE_STRENGTH      UINT8_C(0x0D)  /* IO驱动强度配置寄存器 */
#define ICP20100_REG_OTP_CONFIG1            UINT8_C(0xAC)  /* OTP配置寄存器1 */
#define ICP20100_REG_MASTER_LOCK            UINT8_C(0xBE)  /* 主寄存器锁定 */
#define ICP20100_REG_OTP_STATUS2            UINT8_C(0xBF)  /* OTP状态寄存器2 */
#define ICP20100_REG_MODE_SELECT            UINT8_C(0xC0)  /* 模式选择寄存器 */
#define ICP20100_REG_INTERRUPT_STATUS       UINT8_C(0xC1)  /* 中断状态寄存器 */
#define ICP20100_REG_INTERRUPT_MASK         UINT8_C(0xC2)  /* 中断屏蔽寄存器 */
#define ICP20100_REG_FIFO_CONFIG            UINT8_C(0xC3)  /* FIFO配置寄存器 */
#define ICP20100_REG_FIFO_FILL              UINT8_C(0xC4)  /* FIFO填充寄存器 */
#define ICP20100_REG_SPI_MODE               UINT8_C(0xC5)  /* SPI模式配置寄存器 */
#define ICP20100_REG_PRESS_ABS_LSB          UINT8_C(0xC7)  /* 压力绝对值阈值低字节 */
#define ICP20100_REG_PRESS_ABS_MSB          UINT8_C(0xC8)  /* 压力绝对值阈值高字节 */
#define ICP20100_REG_PRESS_DELTA_LSB        UINT8_C(0xC9)  /* 压力差分阈值低字节 */
#define ICP20100_REG_PRESS_DELTA_MSB        UINT8_C(0xCA)  /* 压力差分阈值高字节 */
#define ICP20100_REG_DEVICE_STATUS          UINT8_C(0xCD)  /* 设备状态寄存器 */
#define ICP20100_REG_I3C_INFO               UINT8_C(0xCE)  /* I3C信息寄存器 */
#define ICP20100_REG_VERSION                UINT8_C(0xD3)  /* 版本寄存器 */
#define ICP20100_REG_PRESS_DATA_0           UINT8_C(0xFA)  /* 压力数据寄存器0 */
#define ICP20100_REG_PRESS_DATA_1           UINT8_C(0xFB)  /* 压力数据寄存器1 */
#define ICP20100_REG_PRESS_DATA_2           UINT8_C(0xFC)  /* 压力数据寄存器2 */
#define ICP20100_REG_TEMP_DATA_0            UINT8_C(0xFD)  /* 温度数据寄存器0 */
#define ICP20100_REG_TEMP_DATA_1            UINT8_C(0xFE)  /* 温度数据寄存器1 */
#define ICP20100_REG_TEMP_DATA_2            UINT8_C(0xFF)  /* 温度数据寄存器2 */

/** OTP寄存器，用于启动序列 */
#define ICP20100_REG_OTP_MR_LSB             UINT8_C(0xAD)  /* OTP MR寄存器低字节 */
#define ICP20100_REG_OTP_MR_MSB             UINT8_C(0xAE)  /* OTP MR寄存器高字节 */
#define ICP20100_REG_OTP_MRA_LSB            UINT8_C(0xAF)  /* OTP MRA寄存器低字节 */
#define ICP20100_REG_OTP_MRA_MSB            UINT8_C(0xB0)  /* OTP MRA寄存器高字节 */
#define ICP20100_REG_OTP_MRB_LSB            UINT8_C(0xB1)  /* OTP MRB寄存器低字节 */
#define ICP20100_REG_OTP_MRB_MSB            UINT8_C(0xB2)  /* OTP MRB寄存器高字节 */
#define ICP20100_REG_OTP_ADDRESS            UINT8_C(0xB5)  /* OTP地址寄存器 */
#define ICP20100_REG_OTP_COMMAND            UINT8_C(0xB6)  /* OTP命令寄存器 */
#define ICP20100_REG_OTP_RDATA              UINT8_C(0xB8)  /* OTP读取数据寄存器 */
#define ICP20100_REG_OTP_STATUS             UINT8_C(0xB9)  /* OTP状态寄存器 */
#define ICP20100_REG_OTP_DBG2               UINT8_C(0xBC)  /* OTP调试寄存器2 */

/** 位掩码和位置 */
#define ICP20100_BOOT_UP_STATUS_MASK        UINT8_C(0x01)  /* 启动状态位掩码 */
#define ICP20100_POWER_MODE_MASK            UINT8_C(0x04)  /* 电源模式位掩码 */
#define ICP20100_MEAS_CONFIG_MASK           UINT8_C(0xE0)  /* 测量配置位掩码 */
#define ICP20100_FORCED_MEAS_TRIGGER_MASK   UINT8_C(0x10)  /* 强制测量触发位掩码 */
#define ICP20100_MEAS_MODE_MASK             UINT8_C(0x08)  /* 测量模式位掩码 */
#define ICP20100_FIFO_READOUT_MODE_MASK     UINT8_C(0x03)  /* FIFO读出模式位掩码 */
#define ICP20100_MASTER_LOCK_UNLOCK         UINT8_C(0x1F)  /* 主寄存器解锁值 */
#define ICP20100_OTP_ENABLE_MASK            UINT8_C(0x01)  /* OTP使能位掩码 */
#define ICP20100_OTP_WRITE_SWITCH_MASK      UINT8_C(0x02)  /* OTP写开关位掩码 */
#define ICP20100_OTP_RESET_MASK             UINT8_C(0x80)  /* OTP复位位掩码 */
#define ICP20100_OTP_BUSY_MASK              UINT8_C(0x01)  /* OTP忙状态位掩码 */
#define ICP20100_FIFO_FLUSH_MASK            UINT8_C(0x80)  /* FIFO刷新位掩码 */
#define ICP20100_FIFO_LEVEL_MASK            UINT8_C(0x1F)  /* FIFO级别位掩码 */
#define ICP20100_FIFO_EMPTY_MASK            UINT8_C(0x40)  /* FIFO空状态位掩码 */
#define ICP20100_FIFO_FULL_MASK             UINT8_C(0x20)  /* FIFO满状态位掩码 */
#define ICP20100_MODE_SYNC_STATUS_MASK      UINT8_C(0x01)  /* 模式同步状态位掩码 */
#define ICP20100_PRESS_DATA_MASK            UINT8_C(0x0F)  /* 压力数据位掩码 */

/** 测量模式 */
#define ICP20100_MODE_0                     UINT8_C(0x00)  /* 模式0 */
#define ICP20100_MODE_1                     UINT8_C(0x01)  /* 模式1 */
#define ICP20100_MODE_2                     UINT8_C(0x02)  /* 模式2 */
#define ICP20100_MODE_3                     UINT8_C(0x03)  /* 模式3 */
#define ICP20100_MODE_4                     UINT8_C(0x04)  /* 模式4 */

/** FIFO读出模式 */
#define ICP20100_FIFO_PRESSURE_FIRST        UINT8_C(0x00)  /* 压力优先模式 */
#define ICP20100_FIFO_TEMP_ONLY             UINT8_C(0x01)  /* 仅温度模式 */

/** 测量类型 */
#define ICP20100_MEAS_MODE_FORCED           UINT8_C(0x00)  /* 强制测量模式 */
#define ICP20100_MEAS_MODE_CONTINUOUS       UINT8_C(0x01)  /* 连续测量模式 */

/** 电源模式 */
#define ICP20100_POWER_MODE_NORMAL          UINT8_C(0x00)  /* 正常电源模式 */
#define ICP20100_POWER_MODE_ACTIVE          UINT8_C(0x01)  /* 活动电源模式 */

/** 不同供电电压下的驱动强度设置 */
#define ICP20100_DRIVE_STRENGTH_1_8V_2MA    UINT8_C(0x00)  /* 1.8V供电，2mA驱动 */
#define ICP20100_DRIVE_STRENGTH_1_8V_4MA    UINT8_C(0x01)  /* 1.8V供电，4mA驱动 */
#define ICP20100_DRIVE_STRENGTH_1_8V_8MA    UINT8_C(0x02)  /* 1.8V供电，8mA驱动 */
#define ICP20100_DRIVE_STRENGTH_1_8V_12MA   UINT8_C(0x03)  /* 1.8V供电，12mA驱动 */
#define ICP20100_DRIVE_STRENGTH_1_2V_2MA    UINT8_C(0x04)  /* 1.2V供电，2mA驱动 */
#define ICP20100_DRIVE_STRENGTH_1_2V_4MA    UINT8_C(0x05)  /* 1.2V供电，4mA驱动 */
#define ICP20100_DRIVE_STRENGTH_1_2V_6MA    UINT8_C(0x06)  /* 1.2V供电，6mA驱动 */
#define ICP20100_DRIVE_STRENGTH_1_2V_8MA    UINT8_C(0x07)  /* 1.2V供电，8mA驱动 */
#define ICP20100_DRIVE_STRENGTH_MASK        UINT8_C(0x07)  /* 驱动强度位掩码 */

/** 转换常数 - 遵循software_imp.md公式 */
#define ICP20100_TEMP_SCALE_FACTOR          (65.0f / (1 << 18))  /* 温度比例因子 */
#define ICP20100_TEMP_OFFSET                (25.0f)               /* 温度偏移量(°C) */
#define ICP20100_PRESS_SCALE_FACTOR         (40.0f / (1 << 17))   /* 压力比例因子(kPa) */
#define ICP20100_PRESS_OFFSET               (70.0f)               /* 压力偏移量(kPa) */

/** 时间常数(毫秒/微秒) */
#define ICP20100_POWER_UP_TIME_MS           UINT8_C(4)    /* 电源启动时间(ms) */
#define ICP20100_OTP_WAIT_TIME_US           UINT8_C(10)   /* OTP等待时间(μs) */
#define ICP20100_OTP_BUSY_TIMEOUT_MS        UINT8_C(100)  /* OTP忙超时时间(ms) */

/** 中断位掩码 */
#define ICP20100_INT_FIFO_OVERFLOW_MASK     UINT8_C(0x01)  /* FIFO溢出中断掩码 */
#define ICP20100_INT_FIFO_UNDERFLOW_MASK    UINT8_C(0x02)  /* FIFO下溢中断掩码 */
#define ICP20100_INT_FIFO_WMK_HIGH_MASK     UINT8_C(0x04)  /* FIFO高水位中断掩码 */
#define ICP20100_INT_FIFO_WMK_LOW_MASK      UINT8_C(0x08)  /* FIFO低水位中断掩码 */
#define ICP20100_INT_PRESS_ABS_MASK         UINT8_C(0x20)  /* 压力绝对值中断掩码 */
#define ICP20100_INT_PRESS_DELTA_MASK       UINT8_C(0x40)  /* 压力差分中断掩码 */

/** 位位置定义 */
#define ICP20100_MEAS_CONFIG_POS            UINT8_C(5)  /* 测量配置位位置 */
#define ICP20100_FORCED_MEAS_TRIGGER_POS    UINT8_C(4)  /* 强制测量触发位位置 */
#define ICP20100_MEAS_MODE_POS              UINT8_C(3)  /* 测量模式位位置 */
#define ICP20100_POWER_MODE_POS             UINT8_C(2)  /* 电源模式位位置 */
#define ICP20100_FIFO_READOUT_MODE_POS      UINT8_C(0)  /* FIFO读出模式位位置 */

/****************************************************************************/
/**\name        类型定义                                                    */
/****************************************************************************/

/*!
 * @brief 总线通信函数指针，应映射到用户特定平台的读取函数
 *
 * @param[in]     reg_addr : 传感器的8位寄存器地址
 * @param[out]    reg_data : 来自指定地址的数据
 * @param[in]     length   : reg_data数组的长度
 * @param[in,out] intf_ptr : void指针，可用于链接接口相关回调的描述符
 * @retval 0 表示成功
 * @retval 非零 表示失败
 */
typedef int8_t (*icp20100_read_fptr_t)(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr);

/*!
 * @brief 总线通信函数指针，应映射到用户特定平台的写入函数
 *
 * @param[in]     reg_addr : 传感器的8位寄存器地址
 * @param[in]     reg_data : 写入指定地址的数据
 * @param[in]     length   : reg_data数组的长度
 * @param[in,out] intf_ptr : void指针，可用于链接接口相关回调的描述符
 * @retval 0 表示成功
 * @retval 非零 表示失败
 */
typedef int8_t (*icp20100_write_fptr_t)(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr);

/*!
 * @brief 延时函数指针，应映射到用户的延时函数
 *
 * @param period - 以微秒为单位的时间周期
 * @param[in,out] intf_ptr : void指针，可用于链接接口相关回调的描述符
 */
typedef void (*icp20100_delay_us_fptr_t)(uint32_t period, void *intf_ptr);

/*!
 * @brief 接口选择枚举
 */
enum icp20100_intf {
    ICP20100_I2C_INTF,   /*!< I2C接口 */
    ICP20100_SPI_INTF    /*!< SPI接口 */
};

/*!
 * @brief 传感器测量配置结构体
 */
struct icp20100_config {
    /*! 测量模式 (MODE_0 到 MODE_4) */
    uint8_t meas_mode;
    
    /*! 测量类型 (强制或连续) */
    uint8_t meas_type;
    
    /*! 电源模式 (正常或活动) */
    uint8_t power_mode;
    
    /*! FIFO读出模式 */
    uint8_t fifo_mode;
    
    /*! 驱动强度设置 */
    uint8_t drive_strength;
};

/*!
 * @brief 传感器数据结构体
 */
struct icp20100_data {
    /*! 压力值，单位为kPa (遵循software_imp.md格式) */
    float pressure;
    
    /*! 温度值，单位为摄氏度 */
    float temperature;
    
    /*! 原始压力数据 */
    int32_t pressure_raw;
    
    /*! 原始温度数据 */
    int32_t temperature_raw;
};

/*!
 * @brief 中断配置结构体
 */
struct icp20100_int_config {
    /*! FIFO高水位中断使能 */
    uint8_t fifo_wmk_high_en;
    
    /*! FIFO低水位中断使能 */
    uint8_t fifo_wmk_low_en;
    
    /*! FIFO溢出中断使能 */
    uint8_t fifo_overflow_en;
    
    /*! FIFO下溢中断使能 */
    uint8_t fifo_underflow_en;
    
    /*! 压力绝对阈值中断使能 */
    uint8_t press_abs_en;
    
    /*! 压力差分阈值中断使能 */
    uint8_t press_delta_en;
    
    /*! FIFO高水位级别 */
    uint8_t fifo_wmk_high;
    
    /*! FIFO低水位级别 */
    uint8_t fifo_wmk_low;
};

/*!
 * @brief ICP20100设备结构体
 */
struct icp20100_dev {
    /*! 芯片ID */
    uint8_t chip_id;
    
    /*! 设备版本 */
    uint8_t version;
    
    /*! 接口函数指针，用于I2C的设备地址和SPI的芯片选择 */
    void *intf_ptr;
    
    /*! 接口选择 */
    enum icp20100_intf intf;
    
    /*! 读取函数指针 */
    icp20100_read_fptr_t read;
    
    /*! 写入函数指针 */
    icp20100_write_fptr_t write;
    
    /*! 延时函数指针 */
    icp20100_delay_us_fptr_t delay_us;
    
    /*! 当前传感器配置 */
    struct icp20100_config config;
    
    /*! 启动序列完成标志 */
    uint8_t boot_completed;
};

/****************************************************************************/
/**\name        函数声明                                                    */
/****************************************************************************/

/*!
 * @brief 此API读取传感器的芯片ID和版本，这是验证传感器的第一步，
 * 同时也配置SPI和I2C接口的读取机制。
 *
 * @param[in,out] dev : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 * @retval ICP20100_E_DEV_NOT_FOUND -> 未找到设备
 */
int8_t icp20100_init(struct icp20100_dev *dev);

/*!
 * @brief 此API执行传感器的软复位。
 *
 * @param[in] dev : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_soft_reset(struct icp20100_dev *dev);

/*!
 * @brief 此API执行传感器正常工作所需的启动序列
 *
 * @param[in] dev : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 * @retval ICP20100_E_BOOT_FAILED -> 启动序列失败
 */
int8_t icp20100_boot_sequence(struct icp20100_dev *dev);

/*!
 * @brief 此API设置传感器的测量配置
 *
 * @param[in] config : icp20100_config结构体实例
 * @param[in] dev    : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 * @retval ICP20100_E_INVALID_CONFIG -> 无效配置
 */
int8_t icp20100_set_config(const struct icp20100_config *config, struct icp20100_dev *dev);

/*!
 * @brief 此API获取传感器的测量配置
 *
 * @param[out] config : icp20100_config结构体实例
 * @param[in]  dev    : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_get_config(struct icp20100_config *config, struct icp20100_dev *dev);

/*!
 * @brief 此API从传感器读取压力和温度数据
 *
 * @param[out] data : icp20100_data结构体实例
 * @param[in]  dev  : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_get_data(struct icp20100_data *data, struct icp20100_dev *dev);

/*!
 * @brief 此API触发一次强制测量
 *
 * @param[in] dev : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_trigger_measurement(struct icp20100_dev *dev);

/*!
 * @brief 此API设置传感器的电源模式
 *
 * @param[in] power_mode : 要设置的电源模式
 * @param[in] dev        : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_set_power_mode(uint8_t power_mode, struct icp20100_dev *dev);

/*!
 * @brief 此API获取传感器的电源模式
 *
 * @param[out] power_mode : 传感器的电源模式
 * @param[in]  dev        : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_get_power_mode(uint8_t *power_mode, struct icp20100_dev *dev);

/*!
 * @brief 此API配置中断设置
 *
 * @param[in] int_config : icp20100_int_config结构体实例
 * @param[in] dev        : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_set_int_config(const struct icp20100_int_config *int_config, struct icp20100_dev *dev);

/*!
 * @brief 此API读取中断状态
 *
 * @param[out] int_status : 中断状态寄存器值
 * @param[in]  dev        : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_get_int_status(uint8_t *int_status, struct icp20100_dev *dev);

/*!
 * @brief 此API清除中断状态
 *
 * @param[in] int_status : 要清除的中断状态位
 * @param[in] dev        : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_clear_int_status(uint8_t int_status, struct icp20100_dev *dev);

/*!
 * @brief 此API获取FIFO长度
 *
 * @param[out] fifo_length : FIFO中的样本数
 * @param[in]  dev         : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_get_fifo_length(uint8_t *fifo_length, struct icp20100_dev *dev);

/*!
 * @brief 此API从FIFO读取数据
 *
 * @param[out] data    : icp20100_data结构体数组
 * @param[in]  length  : 要读取的样本数
 * @param[in]  dev     : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_read_fifo(struct icp20100_data *data, uint8_t length, struct icp20100_dev *dev);

/*!
 * @brief 此API刷新FIFO
 *
 * @param[in] dev : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_flush_fifo(struct icp20100_dev *dev);

/*!
 * @brief 此API执行software_imp.md中描述的FIR滤波器稳定过程
 * 当使用模式0-3和FIR滤波器时，前14个样本应被丢弃
 *
 * @param[in] dev : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_fir_filter_settling(struct icp20100_dev *dev);

#ifdef __cplusplus
}
#endif /* End of CPP guard */

#endif /* ICP20100_H_ */