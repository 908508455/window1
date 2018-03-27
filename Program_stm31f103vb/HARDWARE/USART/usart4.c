
#include "sys.h" 
#include "dma.h"
#include "stdarg.h"
#include "string.h"	 	 
#include "stdio.h"	
#include "USART4.h"
#include "timer.h"
#include "led.h"   
#include "24cxx.h"
#include "SystemUI.h"

__align(8) u8 UART4_TX_BUF[UART4_SEND_LEN];
//u8 UART4_TX_BUF[UART4_SEND_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 UART4_RX_BUF[UART4_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.


//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 UART4_RX_STA = 0;       //接收状态标记

void CLR_UART4_RX_BUF(void)
{
	u16 i;
	for(i=0;i<UART4_REC_LEN;i++)
	{
		UART4_RX_BUF[i] = 0;		
	}
	UART4_RX_STA = 0;
}

void CLR_UART4_TX_BUF(void)
{
	u16 i;
	for(i=0;i<UART4_SEND_LEN;i++)
	{
		UART4_TX_BUF[i] = 0;		
	}
}

//串口3,printf 函数
//确保一次发送数据不超过UART4_MAX_SEND_LEN字节
void u4_printf(char* fmt,...)  
{
	u16 i,len;  

	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)&UART4_TX_BUF[0],fmt,ap);
	va_end(ap);
	len=strlen((char*)&UART4_TX_BUF[0]);
	for(i=0;i<len;)			
	{
		while((UART4->SR&0X40)==0);//循环发送,直到发送完毕   
	    UART4->DR = UART4_TX_BUF[i++]; 
		if(i>=UART4_SEND_LEN) break;     
	}
	CLR_UART4_TX_BUF();
}

//COM4发送N字节数据
void u4_SendNByte(u8 *buf, u8 len)  
{
	u8 i;
	for(i=0;i<len;i++)			
	{
		while((UART4->SR&0X40)==0);//循环发送,直到发送完毕   
	    UART4->DR = buf[i];	   
	}
}

void UART4_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;//串口3
	NVIC_InitTypeDef NVIC_InitStructure;//串口3中断

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能UART4时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//GPIOB,GPIOE时钟

	 //UART4_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    //UART4_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);  

   //UART4 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_Even;//USART_Parity_No;//USART_Parity_Even;//USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(UART4, &USART_InitStructure); //初始化串口
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(UART4, ENABLE);                    //使能串口 
	   
	TIM5_Int_Init(1000,7200);//10Khz的计数频率，计数到1000为100ms 	 
	TIM5_Set(0);

	CLR_UART4_RX_BUF();
}

static u8 startflag=0;

void UART4_IRQHandler(void)
{
	u8 res;	
  
	if(UART4->SR&(1<<5))//接收到数据
	{	 
		res=UART4->DR;
		
		if(res==0x80)//正在模拟纸币输出
		{				
			startflag=1;	
		}
		else if((startflag==1)&&(res==0x8f))
		{
			while((UART4->SR&0X40)==0);//循环发送,直到发送完毕   
	    	UART4->DR = 0x02; 
			while((UART4->SR&0X40)==0);//循环发送,直到发送完毕   
	    	UART4->DR = 0x3e; 
			startflag=0;
		}
		if((res>=0x40)&&(res<=0x45)){			
			switch(res)
			{
				case 0x40 : {CashTmp=1;  break;}
				case 0x41 :	{CashTmp=5;  break;}
				case 0x42 :	{CashTmp=10; break;}
				case 0x43 :	{CashTmp=20; break;}
				case 0x44 :	{CashTmp=50; break;}
				case 0x45 :	{CashTmp=100;break;}
			}
			while((UART4->SR&0X40)==0);//循环发送,直到发送完毕   
	    	UART4->DR = 0x02; 
		} 		
		  				
		if(res==0x10){//回复确认  
			ThisAccount.Cash2 += CashTmp;//本次产生的计数 	 				
			TotalAccount.Cash2 += CashTmp;//总数  			
			MoneyCount += CashTmp;//金额累加
			IntSta |= 0x20;		
			if(SYS_Error&0x80){SYS_Error &= 0x7f;dis_sta |= 1;}				
		} 
		
		else if((res>=0x20)&&(res<=0x28))
		{//纸币机故障
			if((SYS_Error&0x80)==0){SYS_Error |= 0x80;dis_sta |= 1;bill_sta=MISTAKE;}	
			printf("Cash2_Error: %02x \r\n",res);
		}
		else if((res==0x3e)||(res==0x5e))//使能或禁止纸币机
		{
			if(SYS_Error&0x80){SYS_Error &= 0x7f;dis_sta |= 1;bill_sta=NORMAL;}	
		}				
		

/*
		TIM5->CNT=0;//计数器清空
		if(UART4_RX_STA<UART4_REC_LEN)		//还可以接收数据
		{
			if(UART4_RX_STA==0) TIM5_Set(1); 
			UART4_RX_BUF[UART4_RX_STA++]=res;		//记录接收到的值
		}
		else 
		{
			UART4_RX_STA|=1<<15;					//强制标记接收完成
			TIM5_Set(0);
		}
		*/			
	}  										 
}

//void UART4_IRQHandler(void)
//{
//	u8 res;	
//	    
//	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//接收到数据
//	{	 
//		res = USART_ReceiveData(UART4);//(USART1->DR);	//读取接收到的数据
//				 
//		if((UART4_RX_STA&0x8000)==0)		//还可以接收数据
//		{
//			if(UART4_RX_STA&0x4000)//接收到了0x0d
//			{
//				
//				if(res!=0x0a) UART4_RX_STA=0;//接收错误,重新开始
//				else UART4_RX_STA |= 0x8000;	//接收完成了 
//			}			
//			else //还没收到0X0D
//			{	
//				if(res==0x0d) UART4_RX_STA |= 0x4000;
//				else{
//					UART4_RX_BUF[UART4_RX_STA&0X3FFF] = res;
//					UART4_RX_STA++;
//					if(UART4_RX_STA>(UART4_REC_LEN-1))UART4_RX_STA=0;//接收数据错误,重新开始接收		
//				} 					
//			} 	 
//		}
//	}  											 
//} 

