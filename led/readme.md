## LED示例
### 一、概述
本工程文为GPIO验证程序。主
LED --> PB2
实现功能:LED闪烁

### 二、文件结构

├── Application              # 应用层 (业务逻辑、任务调度、UI交互)
├── BSP                      # 板级支持包 (硬件直接操作层)
│   ├── MCU_Peripheral       # 片上外设驱动 (GPIO/SPI/I2C等初始化与读写)
│   └── Sensor_Driver        # 传感器裸寄存器驱动 (xxxx_reg.c)
├── Drivers                  # 设备驱动层 (硬件抽象接口)
│   ├── Sensor               # 传感器标准化接口 
│   └── Sesor_Factory       # 工厂模式抽象层 (统一传感器访问接口)
├── Libraries                # 芯片厂商提供的底层库 
│   ├── CMSIS                # ARM内核抽象层 (如STM32的CMSIS)
│   └── HAL_Driver           # 厂商HAL/标准库 (如STM32 HAL)
├── Middleware               # 通用中间件 (硬件无关)
├── Project                  # 工程文件 (IDE相关)
│   └── MDK-ARM              # Keil工程
└── Include                  #全局头文件 (替代Public)

### 三、工程说明   
