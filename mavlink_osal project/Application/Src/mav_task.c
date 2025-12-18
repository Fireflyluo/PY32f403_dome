/**
 * @file mav_task.c
 * @brief mavlink����
 * @version 1.0
 * @date 2025-12-18
 */
#include "task_event.h"
#include <stdio.h>

#include "task_event.h"
#include "mavlink_hal.h"

uint8 mav_task_id;
void mav_task_init(uint8 task_id)
{
    mav_task_id = task_id;

     // 初始化mavlink
   mavlink_init();
    // 启动定时器，每1000ms触发一次心跳包
    osal_start_reload_timer(mav_task_id, MAV_HEAR_EVENT, 1000);
}
uint16 mav_task_event_process(uint8 task_id, uint16 task_event)
{

 if (task_event & SYS_EVENT_MSG)
    {
        // 处理系统消息（如果有）
        osal_sys_msg_t *msg_pkt;
        msg_pkt = (osal_sys_msg_t *)osal_msg_receive(task_id);
        while (msg_pkt)
        {
            // 系统消息处理
            // ....

            // 释放消息内存
            osal_msg_deallocate((uint8 *)msg_pkt);
            msg_pkt = (osal_sys_msg_t *)osal_msg_receive(task_id); // 读取下一条消息
        }
        return (task_event ^ SYS_EVENT_MSG);
    }

    if (task_event & MAV_HEAR_EVENT)
    {
        // 发送mavlink 心跳包
       
        mavlink_send_heartbeat();
        return task_event ^ MAV_HEAR_EVENT;
    }

  return 0;
}
