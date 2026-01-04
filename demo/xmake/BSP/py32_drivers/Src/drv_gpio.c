#include "drv_gpio.h"
//#include "board.h"

/* ========== 使用新的宏定义生成引脚映射表 ========== */

static const struct gpio_pin_index gpio_pins[] = {
/* 使用GET_PIN宏生成引脚映射，确保与RT-Thread兼容 */
#if defined(GPIOA)
    {GET_PIN(A, 0), GPIOA, GPIO_PIN_0, "PA0"},
    {GET_PIN(A, 1), GPIOA, GPIO_PIN_1, "PA1"},
    {GET_PIN(A, 2), GPIOA, GPIO_PIN_2, "PA2"},
    {GET_PIN(A, 3), GPIOA, GPIO_PIN_3, "PA3"},
    {GET_PIN(A, 4), GPIOA, GPIO_PIN_4, "PA4"},
    {GET_PIN(A, 5), GPIOA, GPIO_PIN_5, "PA5"},
    {GET_PIN(A, 6), GPIOA, GPIO_PIN_6, "PA6"},
    {GET_PIN(A, 7), GPIOA, GPIO_PIN_7, "PA7"},
    {GET_PIN(A, 8), GPIOA, GPIO_PIN_8, "PA8"},
    {GET_PIN(A, 9), GPIOA, GPIO_PIN_9, "PA9"},
    {GET_PIN(A, 10), GPIOA, GPIO_PIN_10, "PA10"},
    {GET_PIN(A, 11), GPIOA, GPIO_PIN_11, "PA11"},
    {GET_PIN(A, 12), GPIOA, GPIO_PIN_12, "PA12"},
    {GET_PIN(A, 13), GPIOA, GPIO_PIN_13, "PA13"},
    {GET_PIN(A, 14), GPIOA, GPIO_PIN_14, "PA14"},
    {GET_PIN(A, 15), GPIOA, GPIO_PIN_15, "PA15"},
#endif
#if defined(GPIOB)
    {GET_PIN(B, 0), GPIOB, GPIO_PIN_0, "PB0"},
    {GET_PIN(B, 1), GPIOB, GPIO_PIN_1, "PB1"},
    {GET_PIN(B, 2), GPIOB, GPIO_PIN_2, "PB2"},
    {GET_PIN(B, 3), GPIOB, GPIO_PIN_3, "PB3"},
    {GET_PIN(B, 4), GPIOB, GPIO_PIN_4, "PB4"},
    {GET_PIN(B, 5), GPIOB, GPIO_PIN_5, "PB5"},
    {GET_PIN(B, 6), GPIOB, GPIO_PIN_6, "PB6"},
    {GET_PIN(B, 7), GPIOB, GPIO_PIN_7, "PB7"},
    {GET_PIN(B, 8), GPIOB, GPIO_PIN_8, "PB8"},
    {GET_PIN(B, 9), GPIOB, GPIO_PIN_9, "PB9"},
    {GET_PIN(B, 10), GPIOB, GPIO_PIN_10, "PB10"},
    {GET_PIN(B, 11), GPIOB, GPIO_PIN_11, "PB11"},
    {GET_PIN(B, 12), GPIOB, GPIO_PIN_12, "PB12"},
    {GET_PIN(B, 13), GPIOB, GPIO_PIN_13, "PB13"},
    {GET_PIN(B, 14), GPIOB, GPIO_PIN_14, "PB14"},
    {GET_PIN(B, 15), GPIOB, GPIO_PIN_15, "PB15"},
#endif
#if defined(GPIOC)
    {GET_PIN(C, 0), GPIOC, GPIO_PIN_0, "PC0"},
    {GET_PIN(C, 1), GPIOC, GPIO_PIN_1, "PC1"},
    {GET_PIN(C, 2), GPIOC, GPIO_PIN_2, "PC2"},
    {GET_PIN(C, 3), GPIOC, GPIO_PIN_3, "PC3"},
    {GET_PIN(C, 4), GPIOC, GPIO_PIN_4, "PC4"},
    {GET_PIN(C, 5), GPIOC, GPIO_PIN_5, "PC5"},
    {GET_PIN(C, 6), GPIOC, GPIO_PIN_6, "PC6"},
    {GET_PIN(C, 7), GPIOC, GPIO_PIN_7, "PC7"},
    {GET_PIN(C, 8), GPIOC, GPIO_PIN_8, "PC8"},
    {GET_PIN(C, 9), GPIOC, GPIO_PIN_9, "PC9"},
    {GET_PIN(C, 10), GPIOC, GPIO_PIN_10, "PC10"},
    {GET_PIN(C, 11), GPIOC, GPIO_PIN_11, "PC11"},
    {GET_PIN(C, 12), GPIOC, GPIO_PIN_12, "PC12"},
    {GET_PIN(C, 13), GPIOC, GPIO_PIN_13, "PC13"},
    {GET_PIN(C, 14), GPIOC, GPIO_PIN_14, "PC14"},
    {GET_PIN(C, 15), GPIOC, GPIO_PIN_15, "PC15"},
#endif
#if defined(GPIOD)
    {GET_PIN(D, 0), GPIOD, GPIO_PIN_0, "PD0"},
    {GET_PIN(D, 1), GPIOD, GPIO_PIN_1, "PD1"},
    {GET_PIN(D, 2), GPIOD, GPIO_PIN_2, "PD2"},
    {GET_PIN(D, 3), GPIOD, GPIO_PIN_3, "PD3"},
    {GET_PIN(D, 4), GPIOD, GPIO_PIN_4, "PD4"},
    {GET_PIN(D, 5), GPIOD, GPIO_PIN_5, "PD5"},
    {GET_PIN(D, 6), GPIOD, GPIO_PIN_6, "PD6"},
    {GET_PIN(D, 7), GPIOD, GPIO_PIN_7, "PD7"},
    {GET_PIN(D, 8), GPIOD, GPIO_PIN_8, "PD8"},
    {GET_PIN(D, 9), GPIOD, GPIO_PIN_9, "PD9"},
    {GET_PIN(D, 10), GPIOD, GPIO_PIN_10, "PD10"},
    {GET_PIN(D, 11), GPIOD, GPIO_PIN_11, "PD11"},
    {GET_PIN(D, 12), GPIOD, GPIO_PIN_12, "PD12"},
    {GET_PIN(D, 13), GPIOD, GPIO_PIN_13, "PD13"},
    {GET_PIN(D, 14), GPIOD, GPIO_PIN_14, "PD14"},
    {GET_PIN(D, 15), GPIOD, GPIO_PIN_15, "PD15"},
#endif
#if defined(GPIOE)
    {GET_PIN(E, 0), GPIOE, GPIO_PIN_0, "PE0"},
    {GET_PIN(E, 1), GPIOE, GPIO_PIN_1, "PE1"},
    {GET_PIN(E, 2), GPIOE, GPIO_PIN_2, "PE2"},
    {GET_PIN(E, 3), GPIOE, GPIO_PIN_3, "PE3"},
    {GET_PIN(E, 4), GPIOE, GPIO_PIN_4, "PE4"},
    {GET_PIN(E, 5), GPIOE, GPIO_PIN_5, "PE5"},
    {GET_PIN(E, 6), GPIOE, GPIO_PIN_6, "PE6"},
    {GET_PIN(E, 7), GPIOE, GPIO_PIN_7, "PE7"},
    {GET_PIN(E, 8), GPIOE, GPIO_PIN_8, "PE8"},
    {GET_PIN(E, 9), GPIOE, GPIO_PIN_9, "PE9"},
    {GET_PIN(E, 10), GPIOE, GPIO_PIN_10, "PE10"},
    {GET_PIN(E, 11), GPIOE, GPIO_PIN_11, "PE11"},
    {GET_PIN(E, 12), GPIOE, GPIO_PIN_12, "PE12"},
    {GET_PIN(E, 13), GPIOE, GPIO_PIN_13, "PE13"},
    {GET_PIN(E, 14), GPIOE, GPIO_PIN_14, "PE14"},
    {GET_PIN(E, 15), GPIOE, GPIO_PIN_15, "PE15"},
#endif
    /* 可以继续添加其他端口... */
};

