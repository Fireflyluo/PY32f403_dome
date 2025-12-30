#ifndef __BOARD_H__
#define __BOARD_H__

#include "drv_include.h"



uart_err_t bsp_uart_init(void);
uart_err_t bsp_uart_send(uint8_t *buff,uint16_t len);





#endif
