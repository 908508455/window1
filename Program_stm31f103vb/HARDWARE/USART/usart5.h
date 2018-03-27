
#ifndef __USART5_H_
#define __USART5_H_

#include "stdio.h"	
#include "sys.h" 

#define SCAN_TG PAout(11)

#define UART5_REC_LEN  		32
#define UART5_SEND_LEN  	32

extern u8 UART5_TX_BUF[UART5_SEND_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

extern u8 UART5_RX_BUF[UART5_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.


//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
extern u16 UART5_RX_STA;       //����״̬���

void u5_printf(char* fmt,...);

void UART5_Init(u32 bound);


#endif
