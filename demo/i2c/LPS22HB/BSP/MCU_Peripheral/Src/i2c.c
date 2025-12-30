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

#define LSM6DSOX_I2C_ADDR       0x6A  // SA0=VCC时的7位地址



// 单寄存器写入
HAL_StatusTypeDef LSM_WriteReg(uint8_t reg, uint8_t value) {
  return HAL_I2C_Mem_Write(&hi2c2, 0x6A << 1, 
                          reg, I2C_MEMADD_SIZE_8BIT, 
                          &value, 1, HAL_MAX_DELAY);
}

// 单寄存器读取
HAL_StatusTypeDef LSM_ReadReg(uint8_t reg, uint8_t *data) {
  return HAL_I2C_Mem_Read(&hi2c2, 0x6A << 1,reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
}

// 多寄存器连续读取（用于FIFO等场景）
HAL_StatusTypeDef LSM_ReadMultiReg(uint8_t reg, uint8_t *data, uint16_t len) {
  return HAL_I2C_Mem_Read(&hi2c2, 0x6A, 
                         reg, I2C_MEMADD_SIZE_8BIT, 
                         data, len, HAL_MAX_DELAY);
}


#define LPS22HB_I2C_ADD         0xB8
/* USER CODE BEGIN 1 */
// 单寄存器写入
HAL_StatusTypeDef LPS_WriteReg(uint8_t reg, uint8_t value) {
  return HAL_I2C_Mem_Write(&hi2c2, 0xB8, 
                          reg, I2C_MEMADD_SIZE_8BIT, 
                          &value, 1, HAL_MAX_DELAY);
}

// 单寄存器读取
HAL_StatusTypeDef LPS_ReadReg(uint8_t reg, uint8_t *data) {
  return HAL_I2C_Mem_Read(&hi2c2, 0xB8, 
                         reg, I2C_MEMADD_SIZE_8BIT, 
                         data, 1, HAL_MAX_DELAY);
}


void get_pressure(void)
{
 uint8_t buf[3];
 uint8_t who_am_i1;
    uint8_t who_am_i;
  uint8_t swreset = 1 << 2;
  HAL_I2C_Mem_Write(&hi2c2,0xBB,0x11,I2C_MEMADD_SIZE_8BIT,&swreset,1,0x100);
  uint8_t ctrl_reg2;
  HAL_I2C_Mem_Read(&hi2c2,0xBB,0x11,I2C_MEMADD_SIZE_8BIT,&ctrl_reg2,1,0x100);
  //读取who am i
  HAL_I2C_Mem_Read(&hi2c2,0xBB,0x0f,I2C_MEMADD_SIZE_8BIT,&who_am_i1,1,0x100);
  HAL_I2C_Mem_Read(&hi2c2,0xBB,0x0f,I2C_MEMADD_SIZE_8BIT,&who_am_i,1,0x100);
  //设置odr
  uint8_t odr = 0x04 << 4;//100 = 50Hz
  HAL_I2C_Mem_Write(&hi2c2,0xB9,0x10,I2C_MEMADD_SIZE_8BIT,&odr,1,0x100);
  //得到压力
  if(1)
  {
    HAL_I2C_Mem_Read(&hi2c2,0xB9,0x28+0,I2C_MEMADD_SIZE_8BIT,&buf[0],3,0x100);
    HAL_I2C_Mem_Read(&hi2c2,0xB9,0x28+1,I2C_MEMADD_SIZE_8BIT,&buf[1],3,0x100);
    HAL_I2C_Mem_Read(&hi2c2,0xB9,0x28+2,I2C_MEMADD_SIZE_8BIT,&buf[2],3,0x100);
  }
  else
  {
    HAL_I2C_Mem_Read(&hi2c2,0xB9,0x28,I2C_MEMADD_SIZE_8BIT,buf,3,0x100);
  }
  int32_t press = buf[2] << 16 | buf[1] << 8 | buf[0];
  if(press & 0x00800000)press |= 0xFF000000;
  press = press / 4096;


}












