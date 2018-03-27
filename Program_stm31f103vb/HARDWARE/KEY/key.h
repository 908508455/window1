#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define KEY_SURE  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//读取按键0
#define KEY_UP    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//读取按键1
#define KEY_DOWN  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//读取按键2


void KEY_Init(void);//IO初始化
u8 KEY_Scan(void);
			    
#endif
