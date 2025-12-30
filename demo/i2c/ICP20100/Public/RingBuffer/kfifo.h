#ifndef __KFIFO_H_
#define __KFIFO_H_

#include "main.h"
#include <string.h>
#include "py32f4xx_hal.h"
// 定义自旋锁类型
//typedef uint32_t spinlock_t;
#define SPIN_LOCK_UNLOCKED 0
#define SPIN_LOCK_LOCKED 	 1


struct kfifo {
    unsigned char *buffer;
    unsigned int size; 
    unsigned int in;   // 写指针
    unsigned int out;  // 读指针
    spinlock_t *lock;  // 自旋锁指针
};
// 定义环形缓冲区的状态枚举
typedef enum {
  KFIFO_EMPTY,      // 环形缓冲区状态码：空
  KFIFO_FULL,       // 环形缓冲区状态码：满
  KFIFO_HALFFULL,   // 环形缓冲区状态码：半满
  KFIFO_ERROR       // 环形缓冲区状态码：错误
} kfifo_state;
// 初始化 kfifo
struct kfifo *kfifo_init(unsigned char *buffer, unsigned int size, int gfp_mask, spinlock_t *lock);
// 分配并初始化 kfifo
struct kfifo *kfifo_alloc(unsigned int size, spinlock_t *lock);
// 向 kfifo 中放入数据
unsigned int kfifo_put(struct kfifo *fifo, const unsigned char *buffer, unsigned int len);
// 从 kfifo 中获取数据
unsigned int kfifo_get(struct kfifo *fifo, unsigned char *buffer, unsigned int len);
// 获取 kfifo 中的数据长度
unsigned int kfifo_len(struct kfifo *fifo);

// 重置 kfifo
void kfifo_reset(struct kfifo *fifo);
// 释放 kfifo
void kfifo_free(struct kfifo *fifo);
// 获取环形缓冲区的状态
kfifo_state kfifo_status(struct kfifo *fifo);

#endif /* __KFIFO_H_ */