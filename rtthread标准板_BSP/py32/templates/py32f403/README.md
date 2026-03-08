# PY32F403 系列 BSP 模板说明

## 简介

本文档为 PY32F403 系列 BSP 模板说明，用于快速创建基于 PY32F403 芯片的新开发板 BSP。

## 模板概述

本模板提供了 PY32F403 系列 MCU 的基础工程结构，包含：

- 标准 RT-Thread 工程配置
- PY32F403 HAL 库集成
- 常用外设驱动配置
- MDK5/IAR/GCC 工程支持

## 芯片特性

PY32F403 系列主要特性：

- **内核**: ARM Cortex-M4F，带 FPU
- **主频**: 最高 144MHz
- **Flash**: 256KB - 512KB
- **SRAM**: 64KB
- **工作电压**: 1.8V - 3.6V
- **工作温度**: -40°C ~ 85°C

## 外设资源

PY32F403 系列包含丰富的外设资源：

| 外设类型 | 数量/规格 |
|----------|-----------|
| UART     | 5 路      |
| SPI      | 3 路      |
| I2C      | 2 路      |
| CAN      | 1 路      |
| USB      | USB 2.0 FS |
| ADC      | 2 路 12位 |
| DAC      | 2 路 12位 |
| 定时器   | 10 个     |
| DMA      | 16 通道   |
| GPIO     | 最多 82 个 |

## 使用模板创建新 BSP

### 步骤 1: 复制模板

```bash
# 在 py32 目录下
cp -r templates/py32f403/ your_board_name/
```

### 步骤 2: 修改配置文件

1. **board/board.c**: 根据硬件时钟配置修改 `SystemClock_Config()`
2. **board/board.h**: 修改内存大小等参数
3. **board/Kconfig**: 根据板载外设修改配置选项

### 步骤 3: 更新描述文件

修改 `README.md`，添加开发板特定的信息：
- 开发板名称和简介
- 板载资源说明
- 外设支持情况
- 使用说明

### 步骤 4: 生成工程

```bash
scons --target=mdk5    # 生成 MDK5 工程
scons --target=iar     # 生成 IAR 工程
scons --target=makefile # 生成 GCC Makefile
```

## 目录结构

```
py32f403/
├── .config              # 默认配置
├── Kconfig              # 配置文件
├── SConstruct           # scons 构建脚本
├── SConscript           # scons 构建脚本
├── rtconfig.py          # RT-Thread 配置
├── board/
│   ├── board.c          # 板级初始化
│   ├── board.h          # 板级头文件
│   ├── Kconfig          # 板级配置
│   ├── SConscript       # 构建脚本
│   ├── CubeMX_Config/   # CubeMX 配置
│   └── linker_scripts/  # 链接脚本
├── applications/
│   └── main.c           # 主程序入口
└── figures/
    └── board.png        # 开发板图片
```

## 默认配置

### 时钟配置

默认时钟配置（可在 board/board.c 修改）：

- HSE: 16MHz 外部晶振
- PLL: 9 倍频
- 系统时钟: 144MHz
- AHB: 144MHz
- APB1: 144MHz
- APB2: 72MHz

### 默认开启外设

- GPIO
- UART1 (用于 shell 输出)

## 驱动配置说明

### UART 配置

在 `board/Kconfig` 中添加：

```kconfig
config BSP_USING_UARTx
    bool "Enable UARTx"
    default n
```

### SPI 配置

```kconfig
config BSP_USING_SPIx
    bool "Enable SPIx"
    default n

config BSP_SPIx_TX_USING_DMA
    bool "SPIx TX DMA"
    depends on BSP_USING_SPIx
    default n
```

### I2C 配置

```kconfig
config BSP_USING_I2Cx
    bool "Enable I2Cx"
    default n
```

## 注意事项

1. **时钟配置**: 请根据实际硬件晶振频率修改时钟配置
2. **引脚复用**: 注意不同外设间的引脚冲突
3. **内存分配**: 根据实际应用调整堆栈大小
4. **调试接口**: 建议使用 SWD 接口进行调试

## 参考资源

- [PY32 系列 BSP 说明](../../README.md)
- [STM32 系列 BSP 外设驱动使用教程](../../docs/STM32系列BSP外设驱动使用教程.md)
- [RT-Thread 编程指南](https://www.rt-thread.org/document/site/programming-manual/introduction/introduction/)
- [PY32F403 参考手册](https://www.puyasemi.com/)

## 维护信息

- 维护者：RT-Thread 社区
- 邮箱：<support@rt-thread.org>

## 许可协议

遵循 Apache-2.0 许可协议。
