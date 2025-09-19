#include "lcd.h"
#include "lcd_font.h"

/**
 * @brief : 初始化LCD使用的GPIO引脚
 * @param : 无
 * @note  : 设置SCLK、MOSI、RES、DC、CS引脚
 * @retval: 无
 */
void LCD_GPIO_Init(void)
{
    LCD_SCLK_Set();
    LCD_MOSI_Set();
    LCD_RES_Set();
    LCD_DC_Set();	
    LCD_CS_Set();	
}

/**
 * @brief : LCD串行数据写入函数
 * @param : dat - 要写入的串行数据
 * @note  : 通过SPI协议写入数据
 * @retval: 无
 */
void LCD_Writ_Bus(uint8_t dat) 
{	
    uint8_t i;
    LCD_CS_Clr();
    for(i=0;i<8;i++)
    {			  
        LCD_SCLK_Clr();
        if(dat&0x80)
        {
            LCD_MOSI_Set();
        }
        else
        {
            LCD_MOSI_Clr();
        }
        LCD_SCLK_Set();
        dat<<=1;
    }	
    LCD_CS_Set();	
}

/**
 * @brief : LCD写入8位数据
 * @param : dat - 写入的数据
 * @note  : 调用LCD_Writ_Bus函数写入8位数据
 * @retval: 无
 */
void LCD_WR_DATA8(uint8_t dat)
{
    LCD_Writ_Bus(dat);
}

/**
 * @brief : LCD写入16位数据
 * @param : dat - 写入的数据
 * @note  : 高8位和低8位分别写入
 * @retval: 无
 */
void LCD_WR_DATA(uint16_t dat)
{
    LCD_Writ_Bus(dat>>8);
    LCD_Writ_Bus(dat);
}

/**
 * @brief : LCD写入命令
 * @param : dat - 写入的命令
 * @note  : 先设置DC为低电平，写入命令，然后设置DC为高电平
 * @retval: 无
 */
void LCD_WR_REG(uint8_t dat)
{
    LCD_DC_Clr(); // 写命令
    LCD_Writ_Bus(dat);
    LCD_DC_Set(); // 写数据
}

/**
 * @brief : 设置起始和结束地址
 * @param : x1 - 设置列的起始地址
 *          x2 - 设置列的结束地址
 *          y1 - 设置行的起始地址
 *          y2 - 设置行的结束地址
 * @note  : 根据USE_HORIZONTAL设置不同的地址偏移
 * @retval: 无
 */
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    if(USE_HORIZONTAL==0)
    {
        LCD_WR_REG(0x2a); // 列地址设置
        LCD_WR_DATA(x1+26);
        LCD_WR_DATA(x2+26);
        LCD_WR_REG(0x2b); // 行地址设置
        LCD_WR_DATA(y1+1);
        LCD_WR_DATA(y2+1);
        LCD_WR_REG(0x2c); // 储存器写
    }
    else if(USE_HORIZONTAL==1)
    {
        LCD_WR_REG(0x2a); // 列地址设置
        LCD_WR_DATA(x1+26);
        LCD_WR_DATA(x2+26);
        LCD_WR_REG(0x2b); // 行地址设置
        LCD_WR_DATA(y1+1);
        LCD_WR_DATA(y2+1);
        LCD_WR_REG(0x2c); // 储存器写
    }
    else if(USE_HORIZONTAL==2)
    {
        LCD_WR_REG(0x2a); // 列地址设置
        LCD_WR_DATA(x1+1);
        LCD_WR_DATA(x2+1);
        LCD_WR_REG(0x2b); // 行地址设置
        LCD_WR_DATA(y1+26);
        LCD_WR_DATA(y2+26);
        LCD_WR_REG(0x2c); // 储存器写
    }
    else
    {
        LCD_WR_REG(0x2a); // 列地址设置
        LCD_WR_DATA(x1+1);
        LCD_WR_DATA(x2+1);
        LCD_WR_REG(0x2b); // 行地址设置
        LCD_WR_DATA(y1+26);
        LCD_WR_DATA(y2+26);
        LCD_WR_REG(0x2c); // 储存器写
    }
}

