// led_task.c
#include "task_event.h"
#include <stdio.h>

#include "drv_gpio.h"

#define LED_PIN GET_PIN(B, 2)

uint8 led_task_id;
static uint8 led_state = 0;
static uint8 blink_pattern[] = {1, 0, 1, 1, 0, 0}; // 闪烁模式
static uint8 pattern_index = 0;

void led_task_init(uint8 task_id)
{
    led_task_id = task_id;

    // 初始化LED硬件
    gpio_init();
    gpio_mode(LED_PIN, GPIO_MODE_OUTPUT_PP);

    // 启动定时器，每500ms触发一次闪烁事件
    osal_start_reload_timer(led_task_id, LED_BLINK_EVENT, 500);

    // 启动模式切换定时器，每3秒切换一次模式
    osal_start_reload_timer(led_task_id, LED_PATTERN_EVENT, 3000);
}

uint16 led_task_event_process(uint8 task_id, uint16 task_event)
{
    if (task_event & SYS_EVENT_MSG)
    {
        // 处理系统消息（如果有）
        osal_sys_msg_t *msg_pkt;
        msg_pkt = (osal_sys_msg_t *)osal_msg_receive(task_id);
        while (msg_pkt)
        {
            // 处理消息
            osal_msg_deallocate((uint8 *)msg_pkt);
            msg_pkt = (osal_sys_msg_t *)osal_msg_receive(task_id);
        }
        return (task_event ^ SYS_EVENT_MSG);
    }

    if (task_event & LED_BLINK_EVENT)
    {
        // 执行LED闪烁
        if (blink_pattern[pattern_index])
        {
            /* 点亮LED */
            gpio_write(LED_PIN, GPIO_PIN_SET);
            // 实际硬件操作: LED_ON();
        }
        else
        {
            /* 熄灭LED */
            gpio_write(LED_PIN, GPIO_PIN_RESET);
            // 实际硬件操作: LED_OFF();
        }
        return task_event ^ LED_BLINK_EVENT;
    }

    if (task_event & LED_PATTERN_EVENT)
    {
        // 切换闪烁模式
        pattern_index = (pattern_index + 1) % (sizeof(blink_pattern) / sizeof(blink_pattern[0]));

        return task_event ^ LED_PATTERN_EVENT;
    }

    return 0;
}