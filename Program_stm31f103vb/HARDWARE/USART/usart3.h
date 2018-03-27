
#ifndef __USART3_H_
#define __USART3_H_

#include "stdio.h"	
#include "sys.h" 

#define USART3_REC_LEN  		64
#define USART3_SEND_LEN  		64

extern u8 USART3_TX_BUF[USART3_SEND_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

extern u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.


//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
extern u16 USART3_RX_STA;       //����״̬���
extern u8 CashInFlag;

void CLR_USART3_RX_BUF(void);

void CLR_USART3_TX_BUF(void);

void u3_printf(char* fmt,...);

void USART3_Init(u32 bound);


#endif
