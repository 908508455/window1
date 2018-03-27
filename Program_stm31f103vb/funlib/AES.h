
#ifndef __AES_H__
#define __AES_H__

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "sys.h"

extern unsigned char AesKey[20];
//extern unsigned char AesKey2[20];

extern  u8 const sBox[256];
extern  u8 const invsBox[256];
//extern  u8 key[16];
//extern  u8 w[10][4][4];

typedef struct
{
	u8 Sbox[256];
	u8 InvSbox[256];
	u8 key[16];
	u8 w[11][4][4];
	u8 KeyLength;
	u8 SuperW[11][4][4];
}AES,*PAES;

extern AES tAes;

extern u8 TestAes[100],TestAes2[100],TestAes3[100];
extern int CipherCnt;
extern int InvCipherCnt;

unsigned char Cipher(unsigned char *input);
unsigned char InvCipher(unsigned char *input);
//void *pCipher(void *input, int length);
//void *pInvCipher(void *input, int length);
void KeyExpansion(unsigned char *key, unsigned char w[][4][4]);
unsigned char FFmul(unsigned char a, unsigned char b);
void SubBytes(unsigned char state[][4]);
void ShiftRows(unsigned char state[][4]);
void MixColumns(unsigned char state[][4]);
void AddRoundKey(unsigned char state[][4], unsigned char k[][4]);
void InvSubBytes(unsigned char state[][4]);
void InvShiftRows(unsigned char state[][4]);
void InvMixColumns(unsigned char state[][4]);

int InvCipher_MemoryByAes(unsigned char *input,unsigned char *output,unsigned char length,u8 *pkey);
int Cipher_MemoryByAes(unsigned char *input ,unsigned char *output ,unsigned char length,u8 *pkey);

int TestAesFuncation(void);
void Init_AesVariable(void);

void print_AES(unsigned char* state,u8 length);

#endif



