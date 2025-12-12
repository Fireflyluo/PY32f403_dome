#ifndef _OS_HARDWARE_H_
#define _OS_HARDWARE_H_

#include "osConfig.h" 
#include "osTypedef.h"
#include "py32f4xx_hal.h"

/* 临界段保护 */
extern uint16_t criticalNesting;
#define DISABLE_IRQ()  	__disable_irq()
#define ENABLE_IRQ()  	__enable_irq()

#define ENTER_CRITICAL()    \
    do { \
        uint32_t primask = __get_PRIMASK(); \
        __disable_irq(); \
        criticalNesting++; \
        (void)primask; \
    } while(0)

#define EXIT_CRITICAL() \
    do { \
        if(--criticalNesting == 0) { \
            __enable_irq(); \
        } \
    } while(0)

/* 系统定时器相关定义 */
#define SYSTICK_CTRL        (SysTick->CTRL)
#define SYSTICK_LOAD        (SysTick->LOAD)
#define SYSTICK_VAL         (SysTick->VAL)

/* 系统时钟频率 */
#define SYS_CLOCK_HZ        144000000UL

/* 系统定时器中断向量 */
#define SYSTEM_TIME_ISR     SysTick_Handler

/* 系统定时器频率（Hz）最小为1Hz,最大不超过1000Hz,推荐频率100Hz~1000Hz*/
#ifndef TICK_RATE_HZ
	#define TICK_RATE_HZ   		1000u
#endif

/* 系统定时器运行周期us */
#define TICK_CYCLICITY_US 	(1000000u / TICK_RATE_HZ)	


/* 看门狗相关（如果使用） */
#ifdef USE_IWDG
    #define OS_CLR_WDT()    __HAL_IWDG_RELOAD_COUNTER(&hiwdg)
#else
    #define OS_CLR_WDT()    do{}while(0)  // 如果不使用看门狗
#endif

extern bool gClrWdt;

/* 函数声明 */
extern void wdt_init(void);
extern void clr_wdt(void);
extern void sys_tick_init(void);
extern uint32_t get_sys_tick_val(void);
extern uint32_t get_sys_tick_load(void);
void os_systick_handler(void);
#endif