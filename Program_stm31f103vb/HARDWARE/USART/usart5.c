
#include "sys.h" 
#include "dma.h"
#include "stdarg.h"
#include "string.h"	 	 
#include "stdio.h"	
#include "USART5.h"
#include "led.h"

__align(8) u8 UART5_TX_BUF[UART5_SEND_LEN];
//u8 UART5_TX_BUF[UART5_SEND_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 UART5_RX_BUF[UART5_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.


//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 UART5_RX_STA = 0;       //接收状态标记

void CLR_UART5_TX_BUF(void)
{
	u16 i;
	for(i=0;i<UART5_SEND_LEN;i++)
	{
		UART5_TX_BUF[i] = 0;		
	}
}

//串口3,printf 函数
//确保一次发送数据不超过UART5_MAX_SEND_LEN字节
void u5_printf(char* fmt,...)  
{
	u16 i,len;  

	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)&UART5_TX_BUF[0],fmt,ap);
	va_end(ap);
	len=strlen((char*)&UART5_TX_BUF[0]);
	for(i=0;i<len;)			
	{
		while((UART5->SR&0X40)==0);//循环发送,直到发送完毕   
	    UART5->DR = UART5_TX_BUF[i++]; 
		if(i>=UART5_SEND_LEN) break;     
	}
	CLR_UART5_TX_BUF();
}

void UART5_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;//串口5
	NVIC_InitTypeDef NVIC_InitStructure;//串口5中断

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//使能UART5时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	//GPIOB,GPIOE时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	SCAN_TG = 0;

	 //UART5_TX   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    //UART5_RX	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);  

   //UART5 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(UART5, &USART_InitStructure); //初始化串口
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(UART5, ENABLE);                    //使能串口 

	UART5_RX_STA = 0;
}

//接收条码,一定要回车结束符
void UART5_IRQHandler(void)
{
	u8 res;	
	    
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		res = USART_ReceiveData(UART5);//读取接收到的数据
				 
		if((UART5_RX_STA&0x8000)==0)		//还可以接收数据
		{
			if(UART5_RX_STA&0x4000)//接收到了0x0d
			{	
				if(res!=0x0a) UART5_RX_STA=0;//接收错误,重新开始
				else UART5_RX_STA |= 0x8000;	//接收完成了 
				UART5_RX_BUF[UART5_RX_STA&0X3FFF] = 0;
			}			
			else //还没收到0X0D
			{	
				if(res==0x0d) UART5_RX_STA |= 0x4000;
				else{
					UART5_RX_BUF[UART5_RX_STA&0X3FFF] = res;
					UART5_RX_STA++;
					if(UART5_RX_STA>(UART5_REC_LEN-1))UART5_RX_STA=0;//接收数据错误,重新开始接收		
				} 					
			} 	 
		}
	}  											 
} 

