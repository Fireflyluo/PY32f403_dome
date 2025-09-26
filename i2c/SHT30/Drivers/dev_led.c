/*-----------------------------------------------File Info------------------------------------------------
** File Name:               dev_led.c  
** Last modified date:      2025.7.1
** Last version:            V0.1
** Description:             led设备驱动
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            实现led的驱动函数open、close、iatl、write、read
**                          
**                          
**--------------------------------------------------------------------------------------------------------*/
#include "dev_led.h"

// LED设备内部状态结构
typedef struct {
    led_config_t config;     // LED配置
    led_control_t control;   // 当前控制参数
    bool is_breathing;       // 是否正在呼吸效果
    uint16_t breath_period;  // 呼吸周期(ms)
    uint32_t last_update;    // 最后更新时间(ms)
    BSP_PWM_HandleTypeDef pwm_handle;  // 新增PWM句柄
    BSP_PWM_HandleTypeDef pwm_r_handle; // RGB LED
    BSP_PWM_HandleTypeDef pwm_g_handle;
    BSP_PWM_HandleTypeDef pwm_b_handle;
} led_device_t;

// 私有函数声明
static void apply_led_state(led_device_t *dev);
static void update_breathing_effect(led_device_t *dev);
static dev_error_t set_gpio_led(led_device_t *dev);
static dev_error_t set_pwm_led(led_device_t *dev);
static dev_error_t set_rgb_led(led_device_t *dev);
static dev_error_t set_addressable_led(led_device_t *dev);

// ================== 设备驱动接口实现 ==================

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
    dev->control.state = LED_STATE_OFF; // 关闭LED
    dev->control.brightness = 0; // 设置亮度为最低
    
    // 配置LED闪烁参数
    dev->control.blink.on_time_ms = 500; // 闪烁时亮的时间为500毫秒
    dev->control.blink.off_time_ms = 500; // 闪烁时灭的时间为500毫秒
    dev->control.blink.cycles = 0; // 无限循环闪烁
    
    // 设置LED的默认RGB颜色值
    dev->control.rgb_color.r = 255; // 红色分量最大
    dev->control.rgb_color.g = 255; // 绿色分量最大
    dev->control.rgb_color.b = 255; // 蓝色分量最大
    
    // 初始化呼吸灯相关参数
    dev->is_breathing = false; // 呼吸灯效果默认关闭
    dev->breath_period = 2000; // 呼吸灯周期为2000毫秒
    dev->last_update = 0; // 上次更新时间初始化为0
    
    // 初始化亮度调节参数
    float duty = 0.5f; // 初始化占空比为50%

 // 根据类型初始化硬件
    switch (dev->config.type) {
        case LED_TYPE_GPIO: {
            // 初始化GPIO引脚为输出
            if (BSP_GPIO_Init_Output(dev->config.gpio.gpiox,
                                    dev->config.gpio.pin,
                                    BSP_GPIO_PIN_OUT_PP) != BSP_GPIO_OK) {
                free(dev);
                return NULL;
            }
            break;
        }
            
        case LED_TYPE_PWM: {
            // 初始化PWM通道
            PWM_ConfigTypeDef pwm_config = {
                .Prescaler = 0,
                .Period = (SystemCoreClock / dev->config.pwm.frequency) - 1,
                .Pulse = 0,  // 初始占空比为0
                .Polarity = BSP_PWM_POLARITY_HIGH,
                .ClockDivision = TIM_CLOCKDIVISION_DIV1,
                .AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE
            };
            // 初始化PWM
            if (BSP_PWM_Init(&dev->pwm_handle, 
                            dev->config.pwm.htim,
                            dev->config.pwm.channel,
                            &pwm_config) != BSP_PWM_OK) {
                free(dev);
                return NULL;
            }
            
            // 启动PWM输出
            if (BSP_PWM_Start(&dev->pwm_handle) != BSP_PWM_OK) {
                free(dev);
                return NULL;
            }
            break;
          }
            
        case LED_TYPE_RGB: {
            // 初始化RGB PWM通道
            PWM_ConfigTypeDef pwm_config = {
                .Prescaler = 0,
                .Period = (SystemCoreClock / dev->config.rgb.frequency) - 1,
                .Pulse = 0,  // 初始占空比为0
                .Polarity = BSP_PWM_POLARITY_HIGH,
                .ClockDivision = TIM_CLOCKDIVISION_DIV1,
                .AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE
            };
            
            // 初始化R通道
            if (BSP_PWM_Init(&dev->pwm_r_handle, 
                            dev->config.rgb.htim_r,
                            dev->config.rgb.channel_r,
                            &pwm_config) != BSP_PWM_OK) {
                free(dev);
                return NULL;
            }
            
            // 初始化G通道
             if (BSP_PWM_Init(&dev->pwm_g_handle, 
                            dev->config.rgb.htim_g,
                            dev->config.rgb.channel_g,
                            &pwm_config) != BSP_PWM_OK) {
                free(dev);
                return NULL;
            }
            
            // 初始化B通道
            if (BSP_PWM_Init(&dev->pwm_b_handle, 
                            dev->config.rgb.htim_b,
                            dev->config.rgb.channel_b,
                            &pwm_config) != BSP_PWM_OK) {
                free(dev);
                return NULL;
            }
                            
               // 启动所有PWM通道
            BSP_PWM_Start(&dev->pwm_r_handle);
            BSP_PWM_Start(&dev->pwm_g_handle);
            BSP_PWM_Start(&dev->pwm_b_handle);
            break;
        }
            
        case LED_TYPE_ADDRESSABLE:
            // 初始化可寻址LED引脚
            // BSP_WS2812_Init(dev->config.hw_config.addressable.pin, 
            //                 dev->config.hw_config.addressable.count);
            break;
            
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
    led_control_t prev = dev->control;
    dev->control.state = LED_STATE_OFF;
    apply_led_state(dev);
    
    // 释放资源
    switch (dev->config.type) {
        case LED_TYPE_PWM:
             BSP_PWM_DeInit(&dev->pwm_handle);
            break;
            
        case LED_TYPE_RGB:
             BSP_PWM_DeInit(&dev->pwm_r_handle);
             BSP_PWM_DeInit(&dev->pwm_g_handle);
             BSP_PWM_DeInit(&dev->pwm_b_handle);
            break;
            
        case LED_TYPE_ADDRESSABLE:
            // BSP_WS2812_Deinit();
            break;
            
        default:
            break;
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
            dev->is_breathing = true;
            if (arg) {
                dev->breath_period = *(uint16_t *)arg;
            }
            apply_led_state(dev);
            break;
            
        case LED_IOCTL_STOP_BREATH:
            dev->is_breathing = false;
            dev->control.state = LED_STATE_OFF;
            apply_led_state(dev);
            break;
            
        case LED_IOCTL_SET_PATTERN:
            // 实现自定义模式设置
            // 需要根据具体硬件实现
            ret = DEV_ERR_NOT_SUPPORT;
            break;
            
        default:
            ret = DEV_ERR_NOT_SUPPORT;
            break;
    }
      return ret;
}

