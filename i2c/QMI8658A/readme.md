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
├── Application              # 应用层 (业务逻辑、任务调度、UI交互)
├── BSP                      # 板级支持包 (硬件直接操作层)
│   ├── MCU_Peripheral       # 片上外设驱动 (GPIO/SPI/I2C等初始化与读写)
│   └── Sensor_Driver        # 传感器裸寄存器驱动 (xxxx_reg.c)
│       ├── lcd_reg.c        # LCD寄存器级操作
│       ├── qmi8658a_reg.c   # IMU寄存器级操作
│       └── si24r1_reg.c     # 射频模块寄存器级操作
├── Drivers                  # 设备驱动层 (硬件抽象接口)
│   ├── Sensor               # 传感器标准化接口 
│   │   ├── lcd.c            # LCD的init/read/write接口
│   │   ├── qmi8658a.c       # IMU的init/read/write接口
│   │   └── si24r1.c         # 射频模块的init/read/write接口
│   └── Sensor_Factory       # 工厂模式抽象层 (统一传感器访问接口)
│       ├── sensor_factory.c 
│       └── sensor_interface.h 
├── Libraries                # 芯片厂商提供的底层库 
│   ├── CMSIS                # ARM内核抽象层 (如STM32的CMSIS)
│   └── HAL_Driver           # 厂商HAL/标准库 (如STM32 HAL)
├── Middleware               # 通用中间件 (硬件无关)
├── Project                  # 工程文件 (IDE相关)
│   └── MDK-ARM              # Keil工程
└── Include                  # 全局头文件 (替代Public)
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

#### 4.2 数据输出寄存器

- `QMI8658A_OUT_TEMP_L/H`：温度数据寄存器
- `QMI8658A_OUTX/Y/Z_L_A/H`：加速度计 XYZ 轴数据寄存器
- `QMI8658A_OUTX/Y/Z_L_G/H`：陀螺仪 XYZ 轴数据寄存器

#### 4.3 状态寄存器

- `QMI8658A_STATUS0`：数据就绪状态寄存器
- `QMI8658A_STATUSINT`：中断状态寄存器

### 五、注意事项

1. 在读取传感器数据前必须先调用初始化函数
2. 传感器数据读取频率不应超过设置的输出数据率
3. 更改量程后可能需要重新校准传感器
4. 在低功耗应用中，适时调用 sleep 函数以降低功耗