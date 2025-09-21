/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @brief   主程序主体
  *          .,:,,,                                        .::,,,,::.
  *        .::::,,;;,                                  .,;;:,,.....:i:
  *        :i,.::::,;i:.      ....,,::::::::::,.....   .;i:,.  ......;i.
  *        :;..:::;::::i;,,:::;:,,,,,,,,,,,..,.,,:::iri:. .,:irsr:,.;i.
  *        ;;..,::::;;;;ri,,,.                    ...,,:;s1s1ssrr;,.;r,
  *        :;. ,::;ii;:,     . .....................     .;iirri;;;,,;i,
  *        ,i. .;ri:.   ... ..............................  .,,:;:,,,;i:
  *        :s,.;r:... ......................................... .::;::s;
  *        ,1r::. .............,,,.,,,:,,.........................,;iir;
  *        ,s;...........     ..::.,,;:,,.          ................,;1s
  *       :i,..,.              .,:,,,::,.          .......... ........;1,
  *      ir,....:rrssr;:,       ,,,.,::.     .r5S9989398G95hr;. .....,.:s,
  *     ;r,..,s9855513XHAG3i   .,,,,,,,,.  ,S931,.,,.;s;s&BHHA8s.,...,..:r:
  *    :r;..rGGh,  :SAG;;G@BS:.,,,,,,,,,,.r83:      hHH1sXMBHHHM3..,,,,,.ir.
  *   ,si,.1GS,   sBMAAX&MBMB5,,,,,,,:,,.:&8       3@HXHBMBHBBH#X,.,,.,,,,rr
  *   ;1:,,SH:   .A@&&B#&8H#BS,,,,,,,,,.,5XS,     3@MHABM&59M#As..,,,,,,:,is,
  *  .rr,,,;9&1   hBHHBB&8AMGr,,,,,,,,,,,:h&&9s;   r9&BMHBHMB9:  . ..,,,,;ri.
  *  :1:....:5&XSi;r8BMBHHA9r:,......,,,,:ii19GG88899XHHH&GSr.      ....,:rs.
  *  ;s.     .:sS8G8GG889hi.        ....,,:;:,.:irssrriii:,.        ....,,i1,
  *  ;1,         ..,....,,isssi;,        .,,.                      .....,.i1,
  *  ;h:               i9HHBMBBHAX9:         .                     ....,,,rs,
  *  ,1i..            :A#MBBBBMHB##s                             .....,,,;si.
  *  .r1,..        ,..;3BMBBBHBB#Bh.     ..                    .....,,,,,i1;
  *   :h;..       .,..;,1XBMMMMBXs,.,, .. :: ,.               .....,,,,,,ss.
  *    ih: ..    .;;;, ;;:s58A3i,..    ,. ,.:,,.             ....,,,,,:,s1,
  *    .s1,....   .,;sh,  ,iSAXs;.    ,.  ,,.i85            ....,,,,,,:i1;
  *     .rh: ...     rXG9XBBM#M#MHAX3hss13&&HHXr         ......,,,,,,,ih;
  *      .s5: .....    i598X&&A&AAAAAA&XG851r:       .........,,,,:,,sh;
  *      . ihr, ...  .         ..                    .........,,,,,;11:.
  *         ,s1i. ...  ..,,,..,,,,.,,.,,.,..       .........,,.,,.;s5i.
  *          .:s1r,......................       ...............;shs,
  *          . .:shr:.  ....     .            ..............,ishs.
  *              .,issr;,... ............................,is1s;.
  *                 .,is1si;:,.....................,:;ir1sr;,
  *                    ..:isssssrrii;:::::::;;iirsssssr;:..
  *                         .,::iiirsssssssssrri;;:.
  *						
  *
  ******************************************************************************
-----------------------------------------------File Info------------------------------------------------
** File Name:               main.c 
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            C程序入口
**--------------------------------------------------------------------------------------------------------
*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <math.h>
/* Private variables ---------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#include "i2c.h"
#include "usart.h"
#include "gyro_device.h"
#include "gyro_factory.h"
#include "qmi8658a_driver.h"
#define QMI8658_WHO_AM_I_REG        0x00  // 文档Table 8
#define QMI8658_WHO_AM_I_VAL        0x05  // 文档固定值

#define LSM_WHO_AM_I_REG         0x0F
#define LSM_WHO_AM_I_VAL         0x6c
#define LPS_WHO_AM_I_VAL        0xB1  // 文档固定值

extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
/* Private function prototypes -----------------------------------------------*/
static void SystemClockConfig(void);


/**
  * @brief  main 函数
  * @retval int
  */
