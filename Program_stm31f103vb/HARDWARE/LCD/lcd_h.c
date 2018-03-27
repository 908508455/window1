#include "lcd_h.h"
#include "stdlib.h"

#include "usart.h"	 
#include "delay.h"

#include "ASCII_1608.h"
#include "ASCII_2412.h"
#include "ASCII_3216.h"
#include "ASCII_6432.h"	
#include "fontlib.h"

				 
//LCD1的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 

u16 cur_xx=0;

	 
//写寄存器函数
//regval:寄存器值
void LCD1_WR_REG(u16 regval)
{ 
	LCD1->LCD_REG=regval;//写入要写的寄存器序号	 
}
//写LCD1数据
//data:要写入的值
void LCD1_WR_DATA(u16 data)
{										    	   
	LCD1->LCD_RAM=data;		 
}
//读LCD1数据
//返回值:读到的值
u16 LCD1_RD_DATA(void)
{										    	   
	return LCD1->LCD_RAM;		 
}					   
//写寄存器
//LCD1_Reg:寄存器地址
//LCD1_RegValue:要写入的数据
void LCD1_WriteReg(u16 LCD1_Reg, u16 LCD1_RegValue)
{	
	LCD1->LCD_REG = LCD1_Reg;		//写入要写的寄存器序号	 
	LCD1->LCD_RAM = LCD1_RegValue;//写入数据	    		 
}	   
//读寄存器
//LCD1_Reg:寄存器地址
//返回值:读到的数据
u16 LCD1_ReadReg(u16 LCD1_Reg)
{										   
	LCD1_WR_REG(LCD1_Reg);		//写入要读的寄存器序号
	delay_us(5);		  
	return LCD1_RD_DATA();		//返回读到的值
}   
//开始写GRAM
void LCD1_WriteRAM_Prepare(void)
{
 	LCD1->LCD_REG=0x002c;	  
}	 
//LCD1写GRAM
//RGB_Code:颜色值
void LCD1_WriteRAM(u16 RGB_Code)
{							    
	LCD1->LCD_RAM = RGB_Code;//写十六位GRAM
}
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 LCD1_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(u8 i)
{
	while(i--);
}
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
u16 LCD1_ReadPoint(u16 x,u16 y)
{
	u16 dat=0;
	LCD1_SetCursor(x,y);		//设置光标位置 
	LCD1_WR_REG(0x2e);
	dat = LCD1->LCD_RAM;	
	dat = LCD1->LCD_RAM;
	return dat;		
}			 
//LCD1开启显示
void LCD1_DisplayOn(void)
{					   
	LCD1_WR_REG(0x0029);
}	 
//LCD1关闭显示
void LCD1_DisplayOff(void)
{	   
	LCD1_WR_REG(0x0028);//关闭显示 
}  
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD1_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD1_WR_REG(0x002A);LCD1_WR_DATA(Xpos>>8);
	LCD1_WR_DATA(0x00FF&Xpos); //设定X坐标
	LCD1_WR_DATA(0x0001);LCD1_WR_DATA(0x00df);

	LCD1_WR_REG(0x002B);LCD1_WR_DATA(Ypos>>8);
	LCD1_WR_DATA(0x00FF&Ypos); //设定Y坐标	
	LCD1_WR_DATA(0x0001);LCD1_WR_DATA(0x003F);

} 		 
//设置LCD1的自动扫描方向
//注意:其他函数可能会受到此函数设置的影响(尤其是9341/6804这两个奇葩),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见LCD1.h)
//9320/9325/9328/4531/4535/1505/b505/8989/5408/9341/5310/5510等IC已经实际测试	   	   
void LCD1_Scan_Dir(u8 dir)
{

}   
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD1_DrawPoint(u16 x,u16 y)
{
	LCD1_SetCursor(x,y);		//设置光标位置 
	LCD1_WR_REG(0x002C);
	LCD1_WR_DATA(POINT_COLOR); 
}
//快速画点
//x,y:坐标
//color:颜色
void LCD1_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   

}	 


