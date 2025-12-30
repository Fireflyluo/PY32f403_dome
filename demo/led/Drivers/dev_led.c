#include "dev_led.h"
#include <stdlib.h>
#include <string.h>

#include "ws2812.h"

// 全局设备指针用于DMA回调
static led_device_t* g_ws2812_device = NULL;

// 私有函数声明
static void apply_led_state(led_device_t *dev);
static dev_error_t set_gpio_led(led_device_t *dev);
static dev_error_t set_addressable_led(led_device_t *dev);
static void ws2812_send_data(led_device_t *dev);

/**
 * @brief 打开并初始化LED设备
 * 
 * @param config LED配置参数
 * @return dev_led_handle_t 初始化成功的LED设备句柄，失败返回NULL
 */
dev_led_handle_t dev_led_open(const led_config_t *config)
{
    if (!config) return NULL;
    
    // 创建设备实例
    led_device_t *dev = (led_device_t *)malloc(sizeof(led_device_t));
    if (!dev) return NULL;
    
    // 初始化配置
    memcpy(&dev->config, config, sizeof(led_config_t));

    // 设置LED设备的初始状态
    dev->control.state = LED_STATE_OFF;
    dev->control.brightness = 255;
    
    // 配置LED闪烁参数
    dev->control.blink.on_time_ms = 500;
    dev->control.blink.off_time_ms = 500;
    dev->control.blink.cycles = 0;
    
    // 设置LED的默认RGB颜色值
    dev->control.rgb_color.r = 255;
    dev->control.rgb_color.g = 255;
    dev->control.rgb_color.b = 255;
    
    // 根据类型初始化硬件
    switch (dev->config.type) {
        case LED_TYPE_GPIO: {
            // 初始化GPIO引脚为输出
            GPIO_InitTypeDef GPIO_InitStruct = {0};
            GPIO_InitStruct.Pin = dev->config.gpio.pin;
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            HAL_GPIO_Init(dev->config.gpio.gpiox, &GPIO_InitStruct);
            
            // 初始化为关闭状态
            HAL_GPIO_WritePin(dev->config.gpio.gpiox, dev->config.gpio.pin, GPIO_PIN_RESET);
            break;
        }
            
        case LED_TYPE_ADDRESSABLE: {
            // 初始化WS2812设备
            WS2812_Init_Dynamic(&htim2, TIM_CHANNEL_2, dev->config.addressable.led_count);
            
            // 保存设备指针用于DMA回调
            g_ws2812_device = dev;
            break;
        }
            
        default:
            free(dev);
            return NULL;
    }
    
    // 应用初始状态
    apply_led_state(dev);
    
    return (dev_led_handle_t)dev;
}

/**
 * @brief 关闭LED设备并释放资源
 * 
 * @param handle LED设备句柄
 * @return dev_error_t 操作结果状态
 */
dev_error_t dev_led_close(dev_led_handle_t handle)
{
    if (!handle) return DEV_ERR_PARAM;
    
    led_device_t *dev = (led_device_t *)handle;
    
    // 关闭LED
    dev->control.state = LED_STATE_OFF;
    apply_led_state(dev);
    
    // 释放WS2812动态分配的内存
    if (dev->config.type == LED_TYPE_ADDRESSABLE) {
        WS2812_DeInit();
        
        // 清除全局设备指针
        if (g_ws2812_device == dev) {
            g_ws2812_device = NULL;
        }
    }
    
    free(dev);
    return DEV_OK;
}

/**
 * @brief 控制LED设备的各种操作
 * 
 * @param handle LED设备句柄
 * @param cmd 控制命令
 * @param arg 控制参数
 * @return dev_error_t 操作结果状态
 */
