#ifndef __WS2812_H__
#define __WS2812_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

#include "gpio.h"
#include "tim.h"

// WS2812B时序参数 800kHz
#define WS2812_BIT_1_PERIOD 120  
#define WS2812_BIT_0_PERIOD 60    
#define WS2812_RESET_PERIOD 0   

// LED数量
#define LED_COUNT 1     

// 颜色数据结构
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} ws2812_color_t;

// WS2812B设备结构
typedef struct {
    TIM_HandleTypeDef* timer_handle;
    uint32_t timer_channel;
    uint16_t* pwm_data;        // 动态分配的pwm_data指针
    uint32_t pwm_data_size;    // pwm_data大小记录
    uint8_t use_static_buffer; // 标志位，1表示使用静态缓冲区，0表示使用动态缓冲区
    uint32_t led_count;        // LED数量（用于动态模式）
    uint8_t (*dynamic_color_data)[3]; // 动态分配的颜色数据指针
} ws2812_device_t;

// 添加静态缓冲区声明
extern uint16_t ws2812_static_pwm_data[LED_COUNT*3*8+1];
extern uint8_t color[LED_COUNT][3];

void WS2812_Init(TIM_HandleTypeDef* timer, uint32_t channel);
void WS2812_Init_Static(TIM_HandleTypeDef* timer, uint32_t channel);  // 使用静态缓冲区初始化
void WS2812_Init_Dynamic(TIM_HandleTypeDef* timer, uint32_t channel, uint32_t led_count); // 使用动态缓冲区初始化
void WS2812_Updata();
void WS2812_Set(uint8_t index,uint8_t R,uint8_t G,uint8_t B);   
void WS2812_Set_All(uint8_t R,uint8_t G,uint8_t B);
void WS2812_DMA_Transfer_Complete(void);
void WS2812_Clear(void);
void WS2812_DeInit(void);  // 释放动态分配的内存

// 获取当前LED数量的函数
uint32_t WS2812_GetLedCount(void);

#ifdef __cplusplus
}
#endif

#endif








///////////////////////////////////////////////////////////////////////////////////////////////////////
