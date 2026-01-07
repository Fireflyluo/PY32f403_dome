// print_task.c
#include "task_event.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "board.h"
#include "drv_include.h"
#include "qmi8658a_driver.h"
#include "data_protocol.h"

extern ring_buffer_t imu_ring_buffer;
extern uart_instance_t log_uart_instance;

uint8 print_task_id;
static uint8 print_state = 0;

static uint8_t uart_cmd_buffer[512]; // 命令缓冲区
static uint16_t uart_cmd_index = 0;  // 命令缓冲区索引
uint16_t accel_data[124] = {0};

void process_uart_command(const char *data, uint16_t length);
static void send_single_data_via_uart(const sensor_data_t *data);
static void process_uart_commands(void);
static void send_error_response(uint8_t seq, const char *error_msg);
static void handle_query_command(uint8_t seq);

void print_task_init(uint8 task_id)
{
    print_task_id = task_id;

    // 启动定时器，每500ms触发一次测试 log 事件
    osal_start_reload_timer(print_task_id, CMD_PRINT_EVENT, 500);
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

    if (task_event & CMD_PRINT_EVENT) // 命令处理事件
    {
        // 执行log
        process_uart_commands();
        printf("tim_log");
        // uint8_t cmd_buffer[20]; // 上位机命令缓冲区
        // uint16_t received_bytes = uart_read_from_ring_buffer(log_uart_instance, cmd_buffer, sizeof(cmd_buffer));
        // if (received_bytes > 0)
        // {

        //     // 立即回显接收到的数据
        //     uart_send_async(log_uart_instance, cmd_buffer, received_bytes);
        // }
        return task_event ^ CMD_PRINT_EVENT;
    }
    if (task_event & DATA_SEND_EVENT)
    {
        sensor_data_t data;
        uint16_t data_count = 0; // 数据计数器
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
                    //                    send_single_data_via_uart(&data);

                    float accel = sqrt((data.gyro[0] * data.gyro[0]) + (data.gyro[1] * data.gyro[1]) + (data.gyro[2] * data.gyro[2]));
                    uint16_t accel_12bit = acceleration_to_12bit(accel, 4.0f);
                    accel_data[sent_count] = accel_12bit;

                    sent_count++;
                }
                else
                {
                    break; // 读取失败，退出循环
                }
            }
            char rx_frame[256];
            uint16_t temperature = temperature_to_12bit(data.temp);
            report_data_decoded_t report = {.tag_id = {0x01, 0x02, 0x03, 0x04, 0x05},
                                            .start_hour = 10,
                                            .start_minute = 30,
                                            .start_second = 45,
                                            .sequence = 1,
                                            .temperature = temperature,
                                            .humidity = humidity_to_12bit(65.0f),
                                            .acceleration = accel_data,
                                            .accel_count = sent_count};

            // 构建一个实际的报告消息
            uint16_t rx_len = build_report_msg(rx_frame, sizeof(rx_frame), 1,
                                               MSG_REPORT_FIRST, &report);
            if (rx_len > 0)
            {

                for (uint16_t i = 0; i < rx_len; i++)
                    printf("%c", rx_frame[i]);
                printf("\n");
            }
        }
        return task_event ^ DATA_SEND_EVENT;
    }
    return 0;
}

static void send_single_data_via_uart(const sensor_data_t *data)
{
    if (data == NULL)
        return;

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

static void process_uart_commands(void)
{
    uint8_t temp_buffer[64];
    uint16_t bytes_read;

    // 从DMA环形缓冲区读取数据
    bytes_read = uart_read_from_ring_buffer(log_uart_instance, temp_buffer, sizeof(temp_buffer));

    if (bytes_read > 0)
    {
        // 将数据添加到命令缓冲区
        if (uart_cmd_index + bytes_read < sizeof(uart_cmd_buffer))
        {
            memcpy(&uart_cmd_buffer[uart_cmd_index], temp_buffer, bytes_read);
            uart_cmd_index += bytes_read;
        }
        else
        {
            // 缓冲区溢出，清空缓冲区
            uart_cmd_index = 0;
            return;
        }

        // 检查是否有完整的帧
        uint16_t frame_start, frame_end;
        while (is_frame_complete((char *)uart_cmd_buffer, uart_cmd_index, &frame_start, &frame_end))
        {
            // 提取完整帧并处理
            uint16_t frame_length = frame_end - frame_start + 1;
            process_uart_command((char *)&uart_cmd_buffer[frame_start], frame_length);

            // 从缓冲区中移除已处理的帧
            if (frame_end + 1 < uart_cmd_index)
            {
                memmove(uart_cmd_buffer, &uart_cmd_buffer[frame_end + 1], uart_cmd_index - frame_end - 1);
                uart_cmd_index -= frame_length;
            }
            else
            {
                uart_cmd_index = 0;
            }
        }

        // 防止缓冲区积累过多不完整数据
        if (uart_cmd_index > 200)
        {
            // 保留最后100字节（可能包含不完整的帧开头）
            memmove(uart_cmd_buffer, &uart_cmd_buffer[uart_cmd_index - 100], 100);
            uart_cmd_index = 100;
        }
    }
}

// 全局变量
static uint8_t cmd_buffer[256];
static uint16_t cmd_buffer_len = 0;
extern uint8_t print_task_id;
extern uint8_t sensor_task_id;

// 命令处理函数
void process_uart_command(const char *data, uint16_t length)
{
    msg_type_t type;
    uint8_t seq;
    char content_buf[128];
    uint16_t content_len;
    uint32_t crc_received;

    frame_status_t status = parse_frame(data, length, &type, &seq, content_buf, &content_len, &crc_received);

    if (status == FRAME_OK)
    {
        switch (type)
        {
        case MSG_QUERY:
            // 处理查询命令
            handle_query_command(seq);
            break;

        case MSG_SET_PARAMS:
            // 处理参数设置命令
            //            handle_set_params_command(seq, content_buf, content_len);
            break;

        case MSG_REPEAT_QUERY:
            // 处理重复查询命令
            // handle_repeat_query_command(seq);
            break;

        default:
            // 发送错误响应
            send_error_response(seq, "Unknown command");
            break;
        }
    }
    else
    {
        // 帧解析错误，发送错误响应
        send_error_response(0, "Frame parse error");
    }
}

// 查询命令处理
static void handle_query_command(uint8_t seq)
{
    osal_set_event(print_task_id, DATA_SEND_EVENT);
}

// 参数设置命令处理
// static void handle_set_params_command(uint8_t seq, const char *content, uint16_t content_len)
//{
//    param_data_decoded_t params;
//    if (parse_param_data(content, content_len, &params))
//    {
//        // 参数解析成功，应用到系统
//        apply_system_params(&params);

//        // 发送确认响应
//        char ack_msg[64];
//        build_ack_msg(ack_msg, sizeof(ack_msg), seq, content, content_len);
//        uart_send_async(log_uart_instance, (uint8_t *)ack_msg, strlen(ack_msg));

//        // 通知传感器任务参数已更新
//        osal_set_event(sensor_task_id, CMD_PARSE_EVENT);
//    }
//    else
//    {
//        send_error_response(seq, "Parameter parse error");
//    }
//}

// 错误响应
static void send_error_response(uint8_t seq, const char *error_msg)
{
    char error_frame[128];
    // 构建错误报告帧
    // TODO: 实现错误消息帧构建
    uart_send_async(log_uart_instance, (uint8_t *)error_frame, strlen(error_frame));
}