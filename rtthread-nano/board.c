/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-24                  the first version
 */

#include <rthw.h>
#include <rtthread.h>

#include "py32f4xx_hal.h"

static void SystemClockConfig(void);

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 1024 means 1024 bytes
 */
#define RT_HEAP_SIZE (15*1024)
static rt_uint8_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

void rt_os_tick_callback(void)
{
    rt_interrupt_enter();
    
    rt_tick_increase();

    rt_interrupt_leave();
}

/**
 * This function will initial your board.
 */
void rt_hw_board_init(void)
{
    HAL_Init();
    SystemClockConfig();
    /* 配置系统时钟节拍，1ms中断一次 */
    SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
    /* 
     * TODO 1: OS Tick Configuration
     * Enable the hardware timer and call the rt_os_tick_callback function
     * periodically with the frequency RT_TICK_PER_SECOND. 
     */

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

#ifdef RT_USING_CONSOLE

static int uart_init(void)
{
#error "TODO 2: Enable the hardware uart and config baudrate."
    return 0;
}
INIT_BOARD_EXPORT(uart_init);

void rt_hw_console_output(const char *str)
{
#error "TODO 3: Output the string 'str' through the uart."
}

#endif

/**
 * @brief  系统时钟配置
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
  RCC_OscInitTypeDef OscInitstruct = {0};
  RCC_ClkInitTypeDef ClkInitstruct = {0};

  // 配置振荡器类型，启用多种振荡器检测
  OscInitstruct.OscillatorType =
      RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE |
      RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSI48M;
  OscInitstruct.HSEState = RCC_HSE_ON;       // 启用外部高速时钟 (HSE)
  OscInitstruct.HSEFreq = RCC_HSE_16_32MHz;  // 选择HSE频率为16-32MHz
  OscInitstruct.HSI48MState = RCC_HSI48M_ON; // 禁用48MHz内部RC振荡器 (HSI48M)
  OscInitstruct.HSIState = RCC_HSI_ON;       // 启用内部高速时钟 (HSI)
  OscInitstruct.LSEState = RCC_LSE_OFF;      // 禁用外部低速时钟 (LSE)
  // OscInitstruct.LSEDriver = RCC_LSEDRIVE_HIGH;                   //
  // 设置驱动能力等级为高
  OscInitstruct.LSIState = RCC_LSI_ON;            // 启用内部低速时钟 (LSI)
  OscInitstruct.PLL.PLLState = RCC_PLL_ON;         // 启用PLL
  OscInitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE; // PLL时钟源选择HSE
  OscInitstruct.PLL.PLLMUL = RCC_PLL_MUL9;         // PLL倍频系数为9   16*9=144MHz

  // 配置振荡器
  if (HAL_RCC_OscConfig(&OscInitstruct) != HAL_OK)
  {
//    APP_ErrorHandler(); // 错误处理函数
  }

  // 配置系统时钟
  ClkInitstruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                            RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  ClkInitstruct.SYSCLKSource =
      RCC_SYSCLKSOURCE_PLLCLK;                   // 系统时钟源选择PLL输出PLLCLK
  ClkInitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1; // AHB总线不分频
  ClkInitstruct.APB1CLKDivider = RCC_HCLK_DIV1;  // APB1总线不分频
  ClkInitstruct.APB2CLKDivider = RCC_HCLK_DIV2;  // APB2总线分频2

  /*
   * 重新初始化RCC时钟
   * -- clock <= 24MHz: FLASH_LATENCY_0
   * -- clock > 24MHz:  FLASH_LATENCY_1
   */
  // 配置时钟
  if (HAL_RCC_ClockConfig(&ClkInitstruct, FLASH_LATENCY_6) != HAL_OK)
  {
//    APP_ErrorHandler(); // 错误处理函数
  }
}

void SysTick_Handler(void)
{
    /* 进入中断 */
    rt_interrupt_enter();

    /* 维护HAL库时钟 */
    HAL_IncTick();

    /* 通知RT-Thread系统心跳 */
    rt_tick_increase();

    /* 离开中断 */
    rt_interrupt_leave();
}