//设置LCD1显示方向
//dir:0,竖屏；1,横屏
void LCD1_Display_Dir(u8 dir)
{

}	 
//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height.
//68042,横屏时不支持窗口设置!! 
void LCD1_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{   

} 
//初始化LCD1
//该初始化函数可以初始化各种ILI93XX液晶,但是其他函数是基于ILI9320的!!!
//在其他型号的驱动芯片上没有测试! 
void LCD1_Init(void)
{ 										  
 	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//使能FSMC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO,ENABLE);//使能PORTB,D,E,G以及AFIO复用功能时钟

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6;			 //PD3 推挽输出 背光
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);

 	//PORTD复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	  
	//PORTE复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	  
 
	readWriteTiming.FSMC_AddressSetupTime = 0x02;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
    
	writeTiming.FSMC_AddressSetupTime = 0x02;	 //地址建立时间（ADDSET）为1个HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A		
    writeTiming.FSMC_DataSetupTime = 0x03;		 ////数据保存时间为4个HCLK	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 

 
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  // 使能BANK1 
	
	LCD1_RST = 1;			 
 	delay_ms(5); // delay 50 ms 
 	LCD1_RST = 0;
	delay_ms(100); // delay 50 ms 
	LCD1_RST = 1;
	delay_ms(150); // delay 50 ms 
									
	LCD1_WR_REG(0XF2);
	LCD1_WR_DATA(0x18);
	LCD1_WR_DATA(0xA3);
	LCD1_WR_DATA(0x12);
	LCD1_WR_DATA(0x02);
	LCD1_WR_DATA(0XB2);
	LCD1_WR_DATA(0x12);
	LCD1_WR_DATA(0xFF);
	LCD1_WR_DATA(0x10);
	LCD1_WR_DATA(0x00);

	LCD1_WR_REG(0XF8);
	LCD1_WR_DATA(0x21);
	LCD1_WR_DATA(0x04);

	LCD1_WR_REG(0XF9);
	LCD1_WR_DATA(0x00);
	LCD1_WR_DATA(0x08);

	LCD1_WR_REG(0x36);
	LCD1_WR_DATA(0xa8);//08

	LCD1_WR_REG(0x3A);
	LCD1_WR_DATA(0x05);

	LCD1_WR_REG(0xB4);
	LCD1_WR_DATA(0x01);//0x00

	LCD1_WR_REG(0xB6);
	LCD1_WR_DATA(0x02);
	LCD1_WR_DATA(0x22);

	LCD1_WR_REG(0xC1);
	LCD1_WR_DATA(0x41);

	LCD1_WR_REG(0xC5);
	LCD1_WR_DATA(0x00);
	LCD1_WR_DATA(0x07);//0X18

	LCD1_WR_REG(0xE0);
	LCD1_WR_DATA(0x0F);
	LCD1_WR_DATA(0x1F);
	LCD1_WR_DATA(0x1C);
	LCD1_WR_DATA(0x0C);
	LCD1_WR_DATA(0x0F);
	LCD1_WR_DATA(0x08);
	LCD1_WR_DATA(0x48);
	LCD1_WR_DATA(0x98);
	LCD1_WR_DATA(0x37);
	LCD1_WR_DATA(0x0A);
	LCD1_WR_DATA(0x13);
	LCD1_WR_DATA(0x04);
	LCD1_WR_DATA(0x11);
	LCD1_WR_DATA(0x0D);
	LCD1_WR_DATA(0x00);

	LCD1_WR_REG(0xE1);
	LCD1_WR_DATA(0x0F);
	LCD1_WR_DATA(0x32);
	LCD1_WR_DATA(0x2E);
	LCD1_WR_DATA(0x0B);
	LCD1_WR_DATA(0x0D);
	LCD1_WR_DATA(0x05);
	LCD1_WR_DATA(0x47);
	LCD1_WR_DATA(0x75);
	LCD1_WR_DATA(0x37);
	LCD1_WR_DATA(0x06);
	LCD1_WR_DATA(0x10);
	LCD1_WR_DATA(0x03);
	LCD1_WR_DATA(0x24);
	LCD1_WR_DATA(0x20);
	LCD1_WR_DATA(0x00);

	LCD1_WR_REG(0x11);

	delay_ms(120);

	LCD1_WR_REG(0x29);

	LCD1_LED=1;					//点亮背光
	LCD1_Clear(WHITE);

}  
//清屏函数
//color:要清屏的填充色
void LCD1_Clear(u16 color)
{
	u32 index=0;      

	LCD1_SetCursor(0x00,0x00);	//设置光标位置 
	LCD1_WR_REG(0x002C);   
	for(index=0;index<153600;index++)
		LCD1_WR_DATA(color);   
}
 
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD1_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	/*u16 i,j;
	 
	for(i=0;i<ey;i++)
	{
	 	LCD1_SetCursor(sx,sy+i);      				//设置光标位置 
		LCD1_WR_REG(0x002C);   			//开始写入GRAM	  
		for(j=0;j<ex;j++)
			LCD1_WR_DATA(color);	//设置光标位置 	    
	}*/
	
	
		u16 i,j;
	u16 xlen=0; 
	xlen=ex-sx+1;	 
	for(i=sy;i<=ey;i++)
	{
	 	LCD1_SetCursor(sx,i);      				//设置光标位置 
		LCD1_WR_REG(0x002C);   			//开始写入GRAM	  
		for(j=0;j<xlen;j++)
			LCD1_WR_DATA(color);	//设置光标位置 	    
	}
	
		 
}  
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD1_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
 	for(i=0;i<height;i++)
	{
 		LCD1_SetCursor(sx,sy+i);   	//设置光标位置 
		LCD1_WR_REG(0x002C);   //开始写入GRAM
		for(j=0;j<width;j++)LCD1->LCD_RAM=color[i*height+j];//写入数据 
	}	  
}  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD1_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD1_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
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
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD1_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD1_DrawLine(x1,y1,x2,y1);
	LCD1_DrawLine(x1,y1,x1,y2);
	LCD1_DrawLine(x1,y2,x2,y2);
	LCD1_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD1_DrawPoint(x0+a,y0-b);             //5
 		LCD1_DrawPoint(x0+b,y0-a);             //0           
		LCD1_DrawPoint(x0+b,y0+a);             //4               
		LCD1_DrawPoint(x0+a,y0+b);             //6 
		LCD1_DrawPoint(x0-a,y0+b);             //1       
 		LCD1_DrawPoint(x0-b,y0+a);             
		LCD1_DrawPoint(x0-a,y0-b);             //2             
  		LCD1_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void LCD1_ShowChar(u16 x,u16 y,char s,u8 size,u8 mode)
{  							  
    u8 asc;
	u16 colortemp=POINT_COLOR,i,j;
	u16 tx=x;
	u8 const *sp;
	u16 byts;//字节数
	if(size==16){
		sp = &ASCII_1608[(s-0x20)][0];
		byts = 16;
	}
	else if(size==24){
		sp = &ASCII_2412[(s-0x20)*(3)][0];
		byts = 48;
	}
	else if(size==32){
		sp = &ASCII_3216[(s-0x20)*(4)][0];
		byts = 64;
	}
	else if(size==64)//只显示数字
	{		
		if(s=='.') sp = &ASCII_6432[(10)*(16)][0];
		else sp = &ASCII_6432[(s-0x30)*(16)][0];
		byts = 256;
	}
	else return ;//不支持 
	//printf("size=%d, byts=%d \r\n",size,byts);
	for(i=0;i<byts;i++)
	{		
		asc = *sp++;//取模
		for(j=0;j<8;j++)//一个字节8个点	
		{
			if(asc&0x80) POINT_COLOR=colortemp;//画笔颜色
			else
			{
				if(!mode)	POINT_COLOR=BACK_COLOR;//背景色
			}
			LCD1_DrawPoint(x,y);//写点
			asc = asc<<1;
			x++;//下一列
			if(x>=LCD1_width){POINT_COLOR=colortemp;return;}//X超区域了  
			if((x-tx)==(size/2))//换行
			{
				x=tx;
				y++;
				if(y>=LCD1_height){POINT_COLOR=colortemp;return;}//Y超区域了
				break;
			} 			
		}
		if(size==24)
		{
			i++;
			asc = *sp++;//取模
			for(j=0;j<4;j++)//一个字节的高4位	
			{
				if(asc&0x80) POINT_COLOR=colortemp;//画笔颜色
				else
				{
					if(!mode)	POINT_COLOR=BACK_COLOR;//背景色
				}
				LCD1_DrawPoint(x,y);//写点
				asc = asc<<1;
				x++;//下一列
				if(x>=LCD1_width){POINT_COLOR=colortemp;return;}//X超区域了  
				if((x-tx)==(size/2))//换行
				{
					x=tx;
					y++;
					if(y>=LCD1_height){POINT_COLOR=colortemp;return;}//Y超区域了
					break;
				} 			
			}
		}
	}
			
	POINT_COLOR=colortemp;	    	   	 	  
}

