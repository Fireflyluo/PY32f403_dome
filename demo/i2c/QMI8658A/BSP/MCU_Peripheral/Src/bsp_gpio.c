/*-----------------------------------------------File Info------------------------------------------------
** File Name:               bsp_gpio.c  
** Last modified date:      2025.6.27
** Last version:            V1.0.0
** Description:             GPIO引脚控制与状态管理
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.6.27
** author:                  fireflyluo
** Version:                 V1.0.0
** Descriptions:            提供GPIO引脚的输出、输入、中断等初始化函数；
**                          支持设置高低电平、翻转电平、读取当前电平状态；
**                          封装常用外设引脚配置接口（如ADC、I2C、UART、TIM）。
**--------------------------------------------------------------------------------------------------------*/
#include "bsp_gpio.h"


/**
 * @brief 获取外部中断类型
 * 
 * 根据传入的外部中断类型枚举值，返回对应的GPIO中断模式配置值。
 * 
 * @param exti_type 外部中断类型枚举，表示所需的中断触发条件
 * @return uint32_t 返回对应的GPIO中断模式配置值，如果类型不匹配则返回0
 */
static uint32_t get_exti_type(bsp_gpio_exti_int_type_t exti_type)
{
    switch (exti_type)
    {
        case BSP_GPIO_EXTI_RISING:   // 下降沿 / 低电平
            return GPIO_MODE_IT_RISING;

        case BSP_GPIO_EXTI_IT_FALLING:  // 上升沿 / 高电平
            return GPIO_MODE_IT_FALLING;

        case BSP_GPIO_EXTI_IT_RISING_FALLING:  // 双边沿 / 任意变化
            return GPIO_MODE_IT_RISING_FALLING;

        case BSP_GPIO_EXTI_EVT_FALLING:   // 下降沿 / 低电平
            return GPIO_MODE_EVT_FALLING;

        case BSP_GPIO_EXTI_EVT_RISING:  // 上升沿 / 高电平
            return GPIO_MODE_EVT_RISING;

        case BSP_GPIO_EXTI_EVT_RISING_FALLING:  // 双边沿 / 任意变化
            return GPIO_MODE_EVT_RISING_FALLING;

        default:
            return 0;
    }
}


/* 初始化函数 --------------------------------------------------------------*/

/**
 * @brief  [初始化] 引脚设置为输出模式 BSP_GPIO_Init_Output
 * @note   NULL
 * @param  *gpiox: gpio组号（GPIOA/GPIOB/GPIOC/GPIOD等等）
 * @param  pin: 引脚号 GPIO_PIN_x
 * @param  out_mode:  BSP_GPIO_PIN_OUT_OD 开漏输出, BSP_GPIO_PIN_OUT_PP 推免输出, BSP_GPIO_PIN_AF_OD 复用开漏, BSP_GPIO_PIN_AF_PP 复用推免
 * @retval None
 */
BSP_GPIO_Status_TypeDef BSP_GPIO_Init_Output(void *gpiox, uint16_t pin, bsp_gpio_pin_out_t out_mode)
{
    if (gpiox == NULL) {
        return BSP_GPIO_ERR_NULL_PTR;
    }

    GPIO_InitTypeDef gpio_init_struct = {0};

    gpio_init_struct.Pin       = pin;
    gpio_init_struct.Pull      = GPIO_NOPULL;
    gpio_init_struct.Speed     = GPIO_SPEED_FREQ_HIGH;

    switch (out_mode)
    {
        case BSP_GPIO_PIN_OUT_OD:       //开漏输出
            gpio_init_struct.Mode         = GPIO_MODE_OUTPUT_PP;
            break;

        case BSP_GPIO_PIN_OUT_PP:       //推挽输出
            gpio_init_struct.Mode         = GPIO_MODE_OUTPUT_PP;
            break;

        case BSP_GPIO_PIN_AF_OD:        //复用开漏
            gpio_init_struct.Mode         = GPIO_MODE_AF_PP;
            break;

        case BSP_GPIO_PIN_AF_PP:        //复用推挽
            gpio_init_struct.Mode         = GPIO_MODE_AF_PP;
            break;

        default:
            // 错误处理或默认配置
            return BSP_GPIO_ERR_INVALID_NUM;
    }

    HAL_GPIO_Init(gpiox, &gpio_init_struct);

    return BSP_GPIO_OK;
}

