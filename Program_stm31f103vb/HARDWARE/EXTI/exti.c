#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"
#define  conter_val   4

void Key_Sure_Init(void)
{ 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB,ENABLE);	//使能复用功能时钟


	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;//按键端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成上拉输入GPIO_Mode_IN_FLOATING/GPIO_Mode_IPU
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	

	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);    
}
 
//外部中断0服务程序
void EXTIX_Init(void)
{ 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOE,ENABLE);	//使能复用功能时钟


	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;//按键端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成上拉输入GPIO_Mode_IN_FLOATING/GPIO_Mode_IPU
 	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);   
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
}

//外部中断0服务程序 
void EXTI0_IRQHandler(void)
{
	delay_ms(1);//消抖
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
	printf("trigger input1 value now is %d\r\n",input2);
	//CRTL1=~CRTL1;
	//CRTL2=~CRTL2;                                                                                         
}
extern volatile uint16_t current_val[4]  ;
volatile uint8_t flag1_down_trigger = 0 ;
volatile uint8_t flag1_up_trigger = 0 ;
void EXTI1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE0上的中断标志位 
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==1){
   	delay_ms(1);//消抖
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==1){
		//	if(flag1_down_trigger==0){
	    	printf("1\r\n");
		//		CRTL1=0;
		//	  CRTL2=0;
		//	  flag1_down_trigger = 4;
		//}
			}
		//TIM_SetCompare3(TIM4,current_val[0]*conter_val);
	}
	else{	
			printf("0\r\n");
		//	CRTL2=1;
		//	CRTL1=1;
		
			//	TIM_SetCompare3(TIM4,0);
	}
}

//外部中断0服务程序 
extern volatile uint8_t current_ch  ;
void EXTI15_10_IRQHandler(void)
{
	delay_ms(1);
	EXTI_ClearITPendingBit(EXTI_Line13); //清除LINE0上的中断标志位
	if(KEY_SURE == 0){
		current_ch++;
		if(current_ch>=4){
			current_ch = 0;
		}
	//	CRTL1=~CRTL1;
	}
}


 
