#ifndef RT_RINGBUFFER_H
#define RT_RINGBUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

// 定义环形缓冲区的状态枚举
typedef enum {
    RT_RINGBUFFER_EMPTY,      // 环形缓冲区状态码：空
    RT_RINGBUFFER_FULL,       // 环形缓冲区状态码：满
    RT_RINGBUFFER_HALFFULL,   // 环形缓冲区状态码：半满
    RT_RINGBUFFER_ERROR       // 环形缓冲区状态码：错误
} rt_ringbuffer_state;

// 定义环形缓冲区结构体
typedef struct {
    uint8_t *buffer_ptr;        // 缓冲区指针
    size_t buffer_size;         // 缓冲区大小
    size_t read_index;          // 读索引
    size_t write_index;         // 写索引
    uint8_t read_mirror;        // 读镜像标志
    uint8_t write_mirror;       // 写镜像标志
} rt_ringbuffer;

// 初始化环形缓冲区
void rt_ringbuffer_init(rt_ringbuffer *rb, uint8_t *pool, size_t size);
//环形缓冲区写操作，容量不足时丢弃超出范围数据
size_t rt_ringbuffer_put(rt_ringbuffer *rb, const uint8_t *ptr, size_t length);
//形缓冲区写操作，容量不足时覆盖已有数据
size_t rt_ringbuffer_put_force(rt_ringbuffer *rb, const uint8_t *ptr, size_t length);
// 从环形缓冲区中获取数据
size_t rt_ringbuffer_get(rt_ringbuffer *rb, uint8_t *ptr, size_t length);
// 获取环形缓冲区的状态
inline rt_ringbuffer_state rt_ringbuffer_status(rt_ringbuffer *rb);

#endif // RT_RINGBUFFER_H