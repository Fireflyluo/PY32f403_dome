# PY32 系列 BSP 说明

## 简介

本文档为 RT-Thread 的 PY32 系列 BSP (板级支持包) 说明，包含 PY32F403 等系列芯片的支持。

PY32 系列是普冉半导体（PUYA）推出的基于 ARM Cortex-M 内核的 32 位微控制器，具有高性能、低功耗、丰富外设等特点。

## 目录结构

```
py32/
├── libraries/              # 驱动库
│   ├── drivers/           # RT-Thread 外设驱动
│   ├── HAL_Drivers/       # HAL 层驱动
│   └── Kconfig            # 库配置
├── PY32F403_HAL/          # PY32F403 HAL 库
│   ├── CMSIS/             # CMSIS 核心文件
│   └── PY32F403_HAL_Driver/  # HAL 驱动代码
├── templates/             # BSP 模板
│   ├── py32f403/         # PY32F403 系列模板
│   └── stm32f4xx/        # STM32F4xx 兼容模板
├── tools/                 # 工具脚本
│   └── sdk_dist.py       # SDK 分发脚本
└── py32f403ahn/          # 具体开发板示例
```

## 支持的芯片系列

| 系列 | 内核 | 主频 | Flash | RAM | 说明 |
|------|------|------|-------|-----|------|
| PY32F403 | Cortex-M4 | 144MHz | 256-512KB | 64KB | 高性能系列 |

## 支持的开发环境

- MDK-ARM (Keil)
- IAR Embedded Workbench
- GCC (ARM Embedded GCC)
- RT-Thread Studio

## 快速开始

### 1. 使用现有 BSP

进入具体开发板目录，如 `py32f403ahn/`，参考该目录下的 README.md 进行快速上手。

### 2. 创建新 BSP

可以使用模板创建新的 BSP：

1. 复制 `templates/py32f403/` 目录到新开发板目录
2. 根据硬件修改 `board/` 目录下的配置文件
3. 更新 `README.md` 描述开发板信息

## 驱动支持情况

### 片上外设驱动

| 外设 | 支持情况 | 说明 |
|------|----------|------|
| GPIO | 支持 | PA0-PE15 |
| UART | 支持 | UART1-UART5 |
| SPI | 支持 | SPI1-SPI3 |
| I2C | 支持 | 硬件 I2C + 软件 I2C |
| TIM | 支持 | 定时器驱动 |
| PWM | 支持 | 基于定时器 |
| ADC | 支持 | 模数转换 |
| DAC | 支持 | 数模转换 |
| RTC | 支持 | 实时时钟 |
| WDT | 支持 | 看门狗 |
| FLASH | 支持 | 片内 Flash |
| CAN | 支持 | CAN 总线 |
| SDIO | 支持 | SD 卡接口 |

### 软件包支持

- 可通过 RT-Thread 包管理器添加各种软件包
- 支持 FATFS 文件系统
- 支持多种网络协议栈

## 配置与编译

### 使用 menuconfig 配置

```bash
scons --menuconfig
```

### 生成工程

```bash
# 生成 MDK5 工程
scons --target=mdk5

# 生成 IAR 工程
scons --target=iar

# 生成 GCC Makefile
scons --target=makefile
scons --target=cmake
scons --target=xmake
```

### 编译

```bash
scons -j8
```

## 注意事项

1. 使用 HAL 库时，请确保时钟配置正确
2. 部分外设可能需要手动开启时钟
3. 建议参考 `py32f403ahn` 示例进行开发

## 参考资料

- [PY32 官方资料](https://www.puyasemi.com/)
- [RT-Thread 文档中心](https://www.rt-thread.org/document/site/)
- [STM32 系列 BSP 外设驱动使用教程](../docs/STM32系列BSP外设驱动使用教程.md)

## 维护信息

- 维护者：RT-Thread 社区
- 邮箱：support@rt-thread.org
- 问题反馈：[GitHub Issues](https://github.com/RT-Thread/rt-thread/issues)

## 许可协议

遵循 Apache-2.0 许可协议，详见 LICENSE 文件。