dev_error_t dev_led_ioctl(dev_led_handle_t handle, led_ioctl_cmd_t cmd, void *arg)
{
    if (!handle) return DEV_ERR_PARAM;
    
    led_device_t *dev = (led_device_t *)handle;
    dev_error_t ret = DEV_OK;
    
    switch (cmd) {
        case LED_IOCTL_SET_STATE:
            if (!arg) return DEV_ERR_PARAM;
            dev->control.state = *(led_state_t *)arg;
            apply_led_state(dev);
            break;
            
        case LED_IOCTL_GET_STATE:
            if (!arg) return DEV_ERR_PARAM;
            *(led_state_t *)arg = dev->control.state;
            break;
            
        case LED_IOCTL_SET_BRIGHTNESS:
            if (!arg) return DEV_ERR_PARAM;
            dev->control.brightness = *(uint8_t *)arg;
            apply_led_state(dev);
            break;
            
        case LED_IOCTL_GET_BRIGHTNESS:
            if (!arg) return DEV_ERR_PARAM;
            *(uint8_t *)arg = dev->control.brightness;
            break;
            
        case LED_IOCTL_SET_BLINK_PARAMS:
            if (!arg) return DEV_ERR_PARAM;
            memcpy(&dev->control.blink, arg, sizeof(dev->control.blink));
            if (dev->control.state == LED_STATE_BLINK) {
                apply_led_state(dev);
            }
            break;
            
        case LED_IOCTL_SET_RGB_COLOR:
            if (!arg) return DEV_ERR_PARAM;
            {
                uint32_t color = *(uint32_t *)arg;
                dev->control.rgb_color.r = (color >> 16) & 0xFF;
                dev->control.rgb_color.g = (color >> 8) & 0xFF;
                dev->control.rgb_color.b = color & 0xFF;
                apply_led_state(dev);
            }
            break;
            
        case LED_IOCTL_GET_RGB_COLOR:
            if (!arg) return DEV_ERR_PARAM;
            {
                uint32_t color = (dev->control.rgb_color.r << 16) | 
                                (dev->control.rgb_color.g << 8) | 
                                dev->control.rgb_color.b;
                *(uint32_t *)arg = color;
            }
            break;
            
        case LED_IOCTL_START_BREATH:
            dev->control.state = LED_STATE_BREATH;
            if (arg) {
                // 简化处理，将参数作为亮度值
                dev->control.brightness = *(uint16_t *)arg;
            }
            apply_led_state(dev);
            break;
            
        case LED_IOCTL_STOP_BREATH:
            dev->control.state = LED_STATE_OFF;
            apply_led_state(dev);
            break;
            
        case LED_IOCTL_SET_PATTERN:
            ret = DEV_ERR_NOT_SUPPORT;
            break;
            
        default:
            ret = DEV_ERR_NOT_SUPPORT;
            break;
    }
    
    return ret;
}

/**
 * @brief 写入数据到LED设备
 * 
 * @param handle LED设备句柄
 * @param data 数据缓冲区
 * @param size 数据大小
 * @return int 成功写入的字节数，出错返回负值
 */
int dev_led_write(dev_led_handle_t handle, const void *data, size_t size)
{
    if (!handle || !data || size == 0) return -DEV_ERR_PARAM;
    
    led_device_t *dev = (led_device_t *)handle;
    
    switch (dev->config.type) {
        case LED_TYPE_GPIO: {
            // 对于GPIO LED，写入操作解释为亮灭控制
            if (size >= 1) {
                uint8_t value = *(const uint8_t *)data;
                
                // 非0值表示点亮，0值表示熄灭
                if (value) {
                    dev->control.state = LED_STATE_ON;
                } else {
                    dev->control.state = LED_STATE_OFF;
                }
                
                apply_led_state(dev);
                return 1;
            }
            break;
        }
            
        case LED_TYPE_ADDRESSABLE: {
            // 对于WS2812 LED，写入操作解释为颜色控制 (GRB格式)
            if (size >= 3) {
                const uint8_t *color_data = (const uint8_t *)data;
                
                // 按照GRB顺序解析颜色数据
                dev->control.rgb_color.g = color_data[0]; // Green
                dev->control.rgb_color.r = color_data[1]; // Red
                dev->control.rgb_color.b = color_data[2]; // Blue
                
                // 确保LED处于开启状态
                if (dev->control.state == LED_STATE_OFF) {
                    dev->control.state = LED_STATE_ON;
                }
                
                apply_led_state(dev);
                return 3;
            } else if (size >= 1) {
                // 兼容模式：单字节写入解释为亮度控制
                uint8_t brightness = *(const uint8_t *)data;
                dev->control.brightness = brightness;
                
                // 如果当前是关闭状态，则切换到开启状态
                if (dev->control.state == LED_STATE_OFF) {
                    dev->control.state = LED_STATE_ON;
                }
                
                apply_led_state(dev);
                return 1;
            }
            break;
        }
    }
    
    return -DEV_ERR_PARAM;
}

