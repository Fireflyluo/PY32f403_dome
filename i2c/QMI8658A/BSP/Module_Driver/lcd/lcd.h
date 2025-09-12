#ifndef __LCD_H__
#define __LCD_H__

#include "py32f4xx_hal.h"
#include "main.h"
#include "userConfig.h"
#define USE_HORIZONTAL 3  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 80
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 80
#endif

//-----------------LCD端口定义---------------- 

#define LCD_SCLK_Clr() HAL_GPIO_WritePin(SRC_SCL_GPIO_Port, SRC_SCL_Pin,GPIO_PIN_RESET);//SCL=SCLK
#define LCD_SCLK_Set() HAL_GPIO_WritePin(SRC_SCL_GPIO_Port, SRC_SCL_Pin,GPIO_PIN_SET);

#define LCD_MOSI_Clr() HAL_GPIO_WritePin(SRC_SDA_GPIO_Port, SRC_SDA_Pin,GPIO_PIN_RESET);//SCL=SCLK//SDA=MOSI
#define LCD_MOSI_Set() HAL_GPIO_WritePin(SRC_SDA_GPIO_Port, SRC_SDA_Pin,GPIO_PIN_RESET);

#define LCD_RES_Clr()  HAL_GPIO_WritePin(SRC_RES_GPIO_Port, SRC_RES_Pin,GPIO_PIN_RESET);//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(SRC_RES_GPIO_Port, SRC_RES_Pin,GPIO_PIN_RESET);

#define LCD_DC_Clr()   HAL_GPIO_WritePin(SRC_DC_GPIO_Port, SRC_DC_Pin,GPIO_PIN_RESET);//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(SRC_DC_GPIO_Port, SRC_DC_Pin,GPIO_PIN_RESET);
 		     
#define LCD_CS_Clr()   HAL_GPIO_WritePin(SRC_CS_GPIO_Port, SRC_CS_Pin,GPIO_PIN_RESET);//CS
#define LCD_CS_Set()   HAL_GPIO_WritePin(SRC_CS_GPIO_Port, SRC_CS_Pin,GPIO_PIN_RESET);

#define SYSTICK_delay_ms(delay)   HAL_Delay(delay)


#define LCD_PRIORITY 10
#define LCD_STACK_SIZE 256
#define LCD_NORMAL_PERIOD 200
// 表格长宽
#define NUM_ROW 7
#define NUM_COL 2



//画笔颜色 0xF81F 0xFFFF
#define WHITE         	 0x0000
#define BLACK         	 0xFFFF	  
#define BLUE           	 0xFFE0  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0xF800
#define RED           	 0X07FF
#define MAGENTA       	 0x07E0
#define GREEN         	 0xF81F
#define CYAN          	 0x7FFF
#define YELLOW        	 0x001F
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


enum FontSize
    {
        FontSize_12 = 12,
        FontSize_16 = 16,
        FontSize_24 = 24,
        FontSize_32 = 32
    };

void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);//指定区域填充颜色
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);//在指定位置画一个点
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);//在指定位置画一条线
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);//在指定位置画一个矩形
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);//在指定位置画一个圆

void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示汉字串
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个12x12汉字
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个16x16汉字
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个24x24汉字
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个32x32汉字

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示一个字符
uint32_t mypow(uint8_t m,uint8_t n);//求幂
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//显示整数变量
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//显示两位小数变量

void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[]);//显示图片
	
extern void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示字符串
	

void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
void LCD_loop(void);
void lcdGlobalInit(void);


#endif


