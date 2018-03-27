
#include "sys.h" 
#include "usart3.h"
#include "stdarg.h"
#include "string.h"	 	 
#include "stdio.h"	
#include "timer.h"
#include "led.h"
#include "beep.h"


__align(8) u8 USART3_TX_BUF[USART3_SEND_LEN];
//u8 USART3_TX_BUF[USART3_SEND_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.


//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART3_RX_STA = 0;       //����״̬���

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

//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
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
		while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
	    USART3->DR = USART3_TX_BUF[i++]; 
		if(i>=USART3_SEND_LEN) break;     
	}
	CLR_USART3_TX_BUF();
}

void USART3_SendData(u8 dat)
{
	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
	USART3->DR = dat; 
}

void USART3_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;//����3
	NVIC_InitTypeDef NVIC_InitStructure;//����3�ж�

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOB,GPIOEʱ��

	 //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    //USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

   //Usart3 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_Even;//USART_Parity_No;//����żУ��λ//USART_Parity_Even
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
	
	TIM4_Int_Init(1000,7200);//10Khz�ļ���Ƶ�ʣ�������1000Ϊ100ms 

	TIM4_Set(0);

	CLR_USART3_RX_BUF();
}



void USART3_IRQHandler(void)
{
	u8 res;	
  
	if(USART3->SR&(1<<5))//���յ�����
	{	 
		res=USART3->DR; 	
	}  										 
}

