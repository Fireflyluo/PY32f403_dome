/*-----------------------------------------------File Info------------------------------------------------
** File Name:               bsp_core.c  
** Last modified date:      2025.7.1
** Last version:            V0.1
** Description:             系统核心外设初始化与中断回调管理
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            实现系统基础外设（LED、UART、按键、射频模块）的初始化；
**                          注册并实现各UART和GPIO的中断回调函数；
**                          支持DMA/中断模式的串口通信。
**--------------------------------------------------------------------------------------------------------*/
#include "bsp_core.h"


UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi2_tx;
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;

BSP_TIM_HandleTypeDef htim10_bsp;
//TIM_HandleTypeDef htim10;

uint8_t uart1_rx_buffer[UART_RX_BUFFER_SIZE];
uint8_t uart2_rx_buffer[UART_RX_BUFFER_SIZE];
uint8_t uart3_rx_buffer[UART_RX_BUFFER_SIZE];
//===========================用户回调函数定义===========================================//
// UART1 用户回调函数声明
void Uart1_TxCallback(UART_Num_TypeDef uart_num);
void Uart1_RxCallback(UART_Num_TypeDef uart_num, uint8_t *buf, uint16_t size);
// UART2 用户回调函数声明
void Uart2_TxCallback(UART_Num_TypeDef uart_num);
void Uart2_RxCallback(UART_Num_TypeDef uart_num, uint8_t *buf, uint16_t size);
// UART3 用户回调函数声明
void Uart3_TxCallback(UART_Num_TypeDef uart_num);
void Uart3_RxCallback(UART_Num_TypeDef uart_num, uint8_t *buf, uint16_t size);

void  BSP_SPI_TxCallback(SPI_Num_TypeDef spi_num);
void  BSP_SPI_RxCallback(SPI_Num_TypeDef spi_num, uint8_t *buf, uint16_t size);
void LCD_TxCompleteCallback(SPI_Num_TypeDef spi_num);
//============================串口初始化==================================================//
/**
 * @brief  初始化 UART1（DMA 模式）
 * @details 配置 UART1 的波特率、数据位、停止位、校验方式，并设置为 DMA 模式。
 *          注册发送与接收回调函数，并启动 DMA 接收。
 */
static void UART1_Init(uint32_t baud_rate)
{
    UART_Config_TypeDef uart_config = {0};

    // 配置串口参数
    uart_config.BaudRate = baud_rate;
    uart_config.WordLength = UART_WORDLENGTH_8B;
    uart_config.StopBits = UART_STOPBITS_1;
    uart_config.Parity = UART_PARITY_NONE;
    uart_config.Mode = UART_MODE_TX_RX; 
    uart_config.dma_mode = UART_MODE_DMA; // 设置为 DMA 模式

    if (BSP_UART_Init(UART_NUM_1, &uart_config, UART_MODE_DMA) != BSP_UART_OK) {
        APP_ErrorHandler();
    }

    BSP_UART_RegisterCallback(UART_NUM_1, Uart1_TxCallback, Uart1_RxCallback);

    if (BSP_UART_Receive_DMA(UART_NUM_1, NULL, UART_RX_BUFFER_SIZE) != BSP_UART_OK) {
        APP_ErrorHandler();
    }
}

/**
 * @brief  初始化 UART2（DMA 模式）
 * @details 配置 UART2 的波特率、数据位、停止位、校验方式，并设置为 DMA 模式。
 *          注册发送与接收回调函数，并启动 DMA 接收。
 */
static void UART2_Init(uint32_t baud_rate)
{
    UART_Config_TypeDef uart_config = {0};

    // 配置串口参数
    uart_config.BaudRate = baud_rate;
    uart_config.WordLength = UART_WORDLENGTH_8B;
    uart_config.StopBits = UART_STOPBITS_1;
    uart_config.Parity = UART_PARITY_NONE;
    uart_config.Mode = UART_MODE_TX_RX; 
    uart_config.dma_mode = UART_MODE_DMA; // 设置为 DMA 模式

    if (BSP_UART_Init(UART_NUM_2, &uart_config, UART_MODE_DMA) != BSP_UART_OK) {
        APP_ErrorHandler();
    }

    BSP_UART_RegisterCallback(UART_NUM_2, Uart2_TxCallback, Uart2_RxCallback);
    
    
    if (BSP_UART_Receive_DMA(UART_NUM_2, NULL, UART_RX_BUFFER_SIZE) != BSP_UART_OK) {
        APP_ErrorHandler();
    }
}

