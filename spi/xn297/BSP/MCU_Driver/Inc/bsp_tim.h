#ifndef __BSP_TIM_H
#define __BSP_TIM_H

#include "bsp_common.h"

typedef enum {
    BSP_TIM1,
    BSP_TIM2,
    BSP_TIM3,
    BSP_TIM4,
    BSP_TIM5,
    BSP_TIM6,
    BSP_TIM7,
    BSP_TIM8,
    BSP_TIM9,
    BSP_TIM10,
    BSP_TIM11,
    BSP_TIM12,
    BSP_TIM13,
    BSP_TIM14,
    BSP_TIM_NUM
} TIM_Num_TypeDef;

typedef enum {
    BSP_TIM_OK       = 0x00,
    BSP_TIM_ERROR    = 0x01,
    BSP_TIM_BUSY     = 0x02,
    BSP_TIM_TIMEOUT  = 0x03
} BSP_TIM_Status_TypeDef;

// Add these to the BSP_TIM_ModeTypeDef enum
typedef enum {
    BSP_TIM_MODE_BASIC,
    BSP_TIM_MODE_BASIC_IT,          // Basic timer with interrupt
    BSP_TIM_MODE_PWM,
    BSP_TIM_MODE_PWM_IT,            // PWM with interrupt
    BSP_TIM_MODE_INPUT_CAPTURE,
    BSP_TIM_MODE_INPUT_CAPTURE_IT,   // Input capture with interrupt
    BSP_TIM_MODE_OUTPUT_COMPARE,
    BSP_TIM_MODE_OUTPUT_COMPARE_IT   // Output compare with interrupt
} BSP_TIM_ModeTypeDef;

// PWM-specific configuration
typedef struct {
    uint32_t Channel;
    uint32_t Pulse;
    uint32_t OCMode;
    uint32_t OCPolarity;
    uint32_t OCNPolarity;
    uint32_t OCFastMode;
    uint32_t OCIdleState;
    uint32_t OCNIdleState;
} TIM_PWM_ConfigTypeDef;

// Input Capture-specific configuration
typedef struct {
    uint32_t Channel;
    uint32_t ICPolarity;
    uint32_t ICSelection;
    uint32_t ICPrescaler;
    uint32_t ICFilter;
} TIM_IC_ConfigTypeDef;

// Output Compare-specific configuration
typedef struct {
    uint32_t Channel;
    uint32_t OCMode;
    uint32_t Pulse;
    uint32_t OCPolarity;
    uint32_t OCNPolarity;
    uint32_t OCIdleState;
    uint32_t OCNIdleState;
} TIM_OC_ConfigTypeDef;

typedef struct {
    uint32_t Prescaler;
    uint32_t CounterMode;
    uint32_t Period;
    uint32_t ClockDivision;
    uint32_t AutoReloadPreload;
    BSP_TIM_ModeTypeDef Mode;
    union {
        TIM_PWM_ConfigTypeDef PWM_Config;
        TIM_IC_ConfigTypeDef IC_Config;
        TIM_OC_ConfigTypeDef OC_Config;
    } ModeConfig;
} TIM_ConfigTypeDef;

typedef void (*BSP_TIM_CallbackTypeDef)(void);

typedef struct {
    TIM_HandleTypeDef htim;
    BSP_TIM_CallbackTypeDef callback;
    BSP_TIM_ModeTypeDef mode;
    uint32_t active_channel;
} BSP_TIM_HandleTypeDef;

BSP_TIM_Status_TypeDef BSP_TIM_Init(TIM_Num_TypeDef tim_num, TIM_ConfigTypeDef *config);
BSP_TIM_Status_TypeDef BSP_TIM_RegisterCallback(TIM_Num_TypeDef tim_num, BSP_TIM_CallbackTypeDef callback);
BSP_TIM_Status_TypeDef BSP_TIM_Start(TIM_Num_TypeDef tim_num, uint32_t mode);
BSP_TIM_Status_TypeDef BSP_TIM_Stop(TIM_Num_TypeDef tim_num);
uint32_t BSP_TIM_GetCounter(TIM_Num_TypeDef tim_num);
void BSP_TIM_IRQHandler(TIM_Num_TypeDef tim_num);

#endif /* __BSP_TIM_H */