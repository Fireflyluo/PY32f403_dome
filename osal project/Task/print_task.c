// print_task.c
#include "task_event.h"
#include <stdio.h>
#include "board.h"
#include "drv_include.h"
#include "qmi8658a_driver.h"

extern ring_buffer_t imu_ring_buffer;

uint8 print_task_id;
static uint8 print_state = 0;
static void send_single_data_via_uart(const sensor_data_t *data);

void print_task_init(uint8 task_id)
{
    print_task_id = task_id;

    // 启动定时器，每500ms触发一次测试 log 事件
    osal_start_reload_timer(print_task_id, LOG_PRINT_EVENT, 500);
}
uint16 print_task_event_process(uint8 task_id, uint16 task_event)
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

    if (task_event & LOG_PRINT_EVENT)
    {
        // 执行log
        printf("hello world\n");
        return task_event ^ LOG_PRINT_EVENT;
    }
    if (task_event & DATA_SEND_EVENT)
    {
        sensor_data_t data;
        uint16_t data_count = 0;
        uint16_t sent_count = 0;

        data_count = ring_buffer_available(&imu_ring_buffer) / SENSOR_DATA_SIZE;
        if (data_count > 0)
        {
            printf("Sending %d sensor data points via UART...\n", data_count);

            // 批量读取并发送数据
            for (uint16_t i = 0; i < data_count; i++)
            {
                if (ring_buffer_get_multiple(&imu_ring_buffer, (uint8_t *)&data, SENSOR_DATA_SIZE) == SENSOR_DATA_SIZE)
                {
                    send_single_data_via_uart(&data);
                    sent_count++;
                }
                else
                {
                    break; // 读取失败，退出循环
                }
            }
        }
        return task_event ^ DATA_SEND_EVENT;
    }
    return 0;
}
static void send_single_data_via_uart(const sensor_data_t *data)
{
if (data == NULL) return;
    
//    // CSV格式：序列号,时间戳,加速度X,加速度Y,加速度Z,陀螺仪X,陀螺仪Y,陀螺仪Z,温度
//    printf("%u,%u,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.2f\n",
//           data->sequence, 
//           data->timestamp,
//           data->accel[0], data->accel[1], data->accel[2],
//           data->gyro[0], data->gyro[1], data->gyro[2],
//           data->temp);
           
    // JSON格式
    printf("{\"seq\":%u,\"ts\":%u,\"accel\":[%.3f,%.3f,%.3f],\"gyro\":[%.3f,%.3f,%.3f],\"temp\":%.2f}\n",
           data->sequence,
           data->timestamp,
           data->accel[0], data->accel[1], data->accel[2],
           data->gyro[0], data->gyro[1], data->gyro[2],
           data->temp);
}