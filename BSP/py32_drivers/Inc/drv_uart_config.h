#ifndef __DRV_UART_CONFIG_H__
#define __DRV_UART_CONFIG_H__

/* ========== 模式选择宏定义 ========== */

/* 工作模式选择 (必须三选一) */
#define UART_MODE_INTERRUPT 1 /* 中断模式 */
#define UART_MODE_DMA 2       /* DMA模式 */
#define UART_MODE_POLLING 3   /* 轮询模式 */

/* 选择工作模式 */
#ifndef UART_WORK_MODE
#define UART_WORK_MODE UART_MODE_INTERRUPT
#endif

/* 缓冲区配置 */
#define UART_RX_BUFFER_SIZE 256 /* 接收缓冲区大小 */
#define UART_TX_BUFFER_SIZE 256 /* 发送缓冲区大小 */

/* DMA配置 (仅在DMA模式下有效) */
#define UART_USE_DMA_IDLE_DETECTION 1 /* 使用DMA空闲中断检测 */
#define UART_DMA_RX_TIMEOUT_MS 10     /* DMA接收超时时间(ms) */

/* 中断优先级配置 */
#define UART_IRQ_PRIORITY 5     /* UART中断优先级 */
#define UART_DMA_IRQ_PRIORITY 4 /* DMA中断优先级 */

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

/* ========== 编译时检查 ========== */

/* 检查HAL库头文件 */
#ifndef HAL_UART_MODULE_ENABLED
#warning "HAL UART module not enabled. Please enable UART in HAL configuration."
#endif

#if (UART_USE_DMA == 1) && !defined(HAL_DMA_MODULE_ENABLED)
#warning "HAL DMA module not enabled. DMA mode may not work properly."
#endif

#endif /* __DRV_UART_CONFIG_H__ */