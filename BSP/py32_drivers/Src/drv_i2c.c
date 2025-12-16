#include "drv_i2c.h"
#include "drv_gpio.h"
/* ==================== 类型定义和全局变量 ============================================ */
//typedef struct
//{
//    I2C_HandleTypeDef hi2c;

//    /* 回调函数 */
//    i2c_tx_complete_callback_t tx_complete_callback;
//    void *tx_complete_callback_arg;

//    i2c_rx_complete_callback_t rx_complete_callback;
//    void *rx_complete_callback_arg;

//    i2c_rx_half_complete_callback_t rx_half_complete_callback;
//    void *rx_half_complete_callback_arg;

//    i2c_error_callback_t error_callback;
//    void *error_callback_arg;

//#if (I2C_USE_DMA == 1)
//    i2c_dma_idle_callback_t dma_idle_callback;
//    void *dma_idle_callback_arg;
//#endif

//    /* 状态标志 */
//    volatile bool tx_busy;
//    volatile bool rx_busy;

//    /* 统计信息 */
//    uint32_t tx_total;    // 发送总字节数
//    uint32_t rx_total;    // 接收总字节数
//    uint32_t error_count; // 发生的错误次数

//#if (I2C_USE_DMA == 1)
//    /* DMA特定字段 */
//    DMA_HandleTypeDef hdma_tx;
//    DMA_HandleTypeDef hdma_rx;

//    uint8_t dma_rx_buffer[UART_DMA_BUFFER_SIZE];
//    uint8_t dma_tx_buffer[UART_DMA_BUFFER_SIZE];

//    volatile uint16_t dma_rx_write_pos;  // DMA写入位置
//    volatile uint16_t dma_rx_read_pos;   // 应用读取位置
//    volatile uint16_t dma_rx_data_count; // 当前接收数据量
//    bool dma_rx_circular_mode;           // 循环模式标志
//    bool dma_tx_busy;                    // DMA发送忙标志
//    bool dma_rx_busy;                    // DMA接收忙标志

//    /* 环形缓冲区（使用drv_tool中的实现） */
//    ring_buffer_t rx_ring_buffer;
//    ring_buffer_t tx_ring_buffer;
//    uint8_t rx_software_buffer[UART_RX_BUFFER_SIZE];
//    uint8_t tx_software_buffer[UART_TX_BUFFER_SIZE];
//#endif

//}i2c_device_t;
