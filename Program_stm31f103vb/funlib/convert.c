#include "convert.h"

//说明：传入要校验的数据，返回一个校验值，
//当收到数据包时，解密后，取出数据包里的校验值和要校验的数据，把要校验的数据传进来，返回的校验值和包里的校验值一致，则包是合法的，否则是非法的
//当发送数据包时，把要校验的数据传进来生成校验值，再把校验值放回数据包里，然后加密并发送
unsigned char check_sum(unsigned char *s,unsigned char len)
{
	unsigned char i;
	unsigned short check = 0;
	unsigned char sum=0;
	for(i = 0; i < len; i ++)
	{
		check += s[i];
	}
	check = (check >> 8) + (check & 0xff);      //高八位+低八位
	check += (check >> 8);                      //为防止进位，再加一次
	sum = check & 0x00ff;

	return ~sum;
}

/* 
 * 功能：将十六进制字符串转换为char字符串, 如"646566" -> 0x64 0x65 0x66
 * */ 
char ConvertHexChar(char ch)
{
	if((ch>='0')&&(ch<='9'))   
		return   (ch-0x30);   
	else if((ch>='A')&&(ch<='F'))   
		return   (ch-'A'+10);   
	else if((ch>='a')&&(ch<='f'))   
		return   (ch-'a'+10);   
	else
		return   (0);  
}
int hex2char(char *str, u16 len, u8 *output)
{
	u16 i,j;
	for(i=0,j=0;str[i]!='\0';)
	{
		output[j] = ConvertHexChar(str[i++]);
		output[j] = output[j]<<4;
		output[j] += ConvertHexChar(str[i++]);
		j++;
	}
	return (j-1);
}

/* 
 * 功能：将十六进制字符串转换为整型(int)数值 
 * */  
int hex2dec(char *hex,int len)  
{  
	int v = 0;
	char format_Str[20];

	memset(format_Str,0,20);
	sprintf(format_Str,"%s%dx","%",len);
	sscanf(hex,format_Str,&v);

	return v;
}  


//把整数v转成ibytes个字节的hex字符串，如ibytes == 1，则100 -> 0x64 -> "64"
void dec2hex(__int64 v, int ibytes,unsigned char *output)
{
	int o_len = ibytes*2;

	char format_Str[20];
	memset(format_Str,0,20);
	sprintf(format_Str,"%s0%dx","%",o_len);
	sprintf((char*)output,format_Str,v);
}

//把input转为output，如"abc" -> 0x616263 -> "616263"
int char2hex(unsigned char *input,int i_len,unsigned char *output)
{
	int i=0;
	unsigned char v = 0;
	int o_len = 0;
	for (i=0;i<i_len;i++)
	{
		v = input[i];
		sprintf((char *)(output+o_len),"%02x",v);
		o_len += 2;
	}
	return o_len;
}

