#include "bsp_spi.h"

/* 外部句柄 */
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

/* SPI实例映射表 */
SPI_HandleTypeDef* SPI_Instance[] = {
    &hspi1,
    &hspi2,
    &hspi3
};

/* 全局DMA缓冲区 */
static uint8_t Spi_RxBuf[SPI_NUM_MAX][SPI_RX_BUFFER_SIZE];
static uint8_t Spi_TxBuf[SPI_NUM_MAX][SPI_RX_BUFFER_SIZE];

/* DMA句柄数组 */
SPI_DMA_Handle_TypeDef SPI_DMA_Handle[SPI_NUM_MAX] = {0};

/* 回调函数指针 */
static void (*TxCallback[SPI_NUM_MAX])(SPI_Num_TypeDef) = {NULL};
static void (*RxCallback[SPI_NUM_MAX])(SPI_Num_TypeDef, uint8_t*, uint16_t) = {NULL};

/**
 * @brief  初始化指定的 SPI 接口
 * 
 * @param  spi_num: 要初始化的 SPI 编号
 * @param  config: 包含 SPI 配置信息的结构体指针
 * 
 * @return 初始化成功返回 BSP_SPI_OK，否则返回错误码
 */
BSP_SPI_Status_TypeDef BSP_SPI_Init(SPI_Num_TypeDef spi_num, SPI_Config_TypeDef *config)
{
    // 参数校验：确保spi_num有效且config不为空
    if (spi_num >= SPI_NUM_MAX || config == NULL) {
        return BSP_SPI_ERR_INVALID_NUM;
    }
    
    SPI_HandleTypeDef *hspi = SPI_Instance[spi_num];
    
    // 关键修复：关联外设实例
    switch(spi_num) {
        case SPI_NUM_1: 
          hspi->Instance = SPI1; 
          __HAL_RCC_SPI1_CLK_ENABLE();
          break;
        case SPI_NUM_2: 
          hspi->Instance = SPI2; 
          __HAL_RCC_SPI2_CLK_ENABLE();
          break;
        case SPI_NUM_3: 
          hspi->Instance = SPI3; 
          __HAL_RCC_SPI3_CLK_ENABLE();
          break;
        default: 
          __HAL_SPI_DISABLE(hspi);
          return BSP_SPI_ERR_INVALID_NUM;
    }
    
    // 设置配置参数
    hspi->Init.BaudRatePrescaler = config->BaudRatePrescaler;
    hspi->Init.Direction         = config->Direction;
    hspi->Init.CLKPolarity       = config->CLKPolarity;
    hspi->Init.CLKPhase          = config->CLKPhase;
    hspi->Init.DataSize          = SPI_DATASIZE_8BIT;
    hspi->Init.FirstBit          = config->FirstBit;
    hspi->Init.NSS               = SPI_NSS_SOFT;
    hspi->Init.Mode              = config->Mode;

    // DMA模式额外配置
    if (config->dma_mode == SPI_MODE_DMA) {
        // 确保MSP初始化已执行（配置了DMA）
        HAL_SPI_MspInit(hspi);
        
        SPI_DMA_Handle[spi_num].hspi = hspi;
        SPI_DMA_Handle[spi_num].hdma_tx = hspi->hdmatx;
        SPI_DMA_Handle[spi_num].hdma_rx = hspi->hdmarx;
        SPI_DMA_Handle[spi_num].dma_status = BSP_DMA_IDLE;

        SPI_DMA_Handle[spi_num].dma_tx_buf = Spi_TxBuf[spi_num];
        SPI_DMA_Handle[spi_num].dma_rx_buf = Spi_RxBuf[spi_num];
    }
    
    // 初始化SPI外设
    if (HAL_SPI_Init(hspi) != HAL_OK) {
        HAL_SPI_MspDeInit(hspi);
        return BSP_SPI_ERR_HAL_FAIL;
    }

    return BSP_SPI_OK;
}

/**
 * @brief  反初始化指定的 SPI 接口
 * 
 * @param  spi_num: 要反初始化的 SPI 编号
 * 
 * @return 成功返回 BSP_SPI_OK，否则返回错误码
 */
BSP_SPI_Status_TypeDef BSP_SPI_DeInit(SPI_Num_TypeDef spi_num)
{
    // 参数校验：确保spi_num有效
    if (spi_num >= SPI_NUM_MAX) {
        return BSP_SPI_ERR_INVALID_NUM;
    }

    // 反初始化SPI外设
    if (HAL_SPI_DeInit(SPI_Instance[spi_num]) != HAL_OK) {
        return BSP_SPI_ERR_HAL_FAIL;
    }

    return BSP_SPI_OK;
}

