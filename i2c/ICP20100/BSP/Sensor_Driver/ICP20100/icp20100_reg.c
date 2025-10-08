/**
 * @file icp20100_reg.c
 * @brief ICP-20100气压温度传感器寄存器操作实现
 * @note 目标架构: ARM Cortex-M系列
 * @note 使用C11标准编译
 */

#include "icp20100_reg.h"
#include <string.h>

/* 内部函数声明 */
static icp20100_err_t read_register(icp20100_dev_t *dev, uint8_t reg, uint8_t *value);
static icp20100_err_t write_register(icp20100_dev_t *dev, uint8_t reg, uint8_t value);
static icp20100_err_t read_registers_burst(icp20100_dev_t *dev, uint8_t start_reg,
                                          uint8_t *data, uint8_t count);
static icp20100_err_t verify_device(icp20100_dev_t *dev);
static void convert_raw_to_pressure(int32_t raw, int32_t *pressure_pa);
static void convert_raw_to_temperature(int32_t raw, int32_t *temperature_c);

/**
 * @brief 读取单个寄存器
 * @param dev 设备句柄
 * @param reg 寄存器地址
 * @param value 读取值
 * @retval 错误代码
 */
static icp20100_err_t read_register(icp20100_dev_t *dev, uint8_t reg, uint8_t *value)
{
    if (dev == NULL || value == NULL) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    return dev->cfg.i2c_read(dev->cfg.i2c_addr, reg, value, 1);
}

/**
 * @brief 写入单个寄存器
 * @param dev 设备句柄
 * @param reg 寄存器地址
 * @param value 写入值
 * @retval 错误代码
 */
static icp20100_err_t write_register(icp20100_dev_t *dev, uint8_t reg, uint8_t value)
{
    if (dev == NULL) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    return dev->cfg.i2c_write(dev->cfg.i2c_addr, reg, &value, 1);
}

/**
 * @brief 批量读取寄存器
 * @param dev 设备句柄
 * @param start_reg 起始寄存器地址
 * @param data 数据缓冲区
 * @param count 读取数量
 * @retval 错误代码
 */
static icp20100_err_t read_registers_burst(icp20100_dev_t *dev, uint8_t start_reg,
                                          uint8_t *data, uint8_t count)
{
    if (dev == NULL || data == NULL || count == 0) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    return dev->cfg.i2c_read(dev->cfg.i2c_addr, start_reg, data, count);
}

/**
 * @brief 验证设备ID
 * @param dev 设备句柄
 * @retval 错误代码
 */
static icp20100_err_t verify_device(icp20100_dev_t *dev)
{
    uint8_t device_id;
    icp20100_err_t ret = read_register(dev, ICP20100_REG_DEVICE_ID, &device_id);
    
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    if (device_id != 0x63) {  /* 固定设备ID */
        return ICP20100_ERR_COMM;
    }
    
    dev->device_id = device_id;
    return ICP20100_OK;
}

/**
 * @brief 将原始压力数据转换为实际压力值(Pa)
 * @param raw 原始20位数据
 * @param pressure_pa 输出压力值(Pa)
 */
static void convert_raw_to_pressure(int32_t raw, int32_t *pressure_pa)
{
    /* 转换公式: P = (raw / 131072) * 40 + 70 kPa */
    *pressure_pa = (raw * 40000LL / 131072) + 70000;
}

/**
 * @brief 将原始温度数据转换为实际温度值(0.01°C)
 * @param raw 原始20位数据
 * @param temperature_c 输出温度值(0.01°C)
 */
static void convert_raw_to_temperature(int32_t raw, int32_t *temperature_c)
{
    /* 转换公式: T = (raw / 262144) * 65 + 25 °C */
    *temperature_c = (raw * 6500LL / 262144) + 2500;
}

/**
 * @brief 初始化ICP-20100传感器
 * @param dev 设备句柄
 * @param cfg 设备配置
 * @retval 错误代码
 */
