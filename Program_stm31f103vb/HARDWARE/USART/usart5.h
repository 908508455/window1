
#ifndef __USART5_H_
#define __USART5_H_

#include "stdio.h"	
#include "sys.h" 

#define SCAN_TG PAout(11)

#define UART5_REC_LEN  		32
#define UART5_SEND_LEN  	32

extern u8 UART5_TX_BUF[UART5_SEND_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

extern u8 UART5_RX_BUF[UART5_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.


//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
extern u16 UART5_RX_STA;       //接收状态标记

void u5_printf(char* fmt,...);

void UART5_Init(u32 bound);


#endif
