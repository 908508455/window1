#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys.h"

unsigned char check_sum(unsigned char *s,unsigned char len);

int hex2char(char *str, u16 len, u8 *output);

/* 
 * 功能：将十六进制字符串转换为整型(int)数值 
 * */  
int hex2dec(char *hex,int len);


//把整数v转成ibytes个字节的hex字符串，如ibytes == 1，则100 -> 0x64 -> "64"
void dec2hex(__int64 v, int ibytes,unsigned char *output);

//把input转为output，如"abc" -> 0x616263 -> "616263"
int char2hex(unsigned char *input,int i_len,unsigned char *output);

