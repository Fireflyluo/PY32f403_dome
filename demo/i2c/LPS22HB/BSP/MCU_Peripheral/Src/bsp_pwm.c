#include "bsp_pwm.h"
#include <math.h>

/**
  * @brief  初始化PWM
  * @param  bsp_pwm: BSP PWM句柄
  * @param  htim: HAL定时器句柄
  * @param  channel: PWM通道
  * @param  config: PWM配置结构体
  * @retval BSP_PWM_Status_TypeDef: 操作状态
  */
BSP_PWM_Status_TypeDef BSP_PWM_Init(BSP_PWM_HandleTypeDef *bsp_pwm, TIM_HandleTypeDef *htim, 
                                   BSP_PWM_ChannelTypeDef channel, const PWM_ConfigTypeDef *config)
{
    TIM_OC_InitTypeDef sConfigOC = {0};
    
    /* 参数检查 */
    if (bsp_pwm == NULL || htim == NULL || config == NULL) {
        return BSP_PWM_ERR_NULL_PTR;
    }

    /* 初始化定时器基础参数 */
    htim->Init.Prescaler = config->Prescaler;
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = config->Period;
    htim->Init.ClockDivision = config->ClockDivision;
    htim->Init.AutoReloadPreload = config->AutoReloadPreload;
    
    if (HAL_TIM_PWM_Init(htim) != HAL_OK) {
        return BSP_PWM_ERR_HAL_FAIL;
    }

    /* 配置PWM通道 */
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = config->Pulse;
    sConfigOC.OCPolarity = config->Polarity;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    
    if (HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, channel) != HAL_OK) {
        return BSP_PWM_ERR_HAL_FAIL;
    }

    /* 初始化BSP PWM句柄 */
    bsp_pwm->htim = htim;
    bsp_pwm->channel = channel;
    bsp_pwm->isInitialized = 1;

    return BSP_PWM_OK;
}
/**
  * @brief  反初始化PWM
  * @param  bsp_pwm: BSP PWM句柄
  * @retval BSP_PWM_Status_TypeDef: 操作状态
  */
BSP_PWM_Status_TypeDef BSP_PWM_DeInit(BSP_PWM_HandleTypeDef *bsp_pwm)
{
    /* 参数检查 */
    if (bsp_pwm == NULL) {
        return BSP_PWM_ERR_NULL_PTR;
    }

    if (bsp_pwm->isInitialized == 0) {
        return BSP_PWM_ERR_NOT_INIT;
    }

    /* 停止PWM输出 */
    if (HAL_TIM_PWM_Stop(bsp_pwm->htim, bsp_pwm->channel) != HAL_OK) {
        return BSP_PWM_ERR_HAL_FAIL;
    }

    /* 反初始化HAL定时器 */
    if (HAL_TIM_PWM_DeInit(bsp_pwm->htim) != HAL_OK) {
        return BSP_PWM_ERR_HAL_FAIL;
    }

    /* 清除初始化标志 */
    bsp_pwm->isInitialized = 0;

    return BSP_PWM_OK;
}
/**
  * @brief  启动PWM输出
  * @param  bsp_pwm: BSP PWM句柄
  * @retval BSP_PWM_Status_TypeDef: 操作状态
  */
BSP_PWM_Status_TypeDef BSP_PWM_Start(BSP_PWM_HandleTypeDef *bsp_pwm)
{
    /* 参数检查 */
    if (bsp_pwm == NULL) {
        return BSP_PWM_ERR_NULL_PTR;
    }
    
    if (bsp_pwm->isInitialized == 0) {
        return BSP_PWM_ERR_NOT_INIT;
    }

    /* 启动PWM */
    if (HAL_TIM_PWM_Start(bsp_pwm->htim, bsp_pwm->channel) != HAL_OK) {
        return BSP_PWM_ERR_HAL_FAIL;
    }

    return BSP_PWM_OK;
}

/**
  * @brief  停止PWM输出
  * @param  bsp_pwm: BSP PWM句柄
  * @retval BSP_PWM_Status_TypeDef: 操作状态
  */