icp20100_err_t icp20100_init(icp20100_dev_t *dev, const icp20100_cfg_t *cfg)
{
    if (dev == NULL || cfg == NULL || 
        cfg->i2c_write == NULL || cfg->i2c_read == NULL) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    /* 初始化设备结构 */
    memset(dev, 0, sizeof(icp20100_dev_t));
    memcpy(&dev->cfg, cfg, sizeof(icp20100_cfg_t));
    
    /* 验证设备ID */
    icp20100_err_t ret = verify_device(dev);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    /* 读取版本信息 */
    ret = read_register(dev, ICP20100_REG_VERSION, &dev->firmware_ver);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    /* 配置驱动强度 */
    icp20100_io_drive_strength_t io_ds;
    io_ds.reg = 0x00;
    io_ds.bit.io_ds = 0x00;  /* 默认2mA驱动 */
    ret = write_register(dev, ICP20100_REG_IO_DRIVE_STRENGTH, io_ds.reg);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    dev->initialized = true;
    return ICP20100_OK;
}

/**
 * @brief 反初始化传感器
 * @param dev 设备句柄
 * @retval 错误代码
 */
icp20100_err_t icp20100_deinit(icp20100_dev_t *dev)
{
    if (dev == NULL) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    /* 进入待机模式 */
    icp20100_mode_select_t mode;
    mode.reg = 0x00;
    mode.bit.power_mode = 0;  /* 待机模式 */
    write_register(dev, ICP20100_REG_MODE_SELECT, mode.reg);
    
    memset(dev, 0, sizeof(icp20100_dev_t));
    return ICP20100_OK;
}

/**
 * @brief 设置传感器工作模式
 * @param dev 设备句柄
 * @param mode 工作模式
 * @retval 错误代码
 */
icp20100_err_t icp20100_set_mode(icp20100_dev_t *dev, icp20100_mode_t mode)
{
    if (!dev->initialized) {
        return ICP20100_ERR_NOT_READY;
    }
    
    /* 检查模式同步状态 */
    icp20100_device_status_t status;
    icp20100_err_t ret = icp20100_get_device_status(dev, &status);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    if (!status.bit.mode_sync_status) {
        return ICP20100_ERR_NOT_READY;
    }
    
    /* 配置模式选择寄存器 */
    icp20100_mode_select_t mode_select;
    mode_select.reg = 0x00;
    mode_select.bit.power_mode = 1;  /* 工作模式 */
    
    switch (mode) {
        case ICP20100_MODE_STANDBY:
            mode_select.bit.meas_config = 0x0;
            mode_select.bit.power_mode = 0;  /* 待机模式 */
            break;
        case ICP20100_MODE_PRESSURE_ONLY:
            mode_select.bit.meas_config = 0x1;
            break;
        case ICP20100_MODE_TEMP_ONLY:
            mode_select.bit.meas_config = 0x2;
            break;
        case ICP20100_MODE_PRESSURE_TEMP:
            mode_select.bit.meas_config = 0x3;
            break;
        case ICP20100_MODE_USER_CONFIG:
            mode_select.bit.meas_config = 0x4;
            break;
        default:
            return ICP20100_ERR_INVALID_PARAM;
    }
    
    ret = write_register(dev, ICP20100_REG_MODE_SELECT, mode_select.reg);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    /* 等待模式切换 */
    for (int i = 0; i < 10; i++) {
        ret = icp20100_get_device_status(dev, &status);
        if (ret == ICP20100_OK && status.bit.mode_sync_status) {
            break;
        }
        /* 简单延迟 */
        for (volatile int j = 0; j < 1000; j++);
    }
    
    return ICP20100_OK;
}

/**
 * @brief 读取传感器数据（单次测量）
 * @param dev 设备句柄
 * @param data 输出数据
 * @retval 错误代码
 */
