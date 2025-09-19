# PY32F403 验证板项目

## 项目概述

本项目是基于自制的PY32F403的学习记录和驱动示例。该验证板集成了多种外设和传感器，用于学习和验证PY32F403系列芯片的各项功能。

## 硬件资源

验证板搭载的主要硬件资源包括：

- **主控芯片**: PY32F403C1DT6
- **传感器**:
  - 陀螺仪（型号: QMI8658A）
  - 气压计（型号: LPS22HHTR/LPS22HBTR/IPC20100）
  - 温湿度计（SHT30）
  - 磁力计（QMC5883P）
- **无线通信**:
  - 2.4G 射频芯片（型号: XN297L）
  - 2.4G 射频模块（型号：si24r1）
- **存储**:
  - W25Q128 Flash 存储器
- **显示**:
  - LCD 屏幕
- **其他外设**:
  - LED 指示灯

## 项目结构

```
├── I2C               -- i2c相关芯片驱动工程
│   ├── qmi8658a         -- 外部设备的驱动库
│   └── ...             
├── SPI               -- spi相关芯片驱动工程        
│   ├── si24r1         -- 外部设备的驱动库
│   └── ...             
└── Readme.md         --文档
```
## 工程结构
```
├── Application              # 应用层 (业务逻辑、任务调度、UI交互)
│   └── Sensor_Factory       # 工厂模式接口实现 (可选，若与业务强相关可放这里)
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
│   ├── Algorithm            # 算法库 (crc, filter, math)
│   ├── Data_Structure       # 数据结构 (ringbuff, queue, list)
│   └── Utilities            # 工具函数 (printf重定向, 位操作)
├── Platform                 # 平台抽象层 (可选)
│   ├── platform.c           # 系统时钟/延时抽象
│   └── gpio_abstract.h      # GPIO操作抽象接口
├── Project                  # 工程文件 (IDE相关)
│   └── MDK-ARM              # Keil工程
└── Include                  # 全局头文件 (替代Public)
    ├── config.h             # 项目配置
    ├── types.h              # 自定义数据类型
    └── sensor_defines.h     # 传感器通用参数定义

```

## 开发环境


- **IDE/编译器**: keil/vscode eide
- **编程语言**: C
- **调试工具**: dap-link
- **依赖库**: HAL库

## 快速开始

1. **克隆项目**
   ```bash
   git clone <项目地址>
   cd py32f403-validation-board
   ```

2. **导入工程**
   - 使用keil/eide 打开项目目录
   - 选择对应的工程文件（如: project.uvprojx）

3. **编译和烧录**
   - 配置正确的目标设备（PY32F403）
   - 编译项目
   - 连接调试器并烧录到验证板

4. **运行示例**
   - 目前可运行的示例：
     - LED闪烁示例（led目录）
     - I2C扫描示例（i2c目录）

## 已实现功能

- [x] LED控制（GPIO）
- [x] I2C基础通信
- [x] 陀螺仪数据读取
- [x] 气压计数据读取
- [x] 2.4G射频通信
- [ ] LCD显示驱动
- [ ] W25Q138读写操作

## 待实现功能

- [ ] SPI通信示例
- [ ] 定时器应用示例
- [ ] 中断处理示例
- [ ] 低功耗模式示例
- [ ] 更多外设驱动...

## 使用说明

每个工程的详细使用说明请查看对应目录下的README文件：

- [LED模块说明](led/readme.md)
- [I2C模块说明](i2c/readme.md)



## 更新日志

- **2025-09-12**: 初始版本提交
  - 添加LED控制示例
  - 添加I2C基础通信示例
  - 添加陀螺仪QMI8658A驱动
  - 添加气压计LPS22HB驱动
  - 修改工程结构和文档描述

## 联系方式

- 作者: Fireflyluo
- qq: 2161486135
- 邮箱: 2161486135@qq.com
- 硬件开源地址：https://oshwhub.com/fireflyluo/py32f403-sensor-and-24g-developm

## 注意事项

1. 使用前请确保硬件连接正确
2. 部分功能可能需要额外配置
3. 如有问题请查看具体模块的文档或提交Issue

---

*持续更新中...*
