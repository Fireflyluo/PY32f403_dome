/*-----------------------------------------------File Info------------------------------------------------
** File Name:               py32f403_hal_msp.c  
** Last modified date:      2025.7.1
** Last version:            V0.1
** Description:             HAL库MSP层初始化代码
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            包含SPI、UART、DMA、GPIO等外设的底层时钟启用与NVIC中断优先级配置；
**                          实现HAL库要求的MspInit/MspDeInit函数；
**                          配置DMA通道映射与中断服务函数关联。
**--------------------------------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//DMA_HandleTypeDef hdma_spi2_tx;
/* Private function prototypes -----------------------------------------------*/
/* External functions --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi1_rx;

DMA_HandleTypeDef hdma_usart1_tx, hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_tx, hdma_usart2_rx;
//DMA_HandleTypeDef hdma_spi2_tx;
//DMA_HandleTypeDef hdma_spi1_tx;
//DMA_HandleTypeDef hdma_spi1_rx;
/**
  * @brief Initialize global MSP
  */
void HAL_MspInit(void)
{
  // 启用基础硬件模块时钟
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  // 启用 SPI1、SPI2、SPI3 时钟
  __HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_SPI2_CLK_ENABLE();
  __HAL_RCC_SPI3_CLK_ENABLE();

  // 启用 UART1、UART2 、UART3 时钟
  __HAL_RCC_USART1_CLK_ENABLE();
  __HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();
	
	//启动 DMA 时钟
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();


  // 启用 GPIOA、GPIOB 时钟（用于 LED 和 IRQ）
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
	//启动 TIM3 时钟
//	__HAL_RCC_TIM4_CLK_ENABLE();
  __HAL_RCC_TIM6_CLK_ENABLE();
  // 配置 NVIC 中断优先级分组
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2); //中断分组2 抢占优先级0~3 相应优先级0~3


}


/**
  * @brief Initialize SPI MSP
  * @param hspi: SPI handle pointer
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0}; // 初始化GPIO配置结构体

  if (hspi->Instance == SPI1)
  {
    // 配置SPI1的引脚（PA5/SCK, PA6/MISO, PA7/MOSI）
    __HAL_RCC_GPIOA_CLK_ENABLE(); 
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; 
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; 
    GPIO_InitStruct.Pull = GPIO_NOPULL; 
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 引脚速度设置为高频
    GPIO_InitStruct.Alternate = GPIO_AF3_SPI1; 
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 

    /* SPI2 DMA Init */
    /* SPI2_TX Init */
    hdma_spi1_tx.Instance = DMA2_Channel1;                // DMA通道DMA1_Channel8
    hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;   // 数据传输方向：内存到外设
    hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;       // 禁用外设地址递增
    hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;           // 启用内存地址递增
    hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  // 外设数据对齐方式：字节
    hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     // 内存数据对齐方式：字节
    hdma_spi1_tx.Init.Mode = DMA_NORMAL;              // DMA工作模式为普通模式
    hdma_spi1_tx.Init.Priority = DMA_PRIORITY_LOW;    // 设置DMA优先级为低

    if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK)  
    {
      APP_ErrorHandler();   
    }

    __HAL_LINKDMA(hspi, hdmatx, hdma_spi1_tx);  // 通道关联

    hdma_spi1_rx.Instance = DMA2_Channel2;                // DMA通道DMA1_Channel9
    hdma_spi1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;   // 数据传输方向：内存到外设
    hdma_spi1_rx.Init.PeriphInc = DMA_PINC_DISABLE;       // 禁用外设地址递增
    hdma_spi1_rx.Init.MemInc = DMA_MINC_ENABLE;           // 启用内存地址递增
    hdma_spi1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  // 外设数据对齐方式：字节
    hdma_spi1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     // 内存数据对齐方式：字节
    hdma_spi1_rx.Init.Mode = DMA_NORMAL;              // DMA工作模式为普通模式
    hdma_spi1_rx.Init.Priority = DMA_PRIORITY_LOW;    // 设置DMA优先级为低

      if (HAL_DMA_Init(&hdma_spi1_rx) != HAL_OK)  
    {
      APP_ErrorHandler();   
    }

    __HAL_LINKDMA(hspi, hdmarx, hdma_spi1_rx);  // 通道关联
    
    HAL_DMA_ChannelMap(&hdma_spi1_tx, DMA_CHANNEL_MAP_SPI1_WR); /* SPI1_TX DMA1_CH1 */
    HAL_DMA_ChannelMap(&hdma_spi1_rx, DMA_CHANNEL_MAP_SPI1_RD); /* SPI1_RX DMA1_CH2 */

       // 配置并启用NVIC中断
    HAL_NVIC_SetPriority(SPI1_IRQn, 0, 2);  // 设置 SPI1 中断优先级
    HAL_NVIC_EnableIRQ(SPI1_IRQn);          // 启用 SPI1 中断

    HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 1,1);   // 设置DMA1通道1中断优先级
    HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);           // 启用DMA1通道1中断

    HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, 1, 2);   // 设置DMA1通道2中断优先级
    HAL_NVIC_EnableIRQ(DMA2_Channel2_IRQn);           // 启用DMA1通道2中断
  }
  else if(hspi->Instance == SPI2)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE(); // 启用GPIOB时钟
    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB15     ------> SPI2_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15; 
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; 
    GPIO_InitStruct.Pull = GPIO_NOPULL; 
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 引脚速度设置为非常高
    GPIO_InitStruct.Alternate = GPIO_AF3_SPI2; 
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); 

    /* SPI2 DMA Init */
    /* SPI2_TX Init */
    hdma_spi2_tx.Instance = DMA1_Channel5;                // DMA通道DMA1_Channel7
    hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;   // 数据传输方向：内存到外设
    hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;       // 禁用外设地址递增
    hdma_spi2_tx.Init.MemInc = DMA_MINC_ENABLE;           // 启用内存地址递增
    hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  // 外设数据对齐方式：字节
    hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     // 内存数据对齐方式：字节
    hdma_spi2_tx.Init.Mode = DMA_NORMAL;              // DMA工作模式为普通模式
    hdma_spi2_tx.Init.Priority = DMA_PRIORITY_LOW;    // 设置DMA优先级为低

    if (HAL_DMA_Init(&hdma_spi2_tx) != HAL_OK)  
    {
      APP_ErrorHandler();   
    }

    __HAL_LINKDMA(hspi, hdmatx, hdma_spi2_tx);  // 通道关联

        /* Set DMA channel map. */
    HAL_DMA_ChannelMap(&hdma_spi2_tx, DMA_CHANNEL_MAP_SPI2_WR); /* SPI1_TX DMA1_CH1 */
