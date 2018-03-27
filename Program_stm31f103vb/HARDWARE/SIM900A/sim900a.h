#ifndef __SIM900A_H
#define __SIM900A_H
	 
#include "sys.h"

#define GPRS_FLAG 0x80
#define GSM_FLAG 0x40


//SIM900A的状态
extern u8 SIM900A_sta;

extern u8 SIM900A_SendSta;

extern u16 hb_time;//心跳时间


//短信中心号码
extern u8 sms_csca[14];

//SIM卡号码
//extern u8 sms_sim[14];

extern const u8 *modetbl[2];//连接模式
extern char gprs_cmd[64];//生成GPRS连接命令

u8 SIM900A_CREG(void);

u8 SIM900A_CGREG(void);

char *SIM900A_send_cmd(char *cmd, char *ack, u16 waittime);

void GPRS_Heartbeat(void);

//u8 SIM900A_GetPhoneNum(void);

//返回0表示没有接收模块指令
//返其他表示处理了多少个模块指令
u8 ReceiveGPRSHandle(void);

void SIM900A_Init(void);

//模块初始化函数
void SIM900A_InitSet(void);

u8 GPRS_Init(void);

//检测SIM900A模块的AT应答,
//x=检测次数,有应答返回0,无应答返回1
u8 SIM900A_CheckAT(u8 x);












#endif
