#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys.h"

unsigned char check_sum(unsigned char *s,unsigned char len);

int hex2char(char *str, u16 len, u8 *output);

/* 
 * ���ܣ���ʮ�������ַ���ת��Ϊ����(int)��ֵ 
 * */  
int hex2dec(char *hex,int len);


//������vת��ibytes���ֽڵ�hex�ַ�������ibytes == 1����100 -> 0x64 -> "64"
void dec2hex(__int64 v, int ibytes,unsigned char *output);

//��inputתΪoutput����"abc" -> 0x616263 -> "616263"
int char2hex(unsigned char *input,int i_len,unsigned char *output);

