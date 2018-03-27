#ifndef __SIM900A_H
#define __SIM900A_H
	 
#include "sys.h"

#define GPRS_FLAG 0x80
#define GSM_FLAG 0x40


//SIM900A��״̬
extern u8 SIM900A_sta;

extern u8 SIM900A_SendSta;

extern u16 hb_time;//����ʱ��


//�������ĺ���
extern u8 sms_csca[14];

//SIM������
//extern u8 sms_sim[14];

extern const u8 *modetbl[2];//����ģʽ
extern char gprs_cmd[64];//����GPRS��������

u8 SIM900A_CREG(void);

u8 SIM900A_CGREG(void);

char *SIM900A_send_cmd(char *cmd, char *ack, u16 waittime);

void GPRS_Heartbeat(void);

//u8 SIM900A_GetPhoneNum(void);

//����0��ʾû�н���ģ��ָ��
//��������ʾ�����˶��ٸ�ģ��ָ��
u8 ReceiveGPRSHandle(void);

void SIM900A_Init(void);

//ģ���ʼ������
void SIM900A_InitSet(void);

u8 GPRS_Init(void);

//���SIM900Aģ���ATӦ��,
//x=������,��Ӧ�𷵻�0,��Ӧ�𷵻�1
u8 SIM900A_CheckAT(u8 x);












#endif
