
#ifndef __USART4_H_
#define __USART4_H_

#include "stdio.h"	
#include "sys.h" 

#define UART4_REC_LEN  		64
#define UART4_SEND_LEN  	64

extern u8 UART4_TX_BUF[UART4_SEND_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

extern u8 UART4_RX_BUF[UART4_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.


//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
extern u16 UART4_RX_STA;       //接收状态标记

extern u8 CashOutFlag;//当有模拟纸币输出的时候,此位置1

void CLR_UART4_RX_BUF(void);

void CLR_UART4_TX_BUF(void);

void u4_printf(char* fmt,...);

//COM4发送N字节数据
void u4_SendNByte(u8 *buf, u8 len);


void UART4_Init(u32 bound);


#endif