/**
 * @brief 从LED设备读取数据
 * 
 * @param handle LED设备句柄
 * @param buffer 数据缓冲区
 * @param size 缓冲区大小
 * @return int 成功读取的字节数，出错返回负值
 */
int dev_led_read(dev_led_handle_t handle, void *buffer, size_t size)
{
    if (!handle || !buffer || size < 1) return -DEV_ERR_PARAM;
    
    led_device_t *dev = (led_device_t *)handle;
    
    switch (dev->config.type) {
        case LED_TYPE_GPIO: {
            // 对于GPIO LED，读取操作返回当前状态 (0表示熄灭，非0表示点亮)
            if (size >= 1) {
                if (dev->control.state == LED_STATE_OFF) {
                    *(uint8_t *)buffer = 0;
                } else {
                    *(uint8_t *)buffer = 1;
                }
                return 1;
            }
            break;
        }
            
        case LED_TYPE_ADDRESSABLE: {
            // 对于WS2812 LED，读取操作返回当前颜色值 (GRB格式)
            if (size >= 3) {
                uint8_t *color_buffer = (uint8_t *)buffer;
                
                // 按照GRB顺序返回颜色数据
                color_buffer[0] = dev->control.rgb_color.g; // Green
                color_buffer[1] = dev->control.rgb_color.r; // Red
                color_buffer[2] = dev->control.rgb_color.b; // Blue
                
                return 3;
            } else if (size >= 1) {
                // 兼容模式：单字节读取返回当前亮度值
                *(uint8_t *)buffer = dev->control.brightness;
                return 1;
            }
            break;
        }
    }
    
    return -DEV_ERR_PARAM;
}

/**
 * @brief 应用当前LED的状态
 * 
 * @param dev LED设备指针
 */
static void apply_led_state(led_device_t *dev)
{
    if (!dev) return;

    switch (dev->config.type) {
        case LED_TYPE_GPIO: {
            set_gpio_led(dev);
            break;
        }
            
        case LED_TYPE_ADDRESSABLE: {
            set_addressable_led(dev);
            break;
        }
    }
}

/**
 * @brief 设置GPIO LED的状态
 * 
 * @param dev LED设备指针
 * @return dev_error_t 操作结果状态
 */
static dev_error_t set_gpio_led(led_device_t *dev)
{
    // 对于GPIO LED，我们只关心开/关状态
    GPIO_PinState state = GPIO_PIN_RESET;
    
    switch (dev->control.state) {
        case LED_STATE_OFF:
            state = GPIO_PIN_RESET;
            break;
            
        case LED_STATE_ON:
            state = (dev->control.brightness > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET;
            break;
            
        case LED_STATE_BLINK:
        case LED_STATE_BREATH:
        case LED_STATE_PATTERN:
            // 这些模式需要定时器支持，此处简化为开启
            state = GPIO_PIN_SET;
            break;
    }
    
    // 设置GPIO状态
    HAL_GPIO_WritePin(dev->config.gpio.gpiox, dev->config.gpio.pin, state);
    return DEV_OK;
}

/**
 * @brief 设置可寻址LED的状态
 * 
 * @param dev LED设备指针
 * @return dev_error_t 操作结果状态
 */
static dev_error_t set_addressable_led(led_device_t *dev)
{
    uint8_t r = dev->control.rgb_color.r;
    uint8_t g = dev->control.rgb_color.g;
    uint8_t b = dev->control.rgb_color.b;
    
    if (dev->control.state == LED_STATE_OFF) {
        r = g = b = 0;
    }
    
    // 使用WS2812库设置所有LED颜色
    WS2812_Set_All(r, g, b);
    
    // 更新WS2812显示
    ws2812_send_data(dev);
    
    return DEV_OK;
}

/**
 * @brief 通过PWM发送WS2812B数据（非阻塞方式）
 * 
 * @param dev LED设备指针
 */
static void ws2812_send_data(led_device_t *dev)
{
    // 使用WS2812库更新显示
    WS2812_Updata();
}

/**
 * @brief DMA传输完成处理函数
 */
void dev_led_dma_transfer_complete(void)
{
    // 调用WS2812的DMA传输完成处理
    WS2812_DMA_Transfer_Complete();
}