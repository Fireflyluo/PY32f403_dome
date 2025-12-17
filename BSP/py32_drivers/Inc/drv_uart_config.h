#ifndef __DRV_UART_CONFIG_H__
#define __DRV_UART_CONFIG_H__
#include "main.h"

/* ========== 模式选择宏定义 ========== */

/* 工作模式选择 (必须三选一) */
#define UART_MODE_INTERRUPT 1 /* 中断模式 */
#define UART_MODE_DMA 2       /* DMA模式 */
#define UART_MODE_POLLING 3   /* 轮询模式 */

/* 选择工作模式 */
#ifndef UART_WORK_MODE
#define UART_WORK_MODE UART_MODE_DMA /* 默认模式 */
#endif

/* 缓冲区配置 */
#define RING_BUFFER_ENABLED
#define UART_RX_BUFFER_SIZE 256  /* 接收缓冲区大小 */
#define UART_TX_BUFFER_SIZE 256  /* 发送缓冲区大小 */
#define UART_DMA_BUFFER_SIZE 256 /* DMA缓冲区大小，建议为2的幂次方 */

/* DMA配置 (仅在DMA模式下有效) */
#define UART_USE_DMA_IDLE_DETECTION 1                /* 使用串口空闲中断检测 */
#define UART_DMA_RX_TIMEOUT_MS 10                    /* DMA接收超时时间(ms) */
#define UART_DMA_DEFAULT_MODE UART_DMA_MODE_CIRCULAR /* 默认DMA模式 */

/* 中断优先级配置 */
#define UART_IRQ_PRIORITY 1         /* UART中断优先级 */
#define UART_IRQ_SUB_PRIORITY 0     /* UART中断子优先级 */
#define UART_DMA_IRQ_PRIORITY 4     /* DMA中断优先级 */
#define UART_DMA_IRQ_SUB_PRIORITY 0 /* DMA中断子优先级 */

/* 调试输出 */
#define UART_DEBUG_ENABLE 1 /* 启用调试信息 */

/* 超时配置 */
#define UART_DEFAULT_TIMEOUT_MS 1000 /* 默认超时时间 */

/* ========== 根据模式选择自动配置 ========== */

#if (UART_WORK_MODE == UART_MODE_INTERRUPT)
#define UART_USE_INTERRUPT 1
#define UART_USE_DMA 0
#define UART_USE_POLLING 0

#elif (UART_WORK_MODE == UART_MODE_DMA)
#define UART_USE_INTERRUPT 0
#define UART_USE_DMA 1
#define UART_USE_POLLING 0

#elif (UART_WORK_MODE == UART_MODE_POLLING)
#define UART_USE_INTERRUPT 0
#define UART_USE_DMA 0
#define UART_USE_POLLING 1

#else
#error "Invalid UART_WORK_MODE! Please choose UART_MODE_INTERRUPT, UART_MODE_DMA or UART_MODE_POLLING"
#endif

/* ========== 硬件相关配置 ========== */

/* UART实例对应的DMA通道映射 */
#if defined(STM32F4xx)
#define UART1_TX_DMA_STREAM DMA2_Stream7
#define UART1_RX_DMA_STREAM DMA2_Stream2
#define UART2_TX_DMA_STREAM DMA1_Stream6
#define UART2_RX_DMA_STREAM DMA1_Stream5
// 其他UART实例的DMA映射...
#elif defined(PY32F403xD)
#define UART1_TX_DMA_CHANNEL        NULL
#define UART1_RX_DMA_CHANNEL        NULL

#define UART2_TX_DMA_CHANNEL        DMA1_Channel1
#define UART2_RX_DMA_CHANNEL        DMA1_Channel2

#define UART3_TX_DMA_CHANNEL        NULL
#define UART3_RX_DMA_CHANNEL        NULL

#define UART4_TX_DMA_CHANNEL        NULL
#define UART4_RX_DMA_CHANNEL        NULL

#define UART5_TX_DMA_CHANNEL        NULL
#define UART5_RX_DMA_CHANNEL        NULL
// 其他UART实例的DMA映射...
#endif



/* ========== 编译时检查 ========== */

/* 检查HAL库头文件 */
#ifndef HAL_UART_MODULE_ENABLED
#warning "HAL UART module not enabled. Please enable UART in HAL configuration."
#endif

#if (UART_USE_DMA == 1) && !defined(HAL_DMA_MODULE_ENABLED)
#warning "HAL DMA module not enabled. DMA mode may not work properly."
#endif

/* 检查环形缓冲区支持 */
#ifndef RING_BUFFER_ENABLED
#warning "Ring buffer support not enabled. Please include drv_tool.h"
#endif

#endif /* __DRV_UART_CONFIG_H__ */