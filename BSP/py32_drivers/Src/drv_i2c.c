#include "drv_include.h"
#include <string.h>
/* ==================== 类型定义和全局变量 ============================================ */
typedef struct
{
  I2C_HandleTypeDef hi2c;
  i2c_mode_t mode;

  /* 回调函数 */
  i2c_tx_complete_callback_t tx_complete_callback;
  void *tx_complete_callback_arg;

  i2c_rx_complete_callback_t rx_complete_callback;
  void *rx_complete_callback_arg;

  i2c_error_callback_t error_callback;
  void *error_callback_arg;
#if (I2C_USE_DMA == 1)
  i2c_dma_idle_callback_t dma_idle_callback;
  void *dma_idle_callback_arg;
#endif
  /* 状态标志 */
  volatile bool tx_busy;
  volatile bool rx_busy;

  /* 统计信息 */
  uint32_t tx_total;    // 发送总字节数
  uint32_t rx_total;    // 接收总字节数
  uint32_t error_count; // 发生的错误次数
#if (I2C_USE_DMA == 1)
  /* DMA特定字段 */
  DMA_HandleTypeDef hdma_tx;
  DMA_HandleTypeDef hdma_rx;

  uint8_t dma_rx_buffer[UART_DMA_BUFFER_SIZE]; /* DMA接收缓冲区 */
  uint8_t dma_tx_buffer[UART_DMA_BUFFER_SIZE]; /* DMA发送缓冲区 */

  /* DMA接收状态管理 */
  volatile uint16_t dma_rx_current_pos;   /* DMA当前写入位置 */
  volatile uint16_t dma_rx_last_idle_pos; /* 上次空闲中断时的位置 */
  volatile bool dma_rx_half_handled;      /* 半满中断已处理标志 */
  volatile bool dma_rx_full_handled;      /* 全满中断已处理标志 */
  bool dma_rx_circular_mode;              /* 循环模式标志 */
  bool dma_tx_busy;                       /* DMA发送忙标志 */
  bool dma_rx_busy;                       /* DMA接收忙标志 */

  /* 环形缓冲区 */
  ring_buffer_t rx_ring_buffer;
  ring_buffer_t tx_ring_buffer;
  uint8_t rx_software_buffer[I2C_RX_BUFFER_SIZE];
  uint8_t tx_software_buffer[I2C_TX_BUFFER_SIZE];
#endif

} i2c_device_t;

/* 全局设备数组 */
static i2c_device_t i2c_devices[I2C_INSTANCE_MAX];

/* i2c基地址映射 */
static I2C_TypeDef *const i2c_bases[I2C_INSTANCE_MAX] = {
    I2C1, I2C2};

/* i2c实例名称 */
static const char *i2c_instance_names[I2C_INSTANCE_MAX] = {
    "I2C1", "I2C2"};

/* i2c中断号映射 */
static const IRQn_Type i2c_irqs[I2C_INSTANCE_MAX] = {
    I2C1_EV_IRQn, I2C1_EV_IRQn};

/* ==================== 静态函数前向声明 ==================== */
static i2c_err_t i2c_configure_irq_priority(i2c_instance_t instance);
#if (I2C_USE_DMA == 1)
/* DMA相关静态函数 */
static i2c_err_t i2c_dma_init(i2c_instance_t instance);
static i2c_err_t i2c_dma_deinit(i2c_instance_t instance);
static void i2c_dma_process_half_data(i2c_instance_t instance);
static void i2c_dma_process_full_data(i2c_instance_t instance);
static uint16_t i2c_dma_process_idle_data(i2c_instance_t instance);
static i2c_err_t i2c_start_tx_from_ring_buffer(i2c_instance_t instance);
#endif
/* ==================== 工具函数 ==================== */
/* 获取 i2c 设备指针 */
static i2c_device_t *get_i2c_device(i2c_instance_t instance)
{
  if (instance >= I2C_INSTANCE_MAX)
  {
    return NULL;
  }
  return &i2c_devices[instance];
}

/* 检查 i2c 是否已初始化 */
static bool is_i2c_initialized(i2c_instance_t instance)
{
  i2c_device_t *dev = get_i2c_device(instance);
  return (dev != NULL && dev->hi2c.Instance != NULL);
}

/* ==================== 初始化/反初始化 ==================== */
/**
 * @brief 初始化 i2c 实例
 */
