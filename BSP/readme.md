# PY32F403 BSP外设驱动包

本文件夹内是py32f403的BSP外设驱动包，包含外设设备驱动程序。

## 一、项目概述
板级支持包（BSP）是PY32F403开发板的基础，提供了硬件初始化、外设驱动等功能。
## 二、、目录结构

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

## 三、主要特性

### 3.1 drv_gpio.c/.h
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
####注意事项
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


### 3.2 drv_uart.c/.h
- 该驱动实现了对py32f403的串口的抽象封装
- 对上提供通用API函数,详情见drv_uart.h
- 支持3种模式(使用前需要在drv_uart_config.h中选择工作模式)
#### 3.2.1. 轮询模式 (Polling Mode)
- 适用场景：简单应用、调试输出、低功耗待机唤醒
- 特点：阻塞式操作，CPU占用率高
- 优势：代码简单，资源消耗最小

API示例：
```c
// 初始化
uart_init(UART_INSTANCE_2, 115200);
// 阻塞发送
uart_send(UART_INSTANCE_2, data, size, 1000);
// 阻塞接收  
uart_receive(UART_INSTANCE_2, buffer, size, 1000);

// 完整件示例：uart_polling_example.c
```
#### 3.2.2 中断模式 (Interrupt Mode)
- 适用场景：中等数据量、实时响应需求
- 特点：非阻塞操作，基于事件驱动
- 优势：良好的实时性，CPU占用可控

API示例：
```c
// 初始化
uart_init(UART_INSTANCE_2, 115200);
// 异步发送（立即返回）
uart_send_async(UART_INSTANCE_2, data, size);
// 注册回调函数
uart_register_tx_complete_callback(UART_INSTANCE_2, my_callback, NULL);

// 完整件示例：uart_polling_interrupt.c
```
#### 3.2.3 DMA模式 (DMA Mode)
- 适用场景：高速数据流、低功耗应用
- 特点：零CPU拷贝，硬件自动传输
- 优势：最高吞吐量，最低CPU占用
- 特殊功能：
    - 环形缓冲区管理
    - DMA空闲中断检测
    - 自动流控

API示例：
```c
// 初始化
uart_init(UART_INSTANCE_2, 115200);
// 启动DMA接收（循环模式）
uart_dma_start_rx(UART_INSTANCE_2, UART_DMA_MODE_CIRCULAR);

// 环形缓冲区操作
uart_write_to_ring_buffer(UART_INSTANCE_2, data, size);

// 完整件示例：uart_polling_interrupt.c
```

#### 3.2.4 编译时配置 
通过 drv_uart_config.h进行模式选择：
```c
// 三选一模式配置
#define UART_WORK_MODE UART_MODE_DMA          // DMA模式
// #define UART_WORK_MODE UART_MODE_INTERRUPT  // 中断模式  
// #define UART_WORK_MODE UART_MODE_POLLING    // 轮询模式

// 缓冲区配置
#define UART_RX_BUFFER_SIZE 256
#define UART_TX_BUFFER_SIZE 256
#define UART_DMA_BUFFER_SIZE 128

// 高级特性
#define UART_USE_DMA_IDLE_DETECTION 1    // DMA空闲检测
```
#### 3.2.5 核心API参考
1.  基础管理
```c
// 初始化/反初始化
uart_err_t uart_init(uart_instance_t instance, uint32_t baudrate);
uart_err_t uart_deinit(uart_instance_t instance);

// 状态查询
bool uart_is_tx_busy(uart_instance_t instance);
bool uart_is_rx_busy(uart_instance_t instance);
```
2.  数据传输
```c
// 同步传输（阻塞）
uart_err_t uart_send(uart_instance_t instance, const uint8_t *data, uint16_t size, uint32_t timeout);
uart_err_t uart_receive(uart_instance_t instance, uint8_t *buffer, uint16_t size, uint32_t timeout);

// 异步传输（非阻塞）
uart_err_t uart_send_async(uart_instance_t instance, const uint8_t *data, uint16_t size);
uart_err_t uart_receive_async(uart_instance_t instance, uint8_t *buffer, uint16_t size);

```
3. 回调管理
```c
// 事件回调注册
uart_err_t uart_register_tx_complete_callback(uart_instance_t instance, 
                                              uart_tx_complete_callback_t callback, 
                                              void *arg);
```

4. DMA扩展功能
```c
#if (UART_USE_DMA == 1)
// 环形缓冲区操作
uint16_t uart_read_from_ring_buffer(uart_instance_t instance, uint8_t *buffer, uint16_t size);
uint16_t uart_write_to_ring_buffer(uart_instance_t instance, const uint8_t *data, uint16_t size);

// DMA控制
uart_err_t uart_dma_start_rx(uart_instance_t instance, uart_dma_mode_t mode);
uart_err_t uart_dma_stop_rx(uart_instance_t instance);
#endif
```
5. 错误码定义
```c
typedef enum {
    UART_OK = 0,            // 操作成功
    UART_ERROR = -1,        // 一般错误
    UART_ERROR_PARAM = -2,  // 参数错误
    UART_ERROR_BUSY = -3,   // 设备忙
    UART_ERROR_TIMEOUT = -4,// 超时错误
    UART_ERROR_DMA = -6,    // DMA错误
} uart_err_t;
```


未完待续。。。。。