#include "rt_ringbuffer.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief 初始化环形缓冲区
 *
 * @param rb            指向环形缓冲区对象的指针
 * @param pool          指向缓冲区池的指针
 * @param size          缓冲区大小
 */
void rt_ringbuffer_init(rt_ringbuffer *rb, uint8_t *pool, size_t size) {
    if (rb == NULL || size == 0) {
        return;
    }

    // 初始化读写索引
    rb->read_mirror = rb->read_index = 0;
    rb->write_mirror = rb->write_index = 0;

    // 设置缓冲区池和大小
    rb->buffer_ptr = pool;
    rb->buffer_size = size - (size % 4); // 假设对齐大小为4
}

/**
 * @brief 将一块数据放入环形缓冲区。如果容量不足，将丢弃超出范围的数据。
 *
 * @param rb            指向环形缓冲区对象的指针
 * @param ptr           指向数据缓冲区的指针
 * @param length        数据大小（字节）
 *
 * @return 返回放入环形缓冲区的数据大小
 */
size_t rt_ringbuffer_put(rt_ringbuffer *rb, const uint8_t *ptr, size_t length) {
    size_t size;

    if (rb == NULL) {
        return 0;
    }

    // 是否有足够的空间
    size = rb->buffer_size - (rb->write_index - rb->read_index);
    if (rb->write_index < rb->read_index) {
        size = rb->read_index - rb->write_index;
    }

    // 没有空间
    if (size == 0) {
        return 0;
    }

    // 丢弃超出范围的数据
    if (size < length) {
        length = size;
    }

    if (rb->buffer_size - rb->write_index > length) {
        // read_index - write_index = 空闲空间
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        // 这不会导致溢出，因为在当前镜像中有足够的空间
        rb->write_index += length;
        return length;
    }

    memcpy(&rb->buffer_ptr[rb->write_index], &ptr[0], rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer_ptr[0], &ptr[rb->buffer_size - rb->write_index], length - (rb->buffer_size - rb->write_index));

  // 进入镜像区间
    rb->write_mirror = ~rb->write_mirror;
    rb->write_index = length - (rb->buffer_size - rb->write_index);

    return length;
}

/**
 * @brief 将一块数据放入环形缓冲区。如果容量不足，将覆盖环形缓冲区中的现有数据。
 *
 * @param rb            指向环形缓冲区对象的指针
 * @param ptr           指向数据缓冲区的指针
 * @param length        数据大小（字节）
 *
 * @return 返回放入环形缓冲区的数据大小
 */
size_t rt_ringbuffer_put_force(rt_ringbuffer *rb, const uint8_t *ptr, size_t length) {
    size_t space_length;

    if (rb == NULL) {
        return 0;
    }

    space_length = rb->buffer_size - (rb->write_index - rb->read_index);
    if (rb->write_index < rb->read_index) {
        space_length = rb->read_index - rb->write_index;
    }

    if (length > rb->buffer_size) {
        ptr = &ptr[length - rb->buffer_size];
        length = rb->buffer_size;
    }

    if (rb->buffer_size - rb->write_index > length) {
        // read_index - write_index = 空闲空间
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        // 这不会导致溢出，因为在当前镜像中有足够的空间
        rb->write_index += length;
        if (length > space_length) {
            rb->read_index = rb->write_index;
        }

        return length;
    }

    memcpy(&rb->buffer_ptr[rb->write_index], &ptr[0], rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer_ptr[0], &ptr[rb->buffer_size - rb->write_index], length - (rb->buffer_size - rb->write_index));

    // 进入镜像区间
    rb->write_mirror = ~rb->write_mirror;
    rb->write_index = length - (rb->buffer_size - rb->write_index);

    if (length > space_length) {
        if (rb->write_index <= rb->read_index) {
            rb->read_mirror = ~rb->read_mirror;
        }
        rb->read_index = rb->write_index;
    }

    return length;
}

/**
 * @brief 从环形缓冲区中获取数据。
 *
 * @param rb            指向环形缓冲区的指针
 * @param ptr           指向数据缓冲区的指针
 * @param length        想要从环形缓冲区中读取的数据大小
 *
 * @return 返回从环形缓冲区中读取的数据大小
 */
size_t rt_ringbuffer_get(rt_ringbuffer *rb, uint8_t *ptr, size_t length) {
    size_t size;

    if (rb == NULL) {
        return 0;
    }

    // 是否有足够的数据
    size = rb->buffer_size - (rb->write_index - rb->read_index);
    if (rb->write_index < rb->read_index) {
        size = rb->read_index - rb->write_index;
    }

    // 没有数据
    if (size == 0) {
        return 0;
    }

    // 数据不足
    if (size < length) {
        length = size;
    }

    if (rb->buffer_size - rb->read_index > length) {
        // 复制所有数据
        memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
        // 这不会导致溢出，因为在当前镜像中有足够的空间
        rb->read_index += length;
        return length;
    }

    memcpy(&ptr[0], &rb->buffer_ptr[rb->read_index], rb->buffer_size - rb->read_index);
    memcpy(&ptr[rb->buffer_size - rb->read_index], &rb->buffer_ptr[0], length - (rb->buffer_size - rb->read_index));

  // 进入镜像区间
    rb->read_mirror = ~rb->read_mirror;
    rb->read_index = length - (rb->buffer_size - rb->read_index);

    return length;
}

/**
 * @brief 获取环形缓冲区的状态
 * @param rb  指向环形缓冲区对象的指针
 * @return 返回环形缓冲区的状态
 * 在读写指针的值相同情况下，如果二者的指示位相同，说明缓冲区为空；如果二者的指示位不同，说明缓冲区为满。
 */
inline rt_ringbuffer_state rt_ringbuffer_status(rt_ringbuffer *rb) {
    if (rb == NULL) {
        return RT_RINGBUFFER_ERROR;
    }

    if (rb->read_index == rb->write_index) {
        if (rb->read_mirror == rb->write_mirror) {
            return RT_RINGBUFFER_EMPTY;
        } else {
            return RT_RINGBUFFER_FULL;
        }
    }
    return RT_RINGBUFFER_HALFFULL;
}
/************************************** */
#include "rt_ringbuffer.h"
#include <stdio.h>

int main() {
    // 定义缓冲区大小
    size_t buffer_size = 16;
    // 分配缓冲区内存
    uint8_t buffer_pool[buffer_size];
    // 定义环形缓冲区对象
    rt_ringbuffer rb;

    // 初始化环形缓冲区
    rt_ringbuffer_init(&rb, buffer_pool, buffer_size);

    // 要写入的数据
    uint8_t data_to_write[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t data_length = sizeof(data_to_write) / sizeof(data_to_write[0]);

    // 写入数据到环形缓冲区
    size_t bytes_written = rt_ringbuffer_put(&rb, data_to_write, data_length);
    printf("Bytes written: %zu\n", bytes_written);

    // 读取数据缓冲区
    uint8_t data_read[buffer_size];
    size_t bytes_read = rt_ringbuffer_get(&rb, data_read, data_length);
    printf("Bytes read: %zu\n", bytes_read);

    // 打印读取的数据
    printf("Data read from ring buffer: ");
    for (size_t i = 0; i < bytes_read; i++) {
        printf("%d ", data_read[i]);
    }
    printf("\n");

    return 0;
}