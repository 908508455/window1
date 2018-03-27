#include "key.h"
#include "delay.h"
#include "stdio.h"

								    
//������ʼ������
void Key_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
}

u8 KEY_Scan(void)
{
  if(KEY_UP==0){
		delay_ms(1);
		if(KEY_UP==0){
			return 2 ;
		}
	}else if(KEY_DOWN==0){
		delay_ms(1);
		if(KEY_DOWN==0){
			return 3 ;
		}
	}else{
		return 0 ;
	}
}