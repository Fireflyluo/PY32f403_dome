#include "drv_iwdg.h"

IWDG_HandleTypeDef hiwdg;

void MX_IWDG_Init(void)
{

    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
    hiwdg.Init.Reload = 1;
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {
        APP_ErrorHandler();
    }
}
