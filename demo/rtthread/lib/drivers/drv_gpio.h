#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__

#include <py32f4xx.h>
#include <rtdevice.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __PY32_PORT(port) GPIO##port##_BASE

#define GET_PIN(PORTx, PIN) \
  (rt_base_t)((16 * (((rt_base_t)__PY32_PORT(PORTx) - (rt_base_t)GPIOA_BASE) / (0x0400UL))) + PIN)

struct pin_irq_map {
  rt_uint16_t pinbit;
  IRQn_Type irqno;
};

int rt_hw_pin_init(void);

#ifdef __cplusplus
}
#endif

#endif
