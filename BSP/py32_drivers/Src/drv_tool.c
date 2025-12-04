#include "drv_tool.h"
#include <string.h>

/* ========================= 环形缓冲区工具函数 =================================================== */
/**
 * @brief 初始化环形缓冲区
 * @param rb: 环形缓冲区指针
 * @param buffer: 存储缓冲区指针
 * @param size: 缓冲区大小
 * @return true: 成功, false: 失败
 */
bool ring_buffer_init(ring_buffer_t *rb, uint8_t *buffer, uint16_t size)
{
    if (rb == NULL || buffer == NULL || size == 0)
    {
        return false;
    }

    rb->buffer = buffer;
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;

    return true;
}

/**
 * @brief 向环形缓冲区放入一个字节
 * @param rb: 环形缓冲区指针
 * @param data: 要放入的数据
 * @return true: 成功, false: 缓冲区已满
 */
bool ring_buffer_put(ring_buffer_t *rb, uint8_t data)
{
    if (rb == NULL || ring_buffer_is_full(rb))
    {
        return false;
    }

    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) % rb->size;
    rb->count++;

    return true;
}

/**
 * @brief 从环形缓冲区取出一个字节
 * @param rb: 环形缓冲区指针
 * @param data: 存储取出的数据
 * @return true: 成功, false: 缓冲区为空
 */
bool ring_buffer_get(ring_buffer_t *rb, uint8_t *data)
{
    if (rb == NULL || data == NULL || ring_buffer_is_empty(rb))
    {
        return false;
    }

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->size;
    rb->count--;

    return true;
}

/**
 * @brief 向环形缓冲区放入多个字节
 * @param rb: 环形缓冲区指针
 * @param data: 数据指针
 * @param size: 数据大小
 * @return 实际放入的字节数
 */
uint16_t ring_buffer_put_multiple(ring_buffer_t *rb, const uint8_t *data, uint16_t size)
{
    if (rb == NULL || data == NULL || size == 0)
    {
        return 0;
    }

    uint16_t free_space = ring_buffer_free_space(rb);
    uint16_t bytes_to_write = (size > free_space) ? free_space : size;

    for (uint16_t i = 0; i < bytes_to_write; i++)
    {
        rb->buffer[rb->head] = data[i];
        rb->head = (rb->head + 1) % rb->size;
    }

    rb->count += bytes_to_write;
    return bytes_to_write;
}

/**
 * @brief 从环形缓冲区取出多个字节
 * @param rb: 环形缓冲区指针
 * @param data: 存储数据的指针
 * @param size: 要取出的字节数
 * @return 实际取出的字节数
 */
uint16_t ring_buffer_get_multiple(ring_buffer_t *rb, uint8_t *data, uint16_t size)
{
    if (rb == NULL || data == NULL || size == 0)
    {
        return 0;
    }

    uint16_t available = ring_buffer_available(rb);
    uint16_t bytes_to_read = (size > available) ? available : size;

    for (uint16_t i = 0; i < bytes_to_read; i++)
    {
        data[i] = rb->buffer[rb->tail];
        rb->tail = (rb->tail + 1) % rb->size;
    }

    rb->count -= bytes_to_read;
    return bytes_to_read;
}

/**
 * @brief 查看环形缓冲区中的数据（不移除）
 * @param rb: 环形缓冲区指针
 * @param data: 存储数据的指针
 * @param size: 要查看的字节数
 * @param offset: 偏移量
 * @return 实际查看的字节数
 */
uint16_t ring_buffer_peek_multiple(const ring_buffer_t *rb, uint8_t *data, uint16_t size, uint16_t offset)
{
    if (rb == NULL || data == NULL || size == 0)
    {
        return 0;
    }

    uint16_t available = ring_buffer_available(rb);
    if (offset >= available)
    {
        return 0;
    }

    uint16_t bytes_to_peek = (size > (available - offset)) ? (available - offset) : size;

    for (uint16_t i = 0; i < bytes_to_peek; i++)
    {
        uint16_t index = (rb->tail + offset + i) % rb->size;
        data[i] = rb->buffer[index];
    }

    return bytes_to_peek;
}

/**
 * @brief 跳过指定数量的字节
 * @param rb: 环形缓冲区指针
 * @param size: 要跳过的字节数
 */
void ring_buffer_skip(ring_buffer_t *rb, uint16_t size)
{
    if (rb == NULL || size == 0)
    {
        return;
    }

    uint16_t available = ring_buffer_available(rb);
    uint16_t bytes_to_skip = (size > available) ? available : size;

    rb->tail = (rb->tail + bytes_to_skip) % rb->size;
    rb->count -= bytes_to_skip;
}

/**
 * @brief 获取环形缓冲区中可用的字节数
 * @param rb: 环形缓冲区指针
 * @return 可用字节数
 */
uint16_t ring_buffer_available(const ring_buffer_t *rb)
{
    return (rb != NULL) ? rb->count : 0;
}

/**
 * @brief 获取环形缓冲区剩余空间
 * @param rb: 环形缓冲区指针
 * @return 剩余空间字节数
 */
uint16_t ring_buffer_free_space(const ring_buffer_t *rb)
{
    return (rb != NULL) ? (rb->size - rb->count) : 0;
}

/**
 * @brief 检查环形缓冲区是否为空
 * @param rb: 环形缓冲区指针
 * @return true: 空, false: 非空
 */
bool ring_buffer_is_empty(const ring_buffer_t *rb)
{
    return (rb == NULL) ? true : (rb->count == 0);
}

/**
 * @brief 检查环形缓冲区是否已满
 * @param rb: 环形缓冲区指针
 * @return true: 满, false: 未满
 */
bool ring_buffer_is_full(const ring_buffer_t *rb)
{
    return (rb == NULL) ? true : (rb->count >= rb->size);
}

/**
 * @brief 清空环形缓冲区
 * @param rb: 环形缓冲区指针
 */
void ring_buffer_clear(ring_buffer_t *rb)
{
    if (rb != NULL)
    {
        rb->head = 0;
        rb->tail = 0;
        rb->count = 0;
    }
}