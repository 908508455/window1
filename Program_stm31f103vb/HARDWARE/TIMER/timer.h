#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#define  conter_val   4
void TIM2_Int_Init(u16 arr,u16 psc);

void TIM3_Int_Init(u16 arr,u16 psc);

void TIM4_Int_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);



//����TIM3�Ŀ���
//sta:0���ر�;1,����;
void TIM3_Set(u8 sta);

//����TIM2�Ŀ���
//sta:0���ر�;1,����;
void TIM2_Set(u8 sta);

//����TIM4�Ŀ���
//sta:0���ر�;1,����;
void TIM4_Set(u8 sta);

 
#endif
