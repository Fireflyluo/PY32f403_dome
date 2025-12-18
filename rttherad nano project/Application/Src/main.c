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
-----------------------------------------------File
Info------------------------------------------------
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
#include "drv_gpio.h"
/* example --------------------------------------------------------------------*/
/* 可以从./BSP/example/... 下导入相关示例    */
// GPIO 示例
// #include "gpio_example.h"

// UART 示例
// #include "uart_example.h"

// OSAL 示例
// #include "osal_examplie.h"

#define LED_PIN GET_PIN(B, 2)

/* Private function prototypes -----------------------------------------------*/
static void SystemClockConfig(void);

/**
 * @brief  main 函数
 * @retval int
 */
int main(void)
{
  /* 外设复位，初始化SysTick */
  //  HAL_Init();
  //  /* 配置系统时钟 */
  //  SystemClockConfig();

  /* 选择示例 */
  gpio_init();
  gpio_mode(LED_PIN, GPIO_MODE_OUTPUT_PP);

  while (1)
  {
    /* 点亮LED */
    gpio_write(LED_PIN, GPIO_PIN_SET);
    HAL_Delay(500);

    /* 熄灭LED */
    gpio_write(LED_PIN, GPIO_PIN_RESET);
    HAL_Delay(500);

    /* 翻转LED */
    gpio_toggle(LED_PIN);
    HAL_Delay(500);
  }
}


/*******************************************************************************************/

#ifdef USE_FULL_ASSERT
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
