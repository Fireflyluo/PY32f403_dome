/*-----------------------------------------------File Info------------------------------------------------
** File Name:               bsp_uasrt.h  
** Last modified date:      2025.7.1
** Last version:            V0.1
** Description:             UART驱动接口声明
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            定义UART编号、DMA模式枚举；
**                          声明UART初始化、发送/接收函数；
**                          支持DMA双缓冲接收机制。
**--------------------------------------------------------------------------------------------------------*/
#ifndef __BSP_UASRT_H__
#define __BSP_UASRT_H__


#include "bsp_common.h"

#ifndef UART_RX_BUFFER_SIZE
#define UART_RX_BUFFER_SIZE     256   // 静态/动态缓冲区大小
#endif

/* 串口编号枚举 */
typedef enum {
    UART_NUM_1 = 0,         // 对应USART1
    UART_NUM_2,             // 对应USART2
    UART_NUM_3,             // 对应USART3
    UART_NUM_MAX            // 最大串口数（防止越界）
} UART_Num_TypeDef;


/* DMA模式枚举 */
typedef enum {
    UART_MODE_POLLING,				//轮询模式
	UART_MODE_INTERRUPT,			//中断模式
	UART_MODE_DMA					//DMA模式
} UART_Mode_TypeDef;

/* 串口配置结构体 */
typedef struct {
    uint32_t BaudRate;      // 波特率
    uint8_t WordLength;     // 数据位（如UART_WORDLENGTH_8B）
    uint8_t StopBits;       // 停止位（如UART_STOPBITS_1）
    uint8_t Parity;         // 校验位（如UART_PARITY_NONE）
    uint8_t Mode;           // 模式（UART_MODE_TX_RX）
    UART_HandleTypeDef* Instance; //指定具体硬件实例
	UART_Mode_TypeDef dma_mode;	//DMA工作模式
} UART_Config_TypeDef;

/* 函数返回值枚举 */
typedef enum {
    BSP_UART_OK = 0,           // UART操作成功
    BSP_UART_ERR_INVALID_NUM,  // 无效的UART编号
    BSP_UART_ERR_NULL_PTR,     // 空指针错误
    BSP_UART_ERR_HAL_FAIL,     // HAL库调用失败
    BSP_UART_ERR_DMA_BUSY      // DMA繁忙
} BSP_UART_Status_TypeDef;

/* DMA相关状态变量 */
typedef struct {
    UART_HandleTypeDef *huart;          // UART句柄
    DMA_HandleTypeDef *hdma_tx;         // TX DMA句柄
    DMA_HandleTypeDef *hdma_rx;         // RX DMA句柄
    uint8_t *dma_tx_buf;                // DMA发送缓冲区（可选）
    uint8_t *dma_rx_buf[2];             // 双缓冲区
    uint8_t current_rx_buf;             // 当前使用的缓冲区索引
    uint16_t dma_tx_size;               // DMA发送数据长度
    uint16_t dma_rx_size;               // DMA接收数据长度
		bsp_dma_status_t dma_status;		// DMA传输状态
} UART_DMA_Handle_TypeDef;

/* 每个串口的DMA句柄数组（需根据实际硬件配置） */
extern UART_DMA_Handle_TypeDef UART_DMA_Handle[UART_NUM_MAX];

/* 函数声明 */
BSP_UART_Status_TypeDef BSP_UART_Init(UART_Num_TypeDef uart_num, UART_Config_TypeDef *config, UART_Mode_TypeDef mode);
BSP_UART_Status_TypeDef BSP_UART_DeInit(UART_Num_TypeDef uart_num);
BSP_UART_Status_TypeDef BSP_UART_Transmit(UART_Num_TypeDef uart_num, uint8_t *data, uint16_t size, uint32_t timeout);
BSP_UART_Status_TypeDef BSP_UART_Receive(UART_Num_TypeDef uart_num, uint8_t *data, uint16_t size, uint32_t timeout);
BSP_UART_Status_TypeDef BSP_UART_Transmit_IT(UART_Num_TypeDef uart_num, uint8_t *data, uint16_t size);
BSP_UART_Status_TypeDef BSP_UART_Receive_IT(UART_Num_TypeDef uart_num, uint8_t *data, uint16_t size);
BSP_UART_Status_TypeDef BSP_UART_Transmit_DMA(UART_Num_TypeDef uart_num, uint8_t *data, uint16_t size);
BSP_UART_Status_TypeDef BSP_UART_Receive_DMA(UART_Num_TypeDef uart_num, uint8_t *data, uint16_t size);
BSP_UART_Status_TypeDef BSP_UART_Abor_DMA(UART_Num_TypeDef uart_num);

void BSP_UART_RegisterCallback(UART_Num_TypeDef uart_num, 
                               void (*tx_callback)(UART_Num_TypeDef), 
                               void (*rx_callback)(UART_Num_TypeDef, uint8_t*, uint16_t));
void BSP_UART_ReceiveIdle(UART_HandleTypeDef *huart);

#endif /* __BSP_UART_H__ */