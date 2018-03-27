
#include "sys.h" 
#include "dma.h"
#include "stdarg.h"
#include "string.h"	 	 
#include "stdio.h"	
#include "USART5.h"
#include "led.h"

__align(8) u8 UART5_TX_BUF[UART5_SEND_LEN];
//u8 UART5_TX_BUF[UART5_SEND_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 UART5_RX_BUF[UART5_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.


//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 UART5_RX_STA = 0;       //����״̬���

void CLR_UART5_TX_BUF(void)
{
	u16 i;
	for(i=0;i<UART5_SEND_LEN;i++)
	{
		UART5_TX_BUF[i] = 0;		
	}
}

//����3,printf ����
//ȷ��һ�η������ݲ�����UART5_MAX_SEND_LEN�ֽ�
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
		while((UART5->SR&0X40)==0);//ѭ������,ֱ���������   
	    UART5->DR = UART5_TX_BUF[i++]; 
		if(i>=UART5_SEND_LEN) break;     
	}
	CLR_UART5_TX_BUF();
}

void UART5_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;//����5
	NVIC_InitTypeDef NVIC_InitStructure;//����5�ж�

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//ʹ��UART5ʱ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	//GPIOB,GPIOEʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	SCAN_TG = 0;

	 //UART5_TX   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    //UART5_RX	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOD, &GPIO_InitStructure);  

   //UART5 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(UART5, &USART_InitStructure); //��ʼ������
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ��� 

	UART5_RX_STA = 0;
}

//��������,һ��Ҫ�س�������
void UART5_IRQHandler(void)
{
	u8 res;	
	    
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		res = USART_ReceiveData(UART5);//��ȡ���յ�������
				 
		if((UART5_RX_STA&0x8000)==0)		//�����Խ�������
		{
			if(UART5_RX_STA&0x4000)//���յ���0x0d
			{	
				if(res!=0x0a) UART5_RX_STA=0;//���մ���,���¿�ʼ
				else UART5_RX_STA |= 0x8000;	//��������� 
				UART5_RX_BUF[UART5_RX_STA&0X3FFF] = 0;
			}			
			else //��û�յ�0X0D
			{	
				if(res==0x0d) UART5_RX_STA |= 0x4000;
				else{
					UART5_RX_BUF[UART5_RX_STA&0X3FFF] = res;
					UART5_RX_STA++;
					if(UART5_RX_STA>(UART5_REC_LEN-1))UART5_RX_STA=0;//�������ݴ���,���¿�ʼ����		
				} 					
			} 	 
		}
	}  											 
} 

