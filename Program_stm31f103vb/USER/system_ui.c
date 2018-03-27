

#include "system_ui.h"

#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "led.h"
#include "timer.h"
#include "beep.h"
#include "rtc.h" 	
#include "24cxx.h" 
#include "key.h"
#include "lcd.h"
#include "delay.h"

u8 RT[4]={0};

void DrawWinFun(void)
{	
	POINT_COLOR = BLACK;
	
	LCD_Fill(10,10,225,140,GREEN);	
	BACK_COLOR = GREEN;
	LCD_ShowString(12,13,24,"Ë®ÏäÎÂ¶È");	
	LCD_ShowString(12,123,24,"×´Ì¬£º");	
	LCD_ShowString(26,48,64,"38.0");
	
	LCD_Fill(245,10,225,140,GBLUE);
	BACK_COLOR = GBLUE;
	LCD_ShowString(247,13,24,"½øË®ÎÂ¶È");
	LCD_ShowString(247,123,24,"×´Ì¬£º");
	LCD_ShowString(245+16,48,64,"20.2");
	
	LCD_Fill(10,160,225,140,CYAN);
	BACK_COLOR = CYAN;
	LCD_ShowString(12,163,24,"Ì«Ñô°åÎÂ¶È");
	LCD_ShowString(12,273,24,"×´Ì¬£º");
	LCD_ShowString(26,160+38,64,"61.2");
	
	LCD_Fill(245,160,225,140,YELLOW);
	BACK_COLOR = YELLOW;
	LCD_ShowString(247,163,24,"¿ÕÑ¹»úÎÂ¶È");
	LCD_ShowString(247,273,24,"×´Ì¬£º");	
	LCD_ShowString(245+16,160+38,64,"46.8");

}