/**
 * @brief  [初始化] 引脚设置为输入模式 BSP_GPIO_Init_Input
 * @note   NULL
 * @param  *gpiox: gpio组号（GPIOA/GPIOB/GPIOC/GPIOD等等）
 * @param  pin: 引脚号 GPIO_PIN_x
 * @param  pull: BSP_GPIO_PIN_NOPULL 无上下拉, BSP_GPIO_PIN_PULLUP 上拉输入, BSP_GPIO_PIN_PULLDOWN 下拉输入
 * @retval None
 */
BSP_GPIO_Status_TypeDef BSP_GPIO_Init_Input(void *gpiox, uint16_t pin, bsp_gpio_pin_pull_t pull)
{
    if (gpiox == NULL) {
        return BSP_GPIO_ERR_NULL_PTR;
    }

    GPIO_InitTypeDef gpio_init_struct= {0};

    gpio_init_struct.Pin              = pin;
    gpio_init_struct.Pull             = pull;
    gpio_init_struct.Mode             = GPIO_MODE_INPUT;

   HAL_GPIO_Init(gpiox, (GPIO_InitTypeDef *)&gpio_init_struct);
 

    return BSP_GPIO_OK;
}

/**
 * @brief 初始化GPIO为输入模式并配置外部中断   BSP_GPIO_Init_Exti
 * @param gpiox GPIO端口地址 GPIOx
 * @param pin GPIO引脚号  GPIO_PIN_x
 * @param irqn 中断向量号  EXTI_xxx_IRQn
 * @param exti_type 外部中断触发类型 
 * @param pull 引脚上下拉配置 
 */
BSP_GPIO_Status_TypeDef BSP_GPIO_Init_Exti(void *gpiox, uint16_t pin, uint8_t irqn,
                              bsp_gpio_exti_int_type_t exti_type,
                              bsp_gpio_pin_pull_t pull)
{
    if (gpiox == NULL) {
        return BSP_GPIO_ERR_NULL_PTR;
    }

    // 初始化GPIO配置结构体
    GPIO_InitTypeDef gpio_init_struct = {0};

    // 设置引脚号和上下拉
    gpio_init_struct.Pin   = pin;
    gpio_init_struct.Pull  = pull;

    // 设置中断模式（上升/下降/双边沿）
    gpio_init_struct.Mode = get_exti_type(exti_type);

    if (gpio_init_struct.Mode == 0) {
        return BSP_GPIO_ERR_INVALID_NUM;
    }

    // 初始化GPIO
    HAL_GPIO_Init(gpiox, &gpio_init_struct); 

    // 清除中断标志（必须在初始化后调用）
    bsp_exit_clear_flag(pin);
    // 设置中断优先级并使能中断
    bsp_exit_set((IRQn_Type)irqn,0,0,gpiox,pin);

    return BSP_GPIO_OK;
}

/**
 * @brief  [反初始化] 关闭指定引脚功能(恢复为浮空输入)
 * @note   NULL
 * @param  *gpiox: gpio组号（GPIOA/GPIOB/GPIOC/GPIOD等等）
 * @param  pin: 引脚号
 * @retval None
 */
void BSP_GPIO_DeInit(void *gpiox, uint16_t pin)
{
    HAL_GPIO_DeInit(gpiox, pin);
}

/*  引脚控制和状态获取 ----------------------------------------------------------------------*/

/**
 * @brief  设置引脚电平状态
 * @note   NULL
 * @param  *gpiox: gpio组号（GPIOA/GPIOB/GPIOC/GPIOD等等）
 * @param  pin: 引脚号
 * @param  state: BSP_GPIO_PIN_RESET 低电平, BSP_GPIO_PIN_SET 高电平
 * @retval None
 */
void BSP_GPIO_SetPin(void *gpiox, uint16_t pin, bsp_gpio_pin_state_t state)
{
    HAL_GPIO_WritePin(gpiox, pin, (GPIO_PinState)state);
}

/**
 * @brief  翻转引脚电平状态
 * @note   NULL
 * @param  *gpiox: gpio组号（GPIOA/GPIOB/GPIOC/GPIOD等等）
 * @param  pin: 引脚号
 * @retval None
 */
void BSP_GPIO_SetToggle(void *gpiox, uint16_t pin)
{
    HAL_GPIO_TogglePin(gpiox, pin);
}

/**
 * @brief  得到指定gpio状态
 * @note   NULL
 * @param  *gpiox: gpio组号（GPIOA/GPIOB/GPIOC/GPIOD等等）
 * @param  pin: 引脚号
 * @retval 0 -- 低电平, 1 -- 高电平
 */
bool BSP_GPIO_GetState(void *gpiox, uint16_t pin)
{
    return (bool)HAL_GPIO_ReadPin(gpiox, pin);
}


