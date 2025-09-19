/*-----------------------------------------------File Info------------------------------------------------
** File Name:               bsp_exti.c  
** Last modified date:      2021.4.9
** Last version:            V1.0.0
** Description:             外部中断管理与回调注册
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2021.4.9
** author:                  Fireflyluo
** Version:                 V1.0.0
** Descriptions:            初始化GPIO为外部中断输入；
**                          支持多个GPIO中断源的注册与回调处理；
**                          提供中断使能、清除标志位等底层控制函数。
**--------------------------------------------------------------------------------------------------------*/


#include "bsp_exti.h"


/* Private Includes ----------------------------------------------------------*/

/* Private Define ------------------------------------------------------------*/
/* Private Variables ---------------------------------------------------------*/
 bool bsp_exti_init = false;

// 在bsp_exti.h中新增定义

typedef struct {
    GPIO_TypeDef *gpiox;
    uint16_t pin;
    bsp_gpio_exti_callback callback;
} EXTI_Callback_t;

#define MAX_CALLBACKS 16  // 根据实际需求调整
static EXTI_Callback_t s_callbacks[MAX_CALLBACKS];
 /* Public Function Prototypes ------------------------------------------------*/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        if (s_callbacks[i].callback && 
            s_callbacks[i].gpiox && 
            (s_callbacks[i].pin == GPIO_Pin)) {
            
            s_callbacks[i].callback(s_callbacks[i].gpiox, s_callbacks[i].pin);
            break;
        }
    }
}

/**
 * @brief  设置外部中断NVIC并配置GPIO为中断模式
 * @note   该函数会配置指定的GPIO为下降沿触发中断，并启用NVIC中断
 * @param  irqn: 中断号（如 EXTI4_15_IRQn）
 * @param  priority: 中断优先级
 * @param  gpiox: GPIO组（如 GPIOA）
 * @param  pin: 引脚号（如 GPIO_PIN_12）
 * @retval None
 */
void bsp_exit_set(uint8_t irqn, uint32_t priority,uint32_t SubPriority, GPIO_TypeDef *gpiox, uint16_t pin)
{
	 bsp_exti_init = true;
	
    HAL_NVIC_EnableIRQ((IRQn_Type)irqn);
    HAL_NVIC_SetPriority((IRQn_Type)irqn, priority,SubPriority);


}

/**
 * @brief  清除外部中断设置
 * @note   NULL
 * @param  irqn: 中断号（在typedef enum IRQn中，例如：USART1_IRQn）
 * @retval None
 */
void bsp_exit_clear_set(uint8_t irqn)
{
    HAL_NVIC_DisableIRQ((IRQn_Type)(irqn));

}

/**
 * @brief  清除外部中断标志位
 * @note   NULL
 * @param  *gpiox: 预留
 * @param  pin: 引脚号
 * @retval None
 */
void bsp_exit_clear_flag( uint16_t gpio_pin_x)
{
    __HAL_GPIO_EXTI_CLEAR_FLAG(gpio_pin_x);
}

/**
 * @brief 注册GPIO中断回调函数
 * 
 * 该函数用于在系统中注册一个GPIO中断回调函数。当中断发生时，回调函数将被调用。
 * 
 * @param gpiox GPIO端口地址，用于指定需要配置的GPIO端口
 * @param pin GPIO引脚号，用于指定端口内的具体引脚
 * @param callback 用户定义的回调函数指针，当中断触发时将调用此函数
 * 
 * @return true 如果回调函数成功注册，则返回true
 * @return false 如果注册失败（例如，回调函数数组已满），则返回false
 */
bool bsp_gpio_exit_irq_register_callback(GPIO_TypeDef *gpiox, uint16_t pin, bsp_gpio_exti_callback callback)
{
    //确保不为NULL
    if (!gpiox || !callback) {
        return false;
    }

    // 遍历回调函数数组，寻找空闲的数组元素
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        // 当找到未注册的数组元素时，注册回调函数
        if (!s_callbacks[i].callback) {
            s_callbacks[i].gpiox = gpiox;
            s_callbacks[i].pin = pin;
            s_callbacks[i].callback = callback;
            return true;
        }
    }
    
    // 如果所有数组元素都已注册，返回注册失败
    return false;
}