/**
 * @brief  使用轮询方式发送数据
 * 
 * @param  spi_num: 要使用的 SPI 编号
 * @param  txData: 发送数据缓冲区
 * @param  size: 数据大小
 * @param  timeout: 超时时间
 * 
 * @return 成功返回 BSP_SPI_OK，否则返回错误码
 */
BSP_SPI_Status_TypeDef BSP_SPI_Transmit(SPI_Num_TypeDef spi_num, uint8_t *txData, uint16_t size, uint32_t timeout)
{
    // 参数校验：确保spi_num有效、txData不为空且size>0
    if (spi_num >= SPI_NUM_MAX || txData == NULL || size == 0) {
        return BSP_SPI_ERR_INVALID_NUM;
    }
    return (HAL_SPI_Transmit(SPI_Instance[spi_num], txData, size, timeout) == HAL_OK) ? 
           BSP_SPI_OK : BSP_SPI_ERR_HAL_FAIL;
}

/**
 * @brief  使用轮询方式接收数据
 * 
 * @param  spi_num: 要使用的 SPI 编号
 * @param  rxData: 接收数据缓冲区
 * @param  size: 数据大小
 * @param  timeout: 超时时间
 * 
 * @return 成功返回 BSP_SPI_OK，否则返回错误码
 */
BSP_SPI_Status_TypeDef BSP_SPI_Receive(SPI_Num_TypeDef spi_num, uint8_t *rxData, uint16_t size, uint32_t timeout)
{
    // 参数校验：确保spi_num有效、rxData不为空且size>0
    if (spi_num >= SPI_NUM_MAX || rxData == NULL || size == 0) {
        return BSP_SPI_ERR_INVALID_NUM;
    }
    return (HAL_SPI_Receive(SPI_Instance[spi_num], rxData, size, timeout) == HAL_OK) ? 
           BSP_SPI_OK : BSP_SPI_ERR_HAL_FAIL;
}

/**
 * @brief  使用轮询方式发送和接收数据
 * 
 * @param  spi_num: 要使用的 SPI 编号
 * @param  txData: 发送数据缓冲区
 * @param  rxData: 接收数据缓冲区
 * @param  size: 数据大小
 * @param  timeout: 超时时间
 * 
 * @return 成功返回 BSP_SPI_OK，否则返回错误码
 */
BSP_SPI_Status_TypeDef BSP_SPI_TransmitReceive(SPI_Num_TypeDef spi_num,
                                              uint8_t *txData,
                                              uint8_t *rxData,
                                              uint16_t size,
                                              uint32_t timeout)
{
    // 参数校验：确保spi_num有效、txData和rxData不为空且size>0
    if (spi_num >= SPI_NUM_MAX || txData == NULL || rxData == NULL || size == 0) {
        return BSP_SPI_ERR_INVALID_NUM;
    }

    SPI_HandleTypeDef *hspi = SPI_Instance[spi_num];
    if (hspi == NULL || hspi->Instance == NULL) {
        return BSP_SPI_ERR_HAL_FAIL;
    }

    return (HAL_SPI_TransmitReceive(hspi, txData, rxData, size, timeout) == HAL_OK) ? 
           BSP_SPI_OK : BSP_SPI_ERR_HAL_FAIL;
}

/**
 * @brief  使用中断方式发送数据
 * 
 * @param  spi_num: 要使用的 SPI 编号
 * @param  txData: 发送数据缓冲区
 * @param  size: 数据大小
 * 
 * @return 成功返回 BSP_SPI_OK，否则返回错误码
 */
BSP_SPI_Status_TypeDef BSP_SPI_Transmit_IT(SPI_Num_TypeDef spi_num, uint8_t *txData, uint16_t size)
{
    // 参数校验：确保spi_num有效、txData不为空且size>0
    if (spi_num >= SPI_NUM_MAX || txData == NULL || size == 0) {
        return BSP_SPI_ERR_INVALID_NUM;
    }
    return (HAL_SPI_Transmit_IT(SPI_Instance[spi_num], txData, size) == HAL_OK) ? 
           BSP_SPI_OK : BSP_SPI_ERR_HAL_FAIL;
}

/**
 * @brief  使用中断方式接收数据
 * 
 * @param  spi_num: 要使用的 SPI 编号
 * @param  rxData: 接收数据缓冲区
 * @param  size: 数据大小
 * 
 * @return 成功返回 BSP_SPI_OK，否则返回错误码
 */
