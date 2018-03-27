#include "lcd_h.h"
#include "stdlib.h"

#include "usart.h"	 
#include "delay.h"

#include "ASCII_1608.h"
#include "ASCII_2412.h"
#include "ASCII_3216.h"
#include "ASCII_6432.h"	
#include "fontlib.h"

				 
//LCD1�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 

u16 cur_xx=0;

	 
//д�Ĵ�������
//regval:�Ĵ���ֵ
void LCD1_WR_REG(u16 regval)
{ 
	LCD1->LCD_REG=regval;//д��Ҫд�ļĴ������	 
}
//дLCD1����
//data:Ҫд���ֵ
void LCD1_WR_DATA(u16 data)
{										    	   
	LCD1->LCD_RAM=data;		 
}
//��LCD1����
//����ֵ:������ֵ
u16 LCD1_RD_DATA(void)
{										    	   
	return LCD1->LCD_RAM;		 
}					   
//д�Ĵ���
//LCD1_Reg:�Ĵ�����ַ
//LCD1_RegValue:Ҫд�������
void LCD1_WriteReg(u16 LCD1_Reg, u16 LCD1_RegValue)
{	
	LCD1->LCD_REG = LCD1_Reg;		//д��Ҫд�ļĴ������	 
	LCD1->LCD_RAM = LCD1_RegValue;//д������	    		 
}	   
//���Ĵ���
//LCD1_Reg:�Ĵ�����ַ
//����ֵ:����������
u16 LCD1_ReadReg(u16 LCD1_Reg)
{										   
	LCD1_WR_REG(LCD1_Reg);		//д��Ҫ���ļĴ������
	delay_us(5);		  
	return LCD1_RD_DATA();		//���ض�����ֵ
}   
//��ʼдGRAM
void LCD1_WriteRAM_Prepare(void)
{
 	LCD1->LCD_REG=0x002c;	  
}	 
//LCD1дGRAM
//RGB_Code:��ɫֵ
void LCD1_WriteRAM(u16 RGB_Code)
{							    
	LCD1->LCD_RAM = RGB_Code;//дʮ��λGRAM
}
//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 LCD1_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
void opt_delay(u8 i)
{
	while(i--);
}
//��ȡ��ĳ�����ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
u16 LCD1_ReadPoint(u16 x,u16 y)
{
	u16 dat=0;
	LCD1_SetCursor(x,y);		//���ù��λ�� 
	LCD1_WR_REG(0x2e);
	dat = LCD1->LCD_RAM;	
	dat = LCD1->LCD_RAM;
	return dat;		
}			 
//LCD1������ʾ
void LCD1_DisplayOn(void)
{					   
	LCD1_WR_REG(0x0029);
}	 
//LCD1�ر���ʾ
void LCD1_DisplayOff(void)
{	   
	LCD1_WR_REG(0x0028);//�ر���ʾ 
}  
//���ù��λ��
//Xpos:������
//Ypos:������
void LCD1_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD1_WR_REG(0x002A);LCD1_WR_DATA(Xpos>>8);
	LCD1_WR_DATA(0x00FF&Xpos); //�趨X����
	LCD1_WR_DATA(0x0001);LCD1_WR_DATA(0x00df);

	LCD1_WR_REG(0x002B);LCD1_WR_DATA(Ypos>>8);
	LCD1_WR_DATA(0x00FF&Ypos); //�趨Y����	
	LCD1_WR_DATA(0x0001);LCD1_WR_DATA(0x003F);

} 		 
//����LCD1���Զ�ɨ�跽��
//ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341/6804����������),
//����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���LCD1.h)
//9320/9325/9328/4531/4535/1505/b505/8989/5408/9341/5310/5510��IC�Ѿ�ʵ�ʲ���	   	   
void LCD1_Scan_Dir(u8 dir)
{

}   
//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD1_DrawPoint(u16 x,u16 y)
{
	LCD1_SetCursor(x,y);		//���ù��λ�� 
	LCD1_WR_REG(0x002C);
	LCD1_WR_DATA(POINT_COLOR); 
}
//���ٻ���
//x,y:����
//color:��ɫ
void LCD1_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   

}	 


//����LCD1��ʾ����
//dir:0,������1,����
void LCD1_Display_Dir(u8 dir)
{

}	 
//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:width*height.
//68042,����ʱ��֧�ִ�������!! 
void LCD1_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{   

} 
//��ʼ��LCD1
//�ó�ʼ���������Գ�ʼ������ILI93XXҺ��,�������������ǻ���ILI9320��!!!
//�������ͺŵ�����оƬ��û�в���! 
void LCD1_Init(void)
{ 										  
 	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//ʹ��FSMCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO,ENABLE);//ʹ��PORTB,D,E,G�Լ�AFIO���ù���ʱ��

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6;			 //PD3 ������� ����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);

 	//PORTD�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	  
	//PORTE�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	  
 
	readWriteTiming.FSMC_AddressSetupTime = 0x02;	 //��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // ���ݱ���ʱ��Ϊ16��HCLK,��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫�죬�����1289���IC��
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
    
	writeTiming.FSMC_AddressSetupTime = 0x02;	 //��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨A		
    writeTiming.FSMC_DataSetupTime = 0x03;		 ////���ݱ���ʱ��Ϊ4��HCLK	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 

 
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ���������ݵ�ַ
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  �洢��дʹ��
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //��дʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //дʱ��

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  // ʹ��BANK1 
	
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

	LCD1_LED=1;					//��������
	LCD1_Clear(WHITE);

}  
//��������
//color:Ҫ���������ɫ
void LCD1_Clear(u16 color)
{
	u32 index=0;      

	LCD1_SetCursor(0x00,0x00);	//���ù��λ�� 
	LCD1_WR_REG(0x002C);   
	for(index=0;index<153600;index++)
		LCD1_WR_DATA(color);   
}
 
