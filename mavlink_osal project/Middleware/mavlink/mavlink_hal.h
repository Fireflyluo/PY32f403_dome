#ifndef MAVLINK_HAL_H
#define MAVLINK_HAL_H

#include "main.h"
#include "drv_include.h"
#include "mavlink.h"
#include "mavlink_types.h"
// »º³åÇø¶¨Òå
#define MAVLINK_RX_BUFFER_SIZE 512
#define MAVLINK_TX_BUFFER_SIZE 512

typedef struct {
    uart_instance_t uart_instamce;
    uint8_t rx_buffer[MAVLINK_RX_BUFFER_SIZE];
    uint8_t tx_buffer[MAVLINK_TX_BUFFER_SIZE];
    uint16_t rx_index;
    uint32_t last_heartbeat_ms; 
    mavlink_status_t status;
    mavlink_message_t rx_msg;
} mavlink_handl_t;

void mavlink_init(void);
void mavlink_receive_byte(uint8_t byte);
void mavlink_periodic_task(void);
void mavlink_send_heartbeat(void);
void send_position_target(float x, float y, float z, float yaw) ;
void mavlink_send_message(const mavlink_message_t *msg);

#endif