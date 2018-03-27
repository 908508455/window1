
#include "sys.h" 
#include "usart3.h"
#include "stdarg.h"
#include "string.h"	 	 
#include "stdio.h"	
#include "timer.h"
#include "led.h"
#include "beep.h"


__align(8) u8 USART3_TX_BUF[USART3_SEND_LEN];
//u8 USART3_TX_BUF[USART3_SEND_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.


//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA = 0;       //接收状态标记

void CLR_USART3_RX_BUF(void)
{
	u16 i;
	for(i=0;i<USART3_REC_LEN;i++)
	{
		USART3_RX_BUF[i] = 0;		
	}
	USART3_RX_STA = 0;
}

void CLR_USART3_TX_BUF(void)
{
	u16 i;
	for(i=0;i<USART3_SEND_LEN;i++)
	{
		USART3_TX_BUF[i] = 0;		
	}
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char* fmt,...)  
{
	u16 i,len;  

	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)&USART3_TX_BUF[0],fmt,ap);
	va_end(ap);
	len=strlen((char*)&USART3_TX_BUF[0]); 
	for(i=0;i<len;)			
	{
		while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
	    USART3->DR = USART3_TX_BUF[i++]; 
		if(i>=USART3_SEND_LEN) break;     
	}
	CLR_USART3_TX_BUF();
}

void USART3_SendData(u8 dat)
{
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
	USART3->DR = dat; 
}

void USART3_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;//串口3
	NVIC_InitTypeDef NVIC_InitStructure;//串口3中断

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOB,GPIOE时钟

	 //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    //USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

   //Usart3 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_Even;//USART_Parity_No;//无奇偶校验位//USART_Parity_Even
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART3, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);                    //使能串口 
	
	TIM4_Int_Init(1000,7200);//10Khz的计数频率，计数到1000为100ms 

	TIM4_Set(0);

	CLR_USART3_RX_BUF();
}



void USART3_IRQHandler(void)
{
	u8 res;	
  
	if(USART3->SR&(1<<5))//接收到数据
	{	 
		res=USART3->DR; 	
	}  										 
}

