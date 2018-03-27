#ifndef __SYSTEM_UI_H
#define __SYSTEM_UI_H

#include "sys.h"


typedef struct{
	u8 sta;
	u16 x_pos;
	u16 y_pos;
	u16 x_size;
	u16 y_size;
	u8 *title;
}_win_typ;







extern u8 RT[4];







void DrawWinFun(void);








		 				    
#endif
