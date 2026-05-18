#include "board.h"
#include "py32f403_hal_conf.h"

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef OscInitstruct = {0};
    RCC_ClkInitTypeDef ClkInitstruct = {0};

    OscInitstruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE |
        RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSI48M;
    OscInitstruct.HSEState = RCC_HSE_ON;
    OscInitstruct.HSEFreq = RCC_HSE_16_32MHz;
    OscInitstruct.HSI48MState = RCC_HSI48M_ON;
    OscInitstruct.HSIState = RCC_HSI_ON;
    OscInitstruct.LSEState = RCC_LSE_OFF;
    OscInitstruct.LSIState = RCC_LSI_ON;
    OscInitstruct.PLL.PLLState = RCC_PLL_ON;
    OscInitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    OscInitstruct.PLL.PLLMUL = RCC_PLL_MUL9;

    if (HAL_RCC_OscConfig(&OscInitstruct) != HAL_OK)
    {
        Error_Handler();
    }

    ClkInitstruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                              RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    ClkInitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    ClkInitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    ClkInitstruct.APB1CLKDivider = RCC_HCLK_DIV1;
    ClkInitstruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&ClkInitstruct, FLASH_LATENCY_6) != HAL_OK)
    {
        Error_Handler();
    }
}