/* 计算引脚映射表大小 */
#define GPIO_PIN_COUNT (sizeof(gpio_pins) / sizeof(gpio_pins[0]))

/* ========== 工具函数 ========== */

/* 根据引脚号查找引脚信息 - 优化版本 */
static const struct gpio_pin_index *gpio_get_pin(gpio_pin_t pin)
{
  /* 由于GET_PIN宏生成的引脚号是连续的，我们可以直接使用索引查找 */
  if (pin < GPIO_PIN_COUNT)
  {
    const struct gpio_pin_index *index = &gpio_pins[pin];
    /* 检查引脚是否有效 */
    if (index->gpio != NULL)
    {
      return index;
    }
  }
  return NULL;
}
/* 获取引脚名称函数 */
const char *gpio_get_pin_name(gpio_pin_t pin)
{
  const struct gpio_pin_index *index;

  index = gpio_get_pin(pin);
  if (index == NULL)
  {
    return "UNKNOWN";
  }

  return index->name;
}
/* 位值转换为位索引 - 修复返回类型 */
static uint8_t gpio_bit_to_index(uint16_t bit)
{
  for (uint8_t i = 0; i < 16; i++)
  {
    if ((1U << i) == bit)
    {
      return i;
    }
  }
  return 0xFF; /* 无效索引 */
}

