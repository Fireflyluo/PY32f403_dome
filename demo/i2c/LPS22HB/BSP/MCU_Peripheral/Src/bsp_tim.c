#include "bsp_tim.h"
#include <string.h>

 BSP_TIM_HandleTypeDef tim_handles[BSP_TIM_NUM];

// 使用查找表替代switch-case
static TIM_TypeDef* const tim_instances[BSP_TIM_NUM] = {
    TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8,
    TIM9, TIM10, TIM11, TIM12, TIM13, TIM14
};

// 使用查找表替代switch-case
static const uint32_t hal_channels[] = {
    TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4
};

static inline uint32_t get_hal_channel(uint32_t channel)
{
    return (channel >= 1 && channel <= 4) ? hal_channels[channel-1] : TIM_CHANNEL_1;
}

static inline bool is_valid_tim_num(TIM_Num_TypeDef tim_num)
{
    return tim_num < BSP_TIM_NUM;
}

BSP_TIM_Status_TypeDef BSP_TIM_Init(TIM_Num_TypeDef tim_num, TIM_ConfigTypeDef *config)
{
    if(!is_valid_tim_num(tim_num) || config == NULL)
    {
        return BSP_TIM_ERROR;
    }
    
    // 初始化句柄
    memset(&tim_handles[tim_num], 0, sizeof(BSP_TIM_HandleTypeDef));
    tim_handles[tim_num].htim.Instance = tim_instances[tim_num];
    tim_handles[tim_num].htim.Init.Prescaler = config->Prescaler;
    tim_handles[tim_num].htim.Init.CounterMode = config->CounterMode;
    tim_handles[tim_num].htim.Init.Period = config->Period;
    tim_handles[tim_num].htim.Init.ClockDivision = config->ClockDivision;
    tim_handles[tim_num].htim.Init.AutoReloadPreload = config->AutoReloadPreload;
    tim_handles[tim_num].mode = config->Mode;
    
    // 初始化定时器基
    if(HAL_TIM_Base_Init(&tim_handles[tim_num].htim) != HAL_OK)
    {
        return BSP_TIM_ERROR;
    }
    
    // 模式特定初始化
    switch(config->Mode)
    {
        case BSP_TIM_MODE_PWM:
        {
            uint32_t channel = get_hal_channel(config->ModeConfig.PWM_Config.Channel);
            tim_handles[tim_num].active_channel = channel;
            
            TIM_OC_InitTypeDef sConfigOC = {
                .OCMode = config->ModeConfig.PWM_Config.OCMode,
                .Pulse = config->ModeConfig.PWM_Config.Pulse,
                .OCPolarity = config->ModeConfig.PWM_Config.OCPolarity,
                .OCNPolarity = config->ModeConfig.PWM_Config.OCNPolarity,
                .OCFastMode = config->ModeConfig.PWM_Config.OCFastMode,
                .OCIdleState = config->ModeConfig.PWM_Config.OCIdleState,
                .OCNIdleState = config->ModeConfig.PWM_Config.OCNIdleState
            };
            
            if(HAL_TIM_PWM_ConfigChannel(&tim_handles[tim_num].htim, &sConfigOC, channel) != HAL_OK)
            {
                return BSP_TIM_ERROR;
            }
            break;
        }
            
        case BSP_TIM_MODE_INPUT_CAPTURE:
        {
            uint32_t channel = get_hal_channel(config->ModeConfig.IC_Config.Channel);
            tim_handles[tim_num].active_channel = channel;
            
            TIM_IC_InitTypeDef sConfigIC = {
                .ICPolarity = config->ModeConfig.IC_Config.ICPolarity,
                .ICSelection = config->ModeConfig.IC_Config.ICSelection,
                .ICPrescaler = config->ModeConfig.IC_Config.ICPrescaler,
                .ICFilter = config->ModeConfig.IC_Config.ICFilter
            };
            
            if(HAL_TIM_IC_ConfigChannel(&tim_handles[tim_num].htim, &sConfigIC, channel) != HAL_OK)
            {
                return BSP_TIM_ERROR;
            }
            break;
        }
            
        case BSP_TIM_MODE_OUTPUT_COMPARE:
        {
            uint32_t channel = get_hal_channel(config->ModeConfig.OC_Config.Channel);
            tim_handles[tim_num].active_channel = channel;
            
            TIM_OC_InitTypeDef sConfigOC = {
                .OCMode = config->ModeConfig.OC_Config.OCMode,
                .Pulse = config->ModeConfig.OC_Config.Pulse,
                .OCPolarity = config->ModeConfig.OC_Config.OCPolarity,
                .OCNPolarity = config->ModeConfig.OC_Config.OCNPolarity,
                .OCIdleState = config->ModeConfig.OC_Config.OCIdleState,
                .OCNIdleState = config->ModeConfig.OC_Config.OCNIdleState
            };
            
            if(HAL_TIM_OC_ConfigChannel(&tim_handles[tim_num].htim, &sConfigOC, channel) != HAL_OK)
            {
                return BSP_TIM_ERROR;
            }
            break;
        }
            
        case BSP_TIM_MODE_BASIC:
        default:
            break;
    }
    
    return BSP_TIM_OK;
}

BSP_TIM_Status_TypeDef BSP_TIM_RegisterCallback(TIM_Num_TypeDef tim_num, BSP_TIM_CallbackTypeDef callback)
{
    if(!is_valid_tim_num(tim_num))
    {
        return BSP_TIM_ERROR;
    }
    
    tim_handles[tim_num].callback = callback;
    return BSP_TIM_OK;
}

