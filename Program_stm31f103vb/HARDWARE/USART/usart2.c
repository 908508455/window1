
#include "USART2.h"
#include "dma.h"
#include "timer.h"//��T3��Ϊ���ճ�ʱ���
#include "delay.h"
#include "string.h"
		   

__align(8) u8 USART2_TX_BUF[USART2_SEND_LEN];
//u8 USART2_TX_BUF[USART2_SEND_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART2_RX_BUF[64];     //���ջ���,���USART_REC_LEN���ֽ�.

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA = 0;       //����״̬���

//���ջ���������
void CLR_USART2_RX_BUF(void)
{
	u16 i;
	for(i=0;i<USART2_REC_LEN;i++)
	{
		USART2_RX_BUF[i] = 0;		
	}
	USART2_RX_STA = 0;
}

//���ͻ���������
void CLR_USART2_TX_BUF(void)
{
	u16 i;
	for(i=0;i<USART2_SEND_LEN;i++)
	{
		USART2_TX_BUF[i] = 0;		
	}
}


//����2,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
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

	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);//ʹ�ܴ���2��DMA����  
	MYDMA_Enable(DMA1_Channel7);//��ʼһ��DMA���䣡	

	while(DMA_GetFlagStatus(DMA1_FLAG_TC7)==RESET);	//�ж�ͨ��7�������
	DMA_ClearFlag(DMA1_FLAG_TC7);//���ͨ��7������ɱ�־
	
	CLR_USART2_TX_BUF();
	
	delay_ms(10);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}


void u2_send_buf(u8 *buf, u16 len)
{
	u16 i ;
  for(i=0;i<len;)			
	{
		while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
	    USART2->DR = buf[i++]; 
		if(i>=len) break;     
	}
}

void USART2_Init(u32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;//����2
		NVIC_InitTypeDef NVIC_InitStructure;//����2�ж�

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ�� 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//GPIOB,GPIOEʱ��
		
		 //RS485 RE   PA.1
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);

	 //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //USART2 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1;//��ռ���ȼ�3
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

    USART_Init(USART2, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
		
		TIM3_Int_Init(99,7199);//10Khz�ļ���Ƶ�ʣ�������1000Ϊ100ms 	
}

volatile uint16_t Usart2FlagCount=0 ;

//��ʱ��3�жϷ������==============================SIM900A���ճ�ʱ���
void TIM3_IRQHandler(void)   //TIM3�ж�
{ 
	if(TIM3->SR&0X01)//�Ǹ����ж�
	{	 			   
		TIM3->SR&=~(1<<0);		//����жϱ�־λ		   
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
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Usart2FlagCount = 2 ;
		Res = USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
		USART2_RX_BUF[USART2_RX_STA] = Res ;
		USART2_RX_STA++ ;
	}  											 
} 

