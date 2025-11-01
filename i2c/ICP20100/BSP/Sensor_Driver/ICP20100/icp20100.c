/**
 * @file icp20100.c
 * @brief ICP-20100 高精度气压和温度传感器驱动程序
 * @version 1.0
 * @date 2025
 * 
 * 驱动程序遵循Bosch Sensortec标准，用于MCU无关的传感器库
 * 兼容ICP-20100数据手册版本1.4
 */

/****************************************************************************/
/**\name        头文件                                                      */
/****************************************************************************/
#include "icp20100.h"

/****************************************************************************/
/**\name        内部函数声明                                                */
/****************************************************************************/

/*!
 * @brief 此内部API用于验证设备结构体指针是否为空
 */
static int8_t null_ptr_check(const struct icp20100_dev *dev);

/*!
 * @brief 此内部API读取传感器的芯片ID和版本
 */
static int8_t read_chip_id(struct icp20100_dev *dev);

/*!
 * @brief 此内部API检查是否需要启动序列
 */
static int8_t check_boot_required(struct icp20100_dev *dev, uint8_t *boot_required);

/*!
 * @brief 此内部API执行OTP读取序列
 */
static int8_t otp_read_sequence(struct icp20100_dev *dev);

/*!
 * @brief 此内部API准备设备进行OTP操作
 */
static int8_t otp_prepare_device(struct icp20100_dev *dev);

/*!
 * @brief 此内部API使能OTP接口
 */
static int8_t otp_enable_interface(struct icp20100_dev *dev);

/*!
 * @brief 此内部API配置OTP冗余读取寄存器
 */
static int8_t otp_configure_redundant_read(struct icp20100_dev *dev);

/*!
 * @brief 此内部API从OTP读取校准数据
 */
static int8_t otp_read_calibration_data(uint8_t *offset, uint8_t *gain, uint8_t *hfosc, struct icp20100_dev *dev);

/*!
 * @brief 此内部API将校准数据写入修调寄存器
 */
static int8_t otp_write_trim_registers(uint8_t offset, uint8_t gain, uint8_t hfosc, struct icp20100_dev *dev);

/*!
 * @brief 此内部API禁用OTP接口并锁定寄存器
 */
static int8_t otp_finalize_sequence(struct icp20100_dev *dev);

/*!
 * @brief 此内部API从指定地址读取OTP数据
 */
static int8_t read_otp_data(uint8_t address, uint8_t *data, struct icp20100_dev *dev);

/*!
 * @brief 此内部API等待OTP操作完成
 */
static int8_t wait_otp_ready(struct icp20100_dev *dev);

/*!
 * @brief 此内部API将原始压力数据转换为帕斯卡
 */
static float convert_pressure(int32_t raw_pressure);

/*!
 * @brief 此内部API将原始温度数据转换为摄氏度
 */
static float convert_temperature(int32_t raw_temperature);

/*!
 * @brief 此内部API从压力和温度寄存器读取原始数据
 */
static int8_t read_raw_data(int32_t *pressure_raw, int32_t *temperature_raw, struct icp20100_dev *dev);

/*!
 * @brief 此内部API等待模式同步
 */
static int8_t wait_mode_sync(struct icp20100_dev *dev);

/****************************************************************************/
/**\name        函数定义                                                    */
/****************************************************************************/

/*!
 * @brief 此API读取传感器的芯片ID和版本，这是验证传感器的第一步，
 * 同时也配置SPI和I2C接口的读取机制。
 */
int8_t icp20100_init(struct icp20100_dev *dev)
{
    int8_t rslt;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK)
    {
        /* 对于I2C接口，使用虚拟事务初始化接口 */
        if (dev->intf == ICP20100_I2C_INTF)
        {
            uint8_t dummy_reg = ICP20100_REG_DEVICE_ID;
            uint8_t dummy_data;
            
            /* 执行虚拟读取以初始化I2C接口 */
            rslt = dev->read(dummy_reg, &dummy_data, 1, dev->intf_ptr);
        }

        if (rslt == ICP20100_OK)
        {
            /* 读取并验证芯片ID */
            rslt = read_chip_id(dev);
        }

        if (rslt == ICP20100_OK)
        {
            /* 初始化默认配置 */
            dev->config.meas_mode = ICP20100_MODE_0;
            dev->config.meas_type = ICP20100_MEAS_MODE_CONTINUOUS;
            dev->config.power_mode = ICP20100_POWER_MODE_NORMAL;
            dev->config.fifo_mode = ICP20100_FIFO_PRESSURE_FIRST;
            dev->config.drive_strength = ICP20100_DRIVE_STRENGTH_1_8V_2MA;
            dev->boot_completed = 0;
        }
    }

    return rslt;
}

/*!
 * @brief 此内部API从指定地址读取OTP数据
 */
