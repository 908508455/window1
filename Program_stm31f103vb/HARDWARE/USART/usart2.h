
#ifndef __USART2_H_
#define __USART2_H_

#include "sys.h" 
#include "stdio.h"
#include "stdarg.h"
#include "string.h"	 	 
#include "stdio.h"	


#define USART2_REC_LEN  		64	//SIM900A的命令响应返回接收缓冲
#define USART2_SEND_LEN  		64


//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
extern u16 USART2_RX_STA;       //接收状态标记
extern u8 USART2_RX_BUF[USART2_REC_LEN]; 

//接收缓冲区清零
void CLR_USART2_RX_BUF(void);

//发送缓冲区清零
void CLR_USART2_TX_BUF(void);

void u2_printf(char* fmt,...);

void u2_send_buf(u8 *buf, u16 len);

void USART2_Init(u32 bound);


#endif
