#include "Ringbuffer.h"

 /*!
 *  @brief        初始化环形缓冲区          
 *  @param        无
 *  @return       无  
 *  @note         
 */
void RingBuffer_Init(RingBuffer* rb)
{
    rb->head = 0;
    rb->tail = 0; 
    memset(rb->buffer,0,sizeof(rb->buffer));
}

// 清空环形缓冲区
void RingBuffer_Clear(RingBuffer* pRingBuffer)
{
    // 重置头指针到0
    pRingBuffer->head = 0;
    
    // 重置尾指针到0
    pRingBuffer->tail = 0;
    
    // 清除缓冲区中的数据
    memset(pRingBuffer->buffer, 0, sizeof(pRingBuffer->buffer));
}
// 获取缓冲区中可用的数据长度
uint16_t RingBuffer_AvailableData(RingBuffer* rb)
{
  return (rb->head - rb->tail) & RING_BUFFER_MASK;
}
// 获取缓冲区中的空闲空间长度
uint16_t RingBuffer_FreeSpace(RingBuffer* rb)
{
  return RING_BUFFER_SIZE - RingBuffer_AvailableData(rb);
}
// 单字节写入
uint8_t RingBuffer_Write(RingBuffer* rb, uint8_t data)
{
  uint16_t next_head = (rb->head + 1) & RING_BUFFER_MASK;
  if(next_head != rb->tail){
    rb->buffer[rb->head] = data;
    rb->head = next_head;
    return 1;
  }
  return 0;
}
// 单字节读取
uint8_t RingBuffer_Read(RingBuffer* rb, uint8_t* data)
{
  if(rb->head == rb->tail) return 0;

  *data = rb->buffer[rb->tail];
  rb->tail = (rb->tail + 1) & RING_BUFFER_MASK;
  return 1;
}
// 多字节写入
uint8_t RingBuffer_WriteMulti(RingBuffer* rb, uint8_t* data, uint16_t length)
{
 uint16_t available = (RING_BUFFER_SIZE - 1) - ((rb->head - rb->tail) & RING_BUFFER_MASK);
 if(length > available) length = available;

 uint16_t space_to_end = RING_BUFFER_SIZE - rb->head;
 if(space_to_end > length){
   memcpy(&rb->buffer[rb->head], data, length);
   rb->head = (rb->head + length) & RING_BUFFER_MASK;
 }else{
   memcpy(&rb->buffer[rb->head], data, space_to_end);
   memcpy(&rb->buffer, data + space_to_end, length - space_to_end);
   rb->head = length - space_to_end;
 }
return length;
}
// 多字节读取
uint8_t RingBuffer_ReadMulti(RingBuffer* rb, uint8_t* data, uint16_t length)
{
  uint16_t available = (rb->head - rb->tail) & RING_BUFFER_MASK;
  if(length > available) length = available;

  uint16_t space_to_end = RING_BUFFER_SIZE - rb->tail;
  if(space_to_end > length){
    memcpy(data, &rb->buffer[rb->tail], length);
    rb->tail = (rb->tail + length) & RING_BUFFER_MASK;
  }else{
    memcpy(data, &rb->buffer[rb->tail], space_to_end);
    memcpy(data + space_to_end, &rb->buffer, length - space_to_end);
    rb->tail = length - space_to_end;
  }
  return length;
}
// 检查缓冲区是否为空
uint8_t RingBuffer_IsEmpty(RingBuffer* rb)
{
  return rb->head == rb->tail;
}
// 检查缓冲区是否已满
uint8_t RingBuffer_IsFull(RingBuffer* rb)
{
  return ((rb->head + 1) & RING_BUFFER_MASK) == rb->tail;
}

// 查看缓冲区下一个字节的数据，但不移动读取指针
inline uint8_t RingBuffer_Peek(const RingBuffer* pRingBuff, uint8_t* data)
{
    // 检查缓冲区是否为空，如果读取指针等于写入指针，则缓冲区为空，函数返回0
    if (pRingBuff->head == pRingBuff->tail) return 0;
    
    // 从当前读取指针位置读取数据，但不移动读取指针
    *data = pRingBuff->buffer[pRingBuff->tail];
    
    // 返回成功读取
    return 1;
}