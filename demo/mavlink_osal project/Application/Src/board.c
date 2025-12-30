#include "board.h"

uart_instance_t uart2_instance = UART_INSTANCE_2;

uart_err_t bsp_uart_init(void)
{
    uart_err_t ret = uart_init(uart2_instance, 921600,UART_MODE_DMA);
    
     if (ret != UART_OK)
        {
            return ret;
        }

       
        uart_dma_start_rx(UART_INSTANCE_2, UART_DMA_MODE_CIRCULAR);
     return UART_OK;
}

uart_err_t bsp_uart_send(uint8_t *buff,uint16_t len)
{
    return uart_send(UART_INSTANCE_2, buff, len, NULL);
}