// 辅助函数：启动定时器
static HAL_StatusTypeDef start_timer(BSP_TIM_HandleTypeDef *handle, uint32_t mode, uint32_t channel)
{
    switch(handle->mode)
    {
        case BSP_TIM_MODE_BASIC:
            return (mode == BSP_TIM_MODE_BASIC_IT) ? 
                   HAL_TIM_Base_Start_IT(&handle->htim) : 
                   HAL_TIM_Base_Start(&handle->htim);
            
        case BSP_TIM_MODE_PWM:
            return (mode == BSP_TIM_MODE_PWM_IT) ? 
                   HAL_TIM_PWM_Start_IT(&handle->htim, channel) : 
                   HAL_TIM_PWM_Start(&handle->htim, channel);
            
        case BSP_TIM_MODE_INPUT_CAPTURE:
            return (mode == BSP_TIM_MODE_INPUT_CAPTURE_IT) ? 
                   HAL_TIM_IC_Start_IT(&handle->htim, channel) : 
                   HAL_TIM_IC_Start(&handle->htim, channel);
            
        case BSP_TIM_MODE_OUTPUT_COMPARE:
            return (mode == BSP_TIM_MODE_OUTPUT_COMPARE_IT) ? 
                   HAL_TIM_OC_Start_IT(&handle->htim, channel) : 
                   HAL_TIM_OC_Start(&handle->htim, channel);
            
        default:
            return HAL_ERROR;
    }
}

BSP_TIM_Status_TypeDef BSP_TIM_Start(TIM_Num_TypeDef tim_num, uint32_t mode)
{
    if(!is_valid_tim_num(tim_num))
    {
        return BSP_TIM_ERROR;
    }
    
    HAL_StatusTypeDef hal_status = start_timer(
        &tim_handles[tim_num], 
        mode, 
        tim_handles[tim_num].active_channel
    );
    
    return (hal_status == HAL_OK) ? BSP_TIM_OK : BSP_TIM_ERROR;
}

// 辅助函数：停止定时器
static HAL_StatusTypeDef stop_timer(BSP_TIM_HandleTypeDef *handle, uint32_t channel)
{
    switch(handle->mode)
    {
        case BSP_TIM_MODE_BASIC:      return HAL_TIM_Base_Stop(&handle->htim);
        case BSP_TIM_MODE_PWM:        return HAL_TIM_PWM_Stop(&handle->htim, channel);
        case BSP_TIM_MODE_INPUT_CAPTURE: return HAL_TIM_IC_Stop(&handle->htim, channel);
        case BSP_TIM_MODE_OUTPUT_COMPARE: return HAL_TIM_OC_Stop(&handle->htim, channel);
        default:                      return HAL_ERROR;
    }
}

BSP_TIM_Status_TypeDef BSP_TIM_Stop(TIM_Num_TypeDef tim_num)
{
    if(!is_valid_tim_num(tim_num))
    {
        return BSP_TIM_ERROR;
    }
    
    HAL_StatusTypeDef hal_status = stop_timer(
        &tim_handles[tim_num], 
        tim_handles[tim_num].active_channel
    );
    
    return (hal_status == HAL_OK) ? BSP_TIM_OK : BSP_TIM_ERROR;
}

uint32_t BSP_TIM_GetCounter(TIM_Num_TypeDef tim_num)
{
    return is_valid_tim_num(tim_num) ? 
           __HAL_TIM_GET_COUNTER(&tim_handles[tim_num].htim) : 
           0;
}

// 辅助函数：处理中断回调
static void handle_interrupt_callback(BSP_TIM_HandleTypeDef *handle)
{
    if(handle->callback != NULL)
    {
        handle->callback();
    }
}

void BSP_TIM_IRQHandler(TIM_Num_TypeDef tim_num)
{
    if(!is_valid_tim_num(tim_num))
    {
        return;
    }
    
    BSP_TIM_HandleTypeDef *handle = &tim_handles[tim_num];
    HAL_TIM_IRQHandler(&handle->htim);
    
    uint32_t it_flag = TIM_FLAG_UPDATE;
    uint32_t it_source = TIM_IT_UPDATE;
    
    // 根据模式确定要检查的标志
    switch(handle->mode)
    {
        case BSP_TIM_MODE_PWM:
        case BSP_TIM_MODE_PWM_IT:
        case BSP_TIM_MODE_INPUT_CAPTURE:
        case BSP_TIM_MODE_INPUT_CAPTURE_IT:
        case BSP_TIM_MODE_OUTPUT_COMPARE:
        case BSP_TIM_MODE_OUTPUT_COMPARE_IT:
            it_flag = TIM_FLAG_CC1 << (handle->active_channel >> 2);
            it_source = TIM_IT_CC1 << (handle->active_channel >> 2);
            break;
            
        case BSP_TIM_MODE_BASIC:
        case BSP_TIM_MODE_BASIC_IT:
        default:
            break;
    }
    
    if(__HAL_TIM_GET_FLAG(&handle->htim, it_flag) && 
       __HAL_TIM_GET_IT_SOURCE(&handle->htim, it_source))
    {
        __HAL_TIM_CLEAR_IT(&handle->htim, it_source);
        handle_interrupt_callback(handle);
    }
}
