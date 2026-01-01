#ifndef __BOARD_H
#define __BOARD_H

#include "py32f403xD.h"
#include "py32f4xx_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif
// #define PY32F403xD

// 板载led
#define LED_PIN GET_PIN(B, 2)

    void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */
