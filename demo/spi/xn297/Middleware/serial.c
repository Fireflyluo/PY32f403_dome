#include "serial.h"
#include "usart.h"
#include "kfifo.h"

uint8_t cmd_data[32];
uint8_t cmd_flag = 0;//默认透传模式

uint8_t usart1_rx_buffer[USART_DMA_RX_BUFFER_MAXIMUM]; //串口1的DMA接收缓冲区
uint8_t usart1_tx_buffer[USART_DMA_TX_BUFFER_MAXIMUM]; //串口1的DMA发送缓冲区
uint8_t usart1_rx_flag; //DMA接收成功标志 0，未接收到/1,接收到等待处理
uint16_t usart1_rx_len; //DMA一次空闲中断接收到的数据长度

uint8_t usart2_rx_buffer[USART_DMA_RX_BUFFER_MAXIMUM]; //串口2的DMA接收缓冲区
uint8_t usart2_tx_buffer[USART_DMA_TX_BUFFER_MAXIMUM]; //串口2的DMA发送缓冲区
uint8_t usart2_rx_flag; //DMA接收成功标志 0，未接收到/1,接收到等待处理
uint16_t usart2_rx_len; //DMA一次空闲中断接收到的数据长度

static uint8_t is_uart1_tx_dma_sending = 0;//UART1 DMA是否正在发送数据
static uint8_t is_uart2_tx_dma_sending = 0;//UART2 DMA是否正在发送数据
// 串口发送接收初始化
void serial_init(void)
{
    // 开始DMA接收
	  HAL_UART_Receive_DMA(&huart1, usart1_rx_buffer, USART_DMA_RX_BUFFER_MAXIMUM);
		// 开始DMA接收
	  HAL_UART_Receive_DMA(&huart2, usart2_rx_buffer, USART_DMA_RX_BUFFER_MAXIMUM);
	
}


/**
 * @brief : 串口1DMA发送
 * @param : 
 *          @buffer 发送数据包
 *          @length 长度  
 * @note  : 串口1的DMA发送
 * @retval: 无
 */
uint8_t UART1_TX_DMA_Send(uint8_t *buffer, uint16_t length)
{
	    //关闭DMA
    __HAL_DMA_DISABLE(&hdma_usart1_tx);

	 if(is_uart1_tx_dma_sending == 1){
        return RETURN_FAILURE;//DMA忙 等待发送完成 本次发送无效
    }else{
         //开始发送数据
    	HAL_UART_Transmit_DMA(&huart1, buffer, length);
        is_uart1_tx_dma_sending = 1;
        return RETURN_SUCCESS;//DMA空闲 开始发送
    }
}
/**
 * @brief : 串口2DMA发送
 * @param : 
 *          @buffer 发送数据包
 *          @length 长度  
 * @note  : 串口2的DMA发送
 * @retval: 无
 */
uint8_t UART2_TX_DMA_Send(uint8_t *buffer, uint16_t length)
{
	    //关闭DMA
    __HAL_DMA_DISABLE(&hdma_usart2_tx);

//	 if(is_uart2_tx_dma_sending == 1){
//        return RETURN_FAILURE;//DMA忙 等待发送完成 本次发送无效
//    }else{
	if (HAL_DMA_GetState(&hdma_usart2_tx) != HAL_DMA_STATE_READY) {
        return RETURN_FAILURE; // DMA不可用，返回失败
    }else{
         //开始发送数据
    	HAL_UART_Transmit_DMA(&huart2, buffer, length);
        is_uart2_tx_dma_sending = 1;
        return RETURN_SUCCESS;//DMA空闲 开始发送
    }
}

///**
// * @brief : 串口接收回调函数
// * @param : 串口句柄 
// * @note  : 无
// * @retval: 无
// */
//void HAL_UART_ReceiveIdle(UART_HandleTypeDef *huart)
//{
//	//当触发了串口空闲中断
//    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET)) 
//    {
//		uint32_t tmp_flag = 0;
//		uint32_t temp;
//			if(huart->Instance == USART2)
//			{
//				/* 1.清除标志 */
//				__HAL_UART_CLEAR_IDLEFLAG(huart); //清除空闲标志

//				/* 2.读取DMA */
//				HAL_UART_DMAStop(huart); //先停止DMA，暂停接收
//				//这里应注意数据接收不要大于 USART_DMA_RX_BUFFER_MAXIMUM
//				usart2_rx_len = USART_DMA_RX_BUFFER_MAXIMUM - (__HAL_DMA_GET_COUNTER(&hdma_usart2_rx)); //接收个数等于接收缓冲区总大小减剩余计数
//				
//				if(usart1_rx_buffer[usart2_rx_len - 1] == 0xEE  && usart2_rx_buffer[0] == 0xFF)//包头和包尾符合数据包结构
//				{
//					if(usart1_rx_buffer[2] == 0x01){//配置数据包
//						
//						memcpy(cmd_data, usart2_rx_buffer, usart2_rx_len); //拷贝到参数设置数据包
//					}
//					else if(usart1_rx_buffer[2] == 0x02){//数据包 
//						kfifo_put(com_fifo,usart1_rx_buffer, usart2_rx_len);//存入串口接收缓冲区
//					}
//					cmd_flag = 1;//切换为数据包模式
//				}
//				else{
//					/* 3.搬移数据进行其他处理 */
//					kfifo_put(com_fifo,usart2_rx_buffer, usart2_rx_len);//存入串口接收缓冲区
//					usart2_rx_flag = 1; //标志已经成功接收到一包等待处理
//				}
//				// 清零接收缓冲区
//				memset(usart2_rx_buffer,0,usart2_rx_len);                                            
//				usart2_rx_len = 0;
//				
//				// 重启开始DMA传输 每次255字节数据
//				HAL_UART_Receive_DMA(&huart1, (uint8_t*)usart2_rx_buffer, USART_DMA_RX_BUFFER_MAXIMUM);  
//			}
//    }
//}
///**
// * @brief : 串口发送回调函数
// * @param : 串口号
// * @note  : 无
// * @retval: 无
// */
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if (huart->Instance == USART1) //如果是串口1
//	{
//		// 在F7系列是可以不写的，F1必须写
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx, DMA_FLAG_TC1); //清除传输完成标志
//		HAL_UART_DMAStop(&huart1);		//传输完成以后关闭串口DMA,缺了这一句会死机
//		
//		is_uart1_tx_dma_sending = 0;//reset
//	}
//		if (huart->Instance == USART2) //如果是串口2
//	{
//		// 在F7系列是可以不写的，F1必须写
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_tx, DMA_FLAG_TC3); //清除传输完成标志
//		HAL_UART_DMAStop(&huart2);		//传输完成以后关闭串口DMA,缺了这一句会死机
//		
//		is_uart2_tx_dma_sending = 0;//reset
//	}
//}
