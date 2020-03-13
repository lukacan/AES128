#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>

using namespace std;
/* AES-128 simple implementation template and testing */

/*
Author: YOUR_NAME_HERE, username@fit.cvut.cz
Template: Jiri Bucek 2017
AES specification:
http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
*/

/* AES Constants */

// forward sbox
const uint8_t SBOX[256] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
}; 



const uint8_t rCon[12] = {
	0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36,
};
uint8_t wbyte(uint32_t w, int pos) {
	return (w >> (pos * 8)) & 0xff;
}
uint8_t xtime(uint8_t a) {
uint8_t mask = 0x80, m = 0x1b;
  uint8_t high_bit = a & mask;
  a = a << 1;
  if (high_bit) {    
    a = a ^ m;
  }
  return a;
	
}
/* Helper functions */
void hexprint16(vector <uint8_t> p,unsigned int how_long) {
		for (unsigned int i = 0; i < how_long; i++)
		{
			printf("%02hhx ", p[i]);
			if (((i + 1) % 16) == 0)
				printf("\n     ");
		}
	puts("");
	
}
void hexprint32(vector <uint32_t> p) {
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


uint32_t word(uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3) {
	return a0 | (uint32_t)a1 << 8 | (uint32_t)a2 << 16 | (uint32_t)a3 << 24;
}



// **************** AES  functions ****************
uint32_t subWord(uint32_t w) {
	return word(SBOX[wbyte(w, 0)], SBOX[wbyte(w, 1)], SBOX[wbyte(w, 2)], SBOX[wbyte(w, 3)]);
}

void subBytes(vector<uint32_t> &s,unsigned & numBlocks) {
	for (unsigned int i = 0 ; i < numBlocks * 4 ; i++)
	{
		s[i] = subWord(s[i]); 
	}
}

void shiftRows(vector<uint32_t> &s,unsigned &numBlocks) {
	
		uint32_t s0;
		uint32_t s1;
		uint32_t s2;
		uint32_t s3;
	for (unsigned int k = 0 ; k < numBlocks; k++)
	{
		
		s0 = word(wbyte(s[4*k+0],0),wbyte(s[4*k+1],1),wbyte(s[4*k+2],2),wbyte(s[4*k+3],3));
		s1 = word(wbyte(s[4*k+1],0),wbyte(s[4*k+2],1),wbyte(s[4*k+3],2),wbyte(s[4*k+0],3));
		s2 = word(wbyte(s[4*k+2],0),wbyte(s[4*k+3],1),wbyte(s[4*k+0],2),wbyte(s[4*k+1],3));
		s3 = word(wbyte(s[4*k+3],0),wbyte(s[4*k+0],1),wbyte(s[4*k+1],2),wbyte(s[4*k+2],3));
		s[4*k+0] = s0;
		s[4*k+1] = s1;
		s[4*k+2] = s2;
		s[4*k+3] = s3;
	}
	
	
}


// not mandatory - mix a single column
uint32_t mixColumn(uint32_t c) {
	return 0; /* ??? */
}


void mixColumns(vector<uint32_t> &s,unsigned & numBlocks) {

	vector<uint32_t> a;

	uint8_t word0,word1,word2,word3;
	for (unsigned int k = 0 ; k < numBlocks ; k++)
	{
		for(unsigned int i = 0 ; i < 4 ; i++)
		{
			a.push_back(word(wbyte(s[4*k+i],0),wbyte(s[4*k+i],1),wbyte(s[4*k+i],2),wbyte(s[4*k+i],3)));
			word0 = (xtime((wbyte(a[4*k+i],0))) ^ (xtime((wbyte(a[4*k+i],1))) ^ (wbyte(a[4*k+i],1))) ^ (wbyte(a[4*k+i],2)) ^ (wbyte(a[4*k+i],3)));
			word1 = (xtime((wbyte(a[4*k+i],1))) ^ (xtime((wbyte(a[4*k+i],2))) ^ (wbyte(a[4*k+i],2))) ^ (wbyte(a[4*k+i],0)) ^ (wbyte(a[4*k+i],3)));
			word2 = (xtime((wbyte(a[4*k+i],2))) ^ (xtime((wbyte(a[4*k+i],3))) ^ (wbyte(a[4*k+i],3))) ^ (wbyte(a[4*k+i],0)) ^ (wbyte(a[4*k+i],1)));
			word3 = (xtime((wbyte(a[4*k+i],3))) ^ (xtime((wbyte(a[4*k+i],0))) ^ (wbyte(a[4*k+i],0))) ^ (wbyte(a[4*k+i],1)) ^ (wbyte(a[4*k+i],2)));
			s[4*k+i] = word(word0,word1,word2,word3);
		} 
	}
	
}
/*
* Key expansion from 128bits (4*32b)
* to 11 round keys (11*4*32b)
* each round key is 4*32b
*/
void expandKey(vector<uint8_t> & k, vector<uint32_t> & ek) {
	unsigned int keyLength = 4;
	uint32_t tmp;
	for (unsigned int i = 0 ; i < 44 ; i++)
	{
		if (i <= 3)
		{
			for (unsigned int ki = 1; ki <= 4 ; ki++  )
			{
				ek.push_back(word(k[4*ki - 4],k[4*ki - 3],k[4*ki - 2],k[4*ki - 1]));
				i++;
			}
		}
		if (i % keyLength == 0)
		{
			tmp = word(wbyte(ek[i-1],1),wbyte(ek[i-1],2),wbyte(ek[i-1],3),wbyte(ek[i-1],0));
			tmp = subWord(tmp);
			tmp ^= ek[i-keyLength];
			tmp ^= rCon[i/keyLength];
			ek.push_back(tmp);
		}
		if (i % keyLength != 0)
		{
			tmp = ek[i-1] ^ ek[i-keyLength];
			ek.push_back(tmp);
		}
	}
}


/* Adding expanded round key (prepared before) */
void addRoundKey(vector<uint32_t> &s, vector<uint32_t> & ek, short round,unsigned & numBlocks) {

	for (unsigned int k = 0 ; k < numBlocks ;k++ )
	{
		for (unsigned int i = 0 ; i < 4 ;i++)
		s[4*k+i] = word((wbyte(s[4*k+i],0) ^ wbyte(ek[4*round + (i)],0)),(wbyte(s[4*k+i],1) ^ wbyte(ek[4*round + (i)],1)),(wbyte(s[4*k+i],2) ^ wbyte(ek[4*round + (i)],2)),(wbyte(s[4*k+i],3) ^ wbyte(ek[4*round + (i)],3)));
	}
}
void aes(vector<uint32_t> in, vector<uint8_t> &out, vector<uint32_t> skey, unsigned  numBlocks)
{
	addRoundKey(in, skey, 0,numBlocks);

	for (unsigned int i = 1 ; i < 10 ; i++)
	{
		subBytes(in,numBlocks);
		shiftRows(in,numBlocks);
		mixColumns(in,numBlocks);
		addRoundKey(in,skey,i,numBlocks);
	}
	subBytes(in,numBlocks);
	shiftRows(in,numBlocks);
	addRoundKey(in,skey,10,numBlocks);


	for (unsigned int k = 0 ; k < numBlocks ; k++)
	{
		for (unsigned int i = 16*k; i < 16*k+16; i++) 
		{
			if 
				(i < 4+16*k) out.push_back(wbyte(in[4*k+0], i % 4));
			else if 
				(i < 8+16*k) out.push_back(wbyte(in[4*k+1], i % 4));
			else if 
				(i < 12+16*k) out.push_back(wbyte(in[4*k+2], i % 4));
			else 
				out.push_back(wbyte(in[4*k+3], i % 4));
		}
	}
	
}

//****************************
// MAIN function: AES testing
//****************************
int main(int argc, char* argv[])
{

	int test_failed = 0;
	
	
	unsigned int count = 1;

	vector	<uint8_t> 	key {	0xde, 0xad, 0xbe, 0xef, 0x20, 0x74, 0x6f, 0x70, 0x20, 0x73, 0x65, 0x63, 0x72, 0x65, 0x74, 0x21};
	vector	<uint8_t> 	out ;
	vector	<uint8_t> 	res {	0x08 ,0x2b ,0xca ,0x4a ,0xd7 ,0x57 ,0xb2 ,0xe7 ,0x7c ,0x6f ,0xaa ,0x14 ,0x79 ,0x41 ,0x7f ,0xd0, 
								0xbe ,0x21 ,0xf5 ,0xfc ,0x74 ,0x8f ,0xfc ,0xf4 ,0x1b ,0x46 ,0x0a ,0x07 ,0xe7 ,0xb6 ,0x76 ,0x85, 
								0x57 ,0x9c ,0x34 ,0xde ,0xad ,0x38 ,0x97 ,0x4d ,0x9c ,0xd2 ,0xe1 ,0x6a ,0x83 ,0xc1 ,0xf5 ,0x3c,
								0xe9 ,0x68 ,0xd8 ,0x87 ,0x87 ,0x6d ,0xbc ,0x9c ,0xba ,0xad ,0xea ,0xb4 ,0x46 ,0xe2 ,0x89 ,0x5a };
	vector 	<uint32_t> 	in	{ 	0x488ceb25 ,0x85cb89ff ,0x8190c04f ,0xfced47cc, 
								0x14b21986 ,0xd49265fe ,0x9ceafc8b ,0x44328e9d, 
								0xf1e9d7d7 ,0x5660def7 ,0x0789e98d ,0x39163d3f, 
								0xd13c8001 ,0x738dd808 ,0xc879eaaf ,0xc683471e }; 
	vector	<uint32_t>	expKey;


	printf("Encryption using simple AES implementation \n\n");
	printf("Key: ");
	hexprint16(key,16);
	puts("");
	printf("In:  ");
	hexprint32(in);
	puts("");
	
	
	
	expandKey(key,expKey);
	for (unsigned int i = 0 ; i < count ; i++)
	{
		aes(in, out, expKey, 4);
	}
	
	
	
	printf("Out: ");
	hexprint16(out,64);
	puts("");
	printf("Res: ");
	hexprint16(res,64);
	puts("");
	for (unsigned i = 0; i < 4 * 16; i++) 
	{
		if (out[i] != res[i]) { printf("Mismatch at out[%d]!\n", i); test_failed = 1; }
	}
	
	if (test_failed) {
		printf("|*********** SOME TEST(S) FAILED ***********|\n");
		printf("Please fix me!\n");
	}
	else {
		printf("============== All tests OK! ===============\n");
	}
	return  test_failed;
}