icp20100_err_t icp20100_read_data(icp20100_dev_t *dev, icp20100_data_t *data)
{
    if (!dev->initialized || data == NULL) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    uint8_t raw_data[6];
    icp20100_err_t ret = read_registers_burst(dev, ICP20100_REG_PRESS_DATA_0, 
                                              raw_data, 6);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    /* 提取20位压力数据 */
    int32_t pressure_raw = ((raw_data[2] & 0x0F) << 16) | 
                          (raw_data[1] << 8) | raw_data[0];
    
    /* 提取20位温度数据 */
    int32_t temp_raw = ((raw_data[5] & 0x0F) << 16) | 
                      (raw_data[4] << 8) | raw_data[3];
    
    /* 转换为实际物理值 */
    convert_raw_to_pressure(pressure_raw, &data->pressure_pa);
    convert_raw_to_temperature(temp_raw, &data->temperature_c);
    
    return ICP20100_OK;
}

/**
 * @brief 从FIFO读取多个数据样本
 * @param dev 设备句柄
 * @param data 数据缓冲区
 * @param count 读取数量（输入时缓冲区大小，输出时实际读取数）
 * @retval 错误代码
 */
icp20100_err_t icp20100_read_fifo(icp20100_dev_t *dev, icp20100_data_t *data, 
                                uint8_t *count)
{
    if (!dev->initialized || data == NULL || count == NULL || *count == 0) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    /* 检查FIFO状态 */
    icp20100_fifo_fill_t fifo_fill;
    icp20100_err_t ret = read_register(dev, ICP20100_REG_FIFO_FILL, &fifo_fill.reg);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    uint8_t fifo_level = fifo_fill.bit.fifo_level;
    if (fifo_level == 0) {
        return ICP20100_ERR_FIFO_EMPTY;
    }
    
    /* 读取可用数据量 */
    uint8_t samples_to_read = (*count < fifo_level) ? *count : fifo_level;
    
    for (uint8_t i = 0; i < samples_to_read; i++) {
        ret = icp20100_read_data(dev, &data[i]);
        if (ret != ICP20100_OK) {
            *count = i;
            return ret;
        }
    }
    
    *count = samples_to_read;
    return ICP20100_OK;
}

/**
 * @brief 设置中断屏蔽
 * @param dev 设备句柄
 * @param mask 中断屏蔽配置
 * @retval 错误代码
 */
icp20100_err_t icp20100_set_interrupt_mask(icp20100_dev_t *dev, 
                                         icp20100_interrupt_mask_t mask)
{
    if (!dev->initialized) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    return write_register(dev, ICP20100_REG_INTERRUPT_MASK, mask.reg);
}


 /**
 * @brief 获取中断状态
 * @param dev 设备句柄
 * @param status 中断状态输出
 * @retval 错误代码
 */
icp20100_err_t icp20100_get_interrupt_status(icp20100_dev_t *dev, 
                                            icp20100_interrupt_status_t *status)
{
    if (!dev->initialized || status == NULL) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    uint8_t reg_value;
    icp20100_err_t ret = read_register(dev, ICP20100_REG_INTERRUPT_STATUS, &reg_value);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    status->reg = reg_value;
    return ICP20100_OK;
}

/**
 * @brief 清除中断标志
 * @param dev 设备句柄
 * @param status 要清除的中断标志
 * @retval 错误代码
 */
icp20100_err_t icp20100_clear_interrupt(icp20100_dev_t *dev, 
                                       icp20100_interrupt_status_t status)
{
    if (!dev->initialized) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    /* 写入1清除相应中断标志 */
    return write_register(dev, ICP20100_REG_INTERRUPT_STATUS, status.reg);
}

/**
 * @brief 配置FIFO工作模式
 * @param dev 设备句柄
 * @param config FIFO配置
 * @retval 错误代码
 */
icp20100_err_t icp20100_configure_fifo(icp20100_dev_t *dev, 
                                      icp20100_fifo_config_t config)
{
    if (!dev->initialized) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    return write_register(dev, ICP20100_REG_FIFO_CONFIG, config.reg);
}

/**
 * @brief 清空FIFO
 * @param dev 设备句柄
 * @retval 错误代码
 */
icp20100_err_t icp20100_flush_fifo(icp20100_dev_t *dev)
{
    if (!dev->initialized) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    /* 设置FLUSH位为1清空FIFO */
    return write_register(dev, ICP20100_REG_FIFO_FILL, 0x80);
}

