
#ifndef __USART2_H_
#define __USART2_H_

#include "sys.h" 
#include "stdio.h"
#include "stdarg.h"
#include "string.h"	 	 
#include "stdio.h"	


#define USART2_REC_LEN  		64	//SIM900A��������Ӧ���ؽ��ջ���
#define USART2_SEND_LEN  		64


//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
extern u16 USART2_RX_STA;       //����״̬���
extern u8 USART2_RX_BUF[USART2_REC_LEN]; 

//���ջ���������
void CLR_USART2_RX_BUF(void);

//���ͻ���������
void CLR_USART2_TX_BUF(void);

void u2_printf(char* fmt,...);

void u2_send_buf(u8 *buf, u16 len);

void USART2_Init(u32 bound);


#endif
