#ifndef __DEV_LED_H__
#define __DEV_LED_H__

#include "dev_common.h"
#include "gpio.h"
#include "tim.h"
#include <math.h>
// WS2812B相关定义
#define WS2812_BIT_1_PERIOD 120   // 逻辑1的比较值 (0.833μs)
#define WS2812_BIT_0_PERIOD 60    // 逻辑0的比较值 (0.417μs)
#define WS2812_RESET_PERIOD 0     // 复位信号比较值 (低电平)
#define M_PI    3.1415926
// LED 设备句柄类型
typedef void* dev_led_handle_t;

// LED 类型
typedef enum {
    LED_TYPE_GPIO = 0,        // 普通 GPIO LED
    LED_TYPE_ADDRESSABLE      // 可寻址 LED (如 WS2812)
} led_type_t;

// LED 状态
typedef enum {
    LED_STATE_OFF = 0,        // 关闭
    LED_STATE_ON,             // 常亮
    LED_STATE_BLINK,          // 闪烁
    LED_STATE_BREATH,         // 呼吸效果
    LED_STATE_PATTERN         // 自定义模式
} led_state_t;

/**
 * @brief LED配置结构体
 */
typedef struct {
    led_type_t type; /**< LED类型 */

    // 硬件配置（根据type选择对应的配置）
    union {
        /**
         * @brief GPIO模式配置
         */
        struct {
            GPIO_TypeDef *gpiox; /**< GPIO端口基地址 */
            uint16_t pin;        /**< GPIO引脚号 */
        } gpio;
        
        /**
         * @brief 可寻址LED模式配置（如WS2812）
         */
        struct {
            GPIO_TypeDef *gpiox;     /**< GPIO端口基地址 */
            uint16_t pin;            /**< 数据引脚号 */
            uint16_t led_count;      /**< LED数量 */
        } addressable;
    };
} led_config_t;

// LED 控制参数
typedef struct {
    led_state_t state;   // 目标状态
    uint8_t brightness;  // 亮度 (0-255)
    
    // 闪烁参数
    struct {
        uint16_t on_time_ms;   // 亮灯时间(ms)
        uint16_t off_time_ms;  // 灭灯时间(ms)
        uint16_t cycles;       // 循环次数 (0=无限)
    } blink;
    
    // RGB 颜色
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    } rgb_color;
} led_control_t;

// I/O 控制命令 (ioctl)
typedef enum {
    // 基础控制命令
    LED_IOCTL_SET_STATE = 0x100,   // 设置LED状态 (arg: led_state_t*)
    LED_IOCTL_GET_STATE,           // 获取LED状态 (arg: led_state_t*)
    LED_IOCTL_SET_BRIGHTNESS,      // 设置亮度 (arg: uint8_t*)
    LED_IOCTL_GET_BRIGHTNESS,      // 获取亮度 (arg: uint8_t*)
    
    // 高级控制命令
    LED_IOCTL_SET_BLINK_PARAMS,    // 设置闪烁参数 (arg: led_control_t*)
    LED_IOCTL_SET_RGB_COLOR,       // 设置RGB颜色 (arg: uint32_t, 0xRRGGBB)
    LED_IOCTL_GET_RGB_COLOR,       // 获取RGB颜色 (arg: uint32_t*)
    
    // 特殊效果命令
    LED_IOCTL_START_BREATH,        // 开始呼吸效果 (arg: 呼吸周期ms)
    LED_IOCTL_STOP_BREATH,         // 停止呼吸效果
    LED_IOCTL_SET_PATTERN,         // 设置自定义模式 (arg: uint8_t* 数组)
} led_ioctl_cmd_t;

// LED设备内部状态结构
typedef struct {
    led_config_t config;              // LED配置
    led_control_t control;            // 当前控制参数
    TIM_HandleTypeDef* timer_handle;  // 定时器句柄
    uint32_t timer_channel;           // 定时器通道
} led_device_t;

/* 函数声明 */
dev_led_handle_t dev_led_open(const led_config_t *config);
dev_error_t dev_led_close(dev_led_handle_t handle);
dev_error_t dev_led_ioctl(dev_led_handle_t handle, led_ioctl_cmd_t cmd, void *arg);
int dev_led_write(dev_led_handle_t handle, const void *data, size_t size);
int dev_led_read(dev_led_handle_t handle, void *buffer, size_t size);

// DMA传输完成回调函数
void dev_led_dma_transfer_complete(void);

#endif