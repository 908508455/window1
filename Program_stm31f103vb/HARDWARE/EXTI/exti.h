#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"
#include "d_led.h"


#define CoinInput1  PEin(0)
#define	CoinInput2	PEin(1)
#define PluseInput1	PEin(2)
#define PluseInput2	PEin(3)

#define input1	PEin(0)
#define input2	PEin(1)

extern u8 IntSta;

 	 
void EXTIX_Init(void);//外部中断初始化
void Key_Sure_Init(void) ;


		 					    
#endif