static int8_t read_otp_data(uint8_t address, uint8_t *data, struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t command_reg;

    /* 设置OTP地址 */
    rslt = dev->write(ICP20100_REG_OTP_ADDRESS, &address, 1, dev->intf_ptr);

    if (rslt == ICP20100_OK)
    {
        /* 读取当前命令寄存器并设置读取命令 */
        rslt = dev->read(ICP20100_REG_OTP_COMMAND, &command_reg, 1, dev->intf_ptr);

        if (rslt == ICP20100_OK)
        {
            command_reg = (command_reg & 0xF0) | 0x01; /* 设置读取命令 */
            rslt = dev->write(ICP20100_REG_OTP_COMMAND, &command_reg, 1, dev->intf_ptr);
        }
    }

    if (rslt == ICP20100_OK)
    {
        /* 等待OTP操作完成 */
        rslt = wait_otp_ready(dev);
    }

    if (rslt == ICP20100_OK)
    {
        /* 从OTP_RDATA寄存器读取数据 */
        rslt = dev->read(ICP20100_REG_OTP_RDATA, data, 1, dev->intf_ptr);
    }

    return rslt;
}

/*!
 * @brief 此内部API等待OTP操作完成
 */
static int8_t wait_otp_ready(struct icp20100_dev *dev)
{
    int8_t rslt = ICP20100_OK;
    uint8_t status;
    uint32_t timeout_count = 0;
    uint32_t max_timeout = ICP20100_OTP_BUSY_TIMEOUT_MS * 100; /* 转换为~10us间隔 */

    do {
        rslt = dev->read(ICP20100_REG_OTP_STATUS, &status, 1, dev->intf_ptr);

        if (rslt != ICP20100_OK)
        {
            break;
        }

        if ((status & ICP20100_OTP_BUSY_MASK) == 0)
        {
            break; /* OTP操作完成 */
        }

        dev->delay_us(10, dev->intf_ptr); /* 小延时 */
        timeout_count++;

    } while (timeout_count < max_timeout);

    if (timeout_count >= max_timeout)
    {
        rslt = ICP20100_E_COMM_FAIL;
    }

    return rslt;
}

/*!
 * @brief 此内部API将原始压力数据转换为帕斯卡
 */
static float convert_pressure(int32_t raw_pressure)
{
    /* 符号扩展20位值到32位 */
    if (raw_pressure & 0x80000)
    {
        raw_pressure |= 0xFFF00000;
    }

    /* 应用转换公式: P = (POUT/2^17)*40kPa + 70kPa */
    return ((float)raw_pressure * ICP20100_PRESS_SCALE_FACTOR) + ICP20100_PRESS_OFFSET;
}

/*!
 * @brief 此内部API将原始温度数据转换为摄氏度
 */
static float convert_temperature(int32_t raw_temperature)
{
    /* 符号扩展20位值到32位 */
    if (raw_temperature & 0x80000)
    {
        raw_temperature |= 0xFFF00000;
    }

    /* 应用转换公式: T = (TOUT/2^18)*65C + 25C */
    return ((float)raw_temperature * ICP20100_TEMP_SCALE_FACTOR) + ICP20100_TEMP_OFFSET;
}

/*!
 * @brief 此内部API从压力和温度寄存器读取原始数据
 */
static int8_t read_raw_data(int32_t *pressure_raw, int32_t *temperature_raw, struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t data[6];

    /* 执行单次6字节突发读取，从压力寄存器开始，按照数据手册说明
     * "压力优先"模式: PRESS_DATA_0, PRESS_DATA_1, PRESS_DATA_2, TEMP_DATA_0, TEMP_DATA_1, TEMP_DATA_2 */
    rslt = dev->read(ICP20100_REG_PRESS_DATA_0, data, 6, dev->intf_ptr);

    if (rslt == ICP20100_OK)
    {
        /* 从前3个字节提取压力数据
         * data[0]=PRESS_DATA_0[7:0], data[1]=PRESS_DATA_1[15:8], data[2]=PRESS_DATA_2[19:16] */
        *pressure_raw = ((int32_t)(data[2] & ICP20100_PRESS_DATA_MASK) << 16) |
                       ((int32_t)data[1] << 8) |
                       data[0];

        /* 从后3个字节提取温度数据
         * data[3]=TEMP_DATA_0[7:0], data[4]=TEMP_DATA_1[15:8], data[5]=TEMP_DATA_2[19:16] */
        *temperature_raw = ((int32_t)(data[5] & ICP20100_PRESS_DATA_MASK) << 16) |
                          ((int32_t)data[4] << 8) |
                          data[3];
    }

    return rslt;
}

/*!
 * @brief 此内部API等待模式同步
 */
static int8_t wait_mode_sync(struct icp20100_dev *dev)
{
    int8_t rslt = ICP20100_OK;
    uint8_t status;
    uint32_t timeout_count = 0;
    uint32_t max_timeout = 1000; /* 10ms超时，10us间隔 */

    do {
        rslt = dev->read(ICP20100_REG_DEVICE_STATUS, &status, 1, dev->intf_ptr);

        if (rslt != ICP20100_OK)
        {
            break;
        }

        if (status & ICP20100_MODE_SYNC_STATUS_MASK)
        {
            break; /* 模式同步完成 */
        }

        dev->delay_us(10, dev->intf_ptr);
        timeout_count++;

    } while (timeout_count < max_timeout);

    if (timeout_count >= max_timeout)
    {
        rslt = ICP20100_E_COMM_FAIL;
    }

    return rslt;
}