//字库搜索->GB2312汉字
unsigned char *hzcode16(unsigned char *hz)
{
	typFNT_GB16 *fp = (typFNT_GB16*)&GB_16[0];;

	while(fp->Index[0]>0)
	{		
		if((fp->Index[0]==hz[0])&&(fp->Index[1]==hz[1]))//中文字符
		{
			return (unsigned char *)&(fp->Msk[0]);
		}
		else if((fp->Index[0]==hz[0])&&(hz[0]<0x80))//英文字符
		{
			return (unsigned char *)&(fp->Msk[0]);
		}
		fp++;
	}
	return 0;
}

//字库搜索->GB2312汉字
unsigned char *hzcode24(unsigned char *hz)
{
	typFNT_GB24 *fp = (typFNT_GB24*)&GB_24[0];;

	while(fp->Index[0]>0)
	{		
		if((fp->Index[0]==hz[0])&&(fp->Index[1]==hz[1]))//中文字符
		{
			return (unsigned char *)&(fp->Msk[0]);
		}
		else if((fp->Index[0]==hz[0])&&(hz[0]<0x80))//英文字符
		{
			return (unsigned char *)&(fp->Msk[0]);
		}
		fp++;
	}
	return 0;
}

//字库搜索->GB2312汉字
unsigned char *hzcode32(unsigned char *hz)
{
	typFNT_GB32 *fp = (typFNT_GB32*)&GB_32[0];;

	while(fp->Index[0]>0)
	{		
		if((fp->Index[0]==hz[0])&&(fp->Index[1]==hz[1]))//中文字符
		{
			return (unsigned char *)&(fp->Msk[0]);
		}
		else if((fp->Index[0]==hz[0])&&(hz[0]<0x80))//英文字符
		{
			return (unsigned char *)&(fp->Msk[0]);
		}
		fp++;
	}
	return 0;
}

