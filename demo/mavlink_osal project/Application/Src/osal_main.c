#include "board.h"
#include "drv_include.h"
#include "task_event.h"

void osal_main(void)
{
    bsp_uart_init();

    HAL_DISABLE_INTERRUPTS();

    osal_init_system();

    osal_add_Task(mav_task_init, mav_task_event_process, 1);

    osal_Task_init();

    osal_mem_kick();

    HAL_ENABLE_INTERRUPTS();

    osal_start_system();
}
