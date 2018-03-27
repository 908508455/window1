
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
//u8 UART4_TX_BUF[UART4_SEND_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 UART4_RX_BUF[UART4_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.


//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 UART4_RX_STA = 0;       //����״̬���

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

//����3,printf ����
//ȷ��һ�η������ݲ�����UART4_MAX_SEND_LEN�ֽ�
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
		while((UART4->SR&0X40)==0);//ѭ������,ֱ���������   
	    UART4->DR = UART4_TX_BUF[i++]; 
		if(i>=UART4_SEND_LEN) break;     
	}
	CLR_UART4_TX_BUF();
}

//COM4����N�ֽ�����
void u4_SendNByte(u8 *buf, u8 len)  
{
	u8 i;
	for(i=0;i<len;i++)			
	{
		while((UART4->SR&0X40)==0);//ѭ������,ֱ���������   
	    UART4->DR = buf[i];	   
	}
}

void UART4_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;//����3
	NVIC_InitTypeDef NVIC_InitStructure;//����3�ж�

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��UART4ʱ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//GPIOB,GPIOEʱ��

	 //UART4_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    //UART4_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);  

   //UART4 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_Even;//USART_Parity_No;//USART_Parity_Even;//USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(UART4, &USART_InitStructure); //��ʼ������
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ��� 
	   
	TIM5_Int_Init(1000,7200);//10Khz�ļ���Ƶ�ʣ�������1000Ϊ100ms 	 
	TIM5_Set(0);

	CLR_UART4_RX_BUF();
}

static u8 startflag=0;

void UART4_IRQHandler(void)
{
	u8 res;	
  
	if(UART4->SR&(1<<5))//���յ�����
	{	 
		res=UART4->DR;
		
		if(res==0x80)//����ģ��ֽ�����
		{				
			startflag=1;	
		}
		else if((startflag==1)&&(res==0x8f))
		{
			while((UART4->SR&0X40)==0);//ѭ������,ֱ���������   
	    	UART4->DR = 0x02; 
			while((UART4->SR&0X40)==0);//ѭ������,ֱ���������   
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
			while((UART4->SR&0X40)==0);//ѭ������,ֱ���������   
	    	UART4->DR = 0x02; 
		} 		
		  				
		if(res==0x10){//�ظ�ȷ��  
			ThisAccount.Cash2 += CashTmp;//���β����ļ��� 	 				
			TotalAccount.Cash2 += CashTmp;//����  			
			MoneyCount += CashTmp;//����ۼ�
			IntSta |= 0x20;		
			if(SYS_Error&0x80){SYS_Error &= 0x7f;dis_sta |= 1;}				
		} 
		
		else if((res>=0x20)&&(res<=0x28))
		{//ֽ�һ�����
			if((SYS_Error&0x80)==0){SYS_Error |= 0x80;dis_sta |= 1;bill_sta=MISTAKE;}	
			printf("Cash2_Error: %02x \r\n",res);
		}
		else if((res==0x3e)||(res==0x5e))//ʹ�ܻ��ֹֽ�һ�
		{
			if(SYS_Error&0x80){SYS_Error &= 0x7f;dis_sta |= 1;bill_sta=NORMAL;}	
		}				
		

/*
		TIM5->CNT=0;//���������
		if(UART4_RX_STA<UART4_REC_LEN)		//�����Խ�������
		{
			if(UART4_RX_STA==0) TIM5_Set(1); 
			UART4_RX_BUF[UART4_RX_STA++]=res;		//��¼���յ���ֵ
		}
		else 
		{
			UART4_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
			TIM5_Set(0);
		}
		*/			
	}  										 
}

//void UART4_IRQHandler(void)
//{
//	u8 res;	
//	    
//	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//���յ�����
//	{	 
//		res = USART_ReceiveData(UART4);//(USART1->DR);	//��ȡ���յ�������
//				 
//		if((UART4_RX_STA&0x8000)==0)		//�����Խ�������
//		{
//			if(UART4_RX_STA&0x4000)//���յ���0x0d
//			{
//				
//				if(res!=0x0a) UART4_RX_STA=0;//���մ���,���¿�ʼ
//				else UART4_RX_STA |= 0x8000;	//��������� 
//			}			
//			else //��û�յ�0X0D
//			{	
//				if(res==0x0d) UART4_RX_STA |= 0x4000;
//				else{
//					UART4_RX_BUF[UART4_RX_STA&0X3FFF] = res;
//					UART4_RX_STA++;
//					if(UART4_RX_STA>(UART4_REC_LEN-1))UART4_RX_STA=0;//�������ݴ���,���¿�ʼ����		
//				} 					
//			} 	 
//		}
//	}  											 
//} 

