#include "mavlink_hal.h"

#include <string.h>

static mavlink_handl_t mav_handl;
static uint8_t mavlink_system_id = 255;
static uint8_t mavlink_component_id = 1;
static void handle_mavlink_message(mavlink_message_t *msg);

extern uart_instance_t uart2_instance;
// ³õÊ¼»¯MAVLink
void mavlink_init(void)
{

    memset(&mav_handl, 0, sizeof(mavlink_handl_t));
        
    mav_handl.uart_instamce = uart2_instance;
    mav_handl.rx_index = 0;
    mav_handl.last_heartbeat_ms = HAL_GetTick();
    
}

// ½ÓÊÕ×Ö½Ú´¦Àí
void mavlink_receive_byte(uint8_t byte)
{
    mavlink_message_t msg;
    mavlink_status_t status;
    
    // ½âÎöMAVLinkÏûÏ¢
    if (mavlink_parse_char(MAVLINK_COMM_0, byte, &msg, &mav_handl.status)) {
        mav_handl.rx_msg = msg;
        handle_mavlink_message(&msg);
    }
}

// ·¢ËÍMAVLinkÏûÏ¢
void mavlink_send_message(const mavlink_message_t *msg)
{
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buffer, msg);
   
    
//    uart_send(uart2_instance, buffer, len, 100);
uart_send(mav_handl.uart_instamce, buffer, len, 100);
  

}


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
    

    msg.seq = sequence++;
    if(sequence > 255) sequence = 0;
    
    mavlink_send_message(&msg);
}

// å‘é€ä½ç½®ç›®æ ‡å‡½æ•°
void send_position_target(float x, float y, float z, float yaw) 
{
    mavlink_message_t msg;
    mavlink_set_position_target_local_ned_t sp;
    
    sp.time_boot_ms = HAL_GetTick();
    sp.target_system = 1;      // PX4 ID
    sp.target_component = 1;   // PX4ç»„ä»¶ID
    sp.coordinate_frame = MAV_FRAME_LOCAL_NED;  // ä½¿ç”¨NEDåæ ‡ç³»
    sp.type_mask = 0xFF8;      // åªæ§åˆ¶ä½ç½®ï¼ˆä¸æ§åˆ¶é€Ÿåº¦ã€åŠ é€Ÿåº¦ï¼‰
    
    sp.x = x;      // åŒ—å‘ï¼ˆç±³ï¼‰
    sp.y = y;      // ä¸œå‘ï¼ˆç±³ï¼‰
    sp.z = z;      // åœ°å‘ï¼ˆç±³ï¼Œå‘ä¸‹ä¸ºæ­£ï¼Œæ‰€ä»¥é«˜åº¦ä¸ºè´Ÿå€¼ï¼‰
    sp.yaw = yaw;  // åèˆªè§’ï¼ˆå¼§åº¦ï¼‰
    
    // è®¾ç½®é€Ÿåº¦å’ŒåŠ é€Ÿåº¦ä¸º0ï¼ˆè™½ç„¶è¢«å±è”½ï¼Œä½†æœ€å¥½è®¾ç½®ï¼‰
    sp.vx = 0.0f;
    sp.vy = 0.0f;
    sp.vz = 0.0f;
    sp.afx = 0.0f;
    sp.afy = 0.0f;
    sp.afz = 0.0f;
    sp.yaw_rate = 0.0f;
    
    // ç¼–ç æ¶ˆæ¯
    mavlink_msg_set_position_target_local_ned_encode(
        mavlink_system_id,
        mavlink_component_id,
        &msg,
        &sp
    );
    
    // å‘é€æ¶ˆæ¯
    mavlink_send_message(&msg);

}

void mavlink_periodic_task(void)
{
    uint32_t now = HAL_GetTick();
    
    // Ã¿Ãë·¢ËÍÒ»´ÎĞÄÌø
    if (now - mav_handl.last_heartbeat_ms > 1000) {
        mavlink_send_heartbeat();
        mav_handl.last_heartbeat_ms = now;
    }
}

// ´¦Àí½ÓÊÕµ½µÄÏûÏ¢
void handle_mavlink_message(mavlink_message_t *msg)
{
    switch (msg->msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT: {
//            mavlink_heartbeat_t heartbeat;
//            mavlink_msg_heartbeat_decode(msg, &heartbeat);
            // ´¦ÀíĞÄÌø
            break;
        }
        case MAVLINK_MSG_ID_COMMAND_LONG: {
//            mavlink_command_long_t cmd;
//            mavlink_msg_command_long_decode(msg, &cmd);
//            handle_command(&cmd);
            break;
        }
        // ÆäËûÏûÏ¢´¦Àí...
    }
}