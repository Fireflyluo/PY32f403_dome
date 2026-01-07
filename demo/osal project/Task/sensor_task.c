// sensor_task.c
#include "task_event.h"
#include <stdio.h>
#include <math.h>

#include "drv_include.h"
#include "board.h"
#include "qmi8658a_driver.h"

extern I2C_HandleTypeDef hi2c2;

ring_buffer_t imu_ring_buffer;
uint8_t imu_buffer[4096];
static uint8_t data_sequence = 0; // 数据序列号

uint8 sensor_task_id;
static uint8 sensor_state = 0;
extern uint8 print_task_id;
void sensor_task_init(uint8 task_id)
{
    sensor_task_id = task_id;

    // 初始化sensor硬件
    qmi8658a_device.init(&hi2c2);
    qmi8658a_device.set_odr(GYRO_ODR_400HZ);
    qmi8658a_device.set_range(GYRO_RANGE_500DPS);

    /* 初始化环形缓冲区 */
    ring_buffer_init(&imu_ring_buffer, imu_buffer, sizeof(imu_buffer));

    // 启动定时器，每100ms触发一次传感器采集
    osal_start_reload_timer(sensor_task_id, SENSOR_COLLECT_EVENT, 10);
}
uint16 sensor_task_event_process(uint8 task_id, uint16 task_event)
{
    sensor_data_t data;
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

    if (task_event & SENSOR_COLLECT_EVENT)
    {
        // 执行imu采集
        int32_t ret = qmi8658a_device.read_data(&data);

        if (ret == 0)
        {
            // 添加时间戳和序列号
            data.timestamp = osal_GetSystemClock();
            data.sequence = data_sequence++;

            uint16_t bytes_written = ring_buffer_put_multiple(&imu_ring_buffer,
                                                              (uint8_t *)&data,
                                                              SENSOR_DATA_SIZE);

            if (bytes_written == SENSOR_DATA_SIZE)
            {
                // 写入成功
                uint16_t current_count = ring_buffer_available(&imu_ring_buffer) / SENSOR_DATA_SIZE;
                uint16_t max_count = sizeof(imu_buffer) / SENSOR_DATA_SIZE;

//                printf("[%u] Data stored. Count: %d/%d, Seq: %d\n",
//                       data.timestamp, current_count, max_count, data.sequence);

                // 检查缓冲区是否达到阈值，触发发送任务
                if (current_count >= 64)
                {
                    osal_set_event(print_task_id, DATA_SEND_EVENT);
                }
            }
            else
            {
                printf("Buffer full! Data lost. Free space: %d, Needed: %d\n",
                       ring_buffer_free_space(&imu_ring_buffer), SENSOR_DATA_SIZE);
            }

            // 执行温湿度采集
        }
        return task_event ^ SENSOR_COLLECT_EVENT;
    }

    return 0;
}