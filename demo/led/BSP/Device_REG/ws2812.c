#include "ws2812.h"

uint8_t color[LED_COUNT][3];
uint16_t ws2812_static_pwm_data[LED_COUNT*3*8+1]; // 静态缓冲区
// 静态设备实例
static ws2812_device_t ws2812_dev = {0};

/**
 * @brief 获取当前LED数量
 * @return LED数量
 */
uint32_t WS2812_GetLedCount(void)
{
    // 如果动态模式下设置了LED数量，则返回该数量
    if(ws2812_dev.led_count > 0) {
        return ws2812_dev.led_count;
    }
    // 否则返回默认的LED_COUNT
    return LED_COUNT;
}

/**
 * @brief 初始化WS2812B设备（默认使用静态缓冲区）
 * @param timer 定时器句柄
 * @param channel 定时器通道
 */
void WS2812_Init(TIM_HandleTypeDef* timer, uint32_t channel)
{
    // WS2812_Init_Static(timer, channel);
    WS2812_Init_Dynamic(timer, channel, LED_COUNT);
}

/**
 * @brief 初始化WS2812B设备（使用静态缓冲区）
 * @param timer 定时器句柄
 * @param channel 定时器通道
 */
void WS2812_Init_Static(TIM_HandleTypeDef* timer, uint32_t channel)
{
    // 定时器句柄和通道
    ws2812_dev.timer_handle = timer;
    ws2812_dev.timer_channel = channel;

    ws2812_dev.use_static_buffer = 1;// 使用静态缓冲区
    ws2812_dev.pwm_data = ws2812_static_pwm_data; // 指向静态PWM数据缓冲区
    ws2812_dev.pwm_data_size = LED_COUNT*3*8 + 1;// PWM数据大小
    
    // 静态模式下无用
    ws2812_dev.led_count = 0;
    ws2812_dev.dynamic_color_data = NULL;
    
    // 清空所有LED数据
    WS2812_Clear();
}

/**
 * @brief 初始化WS2812B设备（使用动态缓冲区）
 * @param timer 定时器句柄
 * @param channel 定时器通道
 * @param led_count LED数量
 */
void WS2812_Init_Dynamic(TIM_HandleTypeDef* timer, uint32_t channel, uint32_t led_count)
{
    // 定时器句柄和通道
    ws2812_dev.timer_handle = timer;
    ws2812_dev.timer_channel = channel;

    ws2812_dev.use_static_buffer = 0; // 使用动态缓冲区
    // 保存LED数量
    ws2812_dev.led_count = led_count;
    
    // 动态分配pwm_data内存
    ws2812_dev.pwm_data_size = led_count*3*8 + 1;
    ws2812_dev.pwm_data = (uint16_t*)malloc(ws2812_dev.pwm_data_size * sizeof(uint16_t));
    
    // 动态分配颜色数据内存
    if(led_count > 0) {
        ws2812_dev.dynamic_color_data = (uint8_t(*)[3])malloc(led_count * 3 * sizeof(uint8_t));
    } else {
        ws2812_dev.dynamic_color_data = NULL;
    }
    
    // 清空所有LED数据
    WS2812_Clear();
}

/**
 * @brief 释放动态分配的内存
 */
void WS2812_DeInit(void)
{
    // 只有在使用动态缓冲区时才释放
    if(!ws2812_dev.use_static_buffer) {
        // 释放PWM数据内存
        if(ws2812_dev.pwm_data != NULL) {
            free(ws2812_dev.pwm_data);
            ws2812_dev.pwm_data = NULL;
            ws2812_dev.pwm_data_size = 0;
        }
        
        // 释放颜色数据内存
        if(ws2812_dev.dynamic_color_data != NULL) {
            free(ws2812_dev.dynamic_color_data);
            ws2812_dev.dynamic_color_data = NULL;
        }
        
        // 重置LED数量
        ws2812_dev.led_count = 0;
    }
}

/**
 * @brief 设置指定LED的颜色
 * @param index LED索引
 * @param R 红色分量
 * @param G 绿色分量
 * @param B 蓝色分量
 */
