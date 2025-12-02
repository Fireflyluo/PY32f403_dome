#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* 使用RT-Thread风格的宏定义 */
#define __N32_PORT(port) GPIO##port##_BASE
#define GET_PIN(PORTx, PIN) (uint16_t)((16 * (((uint16_t)__N32_PORT(PORTx) - (uint16_t)GPIOA_BASE) / (0x0400UL))) + PIN)

  /* 基础类型定义 */
  typedef int32_t gpio_err_t;
  typedef uint16_t gpio_pin_t; /* 使用uint16_t兼容GET_PIN宏 */

/* 错误码定义 */
#define GPIO_OK 0
#define GPIO_ERROR -1
#define GPIO_EINVAL -2
#define GPIO_ERANGE -3

  /* 引脚模式定义 */
  typedef enum
  {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_INPUT_PULLUP,
    GPIO_MODE_INPUT_PULLDOWN,
    GPIO_MODE_OUTPUT_OD,
  } gpio_mode_t;

  /* 中断模式定义 */
  typedef enum
  {
    GPIO_IRQ_MODE_RISING = 0,
    GPIO_IRQ_MODE_FALLING,
    GPIO_IRQ_MODE_RISING_FALLING,
  } gpio_irq_mode_t;

  /* 中断使能定义 */
  typedef enum
  {
    GPIO_IRQ_DISABLE = 0,
    GPIO_IRQ_ENABLE = 1
  } gpio_irq_enable_t;

  /* 引脚电平定义 */
  typedef enum
  {
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET = 1
  } gpio_pin_state_t;

  /* 引脚索引结构 - 适配新的宏定义 */
  struct gpio_pin_index
  {
    gpio_pin_t logical_pin; /* 逻辑引脚号（由GET_PIN宏生成） */
    GPIO_TypeDef *gpio;     /* GPIO端口指针 */
    uint16_t pin_bit;       /* 引脚位掩码 */
    const char *name;       /* 引脚名称（可选） */
  };

  /* API函数声明 */
  gpio_err_t gpio_init(void);
  gpio_err_t gpio_mode(gpio_pin_t pin, gpio_mode_t mode);
  void gpio_write(gpio_pin_t pin, gpio_pin_state_t value);
  gpio_pin_state_t gpio_read(gpio_pin_t pin);
  gpio_err_t gpio_toggle(gpio_pin_t pin);
  gpio_err_t gpio_attach_irq(gpio_pin_t pin, gpio_irq_mode_t mode,
                             void (*hdr)(void *args), void *args);
  gpio_err_t gpio_detach_irq(gpio_pin_t pin);
  gpio_err_t gpio_irq_enable(gpio_pin_t pin, gpio_irq_enable_t enabled);

  /* 工具函数 */
  const char *gpio_get_pin_name(gpio_pin_t pin);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_GPIO_H__ */