/**
 * @brief 写入数据到LED设备（用于设置亮度）
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
    
    // 写入操作解释为设置亮度
    if (size >= 1) {
        uint8_t brightness = *(const uint8_t *)data;
        dev->control.brightness = brightness;
        
        // 如果当前是关闭状态，则切换到开启状态
        if (dev->control.state == LED_STATE_OFF) {
            dev->control.state = LED_STATE_ON;
        }
        
        apply_led_state(dev);
        return 1; // 写入了一个字节
    }
    
    return -DEV_ERR_PARAM;
}

/**
 * @brief 从LED设备读取数据（用于获取当前亮度）
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
    
    // 读取操作返回当前亮度值
    if (size >= 1) {
        *(uint8_t *)buffer = dev->control.brightness;
        return 1; // 读取了一个字节
    }
    
    return -DEV_ERR_PARAM;
}

//=======================私有函数实现 =======================================

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
            // 设置GPIO输出状态
            BSP_GPIO_SetPin(dev->config.gpio.gpiox,
                             dev->config.gpio.pin,
                             (dev->control.state == LED_STATE_ON) ? 
                             BSP_GPIO_PIN_SET : BSP_GPIO_PIN_RESET);
            break;
        }
            
        case LED_TYPE_PWM: {
            // 设置PWM占空比
            float duty = (dev->control.state == LED_STATE_ON) ? 
                        (dev->control.brightness / 255.0f) : 0.0f;
            BSP_PWM_SetDuty(&dev->pwm_handle, duty);
            break;
        }
            
        case LED_TYPE_RGB: {
            // 设置RGB PWM占空比
            float r_duty = (dev->control.state == LED_STATE_ON) ? 
                          (dev->control.rgb_color.r / 255.0f) : 0.0f;
            float g_duty = (dev->control.state == LED_STATE_ON) ? 
                          (dev->control.rgb_color.g / 255.0f) : 0.0f;
            float b_duty = (dev->control.state == LED_STATE_ON) ? 
                          (dev->control.rgb_color.b / 255.0f) : 0.0f;
            
            BSP_PWM_SetDuty(&dev->pwm_r_handle, r_duty);
            BSP_PWM_SetDuty(&dev->pwm_g_handle, g_duty);
            BSP_PWM_SetDuty(&dev->pwm_b_handle, b_duty);
            break;
        }
            
        case LED_TYPE_ADDRESSABLE: {
            // 设置可寻址LED状态
            // 需要实现具体的协议驱动
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
    bool state = false;
    
    switch (dev->control.state) {
        case LED_STATE_OFF:
            state = false;
            break;
            
        case LED_STATE_ON:
            state = (dev->control.brightness > 0);
            break;
            
        case LED_STATE_BLINK:
        case LED_STATE_BREATH:
        case LED_STATE_PATTERN:
            // 这些模式需要定时器支持，此处简化为开启
            state = true;
            break;
    }
    
    // 设置GPIO状态
    BSP_GPIO_SetPin(dev->config.gpio.gpiox,dev->config.gpio.pin, state);
    return DEV_OK;
}

/**
 * @brief 设置PWM LED的状态
 * 
 * @param dev LED设备指针
 * @return dev_error_t 操作结果状态
 */