void WS2812_Set(uint8_t index,uint8_t R,uint8_t G,uint8_t B)
{
    // 检查索引是否超出范围
    if(index >= WS2812_GetLedCount()) return;
    
    if(ws2812_dev.use_static_buffer) {
        // 使用静态颜色数组
        color[index][0]=R;
        color[index][1]=G;
        color[index][2]=B;
    } else {
        // 使用动态颜色数组
        if(ws2812_dev.dynamic_color_data != NULL) {
            ws2812_dev.dynamic_color_data[index][0]=R;
            ws2812_dev.dynamic_color_data[index][1]=G;
            ws2812_dev.dynamic_color_data[index][2]=B;
        }
    }
}

/**
 * @brief 设置所有LED为相同颜色
 * @param R 红色分量
 * @param G 绿色分量
 * @param B 蓝色分量
 */
void WS2812_Set_All(uint8_t R,uint8_t G,uint8_t B)
{
    // 遍历所有LED并设置相同颜色
    for(uint32_t i=0;i<WS2812_GetLedCount();i++)
    {
        WS2812_Set(i,R,G,B);
    }
}

/**
 * @brief 更新LED显示（发送数据到WS2812）
 */
void WS2812_Updata()
{
   // 检查是否已分配内存(对于动态模式)或使用静态缓冲区
   if(!ws2812_dev.use_static_buffer && (ws2812_dev.pwm_data == NULL)) {
       return;
   }

   // 遍历所有LED
   for(uint32_t i=0;i<WS2812_GetLedCount();i++)
   {
    uint8_t R, G, B;
    
    // 根据使用模式获取颜色数据
    if(ws2812_dev.use_static_buffer) {
        // 静态颜色数组
        R = color[i][0];
        G = color[i][1];
        B = color[i][2];
    } else {
        // 动态颜色数组
        if(ws2812_dev.dynamic_color_data != NULL) {
            R = ws2812_dev.dynamic_color_data[i][0];
            G = ws2812_dev.dynamic_color_data[i][1];
            B = ws2812_dev.dynamic_color_data[i][2];
        } else {
            R = G = B = 0;
        }
    }

    // 生成24位颜色数据的PWM波形
    // 每个颜色分量用8位表示，需要生成8个PWM脉冲
       for(uint8_t j=0;j<8;j++)
       {
           // 根据WS2812B协议，高位为1时输出高电平时间长，低位为0时输出高电平时间短
           ws2812_dev.pwm_data[i*3*8+j]=(G & (0x80>>j) ? WS2812_BIT_1_PERIOD : WS2812_BIT_0_PERIOD);
           ws2812_dev.pwm_data[i*3*8+j+8]=(R & (0x80>>j) ? WS2812_BIT_1_PERIOD : WS2812_BIT_0_PERIOD);
           ws2812_dev.pwm_data[i*3*8+j+16]=(B & (0x80>>j) ? WS2812_BIT_1_PERIOD : WS2812_BIT_0_PERIOD);
       }
   }
   // 添加RESET信号（低电平保持时间）
   ws2812_dev.pwm_data[WS2812_GetLedCount()*3*8]=WS2812_RESET_PERIOD;

    // 停止之前的DMA传输
    HAL_TIM_PWM_Stop_DMA(ws2812_dev.timer_handle, ws2812_dev.timer_channel);
     // 设置比较值为0
    __HAL_TIM_SET_COMPARE(ws2812_dev.timer_handle, ws2812_dev.timer_channel, 0);
     // 开始新的DMA传输

    HAL_TIM_PWM_Start_DMA(ws2812_dev.timer_handle, 
                         ws2812_dev.timer_channel,
                         (uint32_t*)ws2812_dev.pwm_data, 
                         ws2812_dev.pwm_data_size);
}

/**
 * @brief 清空所有LED颜色数据（设置为黑色）
 */
void WS2812_Clear(void)
{
     // 遍历所有LED并设置颜色为0（关闭）
     for(uint32_t i=0;i<WS2812_GetLedCount();i++)
    {
        if(ws2812_dev.use_static_buffer) {
            // 静态颜色数组
            color[i][0]=0;
            color[i][1]=0;
            color[i][2]=0;
        } else {
            // 动态颜色数组
            if(ws2812_dev.dynamic_color_data != NULL) {
                ws2812_dev.dynamic_color_data[i][0]=0;
                ws2812_dev.dynamic_color_data[i][1]=0;
                ws2812_dev.dynamic_color_data[i][2]=0;
            }
        }
    }
}

/**
 * @brief DMA传输完成回调
 */
void WS2812_DMA_Transfer_Complete(void)
{
    // 停止PWM输出
    HAL_TIM_PWM_Stop(ws2812_dev.timer_handle, ws2812_dev.timer_channel);
}