//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD1_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	/*u16 i,j;
	 
	for(i=0;i<ey;i++)
	{
	 	LCD1_SetCursor(sx,sy+i);      				//���ù��λ�� 
		LCD1_WR_REG(0x002C);   			//��ʼд��GRAM	  
		for(j=0;j<ex;j++)
			LCD1_WR_DATA(color);	//���ù��λ�� 	    
	}*/
	
	
		u16 i,j;
	u16 xlen=0; 
	xlen=ex-sx+1;	 
	for(i=sy;i<=ey;i++)
	{
	 	LCD1_SetCursor(sx,i);      				//���ù��λ�� 
		LCD1_WR_REG(0x002C);   			//��ʼд��GRAM	  
		for(j=0;j<xlen;j++)
			LCD1_WR_DATA(color);	//���ù��λ�� 	    
	}
	
		 
}  
//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD1_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD1_SetCursor(sx,sy+i);   	//���ù��λ�� 
		LCD1_WR_REG(0x002C);   //��ʼд��GRAM
		for(j=0;j<width;j++)LCD1->LCD_RAM=color[i*height+j];//д������ 
	}	  
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD1_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD1_DrawPoint(uRow,uCol);//���� 
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
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD1_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD1_DrawLine(x1,y1,x2,y1);
	LCD1_DrawLine(x1,y1,x1,y2);
	LCD1_DrawLine(x1,y2,x2,y2);
	LCD1_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD1_ShowChar(u16 x,u16 y,char s,u8 size,u8 mode)
{  							  
    u8 asc;
	u16 colortemp=POINT_COLOR,i,j;
	u16 tx=x;
	u8 const *sp;
	u16 byts;//�ֽ���
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
	else if(size==64)//ֻ��ʾ����
	{		
		if(s=='.') sp = &ASCII_6432[(10)*(16)][0];
		else sp = &ASCII_6432[(s-0x30)*(16)][0];
		byts = 256;
	}
	else return ;//��֧�� 
	//printf("size=%d, byts=%d \r\n",size,byts);
	for(i=0;i<byts;i++)
	{		
		asc = *sp++;//ȡģ
		for(j=0;j<8;j++)//һ���ֽ�8����	
		{
			if(asc&0x80) POINT_COLOR=colortemp;//������ɫ
			else
			{
				if(!mode)	POINT_COLOR=BACK_COLOR;//����ɫ
			}
			LCD1_DrawPoint(x,y);//д��
			asc = asc<<1;
			x++;//��һ��
			if(x>=LCD1_width){POINT_COLOR=colortemp;return;}//X��������  
			if((x-tx)==(size/2))//����
			{
				x=tx;
				y++;
				if(y>=LCD1_height){POINT_COLOR=colortemp;return;}//Y��������
				break;
			} 			
		}
		if(size==24)
		{
			i++;
			asc = *sp++;//ȡģ
			for(j=0;j<4;j++)//һ���ֽڵĸ�4λ	
			{
				if(asc&0x80) POINT_COLOR=colortemp;//������ɫ
				else
				{
					if(!mode)	POINT_COLOR=BACK_COLOR;//����ɫ
				}
				LCD1_DrawPoint(x,y);//д��
				asc = asc<<1;
				x++;//��һ��
				if(x>=LCD1_width){POINT_COLOR=colortemp;return;}//X��������  
				if((x-tx)==(size/2))//����
				{
					x=tx;
					y++;
					if(y>=LCD1_height){POINT_COLOR=colortemp;return;}//Y��������
					break;
				} 			
			}
		}
	}
			
	POINT_COLOR=colortemp;	    	   	 	  
}

//�ֿ�����->GB2312����
unsigned char *hzcode16(unsigned char *hz)
{
	typFNT_GB16 *fp = (typFNT_GB16*)&GB_16[0];;

	while(fp->Index[0]>0)
	{		
		if((fp->Index[0]==hz[0])&&(fp->Index[1]==hz[1]))//�����ַ�
		{
			return (unsigned char *)&(fp->Msk[0]);
		}
		else if((fp->Index[0]==hz[0])&&(hz[0]<0x80))//Ӣ���ַ�
		{
			return (unsigned char *)&(fp->Msk[0]);
		}
		fp++;
	}
	return 0;
}

