#ifndef __UART_EXAMPLE_H__
#define __UART_EXAMPLE_H__

#ifdef __cplusplus
extern "C"
{
#endif
#define UART_EXAMPLE_MODE UART_EXAMPLE_POLLING

#define UART_EXAMPLE_POLLING 1
#define UART_EXAMPLE_INTERRUPT 2
#define UART_EXAMPLE_DMA 3

    int uart_polling_example(void);
    int uart_it_example(void);
    int uart_dma_example(void);
    void uart_echo_test(void);
#ifdef __cplusplus
}
#endif
#endif /* __UART_EXAMPLE_H__ */