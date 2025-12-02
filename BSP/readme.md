# PY32F403 BSP外设驱动包

本文件夹内是py32f403的BSP外设驱动包，包含外设设备驱动程序。

## 项目概述
板级支持包（BSP）是PY32F403开发板的基础，提供了硬件初始化、外设驱动等功能。
## 目录结构

```
BSP
├── py32_driver     # 设备驱动
│   ├── Inc         # 头文件
│   └── Src         # 源文件
├── PY32f403_Firmware_Library   # 官方固件
│   ├── CMSIS                       
│   └── PY32F403_HAL_Driver     # HAL库
├── config.h        # 配置文件
├── board.c         # 板级初始化
└── board.h         # 板级初始化头文件

```

## 主要特性

### drv_gpio.c/.h
- 该GPIO驱动实现了对py32f403的通用输入输出接口的抽象封装
- 对上提供通用API函数
- 关键API:gpio_init|gpio_write|gpio_read|gpio_toggle|gpio_attach_irq|gpio_detach_irq|gpio_irq_enable
- 使用GET_PIN宏定义引脚，格式为GET_PIN(端口字母, 引脚号)

```c
#define LED_PIN    GET_PIN(A, 5)   // PA5
#define BUTTON_PIN GET_PIN(B, 3)   // PB3

// 初始化
gpio_init();
gpio_mode(GET_PIN(A, 5), GPIO_MODE_OUTPUT);

// 开启LED
gpio_write(GET_PIN(A, 5), GPIO_PIN_SET);

// 关闭LED
gpio_write(GET_PIN(A, 5), GPIO_PIN_RESET);

// 翻转LED状态
gpio_toggle(GET_PIN(A, 5));
```
- GPIO中断功能

```c
// 中断处理函数
void button_irq_handler(void *args) {
    // 按键中断处理逻辑
    // args可以传递用户自定义数据
    gpio_toggle(GET_PIN(A, 5));  // 翻转LED作为响应
}

// 配置按键中断
gpio_init();
gpio_mode(GET_PIN(B, 3), GPIO_MODE_INPUT_PULLUP);

// 绑定中断处理函数（下降沿触发）
gpio_attach_irq(GET_PIN(B, 3), GPIO_IRQ_MODE_FALLING, button_irq_handler, NULL);

// 使能中断
gpio_irq_enable(GET_PIN(B, 3), GPIO_IRQ_ENABLE);
```
#### 注意事项
  - 引脚有效性检查：

    - 驱动会自动检查引脚是否有效，无效引脚操作将返回GPIO_EINVAL错误。
    - 例如：gpio_mode(GET_PIN(Z, 0), ...) 会失败，因为不存在Z端口
- 中断资源限制：

    - 每个引脚只能绑定一个中断处理函数
    - 中断触发模式在绑定时设置，后续无法修改（需先解绑再重新绑定）
- 引脚映射表：

    - 驱动内部使用gpio_pins数组管理引脚映射
    - 通过GET_PIN宏生成的引脚号是连续的，范围为0到GPIO_PIN_COUNT-1
- 中断处理注意事项：

    - 中断处理函数应尽量简短，避免在中断中执行耗时操作
    - 中断处理函数的参数args可用于传递上下文数据
- 错误处理：

    - API返回gpio_err_t类型，检查返回值以确保操作成功
    - 例如：if (gpio_mode(pin, mode) != GPIO_OK) { /* 处理错误 */ }


### drv_uart.c/.h
- 该驱动实现了对py32f403的串口的抽象封装

实现中。。。。
