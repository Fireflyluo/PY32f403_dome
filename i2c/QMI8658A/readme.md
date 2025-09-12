## 六轴陀螺仪 QMI8658A
### 一、概述
QMI8658A是一款低噪声、宽带宽6轴惯性测量单元(IMU)，集成了3轴陀螺仪和3轴加速度计，主要特性包括：
- **陀螺仪性能**：13 mdps/√Hz噪声密度，±3%灵敏度公差
- **加速度计性能**：150 μg/√Hz噪声密度，±6%灵敏度公差
- **接口支持**：SPI(3/4线)/I²C/I³C
- **封装尺寸**：2.5×3.0×0.86mm 14引脚LGA
- **工作范围**：-40℃至+85℃工业级温度范围
  
本开发板使用的 I2C 接口, QMI8658 的7位I2C地址为 0x6A。
>- 如果SA0=0，I2C/I3C地址=0x6B
>- 如果SA0=1，I2C/I3C地址=0x6A

### 二、文件结构

├── Application            -- 应用层代码目录
├── DSP                      
│   ├── MCU_Driver         -- 片上外设的驱动
│   └── Module_Driver         -- 外部设备的硬件层驱动库 
│       ├── lcd               -- lcd驱动
│       ├── qmi8658a          -- qmi8658a驱动
│       └── si24r1            -- 射频模块驱动 
├── Device                  -- 驱动层代码          
├── firmware                   -- 固件代码
├── Middleware              --中间层代码
├── Public                     --公共文件
├── MDK-ARM                    --工程文件
└── readme.md                  --文档

### 三、工程说明   

#### 3.1 驱动移植
工程核心文件为[寄存器硬件实现](BSP/Module_Driver/QMI8658A/qmi8658a_reg.c)和[驱动实现](BSP/Module_Driver/QMI8658A/qmi8658a_driver.c)。
移植只需要将qmi8658a_reg.c、qmi8658a_reg.h、qmi8658a_driver.c、
qmi8658a_driver.h复制到自己工程中，再根据自己硬件平台实现I2C操作接口函数qmi8658a_i2c_read和qmi8658a_i2c_write即可。
#### 3.2 软件说明