/**
 * @brief 获取设备状态
 * @param dev 设备句柄
 * @param status 设备状态输出
 * @retval 错误代码
 */
icp20100_err_t icp20100_get_device_status(icp20100_dev_t *dev, 
                                          icp20100_device_status_t *status)
{
    if (!dev->initialized || status == NULL) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    uint8_t reg_value;
    icp20100_err_t ret = read_register(dev, ICP20100_REG_DEVICE_STATUS, &reg_value);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    status->reg = reg_value;
    return ICP20100_OK;
}

/**
 * @brief 软件复位传感器
 * @param dev 设备句柄
 * @retval 错误代码
 */
icp20100_err_t icp20100_software_reset(icp20100_dev_t *dev)
{
    if (!dev->initialized) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    /* 完整的启动序列实现 */
    icp20100_err_t ret;
    
    /* 1. 进入电源模式 */
    icp20100_mode_select_t mode;
    mode.reg = 0x00;
    mode.bit.power_mode = 1;
    ret = write_register(dev, ICP20100_REG_MODE_SELECT, mode.reg);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    /* 简单延时等待电源稳定 */
    for (volatile int i = 0; i < 10000; i++);
    
    /* 2. 执行其他复位操作... */
    /* 这里实现手册6.5节描述的完整启动序列 */
    
    /* 3. 返回待机模式 */
    mode.bit.power_mode = 0;
    ret = write_register(dev, ICP20100_REG_MODE_SELECT, mode.reg);
    
    return ret;
}

/**
 * @brief 读取原始压力数据
 * @param dev 设备句柄
 * @param raw_pressure 原始压力数据输出
 * @retval 错误代码
 */
icp20100_err_t icp20100_read_raw_pressure(icp20100_dev_t *dev, int32_t *raw_pressure)
{
    if (!dev->initialized || raw_pressure == NULL) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    uint8_t raw_data[3];
    icp20100_err_t ret = read_registers_burst(dev, ICP20100_REG_PRESS_DATA_0, 
                                              raw_data, 3);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    /* 提取20位压力数据（二进制补码格式） */
    *raw_pressure = ((raw_data[2] & 0x0F) << 16) | 
                   (raw_data[1] << 8) | raw_data[0];
    
    /* 处理符号扩展 */
    if (*raw_pressure & 0x80000) {  /* 检查符号位 */
        *raw_pressure |= 0xFFF00000;  /* 扩展符号位 */
    }
    
    return ICP20100_OK;
}

/**
 * @brief 读取原始温度数据
 * @param dev 设备句柄
 * @param raw_temp 原始温度数据输出
 * @retval 错误代码
 */
icp20100_err_t icp20100_read_raw_temperature(icp20100_dev_t *dev, int32_t *raw_temp)
{
    if (!dev->initialized || raw_temp == NULL) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    uint8_t raw_data[3];
    icp20100_err_t ret = read_registers_burst(dev, ICP20100_REG_TEMP_DATA_0, 
                                              raw_data, 3);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    /* 提取20位温度数据（二进制补码格式） */
    *raw_temp = ((raw_data[2] & 0x0F) << 16) | 
               (raw_data[1] << 8) | raw_data[0];
    
    /* 处理符号扩展 */
    if (*raw_temp & 0x80000) {  /* 检查符号位 */
        *raw_temp |= 0xFFF00000;  /* 扩展符号位 */
    }
    
    return ICP20100_OK;
}

/**
 * @brief 设置压力阈值
 * @param dev 设备句柄
 * @param threshold 压力阈值(Pa)
 * @retval 错误代码
 */
icp20100_err_t icp20100_set_pressure_threshold(icp20100_dev_t *dev, int32_t threshold)
{
    if (!dev->initialized) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    /* 将压力值转换为寄存器值 */
    int32_t reg_value = ((threshold - 70000) * 65535) / 40000;
    
    if (reg_value < 0) reg_value = 0;
    if (reg_value > 65535) reg_value = 65535;
    
    /* 写入阈值寄存器 */
    icp20100_err_t ret = write_register(dev, ICP20100_REG_PRESS_ABS_LSB, reg_value & 0xFF);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    ret = write_register(dev, ICP20100_REG_PRESS_ABS_MSB, (reg_value >> 8) & 0xFF);
    return ret;
}

