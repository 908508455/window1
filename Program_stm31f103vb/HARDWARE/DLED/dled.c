#include "d_led.h"

unsigned char table[]={//???0~f?????
0x3f,0x06,0x5b,0x4f,     //0~3
0x66,0x6d,0x7d,0x07,    //4~7
0x7f,0x6f,0x77,0x7c,    //8~b
0x39,0x5e,0x79,0x71    //c~f
,0x08,0x3e,
};

void dled_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC
																			|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PD端口时钟
	//+++++++++++++++++COM0-4++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化
	GPIO_ResetBits(GPIOE, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);
	//+++++++++++++++++++++74HC595++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化
	GPIO_ResetBits(GPIOC,  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化
	GPIO_SetBits(GPIOB,  GPIO_Pin_5);
//	GPIO_SetBits(GPIOE,  GPIO_Pin_2);
	SCLR =1 ;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//PB1-RCLK -PC14  
//PA0-SCLK-PB9
//PA8-DIN-PC15
void write_data ( uint8_t Data_Byte)
{
	 uint8_t i ;
	 RCLK = 0;
	 for(i=0;i<8;i++)
		{
				SCLK = 0; 
				delay_us(1);			
				if((Data_Byte&0x80)==0x80)               
					DIN = 1;
				else                                        
					DIN = 0;      
				delay_us(1);
				SCLK = 1 ;      
				Data_Byte = Data_Byte<<1;                                
				delay_us(1);
		}
		RCLK = 1; 
 }
void select_number(uint8_t which){
	COM0 = 0 ;
	COM1 = 0 ;
	COM2 = 0 ;
	COM3 = 0 ;
	switch(which){
		case 0 :
			COM0 = 1;
			break ;
		case 1 :
			COM1 = 1;
			break ;
		case 2 :
			COM2 = 1;
			break ;
		case 3 :
			COM3 = 1;
			break ;
		default:
			break;
	}
}
void show_number(uint8_t data,uint8_t which)
{
  select_number(which);
	if(data&0x80){
		write_data(table[data&0x7f]|0x80);
	}else{
		write_data(table[data]);
	}
}

