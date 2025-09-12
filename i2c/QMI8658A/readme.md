## 六轴陀螺仪 QMI8658A

### 一、概述

QMI8658A 是一款低噪声、宽带宽 6 轴惯性测量单元(IMU)，集成了 3 轴陀螺仪和 3 轴加速度计，主要特性包括：

- **陀螺仪性能**：13 mdps/√Hz 噪声密度，±3%灵敏度公差
- **加速度计性能**：150 μg/√Hz 噪声密度，±6%灵敏度公差
- **接口支持**：SPI(3/4线)/I²C/I³C
- **封装尺寸**：2.5×3.0×0.86mm 14 引脚 LGA
- **工作范围**：-40℃至+85℃工业级温度范围

本开发板使用的 I2C 接口，QMI8658 的 7 位 I2C 地址为 0x6A。

> - 如果 SA0=0，I2C/I3C 地址=0x6B
> - 如果 SA0=1，I2C/I3C 地址=0x6A

### 二、文件结构

```
├── Application            -- 应用层代码目录
├── DSP                      
│   ├── MCU_Driver         -- 片上外设的驱动
│   └── Module_Driver      -- 外部设备的硬件层驱动库 
│       ├── lcd            -- lcd驱动
│       ├── qmi8658a       -- qmi8658a驱动
│       └── si24r1         -- 射频模块驱动 
├── Device                 -- 驱动层代码          
├── firmware               -- 固件代码
├── Middleware             -- 中间层代码
├── Public                 -- 公共文件
├── MDK-ARM                -- 工程文件
└── readme.md              -- 文档
```

### 三、工程说明

#### 3.1 驱动移植

工程核心文件为[寄存器硬件实现](BSP/Module_Driver/QMI8658A/qmi8658a_reg.c)和[驱动实现](BSP/Module_Driver/QMI8658A/qmi8658a_driver.c)。

移植只需要将 [qmi8658a_reg.c](i2c/QMI8658A/BSP/Module_Driver/QMI8658A/qmi8658a_reg.c)、[qmi8658a_reg.h](i2c/QMI8658A/BSP/Module_Driver/QMI8658A/qmi8658a_reg.h)、[qmi8658a_driver.c](QMI8658A/BSP/Module_Driver/QMI8658A/qmi8658a_driver.c)、[qmi8658a_driver.h](i2c/QMI8658A/BSP/Module_Driver/QMI8658A/qmi8658a_driver.h) 复制到自己工程中，再根据自己硬件平台实现 I2C 操作接口函数 `qmi8658a_i2c_read` 和 `qmi8658a_i2c_write` 即可。

#### 3.2 软件说明

##### 3.2.1 初始化

使用 `qmi8658a_init()` 函数初始化传感器，需要传入硬件句柄（如 I2C 句柄）：

```c
I2C_HandleTypeDef hi2c1; // 假设使用 I2C1
if(qmi8658a_init(&hi2c1) != 0) {
    // 初始化失败处理
}
```

初始化过程包括：
1. 验证设备 ID
2. 软复位设备
3. 配置 CTRL1 寄存器启用地址自动递增
4. 配置传感器使能（加速度计和陀螺仪）
5. 设置加速度计量程为 ±4g，数据输出速率 500Hz
6. 设置陀螺仪量程为 ±512dps，数据输出速率 896.8Hz
7. 配置滤波器

##### 3.2.2 数据读取

使用统一接口 `qmi8658a_device.read_data()` 读取传感器数据：

```c
sensor_data_t sensor_data;
if(qmi8658a_device.read_data(&sensor_data) == 0) {
    // 成功读取数据
    float accel_x = sensor_data.accel[0]; // 加速度计 X 轴 (m/s²)
    float gyro_x = sensor_data.gyro[0];   // 陀螺仪 X 轴 (rad/s)
    float temperature = sensor_data.temp; // 温度 (°C)
}
```

##### 3.2.3 量程设置

使用 `qmi8658a_device.set_range()` 设置陀螺仪量程：

```c
// 设置陀螺仪量程为 ±2000dps
qmi8658a_device.set_range(GYRO_RANGE_2000DPS);
```

支持的量程选项：
- `GYRO_RANGE_250DPS`
- `GYRO_RANGE_500DPS`
- `GYRO_RANGE_1000DPS`
- `GYRO_RANGE_2000DPS`

##### 3.2.4 输出数据率设置

使用 `qmi8658a_device.set_odr()` 设置陀螺仪输出数据率：

```c
// 设置陀螺仪输出数据率为 800Hz
qmi8658a_device.set_odr(GYRO_ODR_800HZ);
```

支持的 ODR 选项：
- `GYRO_ODR_100HZ`
- `GYRO_ODR_200HZ`
- `GYRO_ODR_400HZ`
- `GYRO_ODR_800HZ`

##### 3.2.5 功耗管理

使用 `qmi8658a_device.sleep()` 进入低功耗模式：

```c
qmi8658a_device.sleep();
```

使用 `qmi8658a_device.wakeup()` 唤醒设备：

```c
qmi8658a_device.wakeup();
```

### 四、寄存器配置说明

#### 4.1 核心控制寄存器

