#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <immintrin.h>

using namespace std;



// **************** AES  functions ****************
uint8_t wbyte(uint32_t w, int pos) 
{
	return (w >> (pos * 8)) & 0xff;
}
void hexprint16(uint8_t *p,unsigned int how_long) 
{
		for (unsigned int i = 0; i < how_long; i++)
		{
			printf("%02hhx ", p[i]);
			if (((i + 1) % 16) == 0)
				printf("\n     ");
		}
	puts("");
	
}
void hexprint32(uint32_t *p) 
{
for (int k = 0 ;k < 16 ; k++)
	{
		for (int i = 0; i < 4; i++)
		{
			printf("%02hhx ", wbyte(p[k],i) );
		}
		if (((k + 1) % 4) == 0)
		printf("\n     ");
	}
	puts("");
	
}

inline __m128i AES_help (__m128i temp1, __m128i temp2)     
{ 
	__m128i temp3;     
	temp2 = _mm_shuffle_epi32 (temp2 ,0xff);     
	temp3 = _mm_slli_si128 (temp1, 0x4);     
	temp1 = _mm_xor_si128 (temp1, temp3);     
	temp3 = _mm_slli_si128 (temp3, 0x4);     
	temp1 = _mm_xor_si128 (temp1, temp3);     
	temp3 = _mm_slli_si128 (temp3, 0x4);     
	temp1 = _mm_xor_si128 (temp1, temp3);       
	temp1 = _mm_xor_si128 (temp1, temp2);     
	return temp1;     
} 

void AES_KEY_EXP(const unsigned char *userkey,__m128i *Key_Schedule)
{
	__m128i temp1, temp2; 
	
	temp1 = _mm_loadu_si128((__m128i*)userkey);     
	Key_Schedule[0] = temp1;     
	temp2 = _mm_aeskeygenassist_si128 (temp1 ,0x1);     
	temp1 = AES_help(temp1, temp2);     
	Key_Schedule[1] = temp1;     
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x2);     
	temp1 = AES_help(temp1, temp2);     
	Key_Schedule[2] = temp1; 
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x4);     
	temp1 = AES_help(temp1, temp2);     
	Key_Schedule[3] = temp1;     
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x8);     
	temp1 = AES_help(temp1, temp2);     
	Key_Schedule[4] = temp1;     
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x10);     
	temp1 = AES_help(temp1, temp2);     
	Key_Schedule[5] = temp1;
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x20);     
	temp1 = AES_help(temp1, temp2);     
	Key_Schedule[6] = temp1;     
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x40);     
	temp1 = AES_help(temp1, temp2);     
	Key_Schedule[7] = temp1;     
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x80);     
	temp1 = AES_help(temp1, temp2);     
	Key_Schedule[8] = temp1;         
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x1b);     
	temp1 = AES_help(temp1, temp2);     
	Key_Schedule[9] = temp1;     
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x36);     
	temp1 = AES_help(temp1, temp2);     
	Key_Schedule[10] = temp1;
}
void AES(const unsigned char *in, unsigned char *out,unsigned long length,__m128i *Key_Schedule)
{ 
	__m128i tmp;     
	unsigned int i,j;     
	length = length/16;     
	for(i=0; i < length; i++)
	{         
		tmp = _mm_loadu_si128 (&((__m128i*)in)[i]);         
		tmp = _mm_xor_si128 (tmp,Key_Schedule[0]);            
		for(j=1; j <10; j++)
		{             
			tmp = _mm_aesenc_si128 (tmp,Key_Schedule[j]);         
		}         
		tmp = _mm_aesenclast_si128 (tmp,Key_Schedule[j]); 
		_mm_storeu_si128 (&((__m128i*)out)[i],tmp);         
	}
  
} 

//****************************
// MAIN function: AES testing
//****************************
int main(int argc, char* argv[])
{
	unsigned int count = 1;
	
	
	
	uint8_t key[16] = 		{ 	0xde,0xad,0xbe,0xef,0x20,0x74,0x6f,0x70,0x20,0x73,0x65,0x63,0x72,0x65,0x74,0x21 };
	uint8_t in[64] =  		{ 	0x25,0xeb,0x8c,0x48,0xff,0x89,0xcb,0x85,0x4f,0xc0,0x90,0x81,0xcc,0x47,0xed,0xfc, 
								0x86,0x19,0xb2,0x14,0xfe,0x65,0x92,0xd4,0x8b,0xfc,0xea,0x9c,0x9d,0x8e,0x32,0x44, 
								0xd7,0xd7,0xe9,0xf1,0xf7,0xde,0x60,0x56,0x8d,0xe9,0x89,0x07,0x3f,0x3d,0x16,0x39, 
								0x01,0x80,0x3c,0xd1,0x08,0xd8,0x8d,0x73,0xaf,0xea,0x79,0xc8,0x1e,0x47,0x83,0xc6 };
	
	
	uint8_t out[64] = {};
	
	
	__m128i Key_Schedule[44];
	AES_KEY_EXP(key,Key_Schedule);
	
	printf("Encryption using AES-NI \n\n");
	printf("Key: ");
	hexprint16(key,16);
	puts("");
	printf("In:  ");
	hexprint16(in,64);
	puts("");
	
	
	
	for (unsigned int i = 0 ; i < count ; i++)
	{
		AES(in,out,64,Key_Schedule);
	}
		
	
	
	

	int test_failed = 0;
	uint8_t res_out[64] = 	{ 0x08 ,0x2b ,0xca ,0x4a ,0xd7 ,0x57 ,0xb2 ,0xe7 ,0x7c ,0x6f ,0xaa ,0x14 ,0x79 ,0x41 ,0x7f ,0xd0, 
							0xbe ,0x21 ,0xf5 ,0xfc ,0x74 ,0x8f ,0xfc ,0xf4 ,0x1b ,0x46 ,0x0a ,0x07 ,0xe7 ,0xb6 ,0x76 ,0x85, 
							0x57 ,0x9c ,0x34 ,0xde ,0xad ,0x38 ,0x97 ,0x4d ,0x9c ,0xd2 ,0xe1 ,0x6a ,0x83 ,0xc1 ,0xf5 ,0x3c,
							0xe9 ,0x68 ,0xd8 ,0x87 ,0x87 ,0x6d ,0xbc ,0x9c ,0xba ,0xad ,0xea ,0xb4 ,0x46 ,0xe2 ,0x89 ,0x5a };
	printf("Out: ");
	hexprint16(out,64);
	puts("");
	printf("Res: ");
	hexprint16(out,64);
	puts("");
	for (unsigned i = 0; i < 4 * 16; i++) 
	{
		if (out[i] != res_out[i]) { printf("Mismatch at out[%d]!\n", i); test_failed = 1; }
	}
	
	if (test_failed) {
		printf("|*********** SOME TEST(S) FAILED ***********|\n");
		printf("Please fix me!\n");
	}
	else {
		printf("============== All tests OK! ===============\n");
	}
	return  0;
}