/*!
 * @brief 此API执行传感器的软复位
 */
int8_t icp20100_soft_reset(struct icp20100_dev *dev)
{
    int8_t rslt;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK)
    {
        /* 注意: ICP-20100没有软件复位寄存器。
         * 复位通过断电或启动序列实现。 */
        dev->boot_completed = 0;
    }

    return rslt;
}

/*!
 * @brief 此API执行传感器正常工作所需的启动序列
 */
int8_t icp20100_boot_sequence(struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t boot_required = 0;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK)
    {
        /* 检查是否需要启动序列 */
        rslt = check_boot_required(dev, &boot_required);
    }

    if ((rslt == ICP20100_OK) && boot_required)
    {
        /* 执行OTP读取序列以获取校准数据 */
        rslt = otp_read_sequence(dev);
        
        if (rslt == ICP20100_OK)
        {
            /* 标记启动序列已完成 */
            uint8_t boot_status = 0x01;
            rslt = dev->write(ICP20100_REG_OTP_STATUS2, &boot_status, 1, dev->intf_ptr);
            
            if (rslt == ICP20100_OK)
            {
                dev->boot_completed = 1;
            }
        }
    }
    else if (rslt == ICP20100_OK)
    {
        /* 启动序列已完成 */
        dev->boot_completed = 1;
    }

    return rslt;
}

/*!
 * @brief 此API设置传感器的测量配置
 */
int8_t icp20100_set_config(const struct icp20100_config *config, struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK && config == NULL)
    {
        rslt = ICP20100_E_NULL_PTR;
    }

    if (rslt == ICP20100_OK)
    {
        /* 验证配置参数 */
        if (config->meas_mode > ICP20100_MODE_4 ||
            config->meas_type > ICP20100_MEAS_MODE_CONTINUOUS ||
            config->power_mode > ICP20100_POWER_MODE_ACTIVE ||
            config->fifo_mode > ICP20100_FIFO_TEMP_ONLY)
        {
            rslt = ICP20100_E_INVALID_CONFIG;
        }
    }

    if (rslt == ICP20100_OK)
    {
        /* 在更改配置前等待模式同步 */
        rslt = wait_mode_sync(dev);
    }

    if (rslt == ICP20100_OK)
    {
        /* 使用正确的位位置配置测量模式和设置 */
        reg_data = (config->meas_mode << ICP20100_MEAS_CONFIG_POS) |
                   (config->meas_type << ICP20100_MEAS_MODE_POS) |
                   (config->power_mode << ICP20100_POWER_MODE_POS) |
                   (config->fifo_mode << ICP20100_FIFO_READOUT_MODE_POS);
        
        rslt = dev->write(ICP20100_REG_MODE_SELECT, &reg_data, 1, dev->intf_ptr);
    }

    if (rslt == ICP20100_OK)
    {
        /* 设置驱动强度 */
        rslt = dev->write(ICP20100_REG_IO_DRIVE_STRENGTH, &config->drive_strength, 1, dev->intf_ptr);
    }

    if (rslt == ICP20100_OK)
    {
        /* 更新设备配置 */
        dev->config = *config;
    }

    return rslt;
}

/*!
 * @brief 此API获取传感器的测量配置
 */
int8_t icp20100_get_config(struct icp20100_config *config, struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK && config == NULL)
    {
        rslt = ICP20100_E_NULL_PTR;
    }

    if (rslt == ICP20100_OK)
    {
        /* 读取模式选择寄存器 */
        rslt = dev->read(ICP20100_REG_MODE_SELECT, &reg_data, 1, dev->intf_ptr);
        
        if (rslt == ICP20100_OK)
        {
            config->meas_mode = (reg_data & ICP20100_MEAS_CONFIG_MASK) >> ICP20100_MEAS_CONFIG_POS;
            config->meas_type = (reg_data & ICP20100_MEAS_MODE_MASK) >> ICP20100_MEAS_MODE_POS;
            config->power_mode = (reg_data & ICP20100_POWER_MODE_MASK) >> ICP20100_POWER_MODE_POS;
            config->fifo_mode = (reg_data & ICP20100_FIFO_READOUT_MODE_MASK) >> ICP20100_FIFO_READOUT_MODE_POS;
        }
    }

    if (rslt == ICP20100_OK)
    {
        /* 读取驱动强度寄存器 */
        rslt = dev->read(ICP20100_REG_IO_DRIVE_STRENGTH, &reg_data, 1, dev->intf_ptr);
        
        if (rslt == ICP20100_OK)
        {
            config->drive_strength = reg_data & ICP20100_DRIVE_STRENGTH_MASK;
        }
    }

    return rslt;
}

