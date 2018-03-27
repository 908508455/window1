
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

//�������ĺ���
u8 sms_csca[14]={0};

//SIM������
u8 sms_sim[14]={0};

char gprs_cmd[64]={0};

u8 hb_count = 0;//����������
u16 hb_time = 1;//����ʱ��

u8 gprs_init_count = 0;

u16 gprs_send_count = 0;

void GPRS_Heartbeat(void)
{
	if((SIM900A_sta&0x04)==0) return;

	if((hb_time%120)==0)//����֡,2����һ��
	{		
		//Heartbeat(hb_count++,1);//��������֡����
		ReportBoardState();	//�����ϱ���״̬
		hb_time = 1;
	}
}


const u8 *modetbl[2]={"UDP","TCP"};//����ģʽ

u8 GPRS_Init(void)
{
	if(SIM900A_sta&0xc0) return 1;

	GPRS_RX_STA = 0;
	gprs_send_count = 0;

	SIM900A_sta	&= 0xfb;
#ifdef DEBUG
	printf("GPRS CONNECT...\r\n");
#endif
	SIM900A_send_cmd("AT+CIPSHUT\r\n","SHUT OK",500);		//�ر��ƶ����� 
	SIM900A_send_cmd("AT+CIPMODE=0\r\n","OK",500);			//��͸��ģʽ

	if(SocketMode) SocketMode = 1;//TCP
	
	sprintf(gprs_cmd,"AT+CIPSTART=\"%s\",\"%d.%d.%d.%d\",\"%d\"\r\n",
			modetbl[SocketMode],ServerAddr[0],ServerAddr[1],ServerAddr[2],ServerAddr[3],TCPPort);
#ifdef DEBUG	
	printf("%s",gprs_cmd);
#endif
	if(SIM900A_send_cmd(gprs_cmd,"CONNECT OK",500)==0) return 1;//��������
#ifdef DEBUG
	printf("GPRS CONNECT OK!\r\n");
#endif
	SIM900A_sta |= 0x04;

	SendRegister();//ע��֡

	ReportPlayCoinX();//�ϱ������

	ReportAccount();
		
	return 0;
}


//===============����GPRSģ���ָ��===============================
//����0��ʾû�н���ģ��ָ��
//��������ʾ�����˶��ٸ�ģ��ָ��
u8 ReceiveGPRSHandle(void)
{
	u8 i=0;
	char *p1;
	if(USART2_RX_STA&0X8000)//���յ�һ��������
	{		
		p1 = strstr((const char*)&USART2_RX_BUF[0],"+CGREG");//���GPRS�Ƿ����ź�
		if(p1!=NULL)//+CGREG: 0,1
		{
			p1 = strchr(p1,',');
			p1++;
			if((*p1=='1')||(*p1=='5')){//GPRS���ź�
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
//-------------------------------------------------��������
char *SIM900A_send_cmd(char *cmd, char *ack, u16 waittime)
{
	char *res=NULL;
	 	
	CLR_USART2_RX_BUF();
	
	u2_printf("%s",cmd);//��������
//	gprs_flag = 0;

	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				//printf("SIM900A RXD: %s\r\n",(char*)&USART2_RX_BUF[0]);
				//if(strstr((const char*)&USART2_RX_BUF[0],"ERROR")!=NULL){return 0;}	
				res = strstr((const char*)&USART2_RX_BUF[0],ack);//����ȷӦ��
				USART2_RX_STA = 0;				
				if(res) break;														
			} 
		}
		if(waittime==0) res=NULL;//��ʱ 
	}
	return res;
}

//-------------------------------------------------------------------
//SIM900Aģ��Ӳ����λ����ģ���������ʱʹ�á�
void SIM900A_RESET(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delay_ms(30);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delay_ms(30);
}
//-------------------------------------------------------------------
//SIM900Aģ�鿪�ػ����ƣ����ε��� ������ػ�
//��ʱ����2S
extern u8 GPRSCheckTime;
void SIM900A_PCON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);//���ɿ�TERM
	delay_ms(10);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);//����TERM
	delay_ms(1500);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);//���ɿ�TERM
	delay_ms(1500);
	delay_ms(1500);
	GPRSCheckTime = 1;
	SIM900A_sta = 0;
}

