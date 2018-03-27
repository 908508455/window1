#ifndef __D_LED_H
#define __D_LED_H

#include "sys.h"
#include "delay.h"

#define COM0 PEout(6)
#define COM1 PEout(5) 
#define COM2 PEout(4)  
#define COM3 PEout(3)  

#define CRTL1 PEout(2)  
#define CRTL2 PBout(5) 


#define RCLK PCout(0)
#define SCLR PCout(1)
#define SCLK PCout(2)
#define DIN  PCout(3)

void show_number(uint8_t data,uint8_t which);
void dled_init(void) ;

#endif
