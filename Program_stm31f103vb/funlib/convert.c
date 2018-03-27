#include "convert.h"

//˵��������ҪУ������ݣ�����һ��У��ֵ��
//���յ����ݰ�ʱ�����ܺ�ȡ�����ݰ����У��ֵ��ҪУ������ݣ���ҪУ������ݴ����������ص�У��ֵ�Ͱ����У��ֵһ�£�����ǺϷ��ģ������ǷǷ���
//���������ݰ�ʱ����ҪУ������ݴ���������У��ֵ���ٰ�У��ֵ�Ż����ݰ��Ȼ����ܲ�����
unsigned char check_sum(unsigned char *s,unsigned char len)
{
	unsigned char i;
	unsigned short check = 0;
	unsigned char sum=0;
	for(i = 0; i < len; i ++)
	{
		check += s[i];
	}
	check = (check >> 8) + (check & 0xff);      //�߰�λ+�Ͱ�λ
	check += (check >> 8);                      //Ϊ��ֹ��λ���ټ�һ��
	sum = check & 0x00ff;

	return ~sum;
}

/* 
 * ���ܣ���ʮ�������ַ���ת��Ϊchar�ַ���, ��"646566" -> 0x64 0x65 0x66
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
 * ���ܣ���ʮ�������ַ���ת��Ϊ����(int)��ֵ 
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


//������vת��ibytes���ֽڵ�hex�ַ�������ibytes == 1����100 -> 0x64 -> "64"
void dec2hex(__int64 v, int ibytes,unsigned char *output)
{
	int o_len = ibytes*2;

	char format_Str[20];
	memset(format_Str,0,20);
	sprintf(format_Str,"%s0%dx","%",o_len);
	sprintf((char*)output,format_Str,v);
}

//��inputתΪoutput����"abc" -> 0x616263 -> "616263"
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

