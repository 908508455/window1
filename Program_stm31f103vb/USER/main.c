#include "sys.h"
#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "led.h"
#include "timer.h"
#include "key.h"
#include "delay.h"
#include "d_led.h"
#include "stmflash.h"
#include "exti.h"


#define  FLASH_SAVE_ADDR  0X0801ff00


uint8_t open_state[3]={0x00,0x10,0x13};
uint8_t close_state[3]={0x01,0x10,0x13};

extern float money_tmp ;
extern uint8_t fun_enter; 
extern uint8_t enter_data[8];

extern u8 USART2_RX_BUF[64];     //接收缓冲,最大USART_REC_LEN个字节.
extern u16 USART2_RX_STA ; 

uint16_t test_cnt =0  ;

volatile uint8_t current_ch = 0 ;
volatile uint16_t current_val[4] = {0,0,0,0} ;
volatile uint8_t save_flag = 0;
volatile uint16_t save_cnt = 0; 
void HandleBuf(u8* p,u16 len) ;
	
int main(void)
{
	u16 led0pwmval=100;
	delay_init();	    	 //延时函数初始化	 	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	LED_Init();
	Key_Init();    //按键初始化
	Key_Sure_Init();
	EXTIX_Init();
	USART2_Init(115200);
	dled_init();
	TIM4_PWM_Init(999,0);
	TIM2_Int_Init(999,7199);   //100ms中断一次
	USART2_RX_STA = 0 ;
	STMFLASH_Read(FLASH_SAVE_ADDR,current_val,4);
	TIM_SetCompare3(TIM4,current_val[0]*conter_val);
	TIM_SetCompare4(TIM4,current_val[1]*conter_val);
	TIM_SetCompare1(TIM4,current_val[2]*conter_val);
	TIM_SetCompare2(TIM4,current_val[3]*conter_val);
	while(1)
	{
		show_number(current_ch,0);
		delay_ms(2);
		show_number((current_val[current_ch]/100),1);
		delay_ms(2);
		show_number((current_val[current_ch]%100/10),2);
		delay_ms(2);
		show_number((current_val[current_ch]%10),3);
		delay_ms(2);	
		if(save_flag == 2){
			STMFLASH_Write(FLASH_SAVE_ADDR,current_val,4);
			save_flag = 0;
			save_cnt = 0 ;
		}
		if( USART2_RX_STA &0x8000 )
		{ 
	//		printf("recevie data!!!");
			HandleBuf(USART2_RX_BUF,(USART2_RX_STA&0x00ff));
			USART2_RX_STA = 0 ; 
		}
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==0){
				delay_ms(1);
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==0){
					CRTL2=1;
					CRTL1=1;
				}
		}
	}
}


/*
* FE 05 00 00 00 00 D9 C5    --关闭继电器
* FE 05 00 00 FF 00 98 35    --打开继电器
* FE 05 00 01 00 00 88 05    --关闭NPN
* FE 05 00 01 FF 00 C9 F5 	 --打开NPN
*
* 0x55 0x01 0x0y 0xyy 0xc8 0xc9  --设定y通道的亮度值
* 0x55 0x02 0xnum 0xzz  0xc3 0xc4  -- z=0x00 关闭 0xff 打开  num 那个通道
*/
void HandleBuf(u8* p,u16 len)
{
	if(len==8){    //485 发送的数据还是PLC发送的数据
			if(p[0]==0xFE&p[1]==0x05){   //打开或关闭继电器命令
					if(p[2]==0x00&p[3]==0x00&p[4]==0x00&p[5]==0x00&p[6]==0xD9&p[7]==0xC5){
						CRTL1 = 0 ;
						printf("close relay ok!!!\n");
					}else if(p[2]==0x00&p[3]==0x00&p[4]==0xFF&p[5]==0x00&p[6]==0x98&p[7]==0x35){
						CRTL1 = 1 ;	
						printf("open relay ok!!!\n");
					}else if(p[2]==0x00&p[3]==0x01&p[4]==0x00&p[5]==0x00&p[6]==0x88&p[7]==0x05){       //打开或者关闭NPN
						CRTL2 = 1 ;
						printf("close NPN ok !!!\n");
					}else if(p[2]==0x00&p[3]==0x01&p[4]==0xFF&p[5]==0x00&p[6]==0xC9&p[7]==0xF5){
						CRTL2 = 0 ;
						printf("open NPN ok !!!\n");
					}
			}
	}else if(len==6){
			if(p[0]==0x55&p[1]==0x01){
				 if(p[4]==0xc8&p[5]==0xc9){
						switch(p[2]){
							case 0:
								current_val[p[2]]=p[3];
								TIM_SetCompare3(TIM4,p[3]*conter_val);
								printf("setting ch0 value is ok value =%d\r\n",p[3]);
								break ;
							case 1:
								current_val[p[2]]=p[3];
								TIM_SetCompare4(TIM4,p[3]*conter_val);
								printf("setting ch1 value is ok value =%d\r\n",p[3]);
								break ;
							case 2:
								current_val[p[2]]=p[3];
								TIM_SetCompare1(TIM4,p[3]*conter_val);
								printf("setting ch2 value is ok value =%d\r\n",p[3]);
								break ;
							case 3:
								current_val[p[2]]=p[3];
								TIM_SetCompare2(TIM4,p[3]*conter_val);
								printf("setting ch3 value is ok value =%d\r\n",p[3]);
								break ;
							default:
								printf("setting channel larger!!!\r\n");
								break;
						}
						if(p[2]<4){
								save_flag = 1 ;
								save_cnt = 0;
						}
				 }
			}else if(p[0]==0x55&p[1]==0x02){
					 if(p[3]==0x00&p[4]==0xc3&p[5]==0xc4){  //关闭相关通道
							switch(p[2]){
								case 0:
								  TIM_SetCompare3(TIM4,0);
									printf("0\r\n");
								//u2_send_buf(close_state,3);
								//  printf("close ch0\r\n");
								break ;
								case 1:
								  TIM_SetCompare4(TIM4,0);
									printf("close ch1\r\n");
								break ;
								case 2:
								  TIM_SetCompare1(TIM4,0);
									printf("close ch2\r\n");
								break ; 								
								case 3:
								  TIM_SetCompare2(TIM4,0);
									printf("close ch3\r\n");
								break ; 
							}
					 }else if(p[3]==0xff&p[4]==0xc3&p[5]==0xc4){  //打开相关通道 
							switch(p[2]){
								case 0:
								  TIM_SetCompare3(TIM4,current_val[p[2]]*conter_val);
									printf("1\r\n");
								//		u2_send_buf(open_state,3);
								//	printf("open ch0\r\n");
								break ;
								case 1:
								  TIM_SetCompare4(TIM4,current_val[p[2]]*conter_val);
							  	printf("open ch1\r\n");
								break ;
								case 2:
								  TIM_SetCompare1(TIM4,current_val[p[2]]*conter_val);
									printf("open ch2\r\n");
								break ; 								
								case 3:
								  TIM_SetCompare2(TIM4,current_val[p[2]]*conter_val);
									printf("open ch3\r\n");
								break ; 
							}					 
					 }
			}
	}else if(len == 3){
		if(p[0]==0x55&p[1]==0x55&p[2]==0x55){
			printf("\r\nch0= %d ;ch1= %d ;ch2= %d ;ch3= %d\r\n",current_val[0],current_val[1],current_val[2],current_val[3]);
			printf("CRTL1=%d; CRTL2=%d; input1=%d; input2=%d\r\n",CRTL1,!CRTL2,input1,input2);
		}
	}
	else if(len == 17){
		
	}
}