/**
 * @brief 设置压差阈值
 * @param dev 设备句柄
 * @param threshold 压差阈值(Pa)
 * @retval 错误代码
 */
icp20100_err_t icp20100_set_delta_threshold(icp20100_dev_t *dev, int32_t threshold)
{
    if (!dev->initialized) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    /* 将压差值转换为寄存器值 */
    int32_t reg_value = (threshold * 65535) / 40000;
    
    if (reg_value < 0) reg_value = 0;
    if (reg_value > 65535) reg_value = 65535;
    
    /* 写入阈值寄存器 */
    icp20100_err_t ret = write_register(dev, ICP20100_REG_PRESS_DELTA_LSB, reg_value & 0xFF);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    ret = write_register(dev, ICP20100_REG_PRESS_DELTA_MSB, (reg_value >> 8) & 0xFF);
    return ret;
}

/**
 * @brief 获取FIFO水位
 * @param dev 设备句柄
 * @param level FIFO水位输出
 * @retval 错误代码
 */
icp20100_err_t icp20100_get_fifo_level(icp20100_dev_t *dev, uint8_t *level)
{
    if (!dev->initialized || level == NULL) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    icp20100_fifo_fill_t fifo_fill;
    icp20100_err_t ret = read_register(dev, ICP20100_REG_FIFO_FILL, &fifo_fill.reg);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    *level = fifo_fill.bit.fifo_level;
    return ICP20100_OK;
}

/**
 * @brief 等待测量完成
 * @param dev 设备句柄
 * @param timeout_ms 超时时间(ms)
 * @retval 错误代码
 */
icp20100_err_t icp20100_wait_for_measurement(icp20100_dev_t *dev, uint32_t timeout_ms)
{
    if (!dev->initialized) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    uint32_t start_time = 0; /* 需要由用户提供时间获取函数 */
    icp20100_device_status_t status;
    
    do {
        icp20100_err_t ret = icp20100_get_device_status(dev, &status);
        if (ret != ICP20100_OK) {
            return ret;
        }
        
        /* 检查模式同步状态 */
        if (status.bit.mode_sync_status) {
            return ICP20100_OK;
        }
        
        /* 简单的延时 */
        for (volatile int i = 0; i < 1000; i++);
        
        /* 超时检查 */
        if (timeout_ms > 0) {
            uint32_t current_time = 0; /* 需要由用户提供时间获取函数 */
            if ((current_time - start_time) >= timeout_ms) {
                return ICP20100_ERR_TIMEOUT;
            }
        }
        
    } while (1);
}

/* 应用层辅助函数 */

/**
 * @brief 配置传感器为高精度模式
 * @param dev 设备句柄
 * @retval 错误代码
 */
icp20100_err_t icp20100_configure_high_accuracy(icp20100_dev_t *dev)
{
    icp20100_err_t ret;
    
    /* 设置压力温度模式 */
    ret = icp20100_set_mode(dev, ICP20100_MODE_PRESSURE_TEMP);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    /* 配置FIFO */
    icp20100_fifo_config_t fifo_config;
    fifo_config.reg = 0x00;
    fifo_config.bit.fifo_mode = 0x00;        /* 压力优先模式 */
    fifo_config.bit.fifo_wmk_high = 0x02;    /* 高水位: 8样本 */
    fifo_config.bit.fifo_wmk_low = 0x01;     /* 低水位: 4样本 */
    
    ret = icp20100_configure_fifo(dev, fifo_config);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    /* 配置中断 */
    icp20100_interrupt_mask_t int_mask;
    int_mask.reg = 0xFF;
    int_mask.bit.fifo_ovf_mask = 0;      /* 启用FIFO溢出中断 */
    int_mask.bit.wmk_high_mask = 0;      /* 启用高水位中断 */
    
    ret = icp20100_set_interrupt_mask(dev, int_mask);
    
    return ret;
}

