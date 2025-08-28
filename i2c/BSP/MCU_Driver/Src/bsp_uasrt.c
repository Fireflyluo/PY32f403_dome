/*-----------------------------------------------File Info------------------------------------------------
** File Name:               bsp_uasrt.c  
** Last modified date:      2025.7.1
** Last version:            V0.1
** Description:             UART驱动封装与DMA接收机制实现
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            提供UART初始化、发送/接收接口（支持阻塞、中断、DMA三种模式）；
**                          实现双缓冲DMA接收机制；
**                          支持用户注册发送/接收完成回调函数。
**--------------------------------------------------------------------------------------------------------*/

#include "bsp_uasrt.h"


/* 串口句柄数组（需与CubeMX生成的句柄一致） */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
// 定义全局缓冲区
uint8_t Uart_RxBuf[2][UART_RX_BUFFER_SIZE];
uint8_t Uart_TxBuf[UART_RX_BUFFER_SIZE];
/* 串口实例映射表（根据实际硬件修改） */
UART_HandleTypeDef* UART_Instance[] = {
    &huart1,                // UART_NUM_1 -> USART1
    &huart2,                // UART_NUM_2 -> USART2
    &huart3                 // UART_NUM_3 -> USART3
};

/* 初始化UART_DMA_Handle数组，用于管理所有UART的DMA处理 */
UART_DMA_Handle_TypeDef UART_DMA_Handle[UART_NUM_MAX] = {0};

/* 回调函数指针 */
static void (*TxCallback[UART_NUM_MAX])(UART_Num_TypeDef) = {NULL};
static void (*RxCallback[UART_NUM_MAX])(UART_Num_TypeDef, uint8_t*, uint16_t) = {NULL};

/**
  * @brief  初始化指定串口
  * @param  uart_num: 串口编号（UART_NUM_1/UART_NUM_2）
  * @param  config: 串口配置参数
  * @retval BSP_UART_Status_TypeDef: 状态
  */
BSP_UART_Status_TypeDef BSP_UART_Init(UART_Num_TypeDef uart_num, UART_Config_TypeDef *config, UART_Mode_TypeDef mode) {
    /* 参数校验 */
    if (uart_num >= UART_NUM_MAX || config == NULL) {
        return BSP_UART_ERR_INVALID_NUM;
    }

    /* 根据 uart_num 选择实例并关联外设 */
    UART_HandleTypeDef *huart = UART_Instance[uart_num];
    switch(uart_num) {
        case UART_NUM_1: 
            huart->Instance = USART1; 
            __HAL_RCC_USART1_CLK_ENABLE(); 
            break;
        case UART_NUM_2: 
            huart->Instance = USART2; 
            __HAL_RCC_USART2_CLK_ENABLE();  
            break;
        case UART_NUM_3: 
            huart->Instance = USART3;       
            __HAL_RCC_USART3_CLK_ENABLE();  
            break;
        default: 
            return BSP_UART_ERR_INVALID_NUM;
    }

    /* 配置串口参数 */
    huart->Init.BaudRate     = config->BaudRate;
    huart->Init.WordLength   = config->WordLength;
    huart->Init.StopBits     = config->StopBits;
    huart->Init.Parity       = config->Parity;
    huart->Init.Mode         = config->Mode;
    huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;


    /* 初始化 DMA 缓冲区 */
    if (config->dma_mode == UART_MODE_DMA) {
        UART_DMA_Handle[uart_num].huart = huart;
        UART_DMA_Handle[uart_num].dma_status = BSP_DMA_IDLE;
        
        // 修复：为 TX 分配静态缓冲区（或动态分配）
        UART_DMA_Handle[uart_num].dma_tx_buf = Uart_TxBuf;  // 假设已定义
        
#if USE_DYNAMIC_DMA_BUFFER
        for (int j = 0; j < 2; j++) {
            UART_DMA_Handle[uart_num].dma_rx_buf[j] = (uint8_t*)malloc(UART_RX_BUFFER_SIZE);
            if (UART_DMA_Handle[uart_num].dma_rx_buf[j] == NULL) {
                HAL_UART_MspDeInit(huart);
                return BSP_UART_ERR_HAL_FAIL;
            }
        }
#else
        UART_DMA_Handle[uart_num].dma_rx_buf[0] = Uart_RxBuf[0];
        UART_DMA_Handle[uart_num].dma_rx_buf[1] = Uart_RxBuf[1];
#endif
        UART_DMA_Handle[uart_num].current_rx_buf = 0;
    }

    /* 初始化 UART 外设 */
    if (HAL_UART_Init(huart) != HAL_OK) {
        HAL_UART_MspDeInit(huart);
        return BSP_UART_ERR_HAL_FAIL;
    }

    /* 启用空闲中断 */
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);  
    return BSP_UART_OK;
}

