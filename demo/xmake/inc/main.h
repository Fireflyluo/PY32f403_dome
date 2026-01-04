/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "py32f4xx_hal.h"
#include <stdint.h>
/* Private includes ----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#define HAL_UART_MODULE_ENABLED
/* Exported variables prototypes ---------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void APP_ErrorHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
