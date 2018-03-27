
#include "sim900a.h"
#include "systemui.h"
#include "comm.h"
#include "usart2.h"
#include "led.h"
#include "beep.h"
#include "delay.h"

//#define	DEBUG

u8 SIM900A_sta = 0;
u8 SIM900A_SendSta=0;

//短信中心号码
u8 sms_csca[14]={0};

//SIM卡号码
u8 sms_sim[14]={0};

char gprs_cmd[64]={0};

u8 hb_count = 0;//心跳包计数
u16 hb_time = 1;//心跳时间

u8 gprs_init_count = 0;

u16 gprs_send_count = 0;

void GPRS_Heartbeat(void)
{
	if((SIM900A_sta&0x04)==0) return;

	if((hb_time%120)==0)//心跳帧,2分钟一包
	{		
		//Heartbeat(hb_count++,1);//发送心跳帧命令
		ReportBoardState();	//主动上报板状态
		hb_time = 1;
	}
}


const u8 *modetbl[2]={"UDP","TCP"};//连接模式

u8 GPRS_Init(void)
{
	if(SIM900A_sta&0xc0) return 1;

	GPRS_RX_STA = 0;
	gprs_send_count = 0;

	SIM900A_sta	&= 0xfb;
#ifdef DEBUG
	printf("GPRS CONNECT...\r\n");
#endif
	SIM900A_send_cmd("AT+CIPSHUT\r\n","SHUT OK",500);		//关闭移动场景 
	SIM900A_send_cmd("AT+CIPMODE=0\r\n","OK",500);			//非透传模式

	if(SocketMode) SocketMode = 1;//TCP
	
	sprintf(gprs_cmd,"AT+CIPSTART=\"%s\",\"%d.%d.%d.%d\",\"%d\"\r\n",
			modetbl[SocketMode],ServerAddr[0],ServerAddr[1],ServerAddr[2],ServerAddr[3],TCPPort);
#ifdef DEBUG	
	printf("%s",gprs_cmd);
#endif
	if(SIM900A_send_cmd(gprs_cmd,"CONNECT OK",500)==0) return 1;//发起连接
#ifdef DEBUG
	printf("GPRS CONNECT OK!\r\n");
#endif
	SIM900A_sta |= 0x04;

	SendRegister();//注册帧

	ReportPlayCoinX();//上报活动币数

	ReportAccount();
		
	return 0;
}


//===============处理GPRS模块的指令===============================
//返回0表示没有接收模块指令
//返其他表示处理了多少个模块指令
u8 ReceiveGPRSHandle(void)
{
	u8 i=0;
	char *p1;
	if(USART2_RX_STA&0X8000)//接收到一次数据了
	{		
		p1 = strstr((const char*)&USART2_RX_BUF[0],"+CGREG");//检查GPRS是否有信号
		if(p1!=NULL)//+CGREG: 0,1
		{
			p1 = strchr(p1,',');
			p1++;
			if((*p1=='1')||(*p1=='5')){//GPRS有信号
				SIM900A_sta |= 0x02;
				gprs_init_count = 0;
				if((SIM900A_sta&0x04)==0) GPRS_Init();
			}else{
			    SIM900A_sta &= 0xfd;
				SIM900A_sta &= 0xfb;
				SIM900A_InitSet();
			}			
			i++;
		}
		CLR_USART2_RX_BUF();	
	}else
	{
		if((SIM900A_sta&0x80)==0)
			SIM900A_InitSet();
	} 
	return i;
}
//-------------------------------------------------发送命令
char *SIM900A_send_cmd(char *cmd, char *ack, u16 waittime)
{
	char *res=NULL;
	 	
	CLR_USART2_RX_BUF();
	
	u2_printf("%s",cmd);//发送命令
//	gprs_flag = 0;

	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				//printf("SIM900A RXD: %s\r\n",(char*)&USART2_RX_BUF[0]);
				//if(strstr((const char*)&USART2_RX_BUF[0],"ERROR")!=NULL){return 0;}	
				res = strstr((const char*)&USART2_RX_BUF[0],ack);//有正确应答
				USART2_RX_STA = 0;				
				if(res) break;														
			} 
		}
		if(waittime==0) res=NULL;//超时 
	}
	return res;
}

//-------------------------------------------------------------------
//SIM900A模块硬件复位，当模块软件死机时使用。
void SIM900A_RESET(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delay_ms(30);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delay_ms(30);
}
//-------------------------------------------------------------------
//SIM900A模块开关机控制，单次调用 开机或关机
//延时大于2S
extern u8 GPRSCheckTime;
void SIM900A_PCON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);//先松开TERM
	delay_ms(10);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);//拉低TERM
	delay_ms(1500);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);//先松开TERM
	delay_ms(1500);
	delay_ms(1500);
	GPRSCheckTime = 1;
	SIM900A_sta = 0;
}