BSP_SPI_Status_TypeDef BSP_SPI_Receive_IT(SPI_Num_TypeDef spi_num, uint8_t *rxData, uint16_t size)
{
    // 参数校验：确保spi_num有效、rxData不为空且size>0
    if (spi_num >= SPI_NUM_MAX || rxData == NULL || size == 0) {
        return BSP_SPI_ERR_INVALID_NUM;
    }
    return (HAL_SPI_Receive_IT(SPI_Instance[spi_num], rxData, size) == HAL_OK) ? 
           BSP_SPI_OK : BSP_SPI_ERR_HAL_FAIL;
}

/**
 * @brief  使用DMA方式发送数据
 * 
 * @param  spi_num: 要使用的 SPI 编号
 * @param  txData: 发送数据缓冲区
 * @param  size: 数据大小
 * 
 * @return 成功返回 BSP_SPI_OK，否则返回错误码
 */
BSP_SPI_Status_TypeDef BSP_SPI_Transmit_DMA(SPI_Num_TypeDef spi_num, uint8_t *txData, uint16_t size)
{
    SPI_DMA_Handle_TypeDef *dma_handle = &SPI_DMA_Handle[spi_num];
    // 参数校验：确保spi_num有效、txData不为空、size>0且dma句柄有效
    if (spi_num >= SPI_NUM_MAX || txData == NULL || size == 0 || dma_handle->hspi == NULL) {
        return BSP_SPI_ERR_INVALID_NUM;
    }

    if (dma_handle->dma_status != BSP_DMA_IDLE) {
        return BSP_SPI_ERR_HAL_FAIL;
    }

    dma_handle->dma_tx_size = size;
    dma_handle->dma_status = BSP_DMA_TX_BUSY;

    if (HAL_SPI_Transmit_DMA(dma_handle->hspi, txData, size) != HAL_OK) {
        dma_handle->dma_status = BSP_DMA_IDLE;
        return BSP_SPI_ERR_HAL_FAIL;
    }

    return BSP_SPI_OK;
}

/**
 * @brief  使用DMA方式接收数据
 * 
 * @param  spi_num: 要使用的 SPI 编号
 * @param  rxData: 接收数据缓冲区
 * @param  size: 数据大小
 * 
 * @return 成功返回 BSP_SPI_OK，否则返回错误码
 */
BSP_SPI_Status_TypeDef BSP_SPI_Receive_DMA(SPI_Num_TypeDef spi_num, uint8_t *rxData, uint16_t size)
{
    SPI_DMA_Handle_TypeDef *dma_handle = &SPI_DMA_Handle[spi_num];
    // 参数校验：确保spi_num有效、size>0且dma句柄有效
    if (spi_num >= SPI_NUM_MAX || size == 0 || dma_handle->hspi == NULL) {
        return BSP_SPI_ERR_INVALID_NUM;
    }

    // 检查主机模式下的无效接收
    if (dma_handle->hspi->Init.Mode == SPI_MODE_MASTER) {
        return BSP_SPI_ERR_MASTER_RX;
    }

    if (dma_handle->dma_status != BSP_DMA_IDLE) {
        return BSP_SPI_ERR_HAL_FAIL;
    }

    if (rxData == NULL) {
        rxData = dma_handle->dma_rx_buf;
    }

    dma_handle->dma_rx_size = size;
    dma_handle->dma_status = BSP_DMA_RX_BUSY;

    if (HAL_SPI_Receive_DMA(dma_handle->hspi, rxData, size) != HAL_OK) {
        dma_handle->dma_status = BSP_DMA_IDLE;
        return BSP_SPI_ERR_HAL_FAIL;
    }

    return BSP_SPI_OK;
}

/**
 * @brief  使用DMA方式发送和接收数据
 * 
 * @param  spi_num: 要使用的 SPI 编号
 * @param  txData: 发送数据缓冲区
 * @param  rxData: 接收数据缓冲区
 * @param  size: 数据大小
 * 
 * @return 成功返回 BSP_SPI_OK，否则返回错误码
 */
