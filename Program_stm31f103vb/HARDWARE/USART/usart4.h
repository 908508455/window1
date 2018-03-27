
#ifndef __USART4_H_
#define __USART4_H_

#include "stdio.h"	
#include "sys.h" 

#define UART4_REC_LEN  		64
#define UART4_SEND_LEN  	64

extern u8 UART4_TX_BUF[UART4_SEND_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

extern u8 UART4_RX_BUF[UART4_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.


//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
extern u16 UART4_RX_STA;       //����״̬���

extern u8 CashOutFlag;//����ģ��ֽ�������ʱ��,��λ��1

void CLR_UART4_RX_BUF(void);

void CLR_UART4_TX_BUF(void);

void u4_printf(char* fmt,...);

//COM4����N�ֽ�����
void u4_SendNByte(u8 *buf, u8 len);


void UART4_Init(u32 bound);


#endif