//    HAL_DMA_ChannelMap(&HdmaCh2, DMA_CHANNEL_MAP_SPI2_RD); /* SPI1_RX DMA1_CH2 */
    
    /* SPI2 interrupt Init */
    HAL_NVIC_SetPriority(SPI2_IRQn, 1, 0); 
    HAL_NVIC_EnableIRQ(SPI2_IRQn);    

    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 1, 1); 
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn); 
  }
  else if (hspi->Instance == SPI3)
  {
    // 配置SPI3的引脚（PB3/SCK, PB4/MISO, PB5/MOSI）
    __HAL_RCC_GPIOB_CLK_ENABLE(); // 启用GPIOB时钟
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5; 
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; 
    GPIO_InitStruct.Pull = GPIO_NOPULL; 
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
    GPIO_InitStruct.Alternate = GPIO_AF2_SPI3; 
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); 
  }
}

/**
  * @brief Initialize UART MSP
  * @param huart: UART handle pointer
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	

	/* USART1 初始化 */
  if (huart->Instance == USART1)
  {
      __HAL_RCC_GPIOA_CLK_ENABLE();
      /* GPIO Initialization
      PA9     ------> USART1_TX
      PA10    ------> USART1_RX
      */
      GPIO_InitStruct.Pin       = GPIO_PIN_9;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF2_USART1;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_10;
      GPIO_InitStruct.Alternate = GPIO_AF2_USART1;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        
      // USART1 DMA配置
      // USART1_TX初始化
      hdma_usart1_tx.Instance = DMA1_Channel1;                // 配置DMA1通道1为USART1发送通道
      hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;   // 数据传输方向：内存到外设
      hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;       // 禁用外设地址递增
      hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;           // 启用内存地址递增
      hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; // 外设数据对齐方式：字节
      hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    // 内存数据对齐方式：字节
      hdma_usart1_tx.Init.Mode = DMA_NORMAL;                  // 配置为普通模式
      hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;        // 设置DMA优先级为低
      if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)            // 初始化DMA，若失败则调用错误处理函数
      {
        APP_ErrorHandler();
      }

      __HAL_LINKDMA(&huart1, hdmatx, hdma_usart1_tx); // 通道关联
      
      // USART1_RX初始化
      hdma_usart1_rx.Instance = DMA1_Channel2;                // 配置DMA1通道2为USART1接收通道
      hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;   // 数据传输方向：外设到内存
      hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;       // 禁用外设地址递增
      hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;           // 启用内存地址递增
      hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  // 外设数据对齐方式：字节
      hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     // 内存数据对齐方式：字节
      hdma_usart1_rx.Init.Mode = DMA_NORMAL;              // 配置为普通模式
      hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;    // 设置DMA优先级为低
      if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)        // 初始化DMA，若失败则调用错误处理函数
      {
        APP_ErrorHandler();
      }
      __HAL_LINKDMA(&huart1, hdmarx, hdma_usart1_rx); 
        
      /*设置DMA请求映射*/
      HAL_DMA_ChannelMap(&hdma_usart1_tx, DMA_CHANNEL_MAP_USART1_WR);
      HAL_DMA_ChannelMap(&hdma_usart1_rx, DMA_CHANNEL_MAP_USART1_RD);
      
      // 配置并启用NVIC中断
      HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);  // 设置USART1中断优先级
      HAL_NVIC_EnableIRQ(USART1_IRQn);          // 启用USART1中断

      HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 1);   // 设置DMA1通道1中断优先级
      HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);           // 启用DMA1通道1中断

      HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 1, 1);   // 设置DMA1通道2中断优先级
      HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);           // 启用DMA1通道2中断
  }

  /*USART2初始化*/
  else if (huart->Instance == USART2)
  {
      __HAL_RCC_GPIOA_CLK_ENABLE();
      /* GPIO Initialization
      PA2     ------> USART2_TX
      PA3     ------> USART2_RX
      */
    GPIO_InitStruct.Pin       = GPIO_PIN_2;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Alternate = GPIO_AF2_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		
    // USART2 DMA配置
    // USART2_TX初始化
    hdma_usart2_tx.Instance = DMA1_Channel3;                // 配置DMA1通道3为USART2发送通道
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;   // 数据传输方向：内存到外设
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;       // 禁用外设地址递增
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;           // 启用内存地址递增
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; // 外设数据对齐方式：字节
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    // 内存数据对齐方式：字节
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;                  // 配置为普通模式
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;        // 设置DMA优先级为低
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)            // 初始化DMA，若失败则调用错误处理函数
    {
      APP_ErrorHandler();
    }

    __HAL_LINKDMA(&huart2, hdmatx, hdma_usart2_tx); // 将DMA与UART发送通道关联
  
    // USART2_RX初始化
    hdma_usart2_rx.Instance = DMA1_Channel4;                // 配置DMA1通道4为USART2接收通道
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;   // 数据传输方向：外设到内存
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;       // 禁用外设地址递增
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;           // 启用内存地址递增
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  // 外设数据对齐方式：字节
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     // 内存数据对齐方式：字节
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;              // 配置为普通模式
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;    // 设置DMA优先级为低
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)        // 初始化DMA，若失败则调用错误处理函数
    {
      APP_ErrorHandler();
    }
    __HAL_LINKDMA(&huart2, hdmarx, hdma_usart2_rx); // 将DMA与UART接收通道关联
  
    /*设置DMA请求映射*/
    HAL_DMA_ChannelMap(&hdma_usart2_tx, DMA_CHANNEL_MAP_USART2_WR);
    HAL_DMA_ChannelMap(&hdma_usart2_rx, DMA_CHANNEL_MAP_USART2_RD);
  


    // 配置并启用NVIC中断
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 2);  // 设置USART2中断优先级
    HAL_NVIC_EnableIRQ(USART2_IRQn);          // 启用USART2中断

    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 2);   // 设置DMA1通道3中断优先级
    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);           // 启用DMA1通道3中断

    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 3);   // 设置DMA1通道4中断优先级
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);           // 启用DMA1通道4中断
      
  }

/* USART3 Initialization - Interrupt Mode */
if (huart->Instance == USART3)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* GPIO Initialization
     * PB10     ------> USART3_TX
     * PB11     ------> USART3_RX
     */
    GPIO_InitStruct.Pin       = GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Alternate = GPIO_AF2_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 启用 USART3 中断
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 3);          // 设置中断优先级
    HAL_NVIC_EnableIRQ(USART3_IRQn);                 // 启用中断

    // 启动 UART 接收中断（例如：接收一个字节）
    // 假设 huart3 是全局定义的 UART_HandleTypeDef 实例
//    HAL_UART_Receive_IT(&huart3, rx_buffer, 1);      // 开始接收中断
}
}
	
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspInit 0 */

  /* USER CODE END TIM1_MspInit 0 */
    /* TIM1 clock enable */
//    __HAL_RCC_TIM1_CLK_ENABLE();
  /* USER CODE BEGIN TIM1_MspInit 1 */

  /* USER CODE END TIM1_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}
