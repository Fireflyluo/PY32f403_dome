#ifndef __DRV_TOOL_H__
#define __DRV_TOOL_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* ========================= 环形缓冲区工具函数 =================================================== */
    /* 环形缓冲区结构 */
    typedef struct
    {
        uint8_t *buffer;         // 缓冲区指针
        uint16_t size;           // 缓冲区总大小
        volatile uint16_t head;  // 写入位置
        volatile uint16_t tail;  // 读取位置
        volatile uint16_t count; // 当前数据量
    } ring_buffer_t;

    /* 环形缓冲区API */
    bool ring_buffer_init(ring_buffer_t *rb, uint8_t *buffer, uint16_t size);
    bool ring_buffer_put(ring_buffer_t *rb, uint8_t data);
    bool ring_buffer_get(ring_buffer_t *rb, uint8_t *data);
    uint16_t ring_buffer_put_multiple(ring_buffer_t *rb, const uint8_t *data, uint16_t size);
    uint16_t ring_buffer_get_multiple(ring_buffer_t *rb, uint8_t *data, uint16_t size);
    uint16_t ring_buffer_peek_multiple(const ring_buffer_t *rb, uint8_t *data, uint16_t size, uint16_t offset);
    void ring_buffer_skip(ring_buffer_t *rb, uint16_t size);
    uint16_t ring_buffer_available(const ring_buffer_t *rb);
    uint16_t ring_buffer_free_space(const ring_buffer_t *rb);
    bool ring_buffer_is_empty(const ring_buffer_t *rb);
    bool ring_buffer_is_full(const ring_buffer_t *rb);
    void ring_buffer_clear(ring_buffer_t *rb);
    /* =========================  =================================================== */

#ifdef __cplusplus
}
#endif

#endif /* __DRV_TOOL_H__ */