#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define KEY_SURE  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//��ȡ����0
#define KEY_UP    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//��ȡ����1
#define KEY_DOWN  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//��ȡ����2


void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(void);
			    
#endif