/**
 * @brief : 初始化LCD
 * @param : 无
 * @note  : 初始化GPIO，复位LCD，设置各种寄存器参数
 * @retval: 无
 */
void LCD_Init(void)
{
    LCD_GPIO_Init(); // 初始化GPIO
	
    LCD_RES_Clr(); // 复位
    SYSTICK_delay_ms(100);
    LCD_RES_Set();
    SYSTICK_delay_ms(100);
	
    // LCD_BLK_Set(); // 打开背光
    SYSTICK_delay_ms(100);
	
    LCD_WR_REG(0x11);     // Sleep out
    SYSTICK_delay_ms(120);  // Delay 120ms
    LCD_WR_REG(0xB1);     // Normal mode
    LCD_WR_DATA8(0x05);   
    LCD_WR_DATA8(0x3C);   
    LCD_WR_DATA8(0x3C);   
    LCD_WR_REG(0xB2);     // Idle mode
    LCD_WR_DATA8(0x05);   
    LCD_WR_DATA8(0x3C);   
    LCD_WR_DATA8(0x3C);   
    LCD_WR_REG(0xB3);     // Partial mode
    LCD_WR_DATA8(0x05);   
    LCD_WR_DATA8(0x3C);   
    LCD_WR_DATA8(0x3C);   
    LCD_WR_DATA8(0x05);   
    LCD_WR_DATA8(0x3C);   
    LCD_WR_DATA8(0x3C);   
    LCD_WR_REG(0xB4);     // Dot inversion
    LCD_WR_DATA8(0x03);   
    LCD_WR_REG(0xC0);     // AVDD GVDD
    LCD_WR_DATA8(0xAB);   
    LCD_WR_DATA8(0x0B);   
    LCD_WR_DATA8(0x04);   
    LCD_WR_REG(0xC1);     // VGH VGL
    LCD_WR_DATA8(0xC5);   // C0
    LCD_WR_REG(0xC2);     // Normal Mode
    LCD_WR_DATA8(0x0D);   
    LCD_WR_DATA8(0x00);   
    LCD_WR_REG(0xC3);     // Idle
    LCD_WR_DATA8(0x8D);   
    LCD_WR_DATA8(0x6A);   
    LCD_WR_REG(0xC4);     // Partial+Full
    LCD_WR_DATA8(0x8D);   
    LCD_WR_DATA8(0xEE);   
    LCD_WR_REG(0xC5);     // VCOM
    LCD_WR_DATA8(0x0F);   
    LCD_WR_REG(0xE0);     // positive gamma
    LCD_WR_DATA8(0x07);   
    LCD_WR_DATA8(0x0E);   
    LCD_WR_DATA8(0x08);   
    LCD_WR_DATA8(0x07);   
    LCD_WR_DATA8(0x10);   
    LCD_WR_DATA8(0x07);   
    LCD_WR_DATA8(0x02);   
    LCD_WR_DATA8(0x07);   
    LCD_WR_DATA8(0x09);   
    LCD_WR_DATA8(0x0F);   
    LCD_WR_DATA8(0x25);   
    LCD_WR_DATA8(0x36);   
    LCD_WR_DATA8(0x00);   
    LCD_WR_DATA8(0x08);   
    LCD_WR_DATA8(0x04);   
    LCD_WR_DATA8(0x10);   
    LCD_WR_REG(0xE1);     // negative gamma
    LCD_WR_DATA8(0x0A);   
    LCD_WR_DATA8(0x0D);   
    LCD_WR_DATA8(0x08);   
    LCD_WR_DATA8(0x07);   
    LCD_WR_DATA8(0x0F);   
    LCD_WR_DATA8(0x07);   
    LCD_WR_DATA8(0x02);   
    LCD_WR_DATA8(0x07);   
    LCD_WR_DATA8(0x09);   
    LCD_WR_DATA8(0x0F);   
    LCD_WR_DATA8(0x25);   
    LCD_WR_DATA8(0x35);   
    LCD_WR_DATA8(0x00);   
    LCD_WR_DATA8(0x09);   
    LCD_WR_DATA8(0x04);   
    LCD_WR_DATA8(0x10);
		 
    LCD_WR_REG(0xFC);    
    LCD_WR_DATA8(0x80);  
		
    LCD_WR_REG(0x3A);     
    LCD_WR_DATA8(0x05);   
    LCD_WR_REG(0x36);
    if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
    else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);
    else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x78);
    else LCD_WR_DATA8(0xA8);   
    LCD_WR_REG(0x21);     // Display inversion
    LCD_WR_REG(0x29);     // Display on
    LCD_WR_REG(0x2A);     // Set Column Address
    LCD_WR_DATA8(0x00);   
    LCD_WR_DATA8(0x1A);  // 26  
    LCD_WR_DATA8(0x00);   
    LCD_WR_DATA8(0x69);   // 105 
    LCD_WR_REG(0x2B);     // Set Page Address
    LCD_WR_DATA8(0x00);   
    LCD_WR_DATA8(0x01);    // 1
    LCD_WR_DATA8(0x00);   
    LCD_WR_DATA8(0xA0);    // 160
    LCD_WR_REG(0x2C); 
}