/**
  * @brief  去初始化指定串口
  * @param  uart_num: 串口编号
  * @retval BSP_UART_Status_TypeDef: 状态
  */
BSP_UART_Status_TypeDef BSP_UART_DeInit(UART_Num_TypeDef uart_num)
{
    if (uart_num >= UART_NUM_MAX) {
        return BSP_UART_ERR_INVALID_NUM;
    }

#if USE_DYNAMIC_DMA_BUFFER
    if (UART_DMA_Handle[uart_num].dma_rx_buf[0] != NULL) {
        free(UART_DMA_Handle[uart_num].dma_rx_buf[0]);
        UART_DMA_Handle[uart_num].dma_rx_buf[0] = NULL;
    }

    if (UART_DMA_Handle[uart_num].dma_rx_buf[1] != NULL) {
        free(UART_DMA_Handle[uart_num].dma_rx_buf[1]);
        UART_DMA_Handle[uart_num].dma_rx_buf[1] = NULL;
    }
#endif

    if (HAL_UART_DeInit(UART_Instance[uart_num]) != HAL_OK) {
        return BSP_UART_ERR_HAL_FAIL;
    }

    HAL_UART_MspDeInit(UART_Instance[uart_num]);

    return BSP_UART_OK;
}

/**
  * @brief  同步发送数据（阻塞模式）
  * @param  uart_num: 串口编号
  * @param  data: 发送数据指针
  * @param  size: 发送数据长度
  * @param  timeout: 超时时间（ms）
  * @retval BSP_UART_Status_TypeDef: 状态
  */
BSP_UART_Status_TypeDef BSP_UART_Transmit(UART_Num_TypeDef uart_num, uint8_t *data, uint16_t size, uint32_t timeout)
{
    if (uart_num >= UART_NUM_MAX || data == NULL || size == 0) {
        return BSP_UART_ERR_INVALID_NUM;
    }
    return (HAL_UART_Transmit(UART_Instance[uart_num], data, size, timeout) == HAL_OK) ? BSP_UART_OK : BSP_UART_ERR_HAL_FAIL;
}

/**
  * @brief  同步接收数据（阻塞模式）
  * @param  uart_num: 串口编号
  * @param  data: 接收数据指针（需预分配内存）
  * @param  size: 接收数据长度
  * @param  timeout: 超时时间（ms）
  * @retval BSP_UART_Status_TypeDef: 状态
  */
BSP_UART_Status_TypeDef BSP_UART_Receive(UART_Num_TypeDef uart_num, uint8_t *data, uint16_t size, uint32_t timeout)
{
    if (uart_num >= UART_NUM_MAX || data == NULL || size == 0) {
        return BSP_UART_ERR_INVALID_NUM;
    }
    return (HAL_UART_Receive(UART_Instance[uart_num], data, size, timeout) == HAL_OK) ? BSP_UART_OK : BSP_UART_ERR_HAL_FAIL;
}

/**
  * @brief  中断发送数据
  * @param  uart_num: 串口编号
  * @param  data: 发送数据指针
  * @param  size: 发送数据长度
  * @retval BSP_UART_Status_TypeDef: 状态
  */
BSP_UART_Status_TypeDef BSP_UART_Transmit_IT(UART_Num_TypeDef uart_num, uint8_t *data, uint16_t size)
{
    if (uart_num >= UART_NUM_MAX || data == NULL || size == 0) {
        return BSP_UART_ERR_INVALID_NUM;
    }
    return (HAL_UART_Transmit_IT(UART_Instance[uart_num], data, size) == HAL_OK) ? BSP_UART_OK : BSP_UART_ERR_HAL_FAIL;
}

/**
  * @brief  中断接收数据
  * @param  uart_num: 串口编号
  * @param  data: 接收数据指针（需预分配内存）
  * @param  size: 接收数据长度
  * @retval BSP_UART_Status_TypeDef: 状态
  */
BSP_UART_Status_TypeDef BSP_UART_Receive_IT(UART_Num_TypeDef uart_num, uint8_t *data, uint16_t size)
{
    if (uart_num >= UART_NUM_MAX || data == NULL || size == 0) {
        return BSP_UART_ERR_INVALID_NUM;
    }
    return (HAL_UART_Receive_IT(UART_Instance[uart_num], data, size) == HAL_OK) ? BSP_UART_OK : BSP_UART_ERR_HAL_FAIL;
}

/**
  * @brief  DMA模式发送数据
  * @param  uart_num: 串口编号
  * @param  data: 发送数据指针
  * @param  size: 发送数据长度
  * @retval BSP_UART_Status_TypeDef: 状态
  */
