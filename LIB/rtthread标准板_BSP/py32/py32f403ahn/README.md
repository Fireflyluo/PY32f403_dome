# PY32F403AHN 开发板 BSP 说明

## 简介

本文档为 PY32F403AHN 开发板的 BSP (板级支持包) 说明。

主要内容如下：

- 开发板资源介绍
- BSP 快速上手
- 进阶使用方法

通过阅读快速上手章节开发者可以快速地上手该 BSP，将 RT-Thread 运行在开发板上。在进阶使用指南章节，将会介绍更多高级功能，帮助开发者利用 RT-Thread 驱动更多板载资源。

## 开发板介绍

PY32F403AHN 开发板是基于普冉半导体 PY32F403 系列微控制器的评估开发板。

**主要特性：**
- MCU: PY32F403AHN
- 内核: ARM Cortex-M4
- 主频: 144MHz (HSE 16MHz × 9 PLL)
- Flash: 384KB
- SRAM: 64KB

该开发板常用 **板载资源** 如下：

- 常用外设
  - LED：可通过 GPIO 控制
  - 按键：支持用户输入
- 常用接口：USB 转串口、SPI、I2C、CAN 等
- 调试接口：标准 SWD/JTAG

开发板更多详细信息请参考普冉半导体 [PY32F403 产品介绍](https://www.puyasemi.com/)。

## 外设支持

本 BSP 目前对外设的支持情况如下：

| **片上外设**      | **支持情况** | **备注**                              |
| :----------------- | :----------: | :------------------------------------- |
| GPIO              |     支持     | PA0-PE15                              |
| UART              |     支持     | UART1-UART5                           |
| SPI               |     支持     | SPI1-SPI3                             |
| I2C               |     支持     | 硬件 I2C / 软件 I2C                   |
| TIM               |     支持     | 定时器驱动                            |
| PWM               |     支持     | 基于定时器                            |
| ADC               |     支持     | 模数转换                              |
| DAC               |     支持     | 数模转换                              |
| RTC               |     支持     | 实时时钟                              |
| WDT               |     支持     | 独立看门狗/窗口看门狗                 |
| FLASH             |     支持     | 片内 Flash 读写                       |
| CAN               |     支持     | CAN 总线通信                          |
| SDIO              |     支持     | SD 卡接口                             |
| DMA               |     支持     | DMA 数据传输                          |
| PWR               |     支持     | 电源管理                              |

## 使用说明

使用说明分为如下两个章节：

- 快速上手

    本章节是为刚接触 RT-Thread 的新手准备的使用说明，遵循简单的步骤即可将 RT-Thread 操作系统运行在该开发板上，看到实验效果。

- 进阶使用

    本章节是为需要在 RT-Thread 操作系统上使用更多开发板资源的开发者准备的。通过使用 ENV 工具对 BSP 进行配置，可以开启更多板载资源，实现更多高级功能。

### 快速上手

本 BSP 为开发者提供 MDK4、MDK5 和 IAR 工程，并且支持 GCC 开发环境。下面以 MDK5 开发环境为例，介绍如何将系统运行起来。

#### 硬件连接

使用数据线连接开发板到 PC，打开电源开关。

#### 编译下载

双击 project.uvprojx 文件，打开 MDK5 工程，编译并下载程序到开发板。

> 工程默认配置使用 SWD 仿真器下载程序，在通过调试器连接开发板的基础上，点击下载按钮即可下载程序到开发板

#### 运行结果

下载程序成功之后，系统会自动运行。连接开发板对应串口到 PC，在终端工具里打开相应的串口（115200-8-1-N），复位设备后，可以看到 RT-Thread 的输出信息:

```bash
 \ | /
- RT -     Thread Operating System
 / | \     5.1.0 build Mar 8 2026
 2006 - 2025 Copyright by rt-thread team
msh >
```

### 进阶使用

此 BSP 默认只开启了 GPIO 和 串口1 的功能，如果需使用 Flash、SD 卡等更多高级功能，需要利用 ENV 工具对 BSP 进行配置，步骤如下：

1. 在 bsp 下打开 env 工具。

2. 输入`menuconfig`命令配置工程，配置好之后保存退出。

3. 输入`pkgs --update`命令更新软件包。

4. 输入`scons --target=mdk4/mdk5/iar` 命令重新生成工程。

本章节更多详细的介绍请参考 [STM32 系列 BSP 外设驱动使用教程](../docs/STM32系列BSP外设驱动使用教程.md)。

## 系统时钟配置

本 BSP 的系统时钟配置如下：

- 外部高速时钟 (HSE): 16MHz
- PLL 倍频: 9 倍
- 系统时钟: 144MHz
- AHB 总线时钟: 144MHz
- APB1 总线时钟: 144MHz
- APB2 总线时钟: 72MHz

时钟配置代码位于 `board/board.c` 文件中的 `SystemClock_Config()` 函数。

## 注意事项

- 请确保使用正确的时钟配置文件，避免时钟配置错误导致系统不稳定
- 使用外部晶振时，请确认晶振频率与配置匹配
- 部分外设使用前应先在 menuconfig 中启用相应配置
- 调试时建议使用 SWD 接口

## 联系人信息

维护人:

- [RT-Thread 社区](https://github.com/RT-Thread/rt-thread)
- 邮箱：<support@rt-thread.org>

## 参考文档

- [PY32 系列 BSP 说明](../README.md)
- [RT-Thread 编程指南](https://www.rt-thread.org/document/site/programming-manual/introduction/introduction/)