/**
 * @brief : 在指定区域填充颜色
 * @param : xsta,ysta 起始坐标
 *          xend,yend 终止坐标
 *          color 要填充的颜色
 * @note  : 设置显示范围并逐像素填充颜色
 * @retval: 无
 */
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
    uint16_t i,j; 
    LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
    for(i=ysta;i<yend;i++)
    {													   	 	
        for(j=xsta;j<xend;j++)
        {
            LCD_WR_DATA(color);
        }
    } 					  	    
}

/**
 * @brief : 在指定位置画点
 * @param : x,y 画点坐标
 *          color 点的颜色
 * @note  : 设置光标位置并写入颜色数据
 * @retval: 无
 */
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
    LCD_Address_Set(x,y,x,y);//设置光标位置 
    LCD_WR_DATA(color);
} 

/**
 * @brief : 画线
 * @param : x1,y1 起始坐标
 *          x2,y2 终止坐标
 *          color 线的颜色
 * @note  : 使用Bresenham算法绘制直线
 * @retval: 无
 */
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
    uint16_t t; 
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //计算坐标增量 
    delta_y=y2-y1;
    uRow=x1;//画线起点坐标
    uCol=y1;
    if(delta_x>0)incx=1; //设置单步方向 
    else if (delta_x==0)incx=0;//垂直线 
    else {incx=-1;delta_x=-delta_x;}
    if(delta_y>0)incy=1;
    else if (delta_y==0)incy=0;//水平线 
    else {incy=-1;delta_y=-delta_y;}
    if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
    else distance=delta_y;
    for(t=0;t<distance+1;t++)
    {
        LCD_DrawPoint(uRow,uCol,color);//画点
        xerr+=delta_x;
        yerr+=delta_y;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}

/**
 * @brief : 画矩形
 * @param : x1,y1 起始坐标
 *          x2,y2 终止坐标
 *          color 矩形的颜色
 * @note  : 使用四条线段绘制矩形
 * @retval: 无
 */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
    LCD_DrawLine(x1,y1,x2,y1,color);
    LCD_DrawLine(x1,y1,x1,y2,color);
    LCD_DrawLine(x1,y2,x2,y2,color);
    LCD_DrawLine(x2,y1,x2,y2,color);
}