//�ֿ�����->GB2312����
unsigned char *hzcode24(unsigned char *hz)
{
	typFNT_GB24 *fp = (typFNT_GB24*)&GB_24[0];;

	while(fp->Index[0]>0)
	{		
		if((fp->Index[0]==hz[0])&&(fp->Index[1]==hz[1]))//�����ַ�
		{
			return (unsigned char *)&(fp->Msk[0]);
		}
		else if((fp->Index[0]==hz[0])&&(hz[0]<0x80))//Ӣ���ַ�
		{
			return (unsigned char *)&(fp->Msk[0]);
		}
		fp++;
	}
	return 0;
}

//�ֿ�����->GB2312����
unsigned char *hzcode32(unsigned char *hz)
{
	typFNT_GB32 *fp = (typFNT_GB32*)&GB_32[0];;

	while(fp->Index[0]>0)
	{		
		if((fp->Index[0]==hz[0])&&(fp->Index[1]==hz[1]))//�����ַ�
		{
			return (unsigned char *)&(fp->Msk[0]);
		}
		else if((fp->Index[0]==hz[0])&&(hz[0]<0x80))//Ӣ���ַ�
		{
			return (unsigned char *)&(fp->Msk[0]);
		}
		fp++;
	}
	return 0;
}

//��ʾһ�����ĺ���,ָ��λ��,����,��С,ģʽ
void LCM_Write_Word(u16 x,u16 y,u8 *gb,u8 size,u8 mode)
{
	unsigned char *sp;
	u8 tmp;
	u16 colortemp=POINT_COLOR,i,j;
	u16 tx=x;
	u16 byts = ((size/8)*size);//һ�����ֵ��ֽ���

	if(size==16) 	  sp=hzcode16(gb);
	else if(size==24) sp=hzcode24(gb);
	else if(size==32) sp=hzcode32(gb);

	if(sp==0) return; 
		
	for(i=0;i<byts;i++)
	{		
		tmp = *sp++;//ȡģ
		for(j=0;j<8;j++)//һ���ֽ�	
		{
			if(tmp&0x80) POINT_COLOR=colortemp;//������ɫ
			else
			{
				if(!mode)	POINT_COLOR=BACK_COLOR;//����ɫ
			}
			LCD1_DrawPoint(x,y);//д��
			tmp = tmp<<1;
			x++;//��һ��
			if(x>=LCD1_width){POINT_COLOR=colortemp;return;}//X��������  
			if((x-tx)==(size))//����
			{
				x=tx;
				y++;
				if(y>=LCD1_height){POINT_COLOR=colortemp;return;}//Y��������
				break;
			}
		}		
	}
	POINT_COLOR=colortemp;	  
}
   
//m^n����
//����ֵ:m^n�η�.
u32 LCD1_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
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
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
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
//��ʾ�ַ���
//x,y:�������
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD1_ShowString(u16 x,u16 y,u8 size,u8 *p)
{
	while(*p!='\0') 
	{
	    if(*p<0x80)//�ж��ǲ��ǷǷ��ַ�!
	    {       
	        LCD1_ShowChar(x,y,*p,size,0);//�ַ�
	        x += (size/2);
			cur_xx=x;
	        p++;
	    }
		else
		{
	        LCM_Write_Word(x,y,p,size,0);//����
	        x += size;
			cur_xx=x;
	        p += 2;
		}
	}  
}

//��ʾͼƬ,ָ��λ��,��С,����
void LCD1_DisplayBmp(u16 x,u16 y,u16 size_x,u16 size_y,u8 *bmp)
{
	u16 i,j,color;

 	for(i=0;i<size_y;i++)
	{
 		LCD1_SetCursor(x,y+i);   	//���ù��λ�� 
		LCD1_WR_REG(0x002C);   //��ʼд��GRAM	   
		for(j=0;j<size_x;j++)
		{
			color = ((*bmp++)<<8)+(*bmp++);
			LCD1->LCD_RAM = color;//д������ 
		}
	} 
}
void LCD1_DisplayBmp1(u16 x,u16 y,u16 size_x,u16 size_y,const unsigned char *bmp)
{
	u16 i,j,color;

 	for(i=0;i<size_y;i++)
	{
 		LCD1_SetCursor(x,y+i);   	//���ù��λ�� 
		LCD1_WR_REG(0x002C);   //��ʼд��GRAM	   
		for(j=0;j<size_x;j++)
		{
			color = ((*bmp++)<<8)+(*bmp++);
			LCD1->LCD_RAM = color;//д������ 
		}
	} 
}


//��ʾ���
void HMI_DisplayCur(u16 x, u16 y, u8 len, u8 dat)
{
	u8 i;
	u16 colortemp=POINT_COLOR;
	POINT_COLOR=0xffff;//��ɫ 
 	for(i=0;i<len;i++)//�߶�
	{
		if(dat)	POINT_COLOR=0;//��ɫ   
		LCD1_DrawPoint(x,y+i);	
		LCD1_DrawPoint(x+1,y+i);	
	}
	POINT_COLOR=colortemp; 
}