BSP_SPI_Status_TypeDef BSP_SPI_TransmitReceive_DMA(SPI_Num_TypeDef spi_num, 
                                                  uint8_t *txData, 
                                                  uint8_t *rxData, 
                                                  uint16_t size)
{
    SPI_DMA_Handle_TypeDef *dma_handle = &SPI_DMA_Handle[spi_num];
    // 参数校验：确保spi_num有效、txData和rxData不为空、size>0且dma句柄有效
    if (spi_num >= SPI_NUM_MAX || txData == NULL || rxData == NULL || 
        size == 0 || dma_handle->hspi == NULL) {
        return BSP_SPI_ERR_INVALID_NUM;
    }

    if (dma_handle->dma_status != BSP_DMA_IDLE) {
        return BSP_SPI_ERR_HAL_FAIL;
    }

    dma_handle->dma_tx_size = size;
    dma_handle->dma_rx_size = size;
    dma_handle->dma_status = BSP_DMA_TXRX_BUSY;

    if (HAL_SPI_TransmitReceive_DMA(dma_handle->hspi, txData, rxData, size) != HAL_OK) {
        dma_handle->dma_status = BSP_DMA_IDLE;
        return BSP_SPI_ERR_HAL_FAIL;
    }

    return BSP_SPI_OK;
}

/**
 * @brief  终止正在进行的DMA传输
 * 
 * @param  spi_num: 要操作的 SPI 编号
 * 
 * @return 成功返回 BSP_SPI_OK，否则返回错误码
 */
BSP_SPI_Status_TypeDef BSP_SPI_Abor_DMA(SPI_Num_TypeDef spi_num)
{
    SPI_DMA_Handle_TypeDef *dma_handle = &SPI_DMA_Handle[spi_num];
    // 参数校验：确保spi_num有效且dma句柄有效
    if (spi_num >= SPI_NUM_MAX || dma_handle->hspi == NULL) {
        return BSP_SPI_ERR_INVALID_NUM;
    }

    if (dma_handle->dma_status != BSP_DMA_IDLE) {
        HAL_DMA_Abort(dma_handle->hdma_tx);
        HAL_DMA_Abort(dma_handle->hdma_rx);
        dma_handle->dma_status = BSP_DMA_IDLE;
    }

    return BSP_SPI_OK;
}

/**
 * @brief  注册发送和接收完成回调函数
 * 
 * @param  spi_num: 要操作的 SPI 编号
 * @param  tx_callback: 发送完成回调函数
 * @param  rx_callback: 接收完成回调函数
 */
void BSP_SPI_RegisterCallback(SPI_Num_TypeDef spi_num,
                              void (*tx_callback)(SPI_Num_TypeDef),
                              void (*rx_callback)(SPI_Num_TypeDef, uint8_t*, uint16_t))
{
    if (spi_num < SPI_NUM_MAX) {
        TxCallback[spi_num] = tx_callback;
        RxCallback[spi_num] = rx_callback;
    }
}

/**
 * @brief  SPI中断处理函数
 * 
 * @param  spi_num: 中断发生的 SPI 编号
 */
void BSP_SPI_IRQHandler(SPI_Num_TypeDef spi_num)
{
    if (spi_num >= SPI_NUM_MAX) return;
    HAL_SPI_IRQHandler(SPI_Instance[spi_num]);
}

/**
 * @brief  发送完成回调函数
 * 
 * @param  hspi: 指向SPI句柄的指针
 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    for (int i = 0; i < SPI_NUM_MAX; i++) {
        if (SPI_Instance[i] == hspi && SPI_DMA_Handle[i].hspi == hspi) {
            SPI_DMA_Handle[i].dma_status = BSP_DMA_IDLE;
            if (TxCallback[i]) TxCallback[i](i);
            break;
        }
    }
}

/**
 * @brief  接收完成回调函数
 * 
 * @param  hspi: 指向SPI句柄的指针
 */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    for (int i = 0; i < SPI_NUM_MAX; i++) {
        if (SPI_Instance[i] == hspi && SPI_DMA_Handle[i].hspi == hspi) {
            SPI_DMA_Handle_TypeDef *dma_handle = &SPI_DMA_Handle[i];
            dma_handle->dma_status = BSP_DMA_IDLE;
            
            if (RxCallback[i]) {
                RxCallback[i](i, dma_handle->dma_rx_buf, dma_handle->dma_rx_size);
            }
            break;
        }
    }
}

/**
 * @brief  发送和接收完成回调函数
 * 
 * @param  hspi: 指向SPI句柄的指针
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    for (int i = 0; i < SPI_NUM_MAX; i++) {
        if (SPI_Instance[i] == hspi && SPI_DMA_Handle[i].hspi == hspi) {
            SPI_DMA_Handle_TypeDef *dma_handle = &SPI_DMA_Handle[i];
            dma_handle->dma_status = BSP_DMA_IDLE;
            
            // 通知接收完成
            if (RxCallback[i]) {
                RxCallback[i](i, dma_handle->dma_rx_buf, dma_handle->dma_rx_size);
            }
            break;
        }
    }
}