/**
 * @brief  初始化 UART3（中断模式）
 * @details 配置 UART3 的波特率、数据位、停止位、校验方式，并设置为中断模式。
 *          注册发送与接收回调函数，并启动中断接收。
 */
static void UART3_Init(uint32_t baud_rate)
{
    UART_Config_TypeDef uart_config = {0};

    // 配置串口参数
    uart_config.BaudRate = baud_rate;
    uart_config.WordLength = UART_WORDLENGTH_8B;
    uart_config.StopBits = UART_STOPBITS_1;
    uart_config.Parity = UART_PARITY_NONE;
    uart_config.Mode = UART_MODE_TX_RX; 
    uart_config.dma_mode = UART_MODE_INTERRUPT; // 设置为中断模式

    if (BSP_UART_Init(UART_NUM_3, &uart_config, UART_MODE_INTERRUPT) != BSP_UART_OK) {
        APP_ErrorHandler();
    }

    BSP_UART_RegisterCallback(UART_NUM_3, Uart3_TxCallback, Uart3_RxCallback);

    if (BSP_UART_Receive_IT(UART_NUM_3, uart3_rx_buffer, UART_RX_BUFFER_SIZE) != BSP_UART_OK) {
        APP_ErrorHandler();
    }
}
//==========================SPI初始化=======================================//
void SPI_Init(void)
{
    SPI_Config_TypeDef spi_config = {0};
    // ----------------------
    // 配置 SPI1 (用于 W25Q128 Flash，DMA 全双工模式)
    // ----------------------
    spi_config.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    spi_config.CLKPhase = SPI_PHASE_1EDGE;
    spi_config.CLKPolarity = SPI_POLARITY_LOW;
    spi_config.Direction = SPI_DIRECTION_2LINES;
    spi_config.FirstBit = SPI_FIRSTBIT_MSB;
    spi_config.Mode = SPI_MODE_MASTER;
    spi_config.dma_mode = SPI_MODE_DMA;

    if (BSP_SPI_Init(SPI_NUM_1, &spi_config) != BSP_SPI_OK) {
        APP_ErrorHandler();
    }
    
    // 手动调用 MSP 初始化
    HAL_SPI_MspInit(&hspi1);
    
    // 注册回调函数
    BSP_SPI_RegisterCallback(SPI_NUM_1, NULL, NULL);

    // ----------------------
    // 配置 SPI2 (用于 ST7735 LCD，DMA 发送模式)
    // ----------------------
    spi_config.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    spi_config.CLKPhase = SPI_PHASE_1EDGE;
    spi_config.CLKPolarity = SPI_POLARITY_LOW;
    spi_config.Direction = SPI_DIRECTION_1LINE;   // 单线发送
    spi_config.FirstBit = SPI_FIRSTBIT_MSB;
    spi_config.Mode = SPI_MODE_MASTER;
    spi_config.dma_mode = SPI_MODE_DMA;

    if (BSP_SPI_Init(SPI_NUM_2, &spi_config) != BSP_SPI_OK) {
        APP_ErrorHandler();
    }
    
    // 手动调用 MSP 初始化
    HAL_SPI_MspInit(&hspi2);
    
    // 注册发送完成回调
    BSP_SPI_RegisterCallback(SPI_NUM_2, LCD_TxCompleteCallback, NULL);

    // ----------------------
    // 配置 SPI3 (轮询模式)
    // ----------------------
    spi_config.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    spi_config.CLKPhase = SPI_PHASE_1EDGE;
    spi_config.CLKPolarity = SPI_POLARITY_LOW;
    spi_config.Direction = SPI_DIRECTION_2LINES;
    spi_config.FirstBit = SPI_FIRSTBIT_MSB;
    spi_config.Mode = SPI_MODE_MASTER;
    spi_config.dma_mode = SPI_MODE_POLLING;

    if (BSP_SPI_Init(SPI_NUM_3, &spi_config) != BSP_SPI_OK) {
        APP_ErrorHandler();
    }
    
    // 手动调用 MSP 初始化
    HAL_SPI_MspInit(&hspi3);
}