/**
 * @brief : 画圆
 * @param : x0,y0 圆心坐标
 *          r 半径
 *          color 圆的颜色
 * @note  : 使用中点画圆算法绘制圆形
 * @retval: 无
 */
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
    int a,b;
    a=0;b=r;	  
    while(a<=b)
    {
        LCD_DrawPoint(x0-b,y0-a,color);             //3           
        LCD_DrawPoint(x0+b,y0-a,color);             //0           
        LCD_DrawPoint(x0-a,y0+b,color);             //1                
        LCD_DrawPoint(x0-a,y0-b,color);             //2             
        LCD_DrawPoint(x0+b,y0+a,color);             //4               
        LCD_DrawPoint(x0+a,y0-b,color);             //5
        LCD_DrawPoint(x0+a,y0+b,color);             //6 
        LCD_DrawPoint(x0-b,y0+a,color);             //7
        a++;
        if((a*a+b*b)>(r*r))//判断要画的点是否过远
        {
            b--;
        }
    }
}

/**
 * @brief : 显示汉字串
 * @param : x,y 显示坐标
 *          *s 要显示的汉字串
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号 可选 16 24 32
 *          mode: 0非叠加模式 1叠加模式
 * @note  : 根据字号调用对应的汉字显示函数
 * @retval: 无
 */
