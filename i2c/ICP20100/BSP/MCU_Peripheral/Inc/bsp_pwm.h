#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "bsp_common.h"


/* PWM通道定义 */
typedef enum {
    BSP_PWM_CHANNEL_1 = TIM_CHANNEL_1,
    BSP_PWM_CHANNEL_2 = TIM_CHANNEL_2,
    BSP_PWM_CHANNEL_3 = TIM_CHANNEL_3,
    BSP_PWM_CHANNEL_4 = TIM_CHANNEL_4,
} BSP_PWM_ChannelTypeDef;

/* PWM极性定义 */
typedef enum {
    BSP_PWM_POLARITY_HIGH = TIM_OCPOLARITY_HIGH,
    BSP_PWM_POLARITY_LOW = TIM_OCPOLARITY_LOW,
} BSP_PWM_PolarityTypeDef;

/* PWM配置结构体 */
typedef struct {
    uint32_t Prescaler;             // 预分频值
    uint32_t Period;                // 自动重装载值
    uint32_t Pulse;                 // 初始脉冲值(占空比)
    BSP_PWM_PolarityTypeDef Polarity; // 极性
    uint32_t ClockDivision;         // 时钟分频
    uint32_t AutoReloadPreload;     // 自动重装载预装载使能
} PWM_ConfigTypeDef;

/* 函数返回值枚举 */
typedef enum {
    BSP_PWM_OK = 0,           // PWM操作成功
    BSP_PWM_ERR_INVALID_CH,   // 无效的PWM通道
    BSP_PWM_ERR_NULL_PTR,     // 空指针错误
    BSP_PWM_ERR_HAL_FAIL,     // HAL库调用失败
    BSP_PWM_ERR_NOT_INIT,     // PWM未初始化
} BSP_PWM_Status_TypeDef;

/* PWM句柄结构 */
typedef struct {
    TIM_HandleTypeDef *htim;          // HAL定时器句柄
    BSP_PWM_ChannelTypeDef channel;   // PWM通道
    uint8_t isInitialized;            // 初始化标志
} BSP_PWM_HandleTypeDef;

/* 函数声明 */
BSP_PWM_Status_TypeDef BSP_PWM_Init(BSP_PWM_HandleTypeDef *bsp_pwm, TIM_HandleTypeDef *htim, 
                                   BSP_PWM_ChannelTypeDef channel, const PWM_ConfigTypeDef *config);
BSP_PWM_Status_TypeDef BSP_PWM_DeInit(BSP_PWM_HandleTypeDef *bsp_pwm);

BSP_PWM_Status_TypeDef BSP_PWM_Start(BSP_PWM_HandleTypeDef *bsp_pwm);
BSP_PWM_Status_TypeDef BSP_PWM_Stop(BSP_PWM_HandleTypeDef *bsp_pwm);
BSP_PWM_Status_TypeDef BSP_PWM_SetDuty(BSP_PWM_HandleTypeDef *bsp_pwm, float duty_cycle);
BSP_PWM_Status_TypeDef BSP_PWM_SetFrequency(BSP_PWM_HandleTypeDef *bsp_pwm, uint32_t frequency_hz);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_PWM_H */