//======================TIM初始化=================================//

void BSP_Delay_us(uint32_t us)
{
  uint16_t startCnt = __HAL_TIM_GET_COUNTER(&htim6);
  
    while ((__HAL_TIM_GET_COUNTER(&htim6) - startCnt) <= us);
}


//======================用户回调函数======================================//
// LCD 发送完成回调示例
void LCD_TxCompleteCallback(SPI_Num_TypeDef spi_num)
{
    if (spi_num == SPI_NUM_2) {
        // 这里可以通知LCD驱动层传输完成
        // 例如：释放总线、设置标志等
    }
}


/**
 * @brief  UART1 发送完成回调函数
 * @param  uart_num: 串口号（UART_NUM_1）
 */
void Uart1_TxCallback(UART_Num_TypeDef uart_num)
{
    // 处理串口1发送完成事件
}

/**
 * @brief  UART1 接收完成回调函数
 * @param  uart_num: 串口号（UART_NUM_1）
 * @param  data: 接收到的数据指针
 * @param  size: 接收到的数据长度
 */
void Uart1_RxCallback(UART_Num_TypeDef uart_num, uint8_t* data, uint16_t size)
{
    // 处理串口1接收完成事件
}
/**
 * @brief  UART2 发送完成回调函数
 * @param  uart_num: 串口号（UART_NUM_2）
 */
void Uart2_TxCallback(UART_Num_TypeDef uart_num)
{
    // 处理串口2发送完成事件
}

/**
 * @brief  UART2 接收完成回调函数
 * @param  uart_num: 串口号（UART_NUM_2）
 * @param  data: 接收到的数据指针
 * @param  size: 接收到的数据长度
 */
void Uart2_RxCallback(UART_Num_TypeDef uart_num, uint8_t* data, uint16_t size)
{
    // 处理串口2接收完成事件
}
/**
 * @brief  UART3 发送完成回调函数
 * @param  uart_num: 串口号（UART_NUM_3）
 */
void Uart3_TxCallback(UART_Num_TypeDef uart_num)
{
    // 处理串口3发送完成事件
}

/**
 * @brief  UART3 接收完成回调函数
 * @param  uart_num: 串口号（UART_NUM_3）
 * @param  data: 接收到的数据指针
 * @param  size: 接收到的数据长度
 */
void Uart3_RxCallback(UART_Num_TypeDef uart_num, uint8_t* data, uint16_t size)
{
    // 处理串口3接收完成事件
}

void BSP_SPI_TxCallback(SPI_Num_TypeDef spi_num)
{
    if (spi_num == SPI_NUM_1) {
        // 处理SPI1发送完成事件
        // 可以释放缓冲区、触发下一轮发送等
    }
    if (spi_num == SPI_NUM_3) {
        // 处理SPI3发送完成事件
        // 可以释放缓冲区、触发下一轮发送等
    }
}
void BSP_SPI_RxCallback(SPI_Num_TypeDef spi_num, uint8_t* data, uint16_t size)
{
    if (spi_num == SPI_NUM_1) {
        // 处理 SPI1 接收数据
    }
}
/**
 * @brief  射频模块中断回调函数
 * @param  gpiox: 触发中断的GPIO组
 * @param  pin: 触发中断的引脚号
 */
void RF_IRQ_Callback(GPIO_TypeDef *gpiox, uint16_t pin)
{
    if (gpiox == RF_IRQ_GPIO_Port && pin == RF_IRQ_Pin) {
        // 处理射频中断逻辑
        // 例如触发数据接收或状态更新
    }
}
/**
 * @brief  按键1（PB8）按下时的中断回调
 * @param  gpiox: GPIO组
 * @param  pin: 引脚号
 */
