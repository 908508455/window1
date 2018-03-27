#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#define  conter_val   4
void TIM2_Int_Init(u16 arr,u16 psc);

void TIM3_Int_Init(u16 arr,u16 psc);

void TIM4_Int_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);



//设置TIM3的开关
//sta:0，关闭;1,开启;
void TIM3_Set(u8 sta);

//设置TIM2的开关
//sta:0，关闭;1,开启;
void TIM2_Set(u8 sta);

//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM4_Set(u8 sta);

 
#endif
