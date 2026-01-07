#ifndef APPLICATION_H
#define APPLICATION_H

#include "osal.h"
#include "osal_timer.h"
#include "osal_event.h"
#include "osal_memory.h"
#include "osal_msg.h"

// 全局变量声明
/*****************************************************************************/

typedef struct
{
    osal_event_hdr_t hdr; // 操作系统事件结构
    unsigned char *data;  // 命令帧操作数
} general_msg_data_t;     // 自定义通用消息格式结构体

/*****************************************************************************/

// 所有任务的任务ID、初始化函数、事件处理函数、任务事件都统一在此文件声明或定义
/*****************************************************************************/

// 声明任务ID
extern uint8 led_task_id;
extern uint8 mav_task_id;

// 任务初始化函数声明
void led_task_init(uint8 task_id);
void print_task_init(uint8 task_id);
void sensor_task_init(uint8 task_id);

// 任务事件处理函数声明
uint16 led_task_event_process(uint8 task_id, uint16 task_event);
uint16 print_task_event_process(uint8 task_id, uint16 task_event);
uint16 sensor_task_event_process(uint8 task_id, uint16 task_event);

// 任务事件定义
// 系统消息事件，默认保留为osal系统使用，用于收发消息
#define SYS_EVENT_MSG 0x8000

// LED任务事件定义
#define LED_BLINK_EVENT 0x0001

// print 任务的任务事件定义
#define CMD_PRINT_EVENT 0x0001 // 日志打印事件
#define DATA_SEND_EVENT 0x0002 // 数据发送事件

// 传感器任务事件定义
#define SENSOR_COLLECT_EVENT 0x0001 // 传感器采集
#define CMD_PARSE_EVENT 0x0002      // 命令解析事件
// 统计任务的系统消息事件定义
#define PRINTF_STATISTICS 1 // 打印统计消息事件

/*****************************************************************************/

void osal_main(void);

#endif
