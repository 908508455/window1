
#include "USART2.h"
#include "dma.h"
#include "timer.h"//用T3作为接收超时检测
#include "delay.h"
#include "string.h"
		   

__align(8) u8 USART2_TX_BUF[USART2_SEND_LEN];
//u8 USART2_TX_BUF[USART2_SEND_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART2_RX_BUF[64];     //接收缓冲,最大USART_REC_LEN个字节.

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA = 0;       //接收状态标记

//接收缓冲区清零
void CLR_USART2_RX_BUF(void)
{
	u16 i;
	for(i=0;i<USART2_REC_LEN;i++)
	{
		USART2_RX_BUF[i] = 0;		
	}
	USART2_RX_STA = 0;
}

//发送缓冲区清零
void CLR_USART2_TX_BUF(void)
{
	u16 i;
	for(i=0;i<USART2_SEND_LEN;i++)
	{
		USART2_TX_BUF[i] = 0;		
	}
}


//串口2,printf 函数
//确保一次发送数据不超过USART2_MAX_SEND_LEN字节
 void u2_printf(char* fmt,...)  
{	  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)&USART2_TX_BUF[0],fmt,ap);
	va_end(ap);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	
	delay_ms(10);
	
	DMA1_MEM_LEN = strlen((char*)&USART2_TX_BUF[0]); 
	if(DMA1_MEM_LEN>USART2_SEND_LEN) DMA1_MEM_LEN = USART2_SEND_LEN;

	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);//使能串口2的DMA发送  
	MYDMA_Enable(DMA1_Channel7);//开始一次DMA传输！	

	while(DMA_GetFlagStatus(DMA1_FLAG_TC7)==RESET);	//判断通道7传输完成
	DMA_ClearFlag(DMA1_FLAG_TC7);//清除通道7传输完成标志
	
	CLR_USART2_TX_BUF();
	
	delay_ms(10);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}


void u2_send_buf(u8 *buf, u16 len)
{
	u16 i ;
  for(i=0;i<len;)			
	{
		while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
	    USART2->DR = buf[i++]; 
		if(i>=len) break;     
	}
}

void USART2_Init(u32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;//串口2
		NVIC_InitTypeDef NVIC_InitStructure;//串口2中断

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//GPIOB,GPIOE时钟
		
		 //RS485 RE   PA.1
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);

	 //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //USART2 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1;//抢占优先级3
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

    USART_Init(USART2, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART2, ENABLE);                    //使能串口 
		
		TIM3_Int_Init(99,7199);//10Khz的计数频率，计数到1000为100ms 	
}

volatile uint16_t Usart2FlagCount=0 ;

//定时器3中断服务程序==============================SIM900A接收超时检测
void TIM3_IRQHandler(void)   //TIM3中断
{ 
	if(TIM3->SR&0X01)//是更新中断
	{	 			   
		TIM3->SR&=~(1<<0);		//清除中断标志位		   
		if(Usart2FlagCount == 2){
			Usart2FlagCount-- ;
		}else if(Usart2FlagCount ==1){
			Usart2FlagCount-- ;
			USART2_RX_STA |=0x8000;
			TIM3_Set(0);
		}		
	}
}

void USART2_IRQHandler(void)
{
	u8 Res;	
	TIM3_Set(1);
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Usart2FlagCount = 2 ;
		Res = USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
		USART2_RX_BUF[USART2_RX_STA] = Res ;
		USART2_RX_STA++ ;
	}  											 
} 