i2c_err_t i2c_init(i2c_instance_t instance, i2c_mode_t mode)
{
  if (instance >= I2C_INSTANCE_MAX)
  {
    return I2C_ERROR_PARAM;
  }
  /* 检查模式有效性 */
  if (mode == I2C_MODE_DMA)
  {
#if (I2C_USE_DMA != 1)
    return I2C_ERROR_MODE; /* DMA模式未启用 */
#endif
  }

  i2c_device_t *dev = &i2c_devices[instance];

  /* 初始化检查 */
  if (dev->hi2c.Instance != NULL)
  {
    return I2C_ERROR_BUSY;
  }

  /* 初始化设备结构 */
  memset(dev, 0, sizeof(i2c_device_t));

  /* 配置 i2c 句柄 */
  /* I2C initialization */
  dev->hi2c.Instance = i2c_bases[instance];                /* I2C */
  dev->hi2c.Init.ClockSpeed = I2C_SPEEDCLOCK;              /* I2C communication speed */
  dev->hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;                /* I2C Duty cycle */
  dev->hi2c.Init.OwnAddress1 = 0;                /* I2C address */
  dev->hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT; /* 7-bit Addressing mode */
  dev->hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; /* Do not enable dual address */
  /* dev->hi2c.Init.OwnAddress2     = I2C_ADDRESS; */       /* Second address */
  dev->hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; /* Disable general call */
  dev->hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;     /* Enable clock stretching */

  /* 初始化UART */
  if (HAL_I2C_Init(&dev->hi2c) != HAL_OK)
  {
    return I2C_ERROR;
  }

#if (I2C_USE_DMA == 1)
  /* 初始化DMA */
  if (mode == I2C_MODE_DMA)
  {
    i2c_err_t dma_result = i2c_dma_init(instance);
    if (dma_result != UART_OK)
    {
      HAL_I2C_DeInit(&dev->hi2c);
      return dma_result;
    }
  }
#endif

  if (mode != I2C_MODE_POLLING)
  {
    /* 配置中断优先级 */
    i2c_configure_irq_priority(instance);
  }
  return I2C_OK;
}
/* ==================================== 中断相关函数 ==================================================== */
/* 配置中断优先级 */
static i2c_err_t i2c_configure_irq_priority(i2c_instance_t instance)
{
  if (instance >= I2C_INSTANCE_MAX)
  {
    return I2C_ERROR_PARAM;
  }

  IRQn_Type irq = i2c_irqs[instance];

  /* 设置UART中断优先级 */
  HAL_NVIC_SetPriority(irq, 1, 1);
  HAL_NVIC_EnableIRQ(irq);

#if (I2C_USE_DMA == 1)
  /* 设置DMA中断优先级 */
  i2c_device_t *dev = &i2c_devices[instance];
  if (dev->mode == I2C_MODE_DMA)
  {
    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 1, 0);
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  }
#endif

  return I2C_OK;
}
/* ====================================== HAL回调处理 ============================================ */
/**
 * @brief This function handles I2C1 event Interrupt .
 */
void I2C1_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&i2c_devices[0].hi2c);
}

/**
 * @brief This function handles I2C1 error Interrupt .
 */
void I2C1_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&i2c_devices[0].hi2c);
}
/**
 * @brief This function handles I2C1 event Interrupt .
 */
void I2C2_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&i2c_devices[1].hi2c);
}

/**
 * @brief This function handles I2C1 error Interrupt .
 */
void I2C2_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&i2c_devices[1].hi2c);
}

/* ==================== hal库常规使用方法，临时使用 ============================================ */
I2C_HandleTypeDef hi2c2;

/* I2C2 init function */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */
}

/* 全局变量声明 */
uint8_t i2c2_device_list[112]; // 存储找到的设备地址（最多 112 个 7 位地址）
uint8_t i2c2_device_count = 0; // 找到的设备数量

/**
 * @brief  扫描 I2C2 总线上的所有从机设备
 * @param  无
 * @retval 无
 */
void I2C2_ScanDevices(void)
{
  HAL_StatusTypeDef status;
  uint8_t addr_7bit; // 7 位设备地址（0x08 ~ 0x77）
  uint8_t addr_8bit; // 8 位地址（7 位地址左移 1 位 + 写位 0）

  /* 遍历 7 位 I2C 地址范围（0x08 ~ 0x77 是有效从机地址范围） */
  for (addr_7bit = 0x08; addr_7bit <= 0x77; addr_7bit++)
  {
    /* 构造 8 位发送地址（7 位地址左移 1 位，最低位为写操作位 0） */
    addr_8bit = (addr_7bit << 1) | 0x00;

    /* 调用 HAL 库函数发送地址并检测 ACK */
    /* 参数说明：I2C2 句柄、8 位地址、无数据发送（NULL）、数据长度 0、超时时间 100ms */
    status = HAL_I2C_Master_Transmit(&hi2c2, addr_8bit, NULL, 0, 1000);

    /* 判断是否收到 ACK（HAL_OK 表示从机响应） */
    if (status == HAL_OK)
    {
      i2c2_device_list[i2c2_device_count++] = addr_7bit;

      /* 可选：实时打印找到的设备地址（需串口已初始化） */
      printf("I2C2 : 0x%02X\n", addr_7bit);
    }
  }

  /* 扫描结果汇总 */
  if (i2c2_device_count == 0)
  {
    printf("I2C2 NO");
  }
  else
  {
    printf("I2C2 find device %d :", i2c2_device_count);
    for (uint8_t i = 0; i < i2c2_device_count; i++)
    {
      printf("0x%02X ", i2c2_device_list[i]);
    }
    printf("\n");
  }
}