//-------------------------------------------------����ע��
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
//-------------------------------------------------����ע��
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
/*��ȡ���ŷ������ĺ���
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
		printf("�������ĺ���:%s\r\n",(char*)&sms_csca[0]);
		return 1;	
	}
	return 0;	
}

//-------------------------------------------------------------- 
/*��ȡ��������
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
//		printf("��������:%s\r\n",(char*)&USART2_RX_BUF[0]);
//		return 1;	
////	}
//	return 0;	
//}

//���SIM900Aģ���ATӦ��,
//x=������,��Ӧ�𷵻�0,��Ӧ�𷵻�1
u8 SIM900A_CheckAT(u8 x)
{
	while(x--)
	{
		if(SIM900A_send_cmd("AT\r\nAT\r\nAT\r\n","OK",100)==0)//���û��AT�ظ�,����3���ؿ���
		{
			printf("\r\nSIM900A POWER ON %d...!\r\n",x);
			SIM900A_PCON();//�ؿ���		
		}
		else
		{
			return 0;
		}		
	}
	return 1;
}

//��⵽û�ź�ʱ,�����³�ʼ��
//��ⲻ��AT�ظ�ʱ,��ⲻ��������ʱ,ͳ�ƴ���
//5��ʧ�ܺ�,���ٳ�ʼ��.��Ϊ��ģ��,�޿�,�����ʲ���.
//��ʼ���ɹ���,����ź�����,���Զ�����GPRS.
//ģ���ʼ������
void SIM900A_InitSet(void)
{
	if(SIM900A_sta&0x80) return;//����ʼ��	

	SIM900A_send_cmd("ATE0\r\n","OK",100);//�رջ���
			
	if(SIM900A_send_cmd("AT+CPIN?\r\n","+CPIN: READY",200)!=0)//����SIM��
	{		
		if(SIM900A_send_cmd("AT+CNMI=2,1\r\n","OK",200)!=0) printf("AT+CNMI=2,1 -> OK!\r\n");
		else printf("AT+CNMI=2,1 -> NG!\r\n");
		if(SIM900A_send_cmd("AT+CMGF=1\r\n\r\n","OK",200)!=0) printf("AT+CMGF=1 -> OK!\r\n");
		else printf("AT+CMGF=1 -> NG!\r\n");
		if(SIM900A_send_cmd("AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n","OK",200)!=0) printf("AT+CPMS=\"SM\",\"SM\",\"SM\" -> OK!\r\n");
		else printf("AT+CPMS=\"SM\",\"SM\",\"SM\" -> NG!\r\n");		

		SIM900A_GetCSCA();//��ȡ�������ĺ��� 
//		SIM900A_GetPhoneNum();//��ȡ�ֻ�����

//		if(SIM900A_CREG()==0){//GSMע��ɹ�
//			SIM900A_sta |= 0x01;
//			printf("GSM�ź�����!\r\n");
//		}else{
//			SIM900A_sta &= 0xfe; 			
//		}
//		if(SIM900A_CGREG()==0){//GPRSע��ɹ�
//			SIM900A_sta |= 0x02;
//			printf("GPRS�ź�����!\r\n");
//			if((SIM900A_sta&0x04)==0) GPRS_Init();
//		}else{
//			SIM900A_sta &= 0xfd;  			
//		} 	
		SIM900A_sta &= 0x3f;	
	}
	else{//�޿�����Ҫ����	 	
		SIM900A_sta |= 0x40;				
	} 
}

void SIM900A_Port_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//GPIOB,GPIOEʱ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);
}

void SIM900A_Init(void)
{
	SIM900A_Port_Init();//���ƶ˿� 

	USART2_Init(9600);//����3��ʼ��  

#ifdef DEBUG
	printf("USART2 init 9600 ok\r\n");
#endif
	if(SIM900A_CheckAT(5))//ģ����Ӧ��
	{
		SIM900A_sta |= 0x80;
	//	return ;
	}
//
//	USART2_Init(38400);//����3��ʼ��  
//	printf("USART2 init 38400 ok\r\n");
//
//	if(SIM900A_CheckAT(5))//ģ����Ӧ��
//	{
//		SIM900A_sta |= 0x80;
//	//	return ;
//	}
//
//	USART2_Init(115200);//����3��ʼ��  
//	printf("USART2 init 115200 ok\r\n");
//
//	if(SIM900A_CheckAT(5))//ģ����Ӧ��
//	{
//		SIM900A_sta |= 0x80;
//	//	return ;
//	}

	SIM900A_InitSet();
}