int main(void)
{
  /* 外设复位，初始化SysTick */
  HAL_Init();
  
  /* 配置系统时钟 */
  SystemClockConfig();
	

  MX_GPIO_Init();
  HAL_Delay(1);
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  //检查i2c总线上挂载设备
  I2C2_ScanDevices();
  
  // 1. 初始化
  gyro_factory_init(GYRO_QMI8658A,&hi2c2);
  // 2. 获取传感器接口
  const gyro_device_t *sensor = gyro_factory_get_device();
  // 3. 配置传感器参数
    sensor->set_odr(GYRO_ODR_400HZ);       // 设置 ODR 为 100Hz
    sensor->set_range(GYRO_RANGE_500DPS);  // 设置量程为 ±500DPS
    // 4. 循环读取数据
    sensor_data_t data;
    while (1) {
        int32_t ret = sensor->read_data(&data);
            if (ret == 0) {
                printf("Accel: %.2f m/s², %.2f m/s², %.2f m/s²\n",
                       data.accel[0], data.accel[1], data.accel[2]);
                printf("Gyro: %.2f rad/s, %.2f rad/s, %.2f rad/s\n",
                       data.gyro[0], data.gyro[1], data.gyro[2]);
                printf("Temp: %.2f °C\n", data.temp);
                
                int x,y,z,a;
                float a2;
                x=data.accel[0];
                y=data.accel[1];
                z=data.accel[2];
                a=(x*x)+(y*y)+(z*z);
                a2=sqrt(a);
//                printf("%.2f,%.2f,%.2f\n",
//                       data.accel[0], data.accel[1], data.accel[2]);
                printf("%.2f\n",a2);
               HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);

            } else {
                printf("Failed to read data!\n");
            }
        HAL_Delay(100); // 延时 100ms
      }

}


/**
  * @brief  系统时钟配置
  * @param  None
  * @retval None
  */
static void SystemClockConfig(void)
{
  RCC_OscInitTypeDef OscInitstruct = {0};
  RCC_ClkInitTypeDef ClkInitstruct = {0};
  
  // 配置振荡器类型，启用多种振荡器检测
  OscInitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE |
                                 RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSI48M;
  OscInitstruct.HSEState = RCC_HSE_ON;                              // 启用外部高速时钟 (HSE)
  OscInitstruct.HSEFreq = RCC_HSE_16_32MHz;                         // 选择HSE频率为16-32MHz
  OscInitstruct.HSI48MState = RCC_HSI48M_ON;                       // 禁用48MHz内部RC振荡器 (HSI48M)
  OscInitstruct.HSIState = RCC_HSI_ON;                              // 启用内部高速时钟 (HSI)
  OscInitstruct.LSEState = RCC_LSE_OFF;                             // 禁用外部低速时钟 (LSE)
  // OscInitstruct.LSEDriver = RCC_LSEDRIVE_HIGH;                   // 设置驱动能力等级为高
  OscInitstruct.LSIState = RCC_LSI_OFF;                             // 禁用内部低速时钟 (LSI)
  OscInitstruct.PLL.PLLState = RCC_PLL_ON;                          // 启用PLL
  OscInitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;                  // PLL时钟源选择HSE
  OscInitstruct.PLL.PLLMUL = RCC_PLL_MUL9;                          // PLL倍频系数为9   16*9=144MHz

  // 配置振荡器
  if(HAL_RCC_OscConfig(&OscInitstruct) != HAL_OK)
  {
    APP_ErrorHandler(); // 错误处理函数
  }
  
  // 配置系统时钟
  ClkInitstruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  ClkInitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;             // 系统时钟源选择PLL输出PLLCLK
  ClkInitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                    // AHB总线不分频
  ClkInitstruct.APB1CLKDivider = RCC_HCLK_DIV1;                     // APB1总线不分频
  ClkInitstruct.APB2CLKDivider = RCC_HCLK_DIV2;                     // APB2总线分频2
  
  /* 
   * 重新初始化RCC时钟
   * -- clock <= 24MHz: FLASH_LATENCY_0
   * -- clock > 24MHz:  FLASH_LATENCY_1
   */
  // 配置时钟
  if(HAL_RCC_ClockConfig(&ClkInitstruct, FLASH_LATENCY_6) != HAL_OK)
  {
    APP_ErrorHandler(); // 错误处理函数
  }
}
/***********************************Printf重定向********************************************************/
 int fputc(int ch, FILE *f) {
     HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
     return ch;
 }


#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE {
   HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
   return ch;
}

#ifdef __GNUC__
int _write(int fd, char *ptr, int len) {
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}
#endif

/*******************************************************************************************/
/**
  * @brief  在发生错误时执行此函数。
  * @param  None
  * @retval None
  */
void APP_ErrorHandler(void)
{
  /* 无限循环 */
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  报告发生assert_param错误的源文件名和源代码行号。
  * @param  file: 指向源文件名的指针
  * @param  line: assert_param错误的源代码行号
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* 用户可以添加自己的实现来报告文件名和行号，
     例如: printf("错误参数值: 文件 %s 在第 %d 行\r\n", file, line)  */
  /* 无限循环 */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