BSP_UART_Status_TypeDef BSP_UART_Transmit_DMA(UART_Num_TypeDef uart_num, uint8_t *data, uint16_t size)
{
    UART_DMA_Handle_TypeDef *dma_handle = &UART_DMA_Handle[uart_num];
    if (uart_num >= UART_NUM_MAX || data == NULL || size == 0 || dma_handle->huart == NULL) {
        return BSP_UART_ERR_INVALID_NUM;
    }

    /* 检查DMA状态 */
    if (dma_handle->dma_status == BSP_DMA_TX_BUSY) {
        return BSP_UART_ERR_HAL_FAIL;  // 发送中，请等待完成
    }

    /* 配置DMA传输参数 */
    dma_handle->dma_tx_size = size;
    dma_handle->dma_status = BSP_DMA_TX_BUSY;

    /* 启动DMA发送（HAL_UART_Transmit_DMA会自动填充DMA缓冲区） */
    if (HAL_UART_Transmit_DMA(dma_handle->huart, data, size) != HAL_OK) {
        dma_handle->dma_status = BSP_DMA_IDLE;
        return BSP_UART_ERR_HAL_FAIL;
    }

    return BSP_UART_OK;
}

/**
  * @brief  DMA模式接收数据（循环模式）
  * @param  uart_num: 串口编号
  * @param  data: 接收数据指针（需预分配内存，或使用DMA接收缓冲区）
  * @param  size: 接收数据长度（最大不超过DMA接收缓冲区大小）
  * @retval BSP_UART_Status_TypeDef: 状态
  */
BSP_UART_Status_TypeDef BSP_UART_Receive_DMA(UART_Num_TypeDef uart_num, uint8_t *data, uint16_t size)
{
    UART_DMA_Handle_TypeDef *dma_handle = &UART_DMA_Handle[uart_num];
    UART_HandleTypeDef *huart = UART_Instance[uart_num];  // 获取UART句柄

    if (uart_num >= UART_NUM_MAX  || size == 0 || dma_handle->huart == NULL) {
        return BSP_UART_ERR_INVALID_NUM;
    }

    /* 检查DMA状态 */
    if (dma_handle->dma_status == BSP_DMA_RX_BUSY) {
        return BSP_UART_ERR_HAL_FAIL;  // 接收中，请等待完成
    }

    // 如果 data 为空，则使用预分配的 dma_rx_buf
    if (data == NULL) {
        data = dma_handle->dma_rx_buf[dma_handle->current_rx_buf];  // 使用当前缓冲区
        size = UART_RX_BUFFER_SIZE;  // 使用统一宏定义大小
    }

    /* 配置DMA接收参数（若使用循环模式，直接使用DMA接收缓冲区） */
    dma_handle->dma_rx_size = size;
    dma_handle->dma_status = BSP_DMA_RX_BUSY;

 /* 启动DMA接收（使用当前缓冲区） */
    if (HAL_UART_Receive_DMA(huart, dma_handle->dma_rx_buf[dma_handle->current_rx_buf], size) != HAL_OK) {
        dma_handle->dma_status = BSP_DMA_IDLE;
        return BSP_UART_ERR_HAL_FAIL;
    }

    // 新增：记录当前DMA句柄中的缓冲区信息
    dma_handle->hdma_rx = huart->hdmatx; // 假设 hdma_rx 已正确赋值
    dma_handle->dma_rx_size = size;
    return BSP_UART_OK;
}

/**
  * @brief  终止DMA传输
  * @param  uart_num: 串口编号
  * @retval BSP_UART_Status_TypeDef: 状态
  */
BSP_UART_Status_TypeDef BSP_UART_Abor_DMA(UART_Num_TypeDef uart_num)
{
    UART_DMA_Handle_TypeDef *dma_handle = &UART_DMA_Handle[uart_num];
    if (uart_num >= UART_NUM_MAX || dma_handle->huart == NULL) {
        return BSP_UART_ERR_INVALID_NUM;
    }

    /* 终止TX和RX DMA传输 */
    HAL_DMA_Abort(dma_handle->hdma_tx);
    HAL_DMA_Abort(dma_handle->hdma_rx);
    dma_handle->dma_status = BSP_DMA_IDLE;

    return BSP_UART_OK;
}
/**
  * @brief  注册串口回调函数（传输完成时触发）
  * @param  uart_num: 串口编号
  * @param  tx_callback: 发送完成回调（NULL表示不注册）
  * @param  rx_callback: 接收完成回调（NULL表示不注册）
  */
void BSP_UART_RegisterCallback(UART_Num_TypeDef uart_num, 
                               void (*tx_callback)(UART_Num_TypeDef), 
                               void (*rx_callback)(UART_Num_TypeDef, uint8_t*, uint16_t))
{
    if (uart_num < UART_NUM_MAX) {
        TxCallback[uart_num] = tx_callback;
        RxCallback[uart_num] = rx_callback;
    }
}

