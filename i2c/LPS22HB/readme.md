## 气压计 LPS22HBTR

### 一、概述

LPS22HBTR 是一款高性能、超低功耗的压阻式绝对压力传感器，主要用于测量大气压力。主要特性包括：

- **压力传感器**：具有260-1260 hPa绝对压力范围
- **高精度**：在24°C时误差为±0.1 hPa（典型值）
- **超低功耗**：典型值为3.8 μA
- **接口支持**：SPI/I²C接口
- **封装尺寸**：2.0×2.5×0.86mm 10引脚LGA封装
- **工作范围**：-40℃至+85℃工业级温度范围

本开发板使用的 I2C 接口，LPS22HB 的 7 位 I2C 地址为 0xB9（SDO接地）或 0xBB（SDO接VDD）。

### 二、文件结构

```
├── Application              # 应用层 (业务逻辑、任务调度、UI交互)
├── BSP                      # 板级支持包 (硬件直接操作层)
│   ├── MCU_Peripheral       # 片上外设驱动 (GPIO/SPI/I2C等初始化与读写)
│   └── Sensor_Driver        # 传感器裸寄存器驱动 (xxxx_reg.c)
│       ├── lcd              # LCD寄存器级操作
│       └── lps22hb          # 气压计lps22hb
├── Drivers                  # 设备驱动层 (硬件抽象接口)
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

工程核心文件为 [寄存器硬件实现](BSP/Module_Driver/lps22hb/lps22hb_reg.c) 和 [驱动实现](BSP/Module_Driver/lps22hb/lps22hb_driver.c)。

移植只需要将 [lps22hb_reg.c](BSP/Module_Driver/lps22hb/lps22hb_reg.c)、[lps22hb_reg.h](BSP/Module_Driver/lps22hb/lps22hb_reg.h)、[lps22hb_driver.c](BSP/Module_Driver/lps22hb/lps22hb_driver.c)、[lps22hb_driver.h](BSP/Module_Driver/lps22hb/lps22hb_driver.h) 复制到自己工程中，再根据自己硬件平台实现 I2C 操作接口函数。

#### 3.2 软件说明

##### 3.2.1 初始化

使用 `LPS22HB_Init()` 函数初始化传感器，需要传入硬件句柄：

```c
LPS22HB_Object_t lps22hb;
// 初始化设备
if(LPS22HB_Init(&lps22hb) != LPS22HB_OK) {
    // 初始化失败处理
}
```

初始化过程包括：
1. 验证设备 ID
2. 配置低功耗模式
3. 设置数据更新模式
4. 配置滤波器
5. 启用块数据更新

##### 3.2.2 数据读取

使用 `LPS22HB_PRESS_GetPressure()` 函数读取压力数据：

```c
float pressure;
if(LPS22HB_PRESS_GetPressure(&lps22hb, &pressure) == LPS22HB_OK) {
    // 成功读取压力数据 (单位: hPa)
}
```

使用 `LPS22HB_TEMP_GetTemperature()` 函数读取温度数据：

```c
float temperature;
if(LPS22HB_TEMP_GetTemperature(&lps22hb, &temperature) == LPS22HB_OK) {
    // 成功读取温度数据 (单位: °C)
}
```

##### 3.2.3 输出数据率设置

使用 `LPS22HB_PRESS_SetOutputDataRate()` 或 `LPS22HB_TEMP_SetOutputDataRate()` 设置输出数据率：

```c
// 设置输出数据率为 25Hz
LPS22HB_PRESS_SetOutputDataRate(&lps22hb, 25.0f);
```

支持的 ODR 选项：
- 1 Hz
- 10 Hz
- 25 Hz
- 50 Hz
- 75 Hz

##### 3.2.4 使能传感器

使用 `LPS22HB_PRESS_Enable()` 和 `LPS22HB_TEMP_Enable()` 使能压力和温度传感器：

```c
LPS22HB_PRESS_Enable(&lps22hb);  // 使能压力传感器
LPS22HB_TEMP_Enable(&lps22hb);   // 使能温度传感器
```

使用 `LPS22HB_PRESS_Disable()` 和 `LPS22HB_TEMP_Disable()` 禁用传感器：

```c
LPS22HB_PRESS_Disable(&lps22hb); // 禁用压力传感器
LPS22HB_TEMP_Disable(&lps22hb);  // 禁用温度传感器
```

### 四、寄存器配置说明

#### 4.1 核心控制寄存器

- `LPS22HB_CTRL_REG1`：控制寄存器1，配置输出数据率、低功耗模式等
- `LPS22HB_CTRL_REG2`：控制寄存器2，配置软件复位、I2C使能、FIFO等
- `LPS22HB_CTRL_REG3`：控制寄存器3，配置中断引脚等

#### 4.2 数据输出寄存器

- `LPS22HB_PRESS_OUT_XL/L/H`：压力数据输出寄存器（24位）
- `LPS22HB_TEMP_OUT_L/H`：温度数据输出寄存器（16位）

#### 4.3 状态寄存器

- `LPS22HB_STATUS`：状态寄存器，指示数据是否就绪
- `LPS22HB_INT_SOURCE`：中断源寄存器

### 五、FIFO功能

LPS22HB支持FIFO功能，可配置不同模式：
- Bypass模式：直接输出传感器数据
- FIFO模式：当FIFO满时停止采集
- Stream模式：当FIFO满时新数据覆盖旧数据

使用 `LPS22HB_FIFO_Set_Mode()` 配置FIFO模式：
```c
LPS22HB_FIFO_Set_Mode(&lps22hb, LPS22HB_FIFO_MODE);
```

### 六、注意事项

1. 在读取传感器数据前必须先调用初始化函数
2. 传感器数据读取频率不应超过设置的输出数据率
3. 为了获得最佳性能，建议使用外部上拉电阻连接SDO引脚
4. 在低功耗应用中，可配置为单次测量模式以降低功耗