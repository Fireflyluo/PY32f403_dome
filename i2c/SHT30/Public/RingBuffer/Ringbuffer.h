#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "py32f4xx_hal.h"
#include "main.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define RING_BUFFER_SIZE 256
#define RING_BUFFER_MASK (RING_BUFFER_SIZE - 1)
/**
 * @brief 环形缓冲区结构体定义
 */
typedef struct {
   volatile uint16_t head;
   volatile uint16_t tail;
   uint8_t buffer[RING_BUFFER_SIZE];
} RingBuffer;

// 初始化环形缓冲区
void RingBuffer_Init(RingBuffer* rb);
void RingBuffer_Clear(RingBuffer* pRingBuffer);
uint16_t RingBuffer_AvailableData(RingBuffer* rb);
uint16_t RingBuffer_FreeSpace(RingBuffer* rb);
uint8_t RingBuffer_Write(RingBuffer* rb,  uint8_t data);//单字节写入
uint8_t RingBuffer_Read(RingBuffer* rb, uint8_t* data);//单字节读取
uint8_t RingBuffer_WriteMulti(RingBuffer* rb, uint8_t* data, uint16_t length);//多字节写入
uint8_t RingBuffer_ReadMulti(RingBuffer* rb, uint8_t* data, uint16_t length);//多字节读取
uint8_t RingBuffer_IsEmpty(RingBuffer* rb);//检查缓冲区是否为空
uint8_t RingBuffer_IsFull(RingBuffer* rb);//检查缓冲区是否已满
uint8_t RingBuffer_Peek(const RingBuffer* pRingBuff, uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif 

