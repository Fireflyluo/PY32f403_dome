#ifndef __DRV_IWDG_H__
#define __DRV_IWDG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

//#include "board.h"
#include "py32f4xx_hal.h"

    extern IWDG_HandleTypeDef hiwdg;

    void MX_IWDG_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_IWDG_H__ */