void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
    while(*s!=0)
    {
        if(sizey==12) LCD_ShowChinese12x12(x,y,s,fc,bc,sizey,mode);
        else if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
        else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
        else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
        else return;
        s+=2;
        x+=sizey;
    }
}
/**
 * @brief : 显示单个12x12汉字
 * @param : x,y 显示坐标
 *          *s 要显示的汉字
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号
 *          mode 0非叠加模式 1叠加模式
 * @note  : 根据点阵字库显示单个12x12汉字
 * @retval: 无
 */
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
    uint8_t i,j,m=0;
    uint16_t k;
    uint16_t HZnum;//汉字数目
    uint16_t TypefaceNum;//一个字符所占字节大小
    uint16_t x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
                         
    HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//统计汉字数目
    for(k=0;k<HZnum;k++) 
    {
        if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
        { 	
            LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {	
                    if(!mode)//非叠加方式
                    {
                        if(tfont12[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
                        else LCD_WR_DATA(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont12[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }				  	
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
} 

/**
 * @brief : 显示单个16x16汉字
 * @param : x,y 显示坐标
 *          *s 要显示的汉字
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号
 *          mode 0非叠加模式 1叠加模式
 * @note  : 根据点阵字库显示单个16x16汉字
 * @retval: 无
 */
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
    uint8_t i,j,m=0;
    uint16_t k;
    uint16_t HZnum;//汉字数目
    uint16_t TypefaceNum;//一个字符所占字节大小
    uint16_t x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字数目
    for(k=0;k<HZnum;k++) 
    {
        if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
        { 	
            LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {	
                    if(!mode)//非叠加方式
                    {
                        if(tfont16[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
                        else LCD_WR_DATA(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }				  	
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
} 

/**
 * @brief : 显示单个24x24汉字
 * @param : x,y 显示坐标
 *          *s 要显示的汉字
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号
 *          mode 0非叠加模式 1叠加模式
 * @note  : 根据点阵字库显示单个24x24汉字
 * @retval: 无
 */
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
    uint8_t i,j,m=0;
    uint16_t k;
    uint16_t HZnum;//汉字数目
    uint16_t TypefaceNum;//一个字符所占字节大小
    uint16_t x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//统计汉字数目
    for(k=0;k<HZnum;k++) 
    {
        if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
        { 	
            LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {	
                    if(!mode)//非叠加方式
                    {
                        if(tfont24[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
                        else LCD_WR_DATA(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }				  	
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
} 

/**
 * @brief : 显示单个32x32汉字
 * @param : x,y 显示坐标
 *          *s 要显示的汉字
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号
 *          mode 0非叠加模式 1叠加模式
 * @note  : 根据点阵字库显示单个32x32汉字
 * @retval: 无
 */
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
    uint8_t i,j,m=0;
    uint16_t k;
    uint16_t HZnum;//汉字数目
    uint16_t TypefaceNum;//一个字符所占字节大小
    uint16_t x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//统计汉字数目
    for(k=0;k<HZnum;k++) 
    {
        if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
        { 	
            LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {	
                    if(!mode)//非叠加方式
                    {
                        if(tfont32[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
                        else LCD_WR_DATA(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }				  	
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/**
 * @brief : 显示单个字符
 * @param : x,y 显示坐标
 *          num 要显示的字符
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号
 *          mode 0非叠加模式 1叠加模式
 * @note  : 根据字体库显示单个字符
 * @retval: 无
 */
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
    uint8_t temp,sizex,t,m=0;
    uint16_t i,TypefaceNum;//一个字符所占字节大小
    uint16_t x0=x;
    sizex=sizey/2;
    TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
    num=num-' ';    //得到偏移后的值
    LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
    for(i=0;i<TypefaceNum;i++)
    { 
        if(sizey==12)temp=ascii_1206[num][i];		       //调用6x12字体
        else if(sizey==16)temp=ascii_1608[num][i];		 //调用8x16字体
        else if(sizey==24)temp=ascii_2412[num][i];		 //调用12x24字体
        else if(sizey==32)temp=ascii_3216[num][i];		 //调用16x32字体
        else return;
        for(t=0;t<8;t++)
        {
            if(!mode)//非叠加方式
            {
                if(temp&(0x01<<t))LCD_WR_DATA(fc);
                else LCD_WR_DATA(bc);
                m++;
                if(m%sizex==0)
                {
                    m=0;
                    break;
                }
            }
            else//叠加方式
            {
                if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//画一个点
                x++;
                if((x-x0)==sizex)
                {
                    x=x0;
                    y++;
                    break;
                }
            }
        }
    }   	 	  	
}

/**
 * @brief : 显示字符串
 * @param : x,y 显示坐标
 *          *p 要显示的字符串
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号
 *          mode 0非叠加模式 1叠加模式
 * @note  : 根据字体库显示字符串
 * @retval: 无
 */
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{         
    while(*p!='\0')
    {       
        LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
        x+=sizey/2;
        p++;
    }  
}

/**
 * @brief : 计算幂
 * @param : m 底数
 *          n 指数
 * @note  : 计算m的n次幂
 * @retval: m的n次幂
 */
uint32_t mypow(uint8_t m,uint8_t n)
{
    uint32_t result=1;	 
    while(n--)result*=m;
    return result;
}

/**
 * @brief : 显示整数变量
 * @param : x,y 显示坐标
 *          num 要显示整数变量
 *          len 要显示的位数
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号
 * @note  : 根据字体库显示整数变量
 * @retval: 无
 */
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{         	
    uint8_t t,temp;
    uint8_t enshow=0;
    uint8_t sizex=sizey/2;
    for(t=0;t<len;t++)
    {
        temp=(num/mypow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
                continue;
            }else enshow=1; 
		 	 
        }
        LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
    }
} 

/**
 * @brief : 显示两位小数变量
 * @param : x,y 显示坐标
 *          num 要显示小数变量
 *          len 要显示的位数
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号
 * @note  : 根据字体库显示两位小数变量
 * @retval: 无
 */
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{         	
    uint8_t t,temp,sizex;
    uint16_t num1;
    sizex=sizey/2;
    num1=num*100;
    for(t=0;t<len;t++)
    {
        temp=(num1/mypow(10,len-t-1))%10;
        if(t==(len-2))
        {
            LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
            t++;
            len+=1;
        }
        LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
    }
}

/**
 * @brief : 显示图片
 * @param : x,y 起点坐标
 *          length 图片长度
 *          width 图片宽度
 *          pic[] 图片数组
 * @note  : 根据图片数组显示图片
 * @retval: 无
 */
void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[])
{
    uint16_t i,j;
    uint32_t k=0;
    LCD_Address_Set(x,y,x+length-1,y+width-1);
    for(i=0;i<length;i++)
    {
        for(j=0;j<width;j++)
        {
            LCD_WR_DATA8(pic[k*2]);
            LCD_WR_DATA8(pic[k*2+1]);
            k++;
        }
    }			
}