/* ========== GPIO基础API实现 ========== */

/* GPIO驱动初始化 */
gpio_err_t gpio_init(void)
{
  /* 初始化时钟等硬件资源 */
  /* 这里可以初始化所有GPIO端口的时钟 */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  return GPIO_OK;
}

/* 设置GPIO模式 */
gpio_err_t gpio_mode(gpio_pin_t pin, pin_mode_t mode)
{
  const struct gpio_pin_index *index;
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  index = gpio_get_pin(pin);
  if (index == NULL)
  {
    return GPIO_EINVAL;
  }

  /* 配置GPIO引脚 */
  GPIO_InitStruct.Pin = index->pin_bit;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  switch (mode)
  {
  case PIN_MODE_OUTPUT:
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    break;

  case PIN_MODE_INPUT:
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    break;

  case PIN_MODE_INPUT_PULLUP:
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    break;

  case PIN_MODE_INPUT_PULLDOWN:
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    break;

  case PIN_MODE_OUTPUT_OD:
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    break;

  default:
    return GPIO_EINVAL;
  }

  HAL_GPIO_Init(index->gpio, &GPIO_InitStruct);
  return GPIO_OK;
}

/* 写GPIO引脚 */
void gpio_write(gpio_pin_t pin, GPIO_PinState value)
{
  const struct gpio_pin_index *index;

  index = gpio_get_pin(pin);
  if (index == NULL)
  {
    return;
  }

  HAL_GPIO_WritePin(index->gpio, index->pin_bit,
                    (value == GPIO_PIN_SET) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* 读GPIO引脚 */
GPIO_PinState gpio_read(gpio_pin_t pin)
{
  const struct gpio_pin_index *index;

  index = gpio_get_pin(pin);
  if (index == NULL)
  {
    return GPIO_PIN_RESET;
  }

  GPIO_PinState state = HAL_GPIO_ReadPin(index->gpio, index->pin_bit);
  return (state == GPIO_PIN_SET) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}

/* 翻转GPIO引脚 */
gpio_err_t gpio_toggle(gpio_pin_t pin)
{
  const struct gpio_pin_index *index;

  index = gpio_get_pin(pin);
  if (index == NULL)
  {
    return GPIO_EINVAL;
  }

  HAL_GPIO_TogglePin(index->gpio, index->pin_bit);
  return GPIO_OK;
}

/* ========== 中断处理API ========== */

/* 中断映射表 */
static const struct
{
  uint16_t pin_bit;
  IRQn_Type irqno;
} gpio_irq_map[] = {
    {GPIO_PIN_0, EXTI0_IRQn},
    {GPIO_PIN_1, EXTI1_IRQn},
    {GPIO_PIN_2, EXTI2_IRQn},
    {GPIO_PIN_3, EXTI3_IRQn},
    {GPIO_PIN_4, EXTI4_IRQn},
    {GPIO_PIN_5, EXTI9_5_IRQn},
    {GPIO_PIN_6, EXTI9_5_IRQn},
    {GPIO_PIN_7, EXTI9_5_IRQn},
    {GPIO_PIN_8, EXTI9_5_IRQn},
    {GPIO_PIN_9, EXTI9_5_IRQn},
    {GPIO_PIN_10, EXTI15_10_IRQn},
    {GPIO_PIN_11, EXTI15_10_IRQn},
    {GPIO_PIN_12, EXTI15_10_IRQn},
    {GPIO_PIN_13, EXTI15_10_IRQn},
    {GPIO_PIN_14, EXTI15_10_IRQn},
    {GPIO_PIN_15, EXTI15_10_IRQn},
};

/* 中断处理程序表 */
static struct
{
  gpio_pin_t pin;
  uint8_t mode;
  void (*hdr)(void *args);
  void *args;
  uint8_t enabled;
} gpio_irq_handlers[16];

static uint32_t gpio_irq_enable_mask = 0;

/* 绑定中断处理函数 */
gpio_err_t gpio_attach_irq(gpio_pin_t pin, pin_irq_mode_t mode,
                           void (*hdr)(void *args), void *args)
{
  const struct gpio_pin_index *index;
  uint8_t irq_index;

  if (hdr == NULL)
  {
    return GPIO_EINVAL;
  }

  index = gpio_get_pin(pin);
  if (index == NULL)
  {
    return GPIO_EINVAL;
  }

  irq_index = gpio_bit_to_index(index->pin_bit);
  if (irq_index >= 16)
  {
    return GPIO_EINVAL;
  }

  /* 临界区保护 */
  __disable_irq();

  /* 注册中断处理程序 */
  gpio_irq_handlers[irq_index].pin = pin;
  gpio_irq_handlers[irq_index].mode = (uint8_t)mode;
  gpio_irq_handlers[irq_index].hdr = hdr;
  gpio_irq_handlers[irq_index].args = args;
  gpio_irq_handlers[irq_index].enabled = 0;

  __enable_irq();

  return GPIO_OK;
}

/* 解绑中断处理函数 */
gpio_err_t gpio_detach_irq(gpio_pin_t pin)
{
  const struct gpio_pin_index *index;
  uint8_t irq_index;

  index = gpio_get_pin(pin);
  if (index == NULL)
  {
    return GPIO_EINVAL;
  }

  irq_index = gpio_bit_to_index(index->pin_bit);
  if (irq_index >= 16)
  {
    return GPIO_EINVAL;
  }

  /* 临界区保护 */
  __disable_irq();

  /* 清除中断处理程序 */
  gpio_irq_handlers[irq_index].pin = 0xFFFF;
  gpio_irq_handlers[irq_index].hdr = NULL;
  gpio_irq_handlers[irq_index].mode = 0;
  gpio_irq_handlers[irq_index].args = NULL;
  gpio_irq_handlers[irq_index].enabled = 0;

  __enable_irq();

  return GPIO_OK;
}

/* 使能/禁用中断 */
gpio_err_t gpio_irq_enable(gpio_pin_t pin, gpio_irq_enable_t enabled)
{
  const struct gpio_pin_index *index;
  uint8_t irq_index;
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  index = gpio_get_pin(pin);
  if (index == NULL)
  {
    return GPIO_EINVAL;
  }

  irq_index = gpio_bit_to_index(index->pin_bit);
  if (irq_index >= 16)
  {
    return GPIO_EINVAL;
  }

  /* 临界区保护 */
  __disable_irq();

  if (enabled == GPIO_IRQ_ENABLE)
  {
    if (gpio_irq_handlers[irq_index].hdr == NULL)
    {
      __enable_irq();
      return GPIO_EINVAL;
    }

    /* 配置GPIO为中断模式 */
    GPIO_InitStruct.Pin = index->pin_bit;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    switch (gpio_irq_handlers[irq_index].mode)
    {
    case PIN_IRQ_MODE_RISING:
      GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
      break;
    case PIN_IRQ_MODE_FALLING:
      GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
      break;
    case PIN_IRQ_MODE_RISING_FALLING:
      GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
      break;
    default:
      __enable_irq();
      return GPIO_EINVAL;
    }

    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(index->gpio, &GPIO_InitStruct);

    /* 配置NVIC中断 */
    if (irq_index < 5)
    {
      HAL_NVIC_SetPriority((IRQn_Type)(EXTI0_IRQn + irq_index), 5, 0);
      HAL_NVIC_EnableIRQ((IRQn_Type)(EXTI0_IRQn + irq_index));
    }
    else if (irq_index < 10)
    {
      HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
      HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    }
    else
    {
      HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
      HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    }

    gpio_irq_handlers[irq_index].enabled = 1;
    gpio_irq_enable_mask |= (1UL << irq_index);
  }
  else
  {
    /* 禁用中断 */
    if (irq_index < 5)
    {
      HAL_NVIC_DisableIRQ((IRQn_Type)(EXTI0_IRQn + irq_index));
    }
    else if (irq_index < 10)
    {
      /* 只有当该组所有中断都禁用时才禁用NVIC */
      if ((gpio_irq_enable_mask & 0x3E0) == 0)
      {
        HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
      }
    }
    else
    {
      if ((gpio_irq_enable_mask & 0xFC00) == 0)
      {
        HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
      }
    }

    gpio_irq_handlers[irq_index].enabled = 0;
    gpio_irq_enable_mask &= ~(1UL << irq_index);
  }

  __enable_irq();
  return GPIO_OK;
}

/* ========== 中断服务程序 ========== */

static void gpio_irq_handler(uint8_t irq_index)
{
  if (irq_index < 16 && gpio_irq_handlers[irq_index].hdr != NULL)
  {
    gpio_irq_handlers[irq_index].hdr(gpio_irq_handlers[irq_index].args);
  }
}

/* 具体的中断服务程序 */
void EXTI0_IRQHandler(void)
{
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0))
  {
    gpio_irq_handler(0);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
  }
}

void EXTI1_IRQHandler(void)
{
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1))
  {
    gpio_irq_handler(1);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
  }
}

void EXTI2_IRQHandler(void)
{
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2))
  {
    gpio_irq_handler(2);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
  }
}

void EXTI3_IRQHandler(void)
{
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3))
  {
    gpio_irq_handler(3);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
  }
}

void EXTI4_IRQHandler(void)
{
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4))
  {
    gpio_irq_handler(4);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
  }
}

void EXTI9_5_IRQHandler(void)
{
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5))
  {
    gpio_irq_handler(5);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
  }
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6))
  {
    gpio_irq_handler(6);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
  }
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7))
  {
    gpio_irq_handler(7);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
  }
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8))
  {
    gpio_irq_handler(8);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
  }
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9))
  {
    gpio_irq_handler(9);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
  }
}

void EXTI15_10_IRQHandler(void)
{
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10))
  {
    gpio_irq_handler(10);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
  }
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11))
  {
    gpio_irq_handler(11);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
  }
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12))
  {
    gpio_irq_handler(12);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
  }
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13))
  {
    gpio_irq_handler(13);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
  }
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14))
  {
    gpio_irq_handler(14);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);
  }
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15))
  {
    gpio_irq_handler(15);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
  }
}
