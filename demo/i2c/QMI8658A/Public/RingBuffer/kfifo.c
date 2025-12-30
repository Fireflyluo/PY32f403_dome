#include "kfifo.h"
#include <stdlib.h>
#include <string.h>

// 判断是否为2的幂
static inline int is_power_of_2(unsigned int n) {
    return (n != 0) && ((n & (n - 1)) == 0);
}

// 向上扩展成2的幂
static inline unsigned int roundup_pow_of_two(unsigned int n) {
    if (n == 0)
        return 1;
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;
    return n;
}

// 禁用中断实现的自旋锁
static inline void lock(struct kfifo *fifo) {
    __disable_irq(); // 关闭中断，防止中断抢占导致的并发问题
	
}

// 解锁并恢复中断
static inline void unlock(struct kfifo *fifo) {
    __enable_irq(); // 重新开启中断
}

// 计算最小值
static inline unsigned int min(unsigned int a, unsigned int b) {
    return (a < b) ? a : b;
}

/**
 * @brief 初始化 kfifo
 * @param buffer 缓冲区地址
 * @param size 缓冲区大小，必须是2的幂
 * @param gfp_mask 内存分配标志（未使用）
 * @param lock 自旋锁指针
 * @return 初始化后的 kfifo 指针，失败返回 NULL
 */
struct kfifo *kfifo_init(unsigned char *buffer, unsigned int size, int gfp_mask, spinlock_t *lock) {
    struct kfifo *fifo;

    // 检查 size 是否为 2 的幂
    if (!is_power_of_2(size)) {
        return NULL;
    }

    // 分配 kfifo 内存
    fifo = (struct kfifo *)malloc(sizeof(struct kfifo));
    if (!fifo) {
        // fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // 初始化 kfifo 字段
    fifo->buffer = buffer;
    fifo->size = size;
    fifo->in = fifo->out = 0;
    fifo->lock = lock;

    return fifo;
}

/**
 * @brief 分配并初始化 kfifo
 * @param size 缓冲区大小
 * @param lock 自旋锁指针
 * @return 初始化后的 kfifo 指针，失败返回 NULL
 */
struct kfifo *kfifo_alloc(unsigned int size, spinlock_t *lock) {
    unsigned char *buffer;
    struct kfifo *ret;

    // 判断是否为2的幂
    if (!is_power_of_2(size)) {
        if (size > 0x80000000) {
            return NULL;
        }
        // 向上扩展成2的幂
        size = roundup_pow_of_two(size);
    }

    // 分配缓冲区内存
    buffer = (unsigned char *)malloc(size);
    if (!buffer) {
        return NULL;
    }

    // 初始化 kfifo
    ret = kfifo_init(buffer, size, 0, lock);
    if (!ret) {
        free(buffer);
        return NULL;
    }

    return ret;
}


/**
 * @brief 向 kfifo 中放入数据
 * @param fifo kfifo 指针
 * @param buffer 数据缓冲区
 * @param len 数据长度
 * @return 实际放入的数据长度
 */
static unsigned int __kfifo_put(struct kfifo *fifo, const unsigned char *buffer, unsigned int len) {
    unsigned int l; // buffer 中空的长度
    len = min(len, fifo->size - fifo->in + fifo->out);

    // 将数据追加到队列尾部
    l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));
    memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);
    memcpy(fifo->buffer, buffer + l, len - l);

    // 每次累加，到达最大值后溢出，自动转为0
    fifo->in += len;
    return len;
}

/**
 * @brief 向 kfifo 中放入数据（带锁）
 * @param fifo kfifo 指针
 * @param buffer 数据缓冲区
 * @param len 数据长度
 * @return 实际放入的数据长度
 */
inline unsigned int kfifo_put(struct kfifo *fifo, const unsigned char *buffer, unsigned int len) {
    lock(fifo);
    unsigned int ret = __kfifo_put(fifo, buffer, len);
    unlock(fifo);
    return ret;
}

/**
 * @brief 从 kfifo 中获取数据
 * @param fifo kfifo 指针
 * @param buffer 数据缓冲区
 * @param len 数据长度
 * @return 实际获取的数据长度
 */
