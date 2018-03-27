
#ifndef __USART3_H_
#define __USART3_H_

#include "stdio.h"	
#include "sys.h" 

#define USART3_REC_LEN  		64
#define USART3_SEND_LEN  		64

extern u8 USART3_TX_BUF[USART3_SEND_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

extern u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.


//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
extern u16 USART3_RX_STA;       //接收状态标记
extern u8 CashInFlag;

void CLR_USART3_RX_BUF(void);

void CLR_USART3_TX_BUF(void);

void u3_printf(char* fmt,...);

void USART3_Init(u32 bound);


#endif