static dev_error_t set_pwm_led(led_device_t *dev)
{
   uint8_t brightness = dev->control.brightness;
    
    if (dev->control.state == LED_STATE_OFF) {
        brightness = 0;
    }
    
    // 设置PWM占空比 (0-255 映射到 0%-100%)
     float duty = (float)brightness / 255.0f * 100.0f;
     BSP_PWM_SetDuty(&dev->pwm_handle, duty);
    
    return DEV_OK;

}

/**
 * @brief 设置RGB LED的状态
 * 
 * @param dev LED设备指针
 * @return dev_error_t 操作结果状态
 */
static dev_error_t set_rgb_led(led_device_t *dev)
{
    uint8_t r = dev->control.rgb_color.r;
    uint8_t g = dev->control.rgb_color.g;
    uint8_t b = dev->control.rgb_color.b;
    
    if (dev->control.state == LED_STATE_OFF) {
        r = g = b = 0;
    } else {
        // 应用全局亮度
        r = (r * dev->control.brightness) / 255;
        g = (g * dev->control.brightness) / 255;
        b = (b * dev->control.brightness) / 255;
    }
    
    // 设置RGB通道
     BSP_PWM_SetDuty(&dev->pwm_r_handle, (float)r / 255.0f * 100.0f);
     BSP_PWM_SetDuty(&dev->pwm_g_handle, (float)g / 255.0f * 100.0f);
     BSP_PWM_SetDuty(&dev->pwm_b_handle, (float)b / 255.0f * 100.0f);
    
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
  // 对于可寻址LED，我们需要设置所有LED的颜色
  
    uint8_t r = dev->control.rgb_color.r;
    uint8_t g = dev->control.rgb_color.g;
    uint8_t b = dev->control.rgb_color.b;
    
    if (dev->control.state == LED_STATE_OFF) {
        r = g = b = 0;
    } else {
        // 应用全局亮度
        r = (r * dev->control.brightness) / 255;
        g = (g * dev->control.brightness) / 255;
        b = (b * dev->control.brightness) / 255;
    }
    
    // 设置所有LED为相同颜色
    // for (int i = 0; i < dev->config.hw_config.addressable.count; i++) {
    //     BSP_WS2812_SetPixel(i, r, g, b);
    // }
    // BSP_WS2812_Update();
    
    return DEV_OK;
}

/**
 * @brief 更新呼吸效果
 * 
 * @param dev LED设备指针
 */
static void update_breathing_effect(led_device_t *dev)
{
    if (!dev->is_breathing) return;

    // 获取当前时间（单位：ms），例如使用 HAL_GetTick()
    uint32_t current_time = HAL_GetTick();

    // 计算当前呼吸周期中的位置 (0-1)
    float phase = (current_time - dev->last_update) / (float)dev->breath_period;
    phase = fmodf(phase, 1.0f);  // 取模确保在 [0, 1) 范围内

    // 正弦波呼吸效果：从 0 到 1 再到 0，形成平滑的呼吸曲线
    float brightness = 0.5f * (1.0f + sin(2 * M_PI * phase));
    dev->control.brightness = (uint8_t)(brightness * 255);

    // 更新LED状态
    apply_led_state(dev);

    // 记录最后更新时间
    dev->last_update = current_time;
}
//=======================示例=================
// 配置PWM LED
//led_config_t pwm_led_config = {
//    .type = LED_TYPE_PWM,
//    .hw_config = {
//        .pwm = {
//            .htim = &htim2,          // 使用TIM2
//            .channel = TIM_CHANNEL_1, // 使用通道1
//            .pin = GPIO_PIN_0,       // PA0
//            .frequency = 1000         // 1kHz
//        }
//    }
//};

//// 打开LED设备
//dev_led_handle_t led = dev_led_open(&pwm_led_config);

//// 设置LED亮度为75%
//led_device_t *dev = (led_device_t *)led;
//dev->control.state = LED_STATE_ON;
//dev->control.brightness = 191; // 255 * 0.75 ≈ 191
//apply_led_state(dev);