void Key1_Callback(GPIO_TypeDef *gpiox, uint16_t pin)
{
    if (gpiox == GPIOB && pin == GPIO_PIN_8) {
        // 处理按键1事件
    }
}
/**
 * @brief  按键2（PB9）按下时的中断回调
 * @param  gpiox: GPIO组
 * @param  pin: 引脚号
 */
void Key2_Callback(GPIO_TypeDef *gpiox, uint16_t pin)
{
    if (gpiox == GPIOB && pin == GPIO_PIN_9) {
        // 处理按键2事件
    }
}

/**
 * @brief  系统基础外设初始化函数
 * @details 初始化 LED、RF_CSN、RF_CE、RF_IRQ、LCD 控制引脚，
 *          并依次初始化 UART1、UART2 和 UART3。
 *          UART1 使用 DMA 模式，UART2 使用 DMA 模式，UART3 使用中断模式。
 */
void BSP_Init(void)
{
    // ----------------------
    // LED 引脚初始化
    // ----------------------
    BSP_GPIO_Init_Output(LED1_GPIO_Port, LED1_Pin, BSP_GPIO_PIN_OUT_PP);     // LED1
    BSP_GPIO_Init_Output(LED2_GPIO_Port, LED2_Pin, BSP_GPIO_PIN_OUT_PP);     // LED2

    // ----------------------
    // 射频模块控制引脚初始化
    // ----------------------
    BSP_GPIO_Init_Output(RF_CE_GPIO_Port, RF_CE_Pin, BSP_GPIO_PIN_OUT_PP);   // RF_CE (输出推挽)
    BSP_GPIO_Init_Output(RF1_CSN_GPIO_Port, RF1_CSN_Pin, BSP_GPIO_PIN_OUT_PP);  // RF_CSN (输出推挽)
    BSP_GPIO_Init_Output(RF2_CSN_GPIO_Port, RF2_CSN_Pin, BSP_GPIO_PIN_OUT_PP);  // RF_CSN (输出推挽)
    // ----------------------
    // 射频模块中断引脚初始化
    // ----------------------
    BSP_GPIO_Init_Exti(RF_IRQ_GPIO_Port, RF_IRQ_Pin, RF_IRQ_EXTI_IRQn,
                       BSP_GPIO_EXTI_IT_FALLING, BSP_GPIO_PIN_NOPULL);

    // 注册射频中断回调函数
    bsp_gpio_exit_irq_register_callback(RF_IRQ_GPIO_Port, RF_IRQ_Pin, RF_IRQ_Callback);

    // ----------------------
    // 按键引脚初始化（PB8/PB9）
    // ----------------------
    BSP_GPIO_Init_Exti(KEY1_GPIO_Port, KEY1_Pin, KEY1_EXTI_IRQn, BSP_GPIO_EXTI_IT_FALLING, BSP_GPIO_PIN_NOPULL);
    BSP_GPIO_Init_Exti(KEY2_GPIO_Port, KEY2_Pin, KEY2_EXTI_IRQn, BSP_GPIO_EXTI_IT_FALLING, BSP_GPIO_PIN_NOPULL);

    // 注册按键中断回调函数
    bsp_gpio_exit_irq_register_callback(KEY1_GPIO_Port, KEY1_Pin, Key1_Callback);
    bsp_gpio_exit_irq_register_callback(KEY2_GPIO_Port, KEY2_Pin, Key2_Callback);


//    BSP_TIM6_Init();
    MX_TIM6_Init();
    HAL_TIM_Base_Start(&htim6);  
    // ----------------------
    // 串口初始化
    // ----------------------
    UART1_Init(115200);  // UART1 - DMA 模式
    UART2_Init(115200);  // UART2 - DMA 模式
    UART3_Init(9600);  // UART3 - 中断模式

    // ----------------------
    // SPI初始化
    // ----------------------
    SPI_Init();
}
