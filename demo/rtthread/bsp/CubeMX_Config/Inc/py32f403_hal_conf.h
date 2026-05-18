#ifndef __PY32F403_HAL_CONF_H
#define __PY32F403_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* ########################## Module Selection ############################## */
#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_EXTI_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
#define HAL_RTC_MODULE_ENABLED
#define HAL_IWDG_MODULE_ENABLED

/* ########################## Register Callback ############################## */
#define USE_HAL_ADC_REGISTER_CALLBACKS   0U
#define USE_HAL_I2C_REGISTER_CALLBACKS   0U
#define USE_HAL_SPI_REGISTER_CALLBACKS   0U
#define USE_HAL_TIM_REGISTER_CALLBACKS   0U
#define USE_HAL_UART_REGISTER_CALLBACKS  0U
#define USE_HAL_USART_REGISTER_CALLBACKS 0U

/* ########################## HSE/HSI Values ################################ */
#if !defined(HSE_VALUE)
#define HSE_VALUE 16000000U
#endif
#define HSE_STARTUP_TIMEOUT 100U

#if !defined(HSI_VALUE)
#define HSI_VALUE 8000000U
#endif

#if !defined(HSI48_VALUE)
#define HSI48_VALUE 48000000U
#endif

#if !defined(LSI_VALUE)
#define LSI_VALUE 40000U
#endif

#if !defined(LSE_VALUE)
#define LSE_VALUE 32768U
#endif

#if !defined(LSE_STARTUP_TIMEOUT)
#define LSE_STARTUP_TIMEOUT 5000U
#endif

#define VDD_VALUE 3300U
#define TICK_INT_PRIORITY 0x0FU
#define USE_RTOS 0U
#define USE_SPI_CRC 1U

#include "py32f403_hal_def.h"

/* ########################## HAL headers ################################### */
#ifdef HAL_CORTEX_MODULE_ENABLED
#include "py32f403_hal_cortex.h"
#endif
#ifdef HAL_DMA_MODULE_ENABLED
#include "py32f403_hal_dma.h"
#endif
#ifdef HAL_EXTI_MODULE_ENABLED
#include "py32f403_hal_exti.h"
#endif
#ifdef HAL_FLASH_MODULE_ENABLED
#include "py32f403_hal_flash.h"
#endif
#ifdef HAL_GPIO_MODULE_ENABLED
#include "py32f403_hal_gpio.h"
#endif
#ifdef HAL_RCC_MODULE_ENABLED
#include "py32f403_hal_rcc.h"
#endif
#ifdef HAL_I2C_MODULE_ENABLED
#include "py32f403_hal_i2c.h"
#endif
#ifdef HAL_SPI_MODULE_ENABLED
#include "py32f403_hal_spi.h"
#endif
#ifdef HAL_PWR_MODULE_ENABLED
#include "py32f403_hal_pwr.h"
#endif
#ifdef HAL_TIM_MODULE_ENABLED
#include "py32f403_hal_tim.h"
#endif
#ifdef HAL_UART_MODULE_ENABLED
#include "py32f403_hal_uart.h"
#endif
#ifdef HAL_ADC_MODULE_ENABLED
#include "py32f403_hal_adc.h"
#endif
#ifdef HAL_RTC_MODULE_ENABLED
#include "py32f403_hal_rtc.h"
#endif
#ifdef HAL_IWDG_MODULE_ENABLED
#include "py32f403_hal_iwdg.h"
#endif

#ifndef USE_FULL_ASSERT
#define assert_param(expr) ((void)0U)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __PY32F403_HAL_CONF_H */