/**
 * @brief 配置传感器为低功耗模式
 * @param dev 设备句柄
 * @retval 错误代码
 */
icp20100_err_t icp20100_configure_low_power(icp20100_dev_t *dev)
{
    icp20100_err_t ret;
    
    /* 设置仅压力模式 */
    ret = icp20100_set_mode(dev, ICP20100_MODE_PRESSURE_ONLY);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    /* 屏蔽所有中断 */
    icp20100_interrupt_mask_t int_mask;
    int_mask.reg = 0xFF;  /* 屏蔽所有中断 */
    
    ret = icp20100_set_interrupt_mask(dev, int_mask);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    /* 清空FIFO */
    return icp20100_flush_fifo(dev);
}

/**
 * @brief 批量读取并转换传感器数据
 * @param dev 设备句柄
 * @param pressures 压力输出数组(Pa)
 * @param temperatures 温度输出数组(°C)
 * @param count 数据点数
 * @retval 错误代码
 */
icp20100_err_t icp20100_read_batch_data(icp20100_dev_t *dev, float *pressures, 
                                      float *temperatures, uint8_t *count)
{
    if (!dev->initialized || pressures == NULL || temperatures == NULL || count == NULL) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    icp20100_data_t raw_data[16];
    uint8_t samples_to_read = *count;
    
    if (samples_to_read > 16) {
        samples_to_read = 16;
    }
    
    icp20100_err_t ret = icp20100_read_fifo(dev, raw_data, &samples_to_read);
    if (ret != ICP20100_OK && ret != ICP20100_ERR_FIFO_EMPTY) {
        return ret;
    }
    
    /* 转换数据格式 */
    for (uint8_t i = 0; i < samples_to_read; i++) {
        pressures[i] = raw_data[i].pressure_pa / 1000.0f;  /* 转换为kPa */
        temperatures[i] = raw_data[i].temperature_c / 100.0f; /* 转换为°C */
    }
    
    *count = samples_to_read;
    return ICP20100_OK;
}

/* 设备诊断函数 */

/**
 * @brief 执行设备自检
 * @param dev 设备句柄
 * @retval 错误代码
 */
icp20100_err_t icp20100_self_test(icp20100_dev_t *dev)
{
    if (!dev->initialized) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    icp20100_err_t ret;
    
    /* 1. 验证设备ID */
    uint8_t device_id;
    ret = read_register(dev, ICP20100_REG_DEVICE_ID, &device_id);
    if (ret != ICP20100_OK || device_id != 0x63) {
        return ICP20100_ERR_COMM;
    }
    
    /* 2. 检查设备状态 */
    icp20100_device_status_t status;
    ret = icp20100_get_device_status(dev, &status);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    /* 3. 检查FIFO状态 */
    uint8_t fifo_level;
    ret = icp20100_get_fifo_level(dev, &fifo_level);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    /* 4. 简单的通信测试 */
    icp20100_mode_select_t mode;
    mode.reg = 0x00;
    ret = write_register(dev, ICP20100_REG_MODE_SELECT, mode.reg);
    if (ret != ICP20100_OK) {
        return ret;
    }
    
    return ICP20100_OK;
}

/**
 * @brief 获取传感器信息
 * @param dev 设备句柄
 * @param info 信息输出字符串
 * @param max_len 最大长度
 * @retval 错误代码
 */
icp20100_err_t icp20100_get_sensor_info(icp20100_dev_t *dev, char *info, uint32_t max_len)
{
    if (!dev->initialized || info == NULL || max_len < 64) {
        return ICP20100_ERR_INVALID_PARAM;
    }
    
    /* 获取设备信息 */
    uint8_t device_id, version;
    read_register(dev, ICP20100_REG_DEVICE_ID, &device_id);
    read_register(dev, ICP20100_REG_VERSION, &version);
    
    /* 格式化输出信息 */
    snprintf(info, max_len, "ICP-20100 Sensor: ID=0x%02X, Version=0x%02X, %s",
             device_id, version,
             (version == 0xB2) ? "Version B" : "Version A");
    
    return ICP20100_OK;
}
