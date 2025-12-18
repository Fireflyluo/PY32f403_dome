#include "mavlink_hal.h"

#include <string.h>

static mavlink_handl_t mav_handl;
static uint8_t mavlink_system_id = 255;
static uint8_t mavlink_component_id = 1;
static void handle_mavlink_message(mavlink_message_t *msg);

extern uart_instance_t uart2_instance;
// 初始化MAVLink
void mavlink_init(void)
{

    memset(&mav_handl, 0, sizeof(mavlink_handl_t));
        
    mav_handl.uart_instamce = uart2_instance;
    mav_handl.rx_index = 0;
    mav_handl.last_heartbeat_ms = HAL_GetTick();
    
}

// 接收字节处理
void mavlink_receive_byte(uint8_t byte)
{
    mavlink_message_t msg;
    mavlink_status_t status;
    
    // 解析MAVLink消息
    if (mavlink_parse_char(MAVLINK_COMM_0, byte, &msg, &mav_handl.status)) {
        mav_handl.rx_msg = msg;
        handle_mavlink_message(&msg);
    }
}

// 发送MAVLink消息
void mavlink_send_message(const mavlink_message_t *msg)
{
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buffer, msg);
   
    
//    uart_send(uart2_instance, buffer, len, 100);
uart_send(mav_handl.uart_instamce, buffer, len, 100);
  

}

// 发送心跳包
void mavlink_send_heartbeat(void)
{
    static uint32_t sequence = 0;
    
    mavlink_heartbeat_t heartbeat = {
        .type = MAV_TYPE_QUADROTOR,           // 2
        .autopilot = MAV_AUTOPILOT_GENERIC,   // 0
        .base_mode = 192,                     // 128+64
        .custom_mode = 0,
        .system_status = MAV_STATE_ACTIVE,    // 4
        .mavlink_version = 3                  // MAVLink 2.0
    };
    
    mavlink_message_t msg;
    mavlink_msg_heartbeat_encode(mavlink_system_id, 
                                 mavlink_component_id, 
                                 &msg, 
                                 &heartbeat);
    
    // 设置序列号
    msg.seq = sequence++;
    if(sequence > 255) sequence = 0;
    
    mavlink_send_message(&msg);
}

// 定期任务
void mavlink_periodic_task(void)
{
    uint32_t now = HAL_GetTick();
    
    // 每秒发送一次心跳
    if (now - mav_handl.last_heartbeat_ms > 1000) {
        mavlink_send_heartbeat();
        mav_handl.last_heartbeat_ms = now;
    }
}

// 处理接收到的消息
void handle_mavlink_message(mavlink_message_t *msg)
{
    switch (msg->msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT: {
//            mavlink_heartbeat_t heartbeat;
//            mavlink_msg_heartbeat_decode(msg, &heartbeat);
            // 处理心跳
            break;
        }
        case MAVLINK_MSG_ID_COMMAND_LONG: {
//            mavlink_command_long_t cmd;
//            mavlink_msg_command_long_decode(msg, &cmd);
//            handle_command(&cmd);
            break;
        }
        // 其他消息处理...
    }
}