# PY32F403 LED驱动示例工程

本项目是基于PY32F403微控制器的LED驱动示例工程，支持普通GPIO LED和WS2812B可寻址LED的控制。

## 项目概述

该项目实现了统一的LED设备驱动接口，可以控制两种类型的LED：
1. 普通GPIO LED - 通过GPIO引脚直接控制LED的亮灭
2. WS2812B可寻址LED - 通过PWM和DMA控制的智能LED，支持RGB颜色设置
3. 你可以移植ws2812.c文件到你的工程中，并修改相关参数，即可实现WS2812B驱动。也可以移植完整的led设备驱动文件dev_led.c文件。
4. ws2812中实现的动态内存和静态内存分配两种方式，根据你的需求选择。

## 目录结构

```
.
├── Application     # 应用程序代码
│   ├── Inc         # 头文件
│   └── Src         # 源文件
├── BSP             # 板级支持包
│   ├── Device_REG  # 设备驱动
│   └── MCU_Peripheral # MCU外设驱动
├── Drivers         # 设备驱动
│   └── dev_led         # LED设备驱动
├── Libraries       # 库文件
└── Project         # 工程配置文件
```

## 主要特性

### LED驱动功能
- 统一的设备接口，支持不同类型LED
- 支持基本控制：开关、亮度调节、颜色设置
- 支持高级功能：呼吸效果、闪烁等
- 支持数据读写操作

### WS2812B驱动功能
- 通过PWM+DMA方式控制WS2812B LED
- 支持单个或多个LED级联控制
- 支持RGB颜色设置
- 符合WS2812B时序要求

## 硬件配置

### GPIO LED
- 默认连接到GPIOB_PIN_2

### WS2812B LED
- 数据线连接到GPIOA_PIN_1 (TIM2_CH2)
- 使用DMA2_Channel1进行数据传输

## 软件架构

### 核心组件
1. `dev_led.c/.h` - 统一LED设备驱动接口
2. `ws2812.c/.h` - WS2812B专用驱动
3. HAL库 - 硬件抽象层

### 示例程序
通过`EXAMPLE_MODE`宏切换不同的示例程序：
- `EXAMPLE_MODE == 1`: 基本控制示例（LED开关、颜色切换）
- `EXAMPLE_MODE == 2`: 高级控制示例（颜色渐变、状态指示）

## 使用方法

1. 根据需要修改`EXAMPLE_MODE`宏定义选择示例程序
2. 编译并下载程序到PY32F403开发板
3. 观察LED效果

## API说明

### 设备操作接口
- `dev_led_open()` - 打开并初始化LED设备
- `dev_led_close()` - 关闭LED设备并释放资源
- `dev_led_ioctl()` - 控制LED设备（设置状态、亮度、颜色等）
- `dev_led_write()` - 写入数据到LED设备
- `dev_led_read()` - 从LED设备读取数据

### WS2812B库接口
- `WS2812_Init()` - 初始化WS2812B设备
- `WS2812_Set_All()` - 设置所有LED的颜色
- `WS2812_Updata()` - 更新LED显示

## 注意事项

1. WS2812B对时序要求严格，确保系统时钟配置正确
2. 使用DMA传输时，确保中断配置正确
3. 多个LED级联时，注意数据传输的时序要求