/*!
 * @brief 此API从传感器读取压力和温度数据
 */
int8_t icp20100_get_data(struct icp20100_data *data, struct icp20100_dev *dev)
{
    int8_t rslt;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK && data == NULL)
    {
        rslt = ICP20100_E_NULL_PTR;
    }

    if (rslt == ICP20100_OK)
    {
        /* 读取原始压力和温度数据 */
        rslt = read_raw_data(&data->pressure_raw, &data->temperature_raw, dev);
    }

    if (rslt == ICP20100_OK)
    {
        /* 将原始数据转换为物理单位 */
        data->pressure = convert_pressure(data->pressure_raw);
        data->temperature = convert_temperature(data->temperature_raw);
    }

    return rslt;
}

/*!
 * @brief 此API触发一次强制测量
 */
int8_t icp20100_trigger_measurement(struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK)
    {
        /* 等待模式同步 */
        rslt = wait_mode_sync(dev);
    }

    if (rslt == ICP20100_OK)
    {
        /* 读取当前模式选择寄存器 */
        rslt = dev->read(ICP20100_REG_MODE_SELECT, &reg_data, 1, dev->intf_ptr);
    }

    if (rslt == ICP20100_OK)
    {
        /* 设置强制测量触发位 */
        reg_data |= ICP20100_FORCED_MEAS_TRIGGER_MASK;
        rslt = dev->write(ICP20100_REG_MODE_SELECT, &reg_data, 1, dev->intf_ptr);
    }

    return rslt;
}

/*!
 * @brief 此API设置传感器的电源模式
 */
int8_t icp20100_set_power_mode(uint8_t power_mode, struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK)
    {
        if (power_mode > ICP20100_POWER_MODE_ACTIVE)
        {
            rslt = ICP20100_E_INVALID_CONFIG;
        }
    }

    if (rslt == ICP20100_OK)
    {
        /* 等待模式同步 */
        rslt = wait_mode_sync(dev);
    }

    if (rslt == ICP20100_OK)
    {
        /* 读取当前模式选择寄存器 */
        rslt = dev->read(ICP20100_REG_MODE_SELECT, &reg_data, 1, dev->intf_ptr);
    }

    if (rslt == ICP20100_OK)
    {
        /* 更新电源模式位 */
        reg_data = (reg_data & ~ICP20100_POWER_MODE_MASK) | (power_mode << 2);
        rslt = dev->write(ICP20100_REG_MODE_SELECT, &reg_data, 1, dev->intf_ptr);
    }

    if (rslt == ICP20100_OK)
    {
        /* 更新设备配置 */
        dev->config.power_mode = power_mode;
        
        /* 等待电源模式更改，如果切换到活动模式 */
        if (power_mode == ICP20100_POWER_MODE_ACTIVE)
        {
            dev->delay_us(ICP20100_POWER_UP_TIME_MS * 1000, dev->intf_ptr);
        }
    }

    return rslt;
}

/*!
 * @brief 此API获取传感器的电源模式
 */
int8_t icp20100_get_power_mode(uint8_t *power_mode, struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK && power_mode == NULL)
    {
        rslt = ICP20100_E_NULL_PTR;
    }

    if (rslt == ICP20100_OK)
    {
        /* 读取模式选择寄存器 */
        rslt = dev->read(ICP20100_REG_MODE_SELECT, &reg_data, 1, dev->intf_ptr);
    }

    if (rslt == ICP20100_OK)
    {
        /* 提取电源模式位 */
        *power_mode = (reg_data & ICP20100_POWER_MODE_MASK) >> 2;
    }

    return rslt;
}

/*!
 * @brief 此API配置中断设置
 */
int8_t icp20100_set_int_config(const struct icp20100_int_config *int_config, struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t int_mask = 0;
    uint8_t fifo_config = 0;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK && int_config == NULL)
    {
        rslt = ICP20100_E_NULL_PTR;
    }

    if (rslt == ICP20100_OK)
    {
        /* 配置中断屏蔽寄存器 (1 = 屏蔽/禁用, 0 = 使能) */
        int_mask = (int_config->fifo_overflow_en ? 0 : ICP20100_INT_FIFO_OVERFLOW_MASK) |
                   (int_config->fifo_underflow_en ? 0 : ICP20100_INT_FIFO_UNDERFLOW_MASK) |
                   (int_config->fifo_wmk_high_en ? 0 : ICP20100_INT_FIFO_WMK_HIGH_MASK) |
                   (int_config->fifo_wmk_low_en ? 0 : ICP20100_INT_FIFO_WMK_LOW_MASK) |
                   (int_config->press_abs_en ? 0 : ICP20100_INT_PRESS_ABS_MASK) |
                   (int_config->press_delta_en ? 0 : ICP20100_INT_PRESS_DELTA_MASK) |
                   (1 << 7); /* 保留位设置为1 */

        rslt = dev->write(ICP20100_REG_INTERRUPT_MASK, &int_mask, 1, dev->intf_ptr);
    }

    if (rslt == ICP20100_OK)
    {
        /* 配置FIFO水位级别 */
        fifo_config = (int_config->fifo_wmk_high << 4) | int_config->fifo_wmk_low;
        rslt = dev->write(ICP20100_REG_FIFO_CONFIG, &fifo_config, 1, dev->intf_ptr);
    }

    return rslt;
}

