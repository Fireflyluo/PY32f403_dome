#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "bsp_common.h"

/* 定义DMA缓冲区大小 */
#ifndef SPI_RX_BUFFER_SIZE
#define SPI_RX_BUFFER_SIZE    256
#endif

/* SPI编号枚举 */
typedef enum {
    SPI_NUM_1 = 0,
    SPI_NUM_2,
    SPI_NUM_3,
    SPI_NUM_MAX
} SPI_Num_TypeDef;

/* DMA模式枚举 */
typedef enum {
    SPI_MODE_POLLING,
    SPI_MODE_INTERRUPT,
    SPI_MODE_DMA
} SPI_Mode_TypeDef;



/**
 * @brief SPI配置类型定义
 */
typedef struct {
    uint32_t BaudRatePrescaler;   /**< 波特率预分频值 */
    uint32_t Direction;           /**< 通信方向 */
    uint32_t CLKPolarity;         /**< 时钟极性 */
    uint32_t CLKPhase;            /**< 时钟相位 */
    uint32_t FirstBit;            /**< 首位传输的位 */
    uint32_t Mode;                /**< SPI模式（主/从） */
    SPI_Mode_TypeDef dma_mode;    /**< DMA模式选择 */
    SPI_HandleTypeDef* Instance; /**< 指向SPI外设实例的指针 */
} SPI_Config_TypeDef;

/* 返回值类型 */
typedef enum {
    BSP_SPI_OK = 0,               /**< 操作成功 */
    BSP_SPI_ERR_INVALID_NUM,      /**< 无效的SPI编号 */
    BSP_SPI_ERR_NULL_PTR,         /**< 空指针错误 */
    BSP_SPI_ERR_HAL_FAIL,         /**< HAL层操作失败 */
    BSP_SPI_ERR_MASTER_RX         /**< 主机模式下无效的接收操作 */
} BSP_SPI_Status_TypeDef;

/* DMA相关句柄结构体 */
typedef struct {
    SPI_HandleTypeDef *hspi;      /**< 指向SPI外设实例的指针 */
    DMA_HandleTypeDef *hdma_tx;   /**< 发送DMA通道句柄 */
    DMA_HandleTypeDef *hdma_rx;   /**< 接收DMA通道句柄 */
    uint8_t *dma_tx_buf;          /**< 发送DMA缓冲区 */
    uint8_t *dma_rx_buf;          /**< 接收DMA缓冲区 */
    uint16_t dma_tx_size;         /**< 发送DMA数据大小 */
    uint16_t dma_rx_size;         /**< 接收DMA数据大小 */
    bsp_dma_status_t dma_status;  /**< DMA当前状态 */
} SPI_DMA_Handle_TypeDef;

/* 每个SPI的DMA句柄数组 */
extern SPI_DMA_Handle_TypeDef SPI_DMA_Handle[SPI_NUM_MAX];

/* 函数声明 */
BSP_SPI_Status_TypeDef BSP_SPI_Init(SPI_Num_TypeDef spi_num, SPI_Config_TypeDef *config);
BSP_SPI_Status_TypeDef BSP_SPI_DeInit(SPI_Num_TypeDef spi_num);

BSP_SPI_Status_TypeDef BSP_SPI_Transmit(SPI_Num_TypeDef spi_num, uint8_t *txData, uint16_t size, uint32_t timeout);
BSP_SPI_Status_TypeDef BSP_SPI_Receive(SPI_Num_TypeDef spi_num, uint8_t *rxData, uint16_t size, uint32_t timeout);
BSP_SPI_Status_TypeDef BSP_SPI_Transmit_IT(SPI_Num_TypeDef spi_num, uint8_t *txData, uint16_t size);
BSP_SPI_Status_TypeDef BSP_SPI_Receive_IT(SPI_Num_TypeDef spi_num, uint8_t *rxData, uint16_t size);
BSP_SPI_Status_TypeDef BSP_SPI_Transmit_DMA(SPI_Num_TypeDef spi_num, uint8_t *txData, uint16_t size);
BSP_SPI_Status_TypeDef BSP_SPI_Receive_DMA(SPI_Num_TypeDef spi_num, uint8_t *rxData, uint16_t size);
BSP_SPI_Status_TypeDef BSP_SPI_TransmitReceive(SPI_Num_TypeDef spi_num,
                                              uint8_t *txData,
                                              uint8_t *rxData,
                                              uint16_t size,
                                              uint32_t timeout);
BSP_SPI_Status_TypeDef BSP_SPI_TransmitReceive_DMA(SPI_Num_TypeDef spi_num, 
                                                  uint8_t *txData, 
                                                  uint8_t *rxData, 
                                                  uint16_t size);
BSP_SPI_Status_TypeDef BSP_SPI_Abor_DMA(SPI_Num_TypeDef spi_num);


void BSP_SPI_RegisterCallback(SPI_Num_TypeDef spi_num,
                              void (*tx_callback)(SPI_Num_TypeDef),
                              void (*rx_callback)(SPI_Num_TypeDef, uint8_t*, uint16_t));

void BSP_SPI_IRQHandler(SPI_Num_TypeDef spi_num);


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);

#endif /* __BSP_SPI_H__ */