#ifndef _SERIAL_H_
#define _SERIAL_H_
#include "py32f4xx_hal.h"
#include "main.h"

#define RETURN_SUCCESS 0
#define RETURN_FAILURE 1

#define USART_DMA_RX_BUFFER_MAXIMUM 256
#define USART_DMA_TX_BUFFER_MAXIMUM 256

extern uint8_t cmd_data[32];
extern uint8_t cmd_flag ;

extern uint8_t usart1_rx_buffer[USART_DMA_RX_BUFFER_MAXIMUM]; //串口1的DMA接收缓冲区
extern uint8_t usart1_tx_buffer[USART_DMA_TX_BUFFER_MAXIMUM]; //串口1的DMA发送缓冲区
extern uint8_t usart1_rx_flag; //DMA接收成功标志 0，未接收到/1,接收到等待处理
extern uint16_t usart1_rx_len; //DMA一次空闲中断接收到的数据长度

extern uint8_t usart2_rx_buffer[USART_DMA_RX_BUFFER_MAXIMUM]; //串口2的DMA接收缓冲区
extern uint8_t usart2_tx_buffer[USART_DMA_TX_BUFFER_MAXIMUM]; //串口2的DMA发送缓冲区
extern uint8_t usart2_rx_flag; //DMA接收成功标志 0，未接收到/1,接收到等待处理
extern uint16_t usart2_rx_len; //DMA一次空闲中断接收到的数据长度

//extern uint8_t receive_data[];	  //DMA接收数据缓存区

void serial_init(void);
void HAL_UART_ReceiveIdle(UART_HandleTypeDef *huart);
uint8_t UART1_TX_DMA_Send(uint8_t *buffer, uint16_t length);
uint8_t UART2_TX_DMA_Send(uint8_t *buffer, uint16_t length);

#endif