/*!
 * @brief 此API读取中断状态
 */
int8_t icp20100_get_int_status(uint8_t *int_status, struct icp20100_dev *dev)
{
    int8_t rslt;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK && int_status == NULL)
    {
        rslt = ICP20100_E_NULL_PTR;
    }

    if (rslt == ICP20100_OK)
    {
        /* 读取中断状态寄存器 */
        rslt = dev->read(ICP20100_REG_INTERRUPT_STATUS, int_status, 1, dev->intf_ptr);
    }

    return rslt;
}

/*!
 * @brief 此API清除中断状态
 */
int8_t icp20100_clear_int_status(uint8_t int_status, struct icp20100_dev *dev)
{
    int8_t rslt;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK)
    {
        /* 写1清除中断状态位 (W1C) */
        rslt = dev->write(ICP20100_REG_INTERRUPT_STATUS, &int_status, 1, dev->intf_ptr);
    }

    return rslt;
}

/*!
 * @brief 此API获取FIFO长度
 */
int8_t icp20100_get_fifo_length(uint8_t *fifo_length, struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK && fifo_length == NULL)
    {
        rslt = ICP20100_E_NULL_PTR;
    }

    if (rslt == ICP20100_OK)
    {
        /* 读取FIFO填充寄存器 */
        rslt = dev->read(ICP20100_REG_FIFO_FILL, &reg_data, 1, dev->intf_ptr);
    }

    if (rslt == ICP20100_OK)
    {
        /* 提取FIFO级别 */
        *fifo_length = reg_data & ICP20100_FIFO_LEVEL_MASK;
    }

    return rslt;
}

/*!
 * @brief 此API从FIFO读取数据
 */
int8_t icp20100_read_fifo(struct icp20100_data *data, uint8_t length, struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t fifo_level;
    uint8_t samples_to_read;
    uint8_t fifo_data[6];
    uint8_t i;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK && data == NULL)
    {
        rslt = ICP20100_E_NULL_PTR;
    }

    if (rslt == ICP20100_OK)
    {
        /* 获取当前FIFO级别 */
        rslt = icp20100_get_fifo_length(&fifo_level, dev);
    }

    if (rslt == ICP20100_OK)
    {
        /* 确定要读取的样本数 */
        samples_to_read = (length < fifo_level) ? length : fifo_level;

        /* 从FIFO读取样本 */
        for (i = 0; i < samples_to_read; i++)
        {
            /* 根据FIFO模式确定起始寄存器和要读取的字节数 */
            uint8_t start_register;
            uint8_t bytes_to_read;
            
            if (dev->config.fifo_mode == ICP20100_FIFO_PRESSURE_FIRST)
            {
                start_register = ICP20100_REG_PRESS_DATA_0;
                bytes_to_read = 6; /* 压力 + 温度 */
            }
            else /* TEMP_ONLY */
            {
                start_register = ICP20100_REG_TEMP_DATA_0;
                bytes_to_read = 3; /* 仅温度 */
            }
            
            rslt = dev->read(start_register, fifo_data, bytes_to_read, dev->intf_ptr);
            
            if (rslt != ICP20100_OK)
            {
                break;
            }

            /* 根据FIFO模式解析数据 - 遵循software_imp.md突发读取格式
             * FIFO数据从PRESS_DATA_0自动递增到TEMP_DATA_2并循环 */
            if (dev->config.fifo_mode == ICP20100_FIFO_PRESSURE_FIRST)
            {
                /* 压力优先模式: PRESS_DATA_0, PRESS_DATA_1, PRESS_DATA_2, TEMP_DATA_0, TEMP_DATA_1, TEMP_DATA_2 */
                data[i].pressure_raw = ((int32_t)(fifo_data[2] & ICP20100_PRESS_DATA_MASK) << 16) |
                                       ((int32_t)fifo_data[1] << 8) |
                                       fifo_data[0];
                data[i].temperature_raw = ((int32_t)(fifo_data[5] & ICP20100_PRESS_DATA_MASK) << 16) |
                                          ((int32_t)fifo_data[4] << 8) |
                                          fifo_data[3];
            }
            else /* TEMP_ONLY */
            {
                /* 仅温度模式: TEMP_DATA_0, TEMP_DATA_1, TEMP_DATA_2 */
                data[i].temperature_raw = ((int32_t)(fifo_data[2] & ICP20100_PRESS_DATA_MASK) << 16) |
                                          ((int32_t)fifo_data[1] << 8) |
                                          fifo_data[0];
                data[i].pressure_raw = 0; /* 无压力数据 */
            }

            if (rslt == ICP20100_OK)
            {
                /* 将原始数据转换为物理单位 */
                data[i].pressure = convert_pressure(data[i].pressure_raw);
                data[i].temperature = convert_temperature(data[i].temperature_raw);
            }
        }
    }

    return rslt;
}