//显示一个中文汉字,指定位置,数据,大小,模式
void LCM_Write_Word(u16 x,u16 y,u8 *gb,u8 size,u8 mode)
{
	unsigned char *sp;
	u8 tmp;
	u16 colortemp=POINT_COLOR,i,j;
	u16 tx=x;
	u16 byts = ((size/8)*size);//一个汉字的字节数

	if(size==16) 	  sp=hzcode16(gb);
	else if(size==24) sp=hzcode24(gb);
	else if(size==32) sp=hzcode32(gb);

	if(sp==0) return; 
		
	for(i=0;i<byts;i++)
	{		
		tmp = *sp++;//取模
		for(j=0;j<8;j++)//一个字节	
		{
			if(tmp&0x80) POINT_COLOR=colortemp;//画笔颜色
			else
			{
				if(!mode)	POINT_COLOR=BACK_COLOR;//背景色
			}
			LCD1_DrawPoint(x,y);//写点
			tmp = tmp<<1;
			x++;//下一列
			if(x>=LCD1_width){POINT_COLOR=colortemp;return;}//X超区域了  
			if((x-tx)==(size))//换行
			{
				x=tx;
				y++;
				if(y>=LCD1_height){POINT_COLOR=colortemp;return;}//Y超区域了
				break;
			}
		}		
	}
	POINT_COLOR=colortemp;	  
}
   
//m^n函数
//返回值:m^n次方.
u32 LCD1_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD1_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD1_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD1_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD1_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
	//LCD1_ShowChar(x+(size/2)*t,y,' ',size,0);
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD1_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD1_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD1_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD1_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD1_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//size:字体大小
//*p:字符串起始地址		  
void LCD1_ShowString(u16 x,u16 y,u8 size,u8 *p)
{
	while(*p!='\0') 
	{
	    if(*p<0x80)//判断是不是非法字符!
	    {       
	        LCD1_ShowChar(x,y,*p,size,0);//字符
	        x += (size/2);
			cur_xx=x;
	        p++;
	    }
		else
		{
	        LCM_Write_Word(x,y,p,size,0);//汉字
	        x += size;
			cur_xx=x;
	        p += 2;
		}
	}  
}

//显示图片,指定位置,大小,数组
void LCD1_DisplayBmp(u16 x,u16 y,u16 size_x,u16 size_y,u8 *bmp)
{
	u16 i,j,color;

 	for(i=0;i<size_y;i++)
	{
 		LCD1_SetCursor(x,y+i);   	//设置光标位置 
		LCD1_WR_REG(0x002C);   //开始写入GRAM	   
		for(j=0;j<size_x;j++)
		{
			color = ((*bmp++)<<8)+(*bmp++);
			LCD1->LCD_RAM = color;//写入数据 
		}
	} 
}
void LCD1_DisplayBmp1(u16 x,u16 y,u16 size_x,u16 size_y,const unsigned char *bmp)
{
	u16 i,j,color;

 	for(i=0;i<size_y;i++)
	{
 		LCD1_SetCursor(x,y+i);   	//设置光标位置 
		LCD1_WR_REG(0x002C);   //开始写入GRAM	   
		for(j=0;j<size_x;j++)
		{
			color = ((*bmp++)<<8)+(*bmp++);
			LCD1->LCD_RAM = color;//写入数据 
		}
	} 
}


//显示光标
void HMI_DisplayCur(u16 x, u16 y, u8 len, u8 dat)
{
	u8 i;
	u16 colortemp=POINT_COLOR;
	POINT_COLOR=0xffff;//白色 
 	for(i=0;i<len;i++)//高度
	{
		if(dat)	POINT_COLOR=0;//黑色   
		LCD1_DrawPoint(x,y+i);	
		LCD1_DrawPoint(x+1,y+i);	
	}
	POINT_COLOR=colortemp; 
}