BSP_PWM_Status_TypeDef BSP_PWM_Stop(BSP_PWM_HandleTypeDef *bsp_pwm)
{
    /* 参数检查 */
    if (bsp_pwm == NULL) {
        return BSP_PWM_ERR_NULL_PTR;
    }
    
    if (bsp_pwm->isInitialized == 0) {
        return BSP_PWM_ERR_NOT_INIT;
    }

    /* 停止PWM */
    if (HAL_TIM_PWM_Stop(bsp_pwm->htim, bsp_pwm->channel) != HAL_OK) {
        return BSP_PWM_ERR_HAL_FAIL;
    }

    return BSP_PWM_OK;
}

/**
  * @brief  设置PWM占空比
  * @param  bsp_pwm: BSP PWM句柄
  * @param  duty_cycle: 占空比(0.0 - 1.0)
  * @retval BSP_PWM_Status_TypeDef: 操作状态
  */
BSP_PWM_Status_TypeDef BSP_PWM_SetDuty(BSP_PWM_HandleTypeDef *bsp_pwm, float duty_cycle)
{
    TIM_OC_InitTypeDef sConfigOC = {0};
    uint32_t pulse;
    
    /* 参数检查 */
    if (bsp_pwm == NULL) {
        return BSP_PWM_ERR_NULL_PTR;
    }
    
    if (bsp_pwm->isInitialized == 0) {
        return BSP_PWM_ERR_NOT_INIT;
    }

    if (duty_cycle < 0.0f || duty_cycle > 1.0f) {
        return BSP_PWM_ERR_HAL_FAIL;  // 占空比超出范围
    }

    /* 计算脉冲值 */
    pulse = (uint32_t)(duty_cycle * (float)(bsp_pwm->htim->Init.Period + 1));
    
    /* 更新PWM占空比 */
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = pulse;
    sConfigOC.OCPolarity = (bsp_pwm->channel == BSP_PWM_CHANNEL_1) ? 
                          bsp_pwm->htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 :
                          (bsp_pwm->channel == BSP_PWM_CHANNEL_2) ? 
                          bsp_pwm->htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2 :
                          (bsp_pwm->channel == BSP_PWM_CHANNEL_3) ? 
                          bsp_pwm->htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3 :
                          bsp_pwm->htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    
    if (HAL_TIM_PWM_ConfigChannel(bsp_pwm->htim, &sConfigOC, bsp_pwm->channel) != HAL_OK) {
        return BSP_PWM_ERR_HAL_FAIL;
    }
    
    /* 重新启动PWM以应用新配置 */
    return BSP_PWM_Start(bsp_pwm);
}

/**
  * @brief  设置PWM频率
  * @param  bsp_pwm: BSP PWM句柄
  * @param  frequency_hz: 频率(Hz)
  * @retval BSP_PWM_Status_TypeDef: 操作状态
  */
BSP_PWM_Status_TypeDef BSP_PWM_SetFrequency(BSP_PWM_HandleTypeDef *bsp_pwm, uint32_t frequency_hz)
{
    uint32_t timer_clock;
    uint32_t prescaler;
    uint32_t period;
    
    /* 参数检查 */
    if (bsp_pwm == NULL) {
        return BSP_PWM_ERR_NULL_PTR;
    }
    
    if (bsp_pwm->isInitialized == 0) {
        return BSP_PWM_ERR_NOT_INIT;
    }

    if (frequency_hz == 0) {
        return BSP_PWM_ERR_HAL_FAIL;  // 频率不能为0
    }

    /* 获取定时器时钟频率 */
    if (bsp_pwm->htim->Instance == TIM1 || bsp_pwm->htim->Instance == TIM8 || 
        bsp_pwm->htim->Instance == TIM9 || bsp_pwm->htim->Instance == TIM10 || 
        bsp_pwm->htim->Instance == TIM11) {
        // 高级定时器时钟可能不同
        timer_clock = HAL_RCC_GetPCLK2Freq() * 2;
    } else {
        timer_clock = HAL_RCC_GetPCLK1Freq() * 2;
    }

    /* 计算预分频和周期值 */
    prescaler = (uint32_t)(timer_clock / (frequency_hz * 65536)) + 1;
    period = (timer_clock / (prescaler * frequency_hz)) - 1;

    /* 更新定时器配置 */
    bsp_pwm->htim->Init.Prescaler = prescaler - 1;
    bsp_pwm->htim->Init.Period = period;
    
    if (HAL_TIM_PWM_Init(bsp_pwm->htim) != HAL_OK) {
        return BSP_PWM_ERR_HAL_FAIL;
    }

    /* 重新启动PWM以应用新配置 */
    return BSP_PWM_Start(bsp_pwm);
}
