#include "i2c.h"
#include "gpio.h"
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

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();

    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
}

/* 全局变量声明 */
uint8_t i2c2_device_list[112];  // 存储找到的设备地址（最多 112 个 7 位地址）
uint8_t i2c2_device_count = 0;   // 找到的设备数量

/**
  * @brief  扫描 I2C2 总线上的所有从机设备
  * @param  无
  * @retval 无
  */
void I2C2_ScanDevices(void)
{
    HAL_StatusTypeDef status;
    uint8_t addr_7bit;          // 7 位设备地址（0x08 ~ 0x77）
    uint8_t addr_8bit;          // 8 位地址（7 位地址左移 1 位 + 写位 0）

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
            printf("I2C2 : 0x%02X", addr_7bit);
        }
    }

    /* 扫描结果汇总 */
    if (i2c2_device_count == 0)
    {
        printf("I2C2 NO");
    }
    else
    {
     printf("I2C2 get %d :", i2c2_device_count);
        for (uint8_t i = 0; i < i2c2_device_count; i++)
        {
            printf("0x%02X ", i2c2_device_list[i]);
        }
        printf("/n");
    }
}