- `QMI8658A_CTRL1`：接口配置寄存器，控制地址自增、大小端格式等
- `QMI8658A_CTRL2`：加速度计配置寄存器，设置量程和输出数据率
- `QMI8658A_CTRL3`：陀螺仪配置寄存器，设置量程和输出数据率
- `QMI8658A_CTRL5`：滤波器配置寄存器，控制低通滤波器
- `QMI8658A_CTRL7`：传感器使能控制寄存器，控制加速度计和陀螺仪的使能状态
- `QMI8658A_CTRL8`：运动检测控制寄存器，控制各种运动检测功能的使能
- `QMI8658A_CTRL9`：命令寄存器，用于发送特定命令

#### 4.2 数据输出寄存器

- `QMI8658A_OUT_TEMP_L/H`：温度数据寄存器
- `QMI8658A_OUTX/Y/Z_L_A/H`：加速度计 XYZ 轴数据寄存器
- `QMI8658A_OUTX/Y/Z_L_G/H`：陀螺仪 XYZ 轴数据寄存器
- `QMI8658A_TIMESTAMP_L/M/H`：时间戳寄存器

#### 4.3 状态寄存器

- `QMI8658A_STATUS0`：数据就绪状态寄存器
- `QMI8658A_STATUSINT`：中断状态寄存器

### 五、高级功能

#### 5.1 FIFO 功能使用

QMI8658A 提供了 FIFO 缓冲功能，可以存储传感器数据以减少主控 MCU 的处理频率。使用 FIFO 需要以下步骤：

1. 配置 FIFO 模式和大小：
```c
// 配置 FIFO 为 FIFO 模式，大小为 64 样本
qmi8658a_fifo_config(&ctx.reg_ctx, QMI8658A_FIFO_MODE_FIFO, QMI8658A_FIFO_SIZE_64SAMPLES);
```

2. 读取 FIFO 数据：
```c
uint8_t fifo_buffer[128];
int32_t bytes_read = qmi8658a_fifo_read(&ctx.reg_ctx, fifo_buffer, sizeof(fifo_buffer));
if (bytes_read > 0) {
    // 处理 FIFO 数据
}
```

#### 5.2 自检功能

传感器支持自检功能，可以通过以下方式启用：

1. 加速度计自检：
   - 在 `QMI8658A_CTRL2` 寄存器中设置 `aST` 位启用加速度计自检

2. 陀螺仪自检：
   - 在 `QMI8658A_CTRL3` 寄存器中设置 `gST` 位启用陀螺仪自检

#### 5.3 滤波器配置

传感器内置低通滤波器，可通过 `QMI8658A_CTRL5` 寄存器配置：

```c
// 配置加速度计和陀螺仪的低通滤波器
qmi8658a_filter_config(&ctx.reg_ctx, 0x05, 0x05); // 启用加速度计和陀螺仪滤波器
```

#### 5.4 运动检测功能

QMI8658A 支持多种运动检测功能，包括：
- 敲击检测
- 计步器
- 任意运动检测
- 无运动检测
- 显著运动检测

这些功能可通过 `QMI8658A_CTRL8` 寄存器配置启用。

#### 5.5 时间戳功能

传感器提供 24 位时间戳功能，可通过以下方式读取：

```c
uint32_t timestamp;
qmi8658a_timestamp_get(&ctx.reg_ctx, &timestamp);
```

### 六、API 函数列表

#### 初始化和基本信息
- `qmi8658a_init()` - 初始化传感器
- `qmi8658a_device_id_get()` - 读取设备 ID
- `qmi8658a_soft_reset()` - 软复位设备

#### 传感器配置
- `qmi8658a_accel_data_rate_set()/get()` - 设置/获取加速度计输出数据率
- `qmi8658a_accel_full_scale_set()/get()` - 设置/获取加速度计量程
- `qmi8658a_gyro_data_rate_set()/get()` - 设置/获取陀螺仪输出数据率
- `qmi8658a_gyro_full_scale_set()/get()` - 设置/获取陀螺仪量程
- `qmi8658a_filter_config()` - 配置滤波器
- `qmi8658a_accel_enable()` - 使能/禁用加速度计
- `qmi8658a_gyro_enable()` - 使能/禁用陀螺仪

#### 数据读取
- `qmi8658a_temperature_get()` - 读取温度数据
- `qmi8658a_acceleration_raw()` - 读取原始加速度数据
- `qmi8658a_angular_rate_raw()` - 读取原始陀螺仪数据
- `qmi8658a_timestamp_get()` - 读取时间戳

#### FIFO 操作
- `qmi8658a_fifo_config()` - 配置 FIFO
- `qmi8658a_fifo_read()` - 读取 FIFO 数据
- `qmi8658a_fifo_read_mode_disable()` - 禁用 FIFO 读模式

#### 命令接口
- `qmi8658a_ctrl9_command()` - 发送 CTRL9 命令

### 七、注意事项

1. 在读取传感器数据前必须先调用初始化函数
2. 传感器数据读取频率不应超过设置的输出数据率
3. 更改量程后可能需要重新校准传感器
4. 在低功耗应用中，适时调用 sleep 函数以降低功耗
5. 使用 FIFO 功能时要注意缓冲区大小与配置的 FIFO 大小匹配
6. 读写寄存器时要确保接口函数正确实现