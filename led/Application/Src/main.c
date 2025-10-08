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
** Created date:            2025.9.21
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            C程序入口
**                          包含一个最简单的点灯程序
**--------------------------------------------------------------------------------------------------------
*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "ws2812.h"
#include "dev_led.h"
/* Private variables ---------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define code0   60
#define code1   120
#define code2   0
/* Private function prototypes -----------------------------------------------*/
static void SystemClockConfig(void);
uint32_t R_data[25]={
                    code0,code0,code0,code0,code0,code0,code0,code0,
                    code1,code1,code1,code1,code1,code1,code1,code1,
                    code0,code0,code0,code0,code0,code0,code0,code0,
                    code2
                    };
uint32_t G_data[25]={
                    code1,code1,code1,code1,code1,code1,code1,code1,
                    code0,code0,code0,code0,code0,code0,code0,code0,
                    code0,code0,code0,code0,code0,code0,code0,code0,
                    code2
                    };
uint32_t B_data[25]={
                    code0,code0,code0,code0,code0,code0,code0,code0,
                    code0,code0,code0,code0,code0,code0,code0,code0,
                    code1,code1,code1,code1,code1,code1,code1,code1,
                    code2
                    };
     
                    
// GPIO LED配置示例
static const led_config_t gpio_led_config = {
    .type = LED_TYPE_GPIO,
    .gpio = {
        .gpiox = GPIOB,
        .pin = GPIO_PIN_2
    }
};

// WS2812 LED配置示例
static const led_config_t ws2812_led_config = {
    .type = LED_TYPE_ADDRESSABLE,
    .addressable = {
        .gpiox = GPIOA,
        .pin = GPIO_PIN_1,
        .led_count = 1  // 1个LED灯珠
    }
};
/**
  * @brief  main 函数
  * @retval int
  */

#define EXAMPLE_MODE 1  // 1: 示例1, 2: 示例2
int main(void)
{
  /* 外设复位，初始化SysTick */
  HAL_Init();
  /* 配置系统时钟 */
  SystemClockConfig();
  
//    HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 1, 0);
//    HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);
    
  MX_GPIO_Init();
  MX_TIM2_Init();
  

#if EXAMPLE_MODE == 1

   // 打开GPIO LED设备
    dev_led_handle_t gpio_led = dev_led_open(&gpio_led_config);
    if (gpio_led == NULL) {
        // 错误处理
        APP_ErrorHandler();
    }
    
    // 打开WS2812 LED设备
    dev_led_handle_t ws2812_led = dev_led_open(&ws2812_led_config);
    if (ws2812_led == NULL) {
        // 错误处理
        APP_ErrorHandler();
    }

    
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  
//        // 控制GPIO LED - 点亮
//        led_state_t state = LED_STATE_ON;
//        dev_led_ioctl(gpio_led, LED_IOCTL_SET_STATE, &state);
//        HAL_Delay(500);
//        
//        // 控制GPIO LED - 熄灭
//        state = LED_STATE_OFF;
//        dev_led_ioctl(gpio_led, LED_IOCTL_SET_STATE, &state);
//        HAL_Delay(500);

        // 通过write接口控制WS2812 LED - 设置为紫色 (GRB格式)
        uint8_t purple_color[3] = {0xFF, 0xFF, 0xFF}; // GRB: Green=0xFF, Red=0x00, Blue=0xFF 实际显示紫色
        dev_led_write(ws2812_led, purple_color, 3);
        HAL_Delay(1000);
//        
//        // 控制WS2812 LED - 设置为红色
//        uint32_t red_color = 0xFF0000;
//        dev_led_ioctl(ws2812_led, LED_IOCTL_SET_RGB_COLOR, &red_color);
//        HAL_Delay(1000);
//        
//        // 控制WS2812 LED - 设置为绿色
//        uint32_t green_color = 0x00FF00;
//        dev_led_ioctl(ws2812_led, LED_IOCTL_SET_RGB_COLOR, &green_color);
//        HAL_Delay(1000);
//        
//        // 控制WS2812 LED - 设置为蓝色
//        uint32_t blue_color = 0x0000FF;
//        dev_led_ioctl(ws2812_led, LED_IOCTL_SET_RGB_COLOR, &blue_color);
//        HAL_Delay(1000);
//        
//        // 通过write接口控制GPIO LED - 点亮 (写入非0值)
//        uint8_t led_on = 1;
//        dev_led_write(gpio_led, &led_on, 1);
//        HAL_Delay(500);
//        
//        // 通过write接口控制GPIO LED - 熄灭 (写入0值)
//        uint8_t led_off = 0;
//        dev_led_write(gpio_led, &led_off, 1);
//        HAL_Delay(500);


  }
  #endif
#if EXAMPLE_MODE == 2


WS2812_Init(&htim2,TIM_CHANNEL_2);
  while(1)
  {
    for(uint8_t r=0;r <= 0xFF;r++)
    {
      WS2812_Set_All(r,-r,-r);
      WS2812_Updata();
  // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
      HAL_Delay(10);
      if(r % 50 == 0)
      {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
      }
    }
  }
 
#endif
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
//int fputc(int ch, FILE* f)
//{
//	HAL_UART_Transmit_DMA(&huart2,(uint8_t *)&ch,1);//,1000
//  
//    return (ch);
//}
//int fgetc(FILE *f)
//{

//  uint8_t ch = 0;
//	HAL_UART_Receive(&huart2,&ch,1,0xffff);
//	return ch;
//}
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
