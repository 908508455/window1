#ifndef __DMA_H
#define	__DMA_H	
   
#include "sys.h"

extern u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 
							    					    

void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);//配置DMA1_CHx

void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);//使能DMA1_CHx

void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar);

void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len);
		   
#endif