/**
  * @brief  UART中断处理函数（需在stm32f4xx_it.c中调用）
  * @param  uart_num: 串口编号
  */
void BSP_UART_IRQHandler(UART_Num_TypeDef uart_num)
{
    if (uart_num >= UART_NUM_MAX) {
        return;
    }
    HAL_UART_IRQHandler(UART_Instance[uart_num]);
}

/**
  * @brief  HAL_UART_TxCpltCallback（HAL层回调，需在HAL_UART_TxCpltCallback中调用）
  * @param  huart: UART句柄
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_NUM_MAX; i++) {
        if (UART_Instance[i] == huart && UART_DMA_Handle[i].huart == huart ){
					UART_DMA_Handle[i].dma_status = BSP_DMA_IDLE;
					if(TxCallback[i] != NULL)
            TxCallback[i]((UART_Num_TypeDef)i);  // 触发用户回调
            break;
        }
    }
}

/**
  * @brief  HAL_UART_RxCpltCallback（HAL层回调，需在HAL_UART_RxCpltCallback中调用）
  * @param  huart: UART句柄
  * @param  size: 实际接收数据长度
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_NUM_MAX; i++) {
        if (UART_Instance[i] == huart && UART_DMA_Handle[i].huart == huart) {
            UART_DMA_Handle_TypeDef *dma_handle = &UART_DMA_Handle[i];

            // 获取实际接收字节数
            uint16_t current_counter =  __HAL_DMA_GET_COUNTER(dma_handle->hdma_rx);
            uint16_t received_bytes = dma_handle->dma_rx_size - current_counter;

            if (received_bytes > 0 && RxCallback[i] != NULL) {
                RxCallback[i]((UART_Num_TypeDef)i, 
                              dma_handle->dma_rx_buf[dma_handle->current_rx_buf], 
                              received_bytes);
            }

            // 切换缓冲区并重新启动DMA
            dma_handle->current_rx_buf ^= 1;
            HAL_UART_Receive_DMA(huart, 
                                 dma_handle->dma_rx_buf[dma_handle->current_rx_buf], 
                                 dma_handle->dma_rx_size);

            break;
        }
    }
}

/**
 * UART接收半完成回调函数
 * 当DMA传输完成一半时触发，处理UART接收缓冲区切换
 * 
 * @param huart UART句柄指针，指向UART_HandleTypeDef结构体
 * 
 * @return 无
 */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_NUM_MAX; i++) {
        if (UART_Instance[i] == huart && UART_DMA_Handle[i].huart == huart) {
            UART_DMA_Handle_TypeDef *dma_handle = &UART_DMA_Handle[i];

        // 根据当前使用的接收缓冲区进行数据处理和缓冲区切换
        uint16_t half_size = dma_handle->dma_rx_size / 2;
        
        if (dma_handle->current_rx_buf == 0) {
            if (RxCallback[i] != NULL) {
                RxCallback[i]((UART_Num_TypeDef)i, dma_handle->dma_rx_buf[0], half_size);
            }
            dma_handle->current_rx_buf = 1;
        } else {
            if (RxCallback[i] != NULL) {
                RxCallback[i]((UART_Num_TypeDef)i, dma_handle->dma_rx_buf[1], half_size);
            }
            dma_handle->current_rx_buf = 0;
        }
            break;
        }
    }
}
/**
 * @brief 处理UART空闲中断（IDLE），调用用户回调并切换DMA缓冲区
 * @param huart UART句柄指针
 */
void BSP_UART_ReceiveIdle(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_NUM_MAX; i++) {
        if (UART_Instance[i] == huart && UART_DMA_Handle[i].huart == huart) {
            UART_DMA_Handle_TypeDef *dma_handle = &UART_DMA_Handle[i];

            // 获取当前接收数据量
            uint16_t current_counter = __HAL_DMA_GET_COUNTER(dma_handle->hdma_rx);
            uint16_t received_bytes = dma_handle->dma_rx_size - current_counter;

            // 如果有注册回调函数且收到数据，则调用回调
            if (received_bytes > 0 && RxCallback[i] != NULL) {
                RxCallback[i]((UART_Num_TypeDef)i,
                              dma_handle->dma_rx_buf[dma_handle->current_rx_buf],
                              received_bytes);
            }

            // 切换DMA缓冲区并重启接收
            dma_handle->current_rx_buf ^= 1;
            HAL_UART_Receive_DMA(huart,
                                 dma_handle->dma_rx_buf[dma_handle->current_rx_buf],
                                 dma_handle->dma_rx_size);

            break;
        }
    }
}