//-------------------------------------------------网络注册
//AT+CREG?
//+CREG: 0,1
//OK
u8 SIM900A_CREG(void)
{
	char *p1;
	p1 = SIM900A_send_cmd("AT+CREG?\r\n","+CREG",200);
	if(p1!=NULL)
	{
		p1 = strchr(p1,',');
		p1++;
		if((*p1=='1')||(*p1=='5')) {SIM900A_sta &= 0xdf;return 0;}
		else if((*p1=='0')||(*p1=='3')||(*p1=='4')){SIM900A_sta |= 0x20;}
	}
	return 1;
}
//-------------------------------------------------网络注册
//AT+CGREG?
//+CGREG: 0,1
//OK
u8 SIM900A_CGREG(void)
{
	char *p1;
	p1 = SIM900A_send_cmd("AT+CGREG?\r\n","+CGREG",200);
	if(p1!=NULL)
	{
		p1 = strchr(p1,',');
		p1++;
		if((*p1=='1')||(*p1=='5')) {SIM900A_sta &= 0xef;return 0;}
		else if((*p1=='0')||(*p1=='3')||(*p1=='4')){SIM900A_sta |= 0x10;}
	}
	return 1;
}
//-------------------------------------------------------------- 
/*获取短信服务中心号码
	AT+CSCA?
	+CSCA: "+8613010200500",145
	OK
*/	 		 
u8 SIM900A_GetCSCA(void)
{
	char *p,*pe;
	p = SIM900A_send_cmd("AT+CSCA?\r\n","+8613",300);
	if(p!=NULL)
	{		
		p++;
		pe = strchr(p,'\"');
		pe[0] = '\0';
		sprintf((char*)&sms_csca[0],"%s",p);
		if(strlen((char*)&sms_csca[0])<13) return 0;
		printf("短信中心号码:%s\r\n",(char*)&sms_csca[0]);
		return 1;	
	}
	return 0;	
}

//-------------------------------------------------------------- 
/*获取本机号码
	AT+CNUM
	+CNUM: ,"+8618503010262",145,,4
	OK
*/	 		 
//u8 SIM900A_GetPhoneNum(void)
//{
//	char *p,*pe;
//	p = SIM900A_send_cmd("AT+CNUM\r\n","CNUM",300);
////	u2_printf("AT+CNUM\r\n");
////	delay_ms(200);
////	if(p!=NULL)
////	{		
////		p++;
////		pe = strchr(p,'\"');
////		pe[0] = '\0';
//		//sprintf((char*)&sms_sim[0],"%s",p);
//		//if(strlen((char*)&sms_sim[0])<13) return 0;
//		printf("本机号码:%s\r\n",(char*)&USART2_RX_BUF[0]);
//		return 1;	
////	}
//	return 0;	
//}

//检测SIM900A模块的AT应答,
//x=检测次数,有应答返回0,无应答返回1
u8 SIM900A_CheckAT(u8 x)
{
	while(x--)
	{
		if(SIM900A_send_cmd("AT\r\nAT\r\nAT\r\n","OK",100)==0)//如果没有AT回复,尝试3次重开机
		{
			printf("\r\nSIM900A POWER ON %d...!\r\n",x);
			SIM900A_PCON();//重开机		
		}
		else
		{
			return 0;
		}		
	}
	return 1;
}

//检测到没信号时,就重新初始化
//检测不到AT回复时,检测不到卡就绪时,统计次数
//5次失败后,不再初始化.认为无模块,无卡,或波特率不对.
//初始化成功后,检测信号正常,则自动连接GPRS.
//模块初始化函数
void SIM900A_InitSet(void)
{
	if(SIM900A_sta&0x80) return;//不初始化	

	SIM900A_send_cmd("ATE0\r\n","OK",100);//关闭回显
			
	if(SIM900A_send_cmd("AT+CPIN?\r\n","+CPIN: READY",200)!=0)//存在SIM卡
	{		
		if(SIM900A_send_cmd("AT+CNMI=2,1\r\n","OK",200)!=0) printf("AT+CNMI=2,1 -> OK!\r\n");
		else printf("AT+CNMI=2,1 -> NG!\r\n");
		if(SIM900A_send_cmd("AT+CMGF=1\r\n\r\n","OK",200)!=0) printf("AT+CMGF=1 -> OK!\r\n");
		else printf("AT+CMGF=1 -> NG!\r\n");
		if(SIM900A_send_cmd("AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n","OK",200)!=0) printf("AT+CPMS=\"SM\",\"SM\",\"SM\" -> OK!\r\n");
		else printf("AT+CPMS=\"SM\",\"SM\",\"SM\" -> NG!\r\n");		

		SIM900A_GetCSCA();//获取短信中心号码 
//		SIM900A_GetPhoneNum();//获取手机号码

//		if(SIM900A_CREG()==0){//GSM注册成功
//			SIM900A_sta |= 0x01;
//			printf("GSM信号正常!\r\n");
//		}else{
//			SIM900A_sta &= 0xfe; 			
//		}
//		if(SIM900A_CGREG()==0){//GPRS注册成功
//			SIM900A_sta |= 0x02;
//			printf("GPRS信号正常!\r\n");
//			if((SIM900A_sta&0x04)==0) GPRS_Init();
//		}else{
//			SIM900A_sta &= 0xfd;  			
//		} 	
		SIM900A_sta &= 0x3f;	
	}
	else{//无卡或卡需要密码	 	
		SIM900A_sta |= 0x40;				
	} 
}

void SIM900A_Port_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//GPIOB,GPIOE时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);
}

void SIM900A_Init(void)
{
	SIM900A_Port_Init();//控制端口 

	USART2_Init(9600);//串口3初始化  

#ifdef DEBUG
	printf("USART2 init 9600 ok\r\n");
#endif
	if(SIM900A_CheckAT(5))//模块无应答
	{
		SIM900A_sta |= 0x80;
	//	return ;
	}
//
//	USART2_Init(38400);//串口3初始化  
//	printf("USART2 init 38400 ok\r\n");
//
//	if(SIM900A_CheckAT(5))//模块无应答
//	{
//		SIM900A_sta |= 0x80;
//	//	return ;
//	}
//
//	USART2_Init(115200);//串口3初始化  
//	printf("USART2 init 115200 ok\r\n");
//
//	if(SIM900A_CheckAT(5))//模块无应答
//	{
//		SIM900A_sta |= 0x80;
//	//	return ;
//	}

	SIM900A_InitSet();
}