/*!
 * @brief 此API刷新FIFO
 */
int8_t icp20100_flush_fifo(struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK)
    {
        /* 读取当前FIFO填充寄存器 */
        rslt = dev->read(ICP20100_REG_FIFO_FILL, &reg_data, 1, dev->intf_ptr);
    }

    if (rslt == ICP20100_OK)
    {
        /* 设置刷新位 */
        reg_data |= ICP20100_FIFO_FLUSH_MASK;
        rslt = dev->write(ICP20100_REG_FIFO_FILL, &reg_data, 1, dev->intf_ptr);
    }

    return rslt;
}

/****************************************************************************/
/**\name        内部函数定义                                                */
/****************************************************************************/

/*!
 * @brief 此内部API用于验证设备结构体指针是否为空
 */
static int8_t null_ptr_check(const struct icp20100_dev *dev)
{
    int8_t rslt = ICP20100_OK;

    if ((dev == NULL) || (dev->read == NULL) || (dev->write == NULL) || (dev->delay_us == NULL))
    {
        rslt = ICP20100_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief 此内部API读取传感器的芯片ID和版本
 */
static int8_t read_chip_id(struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t chip_id;
    uint8_t version;

    /* 读取芯片ID */
    rslt = dev->read(ICP20100_REG_DEVICE_ID, &chip_id, 1, dev->intf_ptr);

    if (rslt == ICP20100_OK)
    {
        if (chip_id == ICP20100_CHIP_ID)
        {
            dev->chip_id = chip_id;
            
            /* 读取版本 */
            rslt = dev->read(ICP20100_REG_VERSION, &version, 1, dev->intf_ptr);
            
            if (rslt == ICP20100_OK)
            {
                dev->version = version;
            }
        }
        else
        {
            rslt = ICP20100_E_DEV_NOT_FOUND;
        }
    }

    return rslt;
}

/*!
 * @brief 此内部API检查是否需要启动序列
 */
static int8_t check_boot_required(struct icp20100_dev *dev, uint8_t *boot_required)
{
    int8_t rslt;
    uint8_t boot_status;

    *boot_required = 0;

    /* 仅版本A需要启动序列 */
    if (dev->version == ICP20100_VERSION_A)
    {
        /* 读取启动状态 */
        rslt = dev->read(ICP20100_REG_OTP_STATUS2, &boot_status, 1, dev->intf_ptr);
        
        if (rslt == ICP20100_OK)
        {
            /* 检查启动序列是否已完成 */
            if ((boot_status & ICP20100_BOOT_UP_STATUS_MASK) == 0)
            {
                *boot_required = 1;
            }
        }
    }
    else
    {
        /* 版本B不需要启动序列 */
        rslt = ICP20100_OK;
    }

    return rslt;
}

/*!
 * @brief 此内部API执行OTP读取序列
 */
static int8_t otp_read_sequence(struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t offset, gain, hfosc;

    /* 准备设备进行OTP操作 */
    rslt = otp_prepare_device(dev);

    if (rslt == ICP20100_OK)
    {
        /* 使能OTP接口 */
        rslt = otp_enable_interface(dev);
    }

    if (rslt == ICP20100_OK)
    {
        /* 配置冗余读取寄存器 */
        rslt = otp_configure_redundant_read(dev);
    }

    if (rslt == ICP20100_OK)
    {
        /* 读取校准数据 */
        rslt = otp_read_calibration_data(&offset, &gain, &hfosc, dev);
    }

    if (rslt == ICP20100_OK)
    {
        /* 将校准数据写入修调寄存器 */
        rslt = otp_write_trim_registers(offset, gain, hfosc, dev);
    }

    /* 即使前面的步骤失败，也始终尝试完成序列 */
    int8_t finalize_rslt = otp_finalize_sequence(dev);
    
    /* 返回第一个发生的错误，或者如果一切正常则返回完成结果 */
    return (rslt != ICP20100_OK) ? rslt : finalize_rslt;
}

/*!
 * @brief 此内部API准备设备进行OTP操作
 */
static int8_t otp_prepare_device(struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    /* 使能电源模式 (设置位2为1表示活动模式) */
    reg_data = (ICP20100_POWER_MODE_ACTIVE << ICP20100_POWER_MODE_POS);
    rslt = dev->write(ICP20100_REG_MODE_SELECT, &reg_data, 1, dev->intf_ptr);
    
    if (rslt == ICP20100_OK)
    {
        dev->delay_us(ICP20100_POWER_UP_TIME_MS * 1000, dev->intf_ptr);
        
        /* 解锁主寄存器 */
        reg_data = ICP20100_MASTER_LOCK_UNLOCK;
        rslt = dev->write(ICP20100_REG_MASTER_LOCK, &reg_data, 1, dev->intf_ptr);
    }

    return rslt;
}

/*!
 * @brief 此内部API使能OTP接口
 */
static int8_t otp_enable_interface(struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    /* 使能OTP和写入开关 */
    rslt = dev->read(ICP20100_REG_OTP_CONFIG1, &reg_data, 1, dev->intf_ptr);
    
    if (rslt == ICP20100_OK)
    {
        reg_data |= (ICP20100_OTP_ENABLE_MASK | ICP20100_OTP_WRITE_SWITCH_MASK);
        rslt = dev->write(ICP20100_REG_OTP_CONFIG1, &reg_data, 1, dev->intf_ptr);
        
        if (rslt == ICP20100_OK)
        {
            dev->delay_us(ICP20100_OTP_WAIT_TIME_US, dev->intf_ptr);
            
            /* 切换OTP复位 */
            rslt = dev->read(ICP20100_REG_OTP_DBG2, &reg_data, 1, dev->intf_ptr);
            
            if (rslt == ICP20100_OK)
            {
                reg_data |= ICP20100_OTP_RESET_MASK;
                rslt = dev->write(ICP20100_REG_OTP_DBG2, &reg_data, 1, dev->intf_ptr);
                
                if (rslt == ICP20100_OK)
                {
                    dev->delay_us(ICP20100_OTP_WAIT_TIME_US, dev->intf_ptr);
                    
                    reg_data &= ~ICP20100_OTP_RESET_MASK;
                    rslt = dev->write(ICP20100_REG_OTP_DBG2, &reg_data, 1, dev->intf_ptr);
                    
                    if (rslt == ICP20100_OK)
                    {
                        dev->delay_us(ICP20100_OTP_WAIT_TIME_US, dev->intf_ptr);
                    }
                }
            }
        }
    }

    return rslt;
}

/*!
 * @brief 此内部API配置OTP冗余读取寄存器
 */
static int8_t otp_configure_redundant_read(struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    /* 配置MRA寄存器 */
    reg_data = 0x04;
    rslt = dev->write(ICP20100_REG_OTP_MRA_LSB, &reg_data, 1, dev->intf_ptr);
    
    if (rslt == ICP20100_OK)
    {
        rslt = dev->write(ICP20100_REG_OTP_MRA_MSB, &reg_data, 1, dev->intf_ptr);
    }

    /* 配置MRB寄存器 */
    if (rslt == ICP20100_OK)
    {
        reg_data = 0x21;
        rslt = dev->write(ICP20100_REG_OTP_MRB_LSB, &reg_data, 1, dev->intf_ptr);
        
        if (rslt == ICP20100_OK)
        {
            reg_data = 0x20;
            rslt = dev->write(ICP20100_REG_OTP_MRB_MSB, &reg_data, 1, dev->intf_ptr);
        }
    }

    /* 配置MR寄存器 */
    if (rslt == ICP20100_OK)
    {
        reg_data = 0x10;
        rslt = dev->write(ICP20100_REG_OTP_MR_LSB, &reg_data, 1, dev->intf_ptr);
        
        if (rslt == ICP20100_OK)
        {
            reg_data = 0x80;
            rslt = dev->write(ICP20100_REG_OTP_MR_MSB, &reg_data, 1, dev->intf_ptr);
        }
    }

    return rslt;
}

/*!
 * @brief 此内部API从OTP读取校准数据
 */
static int8_t otp_read_calibration_data(uint8_t *offset, uint8_t *gain, uint8_t *hfosc, struct icp20100_dev *dev)
{
    int8_t rslt;

    /* 从地址0xF8读取偏移校准数据 */
    rslt = read_otp_data(0xF8, offset, dev);

    if (rslt == ICP20100_OK)
    {
        /* 从地址0xF9读取增益校准数据 */
        rslt = read_otp_data(0xF9, gain, dev);
    }

    if (rslt == ICP20100_OK)
    {
        /* 从地址0xFA读取HFosc校准数据 */
        rslt = read_otp_data(0xFA, hfosc, dev);
    }

    return rslt;
}

/*!
 * @brief 此内部API将校准数据写入修调寄存器
 */
static int8_t otp_write_trim_registers(uint8_t offset, uint8_t gain, uint8_t hfosc, struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t trim_data;

    /* 将偏移写入TRIM1_MSB */
    rslt = dev->read(ICP20100_REG_TRIM1_MSB, &trim_data, 1, dev->intf_ptr);
    
    if (rslt == ICP20100_OK)
    {
        trim_data = (trim_data & 0xC0) | (offset & 0x3F);
        rslt = dev->write(ICP20100_REG_TRIM1_MSB, &trim_data, 1, dev->intf_ptr);
    }

    /* 将增益写入TRIM2_MSB */
    if (rslt == ICP20100_OK)
    {
        rslt = dev->read(ICP20100_REG_TRIM2_MSB, &trim_data, 1, dev->intf_ptr);
        
        if (rslt == ICP20100_OK)
        {
            trim_data = (trim_data & 0x8F) | ((gain & 0x07) << 4);
            rslt = dev->write(ICP20100_REG_TRIM2_MSB, &trim_data, 1, dev->intf_ptr);
        }
    }

    /* 将HFosc写入TRIM2_LSB */
    if (rslt == ICP20100_OK)
    {
        rslt = dev->read(ICP20100_REG_TRIM2_LSB, &trim_data, 1, dev->intf_ptr);
        
        if (rslt == ICP20100_OK)
        {
            trim_data = (trim_data & 0x80) | (hfosc & 0x7F);
            rslt = dev->write(ICP20100_REG_TRIM2_LSB, &trim_data, 1, dev->intf_ptr);
        }
    }

    return rslt;
}

/*!
 * @brief 此内部API禁用OTP接口并锁定寄存器
 */
static int8_t otp_finalize_sequence(struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    /* 禁用OTP和写入开关 */
    rslt = dev->read(ICP20100_REG_OTP_CONFIG1, &reg_data, 1, dev->intf_ptr);
    
    if (rslt == ICP20100_OK)
    {
        reg_data &= ~(ICP20100_OTP_ENABLE_MASK | ICP20100_OTP_WRITE_SWITCH_MASK);
        rslt = dev->write(ICP20100_REG_OTP_CONFIG1, &reg_data, 1, dev->intf_ptr);
        
        if (rslt == ICP20100_OK)
        {
            dev->delay_us(ICP20100_OTP_WAIT_TIME_US, dev->intf_ptr);
            
            /* 锁定主寄存器 */
            reg_data = 0x00;
            rslt = dev->write(ICP20100_REG_MASTER_LOCK, &reg_data, 1, dev->intf_ptr);
            
            if (rslt == ICP20100_OK)
            {
                /* 返回待机模式 */
                reg_data = 0x00;
                rslt = dev->write(ICP20100_REG_MODE_SELECT, &reg_data, 1, dev->intf_ptr);
            }
        }
    }

    return rslt;
}

/*!
 * @brief 此API执行software_imp.md中描述的FIR滤波器稳定过程
 */
int8_t icp20100_fir_filter_settling(struct icp20100_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;
    uint8_t int_status;
    uint16_t timeout_count = 0;
    const uint16_t max_timeout = 10000; /* 10秒超时 */

    /* 检查设备结构体中的空指针 */
    rslt = null_ptr_check(dev);

    if (rslt == ICP20100_OK)
    {
        /* 步骤1: 配置FIFO高水位为14个样本 */
        reg_data = (14 << 4); /* FIFO_WM_HIGH = 14 */
        rslt = dev->write(ICP20100_REG_FIFO_CONFIG, &reg_data, 1, dev->intf_ptr);
    }

    if (rslt == ICP20100_OK)
    {
        /* 步骤2: 取消屏蔽水位高中断 */
        rslt = dev->read(ICP20100_REG_INTERRUPT_MASK, &reg_data, 1, dev->intf_ptr);
        if (rslt == ICP20100_OK)
        {
            reg_data &= ~ICP20100_INT_FIFO_WMK_HIGH_MASK; /* 取消屏蔽(使能)中断 */
            rslt = dev->write(ICP20100_REG_INTERRUPT_MASK, &reg_data, 1, dev->intf_ptr);
        }
    }

    if (rslt == ICP20100_OK)
    {
        /* 步骤3: 等待中断发生(表示已收集14个样本) */
        do {
            rslt = dev->read(ICP20100_REG_INTERRUPT_STATUS, &int_status, 1, dev->intf_ptr);
            
            if (rslt != ICP20100_OK)
            {
                break;
            }

            if (int_status & ICP20100_INT_FIFO_WMK_HIGH_MASK)
            {
                /* 步骤4: 清除中断 */
                reg_data = ICP20100_INT_FIFO_WMK_HIGH_MASK;
                rslt = dev->write(ICP20100_REG_INTERRUPT_STATUS, &reg_data, 1, dev->intf_ptr);
                break; /* 中断已发生，滤波器已稳定 */
            }

            dev->delay_us(1000, dev->intf_ptr); /* 1ms延时 */
            timeout_count++;

        } while (timeout_count < max_timeout);

        if (timeout_count >= max_timeout)
        {
            rslt = ICP20100_E_COMM_FAIL;
        }
    }

    if (rslt == ICP20100_OK)
    {
        /* 步骤5: 停止测量 */
        reg_data = 0x00;
        rslt = dev->write(ICP20100_REG_MODE_SELECT, &reg_data, 1, dev->intf_ptr);
    }

    if (rslt == ICP20100_OK)
    {
        /* 步骤6: 刷新FIFO */
        rslt = icp20100_flush_fifo(dev);
    }

    return rslt;
}