# PY32F403 开发板硬件描述

## 芯片规格

| 项目 | 参数 |
|------|------|
| MCU | PY32F403C1DT6 (Puya) |
| 内核 | ARM Cortex-M4, 最高 144MHz |
| FPU | FPv4-SP-D16（单精度硬浮点） |
| Flash | 384 KB |
| SRAM | 64 KB |
| 封装 | LQFP48 |

## 时钟树

```
HSE 16MHz → PLL ×9 → SYSCLK 144MHz
                        ├── HCLK  144MHz (AHB ÷1)
                        │   ├── APB1 144MHz (÷1) → USART2/3/4/5, I2C1/2, TIM2~7/12~14, SPI2/3
                        │   └── APB2  72MHz (÷2) → USART1/6, SPI1, TIM1/8~11, ADC1~3
                        └── SysTick = HCLK/8 = 18MHz
```

## 引脚映射总表

### 调试接口

| 功能 | 引脚 | 说明 |
|------|------|------|
| SWDIO | PA13 | CMSIS-DAP 调试器数据线 |
| SWCLK | PA14 | CMSIS-DAP 调试器时钟线 |
| 虚拟串口 | — | DAPLink 自带 CDC 串口 (COM13) |

### 控制台串口

| 功能 | 引脚 | 复用 | 说明 |
|------|------|------|------|
| USART2 TX | PA2 | AF2 | CH340 USB-TTL → 电脑 COM14 |
| USART2 RX | PA3 | AF2 | CH340 USB-TTL |

参数：115200 波特率、8 数据位、无校验、1 停止位

### GPIO / LED

| 功能 | 引脚 | 说明 |
|------|------|------|
| 板载 LED | PB2 | 高电平点亮，RTT 默认心跳指示 |

### WS2812 灯带

| 功能 | 引脚 | 复用 | 说明 |
|------|------|------|------|
| DIN 数据输入 | PA1 | AF6 (TIM2_CH2) | TIM2 PWM + DMA 驱动 |
| 供电 | 5V 外部 | — | 灯带独立供电 |

参数：8 灯串联、800kHz PWM 载波、高位先发 GRB 色序

### SPI1 —— W25Q128 Flash

| 功能 | 引脚 | 复用 | 说明 |
|------|------|------|------|
| CS 片选 | PA4 | GPIO 输出 | 软件控制、低有效 |
| CLK 时钟 | PA5 | AF3 (SPI1) | |
| MISO 主入从出 | PA6 | AF3 (SPI1) | |
| MOSI 主出从入 | PA7 | AF3 (SPI1) | |

W25Q128JV：16 MB、JEDEC ID `EF 60 18`、SPI 模式 0 (CPOL=0, CPHA=0)

### SPI3 —— XN297L 射频

| 功能 | 引脚 | 复用 | 说明 |
|------|------|------|------|
| CSN 片选 | PB1 | GPIO 输出 | 软件控制、低有效 |
| CLK 时钟 | PB3 | AF2 (SPI3) | |
| MISO 主入从出 | PB4 | AF2 (SPI3) | |
| MOSI 主出从入 | PB5 | AF2 (SPI3) | |

XN297L：2.4GHz 射频收发、SPI 模式 0、无硬件 CE（软件命令控制收发切换）

### I2C2 —— 传感器总线

| 功能 | 引脚 | 复用 | 说明 |
|------|------|------|------|
| SCL 时钟 | PB10 | AF1 (I2C2) | 开漏输出、外部上拉 |
| SDA 数据 | PB11 | AF1 (I2C2) | 开漏输出、外部上拉 |

参数：100kHz 标准模式、7 位地址

**已扫描到的设备：**

| 地址 | 推测型号 | 类型 |
|------|---------|------|
| 0x2C | 待确认 | — |
| 0x44 | SHT30 | 温湿度传感器 |
| 0x64 | 待确认 | — |
| 0x69 | ICM-42688-P | 6 轴 IMU（加速度+陀螺仪） |
| 0x6A | QMI8658A | 6 轴 IMU（加速度+陀螺仪） |
| 0x7E | 待确认 | — |

## DMA 通道分配

PY32F403 有 2 个 DMA 控制器（DMA1：7 通道，DMA2：5 通道）。
所有通道均通过 `LL_SYSCFG_SetDMARemap()` 完成外设到通道的映射。

| DMA 通道 | 外设 | 方向 | 模式 |
|----------|------|------|------|
| DMA1_Ch1 | SPI1 RX | 外设→内存 | 普通 |
| DMA1_Ch2 | SPI1 TX | 内存→外设 | 普通 |
| DMA1_Ch3 | SPI3 RX | 外设→内存 | 普通 |
| DMA1_Ch4 | SPI3 TX | 内存→外设 | 普通 |
| DMA1_Ch5 | USART2 RX | 外设→内存 | 循环 |
| DMA1_Ch6 | USART2 TX | 内存→外设 | 普通 |
| DMA2_Ch1 | I2C2 TX | 内存→外设 | 普通 |
| DMA2_Ch5 | TIM2_CH2 | 内存→外设 | 普通 (WS2812) |

## 电源与接口

| 接口 | 说明 |
|------|------|
| USB-A (DAPLink) | CMSIS-DAP 调试 + CDC 虚拟串口 (COM13) + 5V 供电 |
| USB-A (CH340) | UART2 转 USB 串口 (COM14) |
| SWD 排针 | PA13, PA14, GND, 3.3V |
| WS2812 灯带座 | 5V, GND, PA1(DIN) |