static unsigned int __kfifo_get(struct kfifo *fifo, unsigned char *buffer, unsigned int len) {
    unsigned int l;
    // 有数据的缓冲区的长度
    len = min(len, fifo->in - fifo->out);
    // 从 fifo->out 开始读取到缓冲区末尾的数据
    l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));
    memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);
    // 读取剩余的数据（如果有）从缓冲区的开头
    memcpy(buffer + l, fifo->buffer, len - l);

    fifo->out += len; // 每次累加，到达最大值后溢出，自动转为0
    return len;
}

/**
 * @brief 从 kfifo 中获取数据（带锁）
 * @param fifo kfifo 指针
 * @param buffer 数据缓冲区
 * @param len 数据长度
 * @return 实际获取的数据长度
 */
inline unsigned int kfifo_get(struct kfifo *fifo, unsigned char *buffer, unsigned int len) {
  lock(fifo);
  unsigned int ret = __kfifo_get(fifo, buffer, len);
  // 当 fifo->in == fifo->out 时，buffer 为空
  if (fifo->in == fifo->out) {
      fifo->in = fifo->out = 0;
  }
  unlock(fifo);
  return ret;
}

/**
 * @brief 获取 kfifo 中的数据长度
 * @param fifo kfifo 指针
 * @return 当前 kfifo 中的数据长度
 */
static inline unsigned int __kfifo_len(struct kfifo *fifo) {
    return fifo->in - fifo->out;
}

/**
 * @brief 获取 kfifo 中的数据长度（带锁）
 * @param fifo kfifo 指针
 * @return 当前 kfifo 中的数据长度
 */
unsigned int kfifo_len(struct kfifo *fifo) {
    lock(fifo);
    unsigned int ret = __kfifo_len(fifo);
    unlock(fifo);
    return ret;
}

/**
 * @brief 重置 kfifo
 * @param fifo kfifo 指针
 */
static inline void __kfifo_reset(struct kfifo *fifo) {
    fifo->in = fifo->out = 0;
}

/**
 * @brief 重置 kfifo（带锁）
 * @param fifo kfifo 指针
 */
inline void kfifo_reset(struct kfifo *fifo) {
    lock(fifo);
    __kfifo_reset(fifo);
    unlock(fifo);
}

/**
 * @brief 释放 kfifo
 * @param fifo kfifo 指针
 */
void kfifo_free(struct kfifo *fifo) {
    if (fifo) {
        free(fifo->buffer);
        free(fifo);
    }
}

/**
 * @brief 获取环形缓冲区的状态
 * @param fifo  指向环形缓冲区对象的指针
 * @return 返回环形缓冲区的状态
 * 如果缓冲区已存储的数据长度为零，说明缓冲区为空；如果其值等于缓冲区大小，说明缓冲区满；否则说明缓冲区为半满。
 */
inline kfifo_state kfifo_status(struct kfifo *fifo) {
    if (fifo == NULL) {
        return KFIFO_ERROR;
    }

    unsigned int len = __kfifo_len(fifo);

    if (len == 0) {
        return KFIFO_EMPTY;
    } else if (len == fifo->size) {
        return KFIFO_FULL;
    } else {
        return KFIFO_HALFFULL;
    }
}
/*********************************示例****************************************** */

#include "kfifo.h"
#include <stdio.h>

int test() {
    spinlock_t lock = 0;
    struct kfifo *fifo = kfifo_alloc(16, &lock); // 分配一个大小为16的kfifo

    if (!fifo) {
        printf("Failed to allocate kfifo\n");
        return 1;
    }

    unsigned char data_to_put[] = "Hello, KFIFO!";
    unsigned int len_put = kfifo_put(fifo, data_to_put, sizeof(data_to_put) - 1);

    printf("Put %u bytes into kfifo\n", len_put);

    unsigned char data_to_get[32];
    unsigned int len_get = kfifo_get(fifo, data_to_get, sizeof(data_to_get) - 1);

    data_to_get[len_get] = '\0'; // 添加字符串结束符
    printf("Got %u bytes from kfifo: %s\n", len_get, data_to_get);

    kfifo_free(fifo); // 释放kfifo

    return 0;
}

