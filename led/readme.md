## LED示例
### 一、概述
本工程文为GPIO验证程序。主
LED --> PB2
实现功能:LED闪烁

### 二、文件结构

├── Application               -- 应用层代码目录
├── DSP                      
│   ├── MCU_Driver            -- 片上外设的驱动 
|   │   ├── gpio                 -- gpio配置
│   │   └── led                  -- lcd驱动
│   └── Module_Driver         -- 外部设备的驱动库             
├── Firmware                  -- 固件代码
├── Middleware                --中间层代码
├── Public                    --公共文件
├── MDK-ARM                   --工程文件
└── Readme.md                 --文档

### 三、工程说明   
