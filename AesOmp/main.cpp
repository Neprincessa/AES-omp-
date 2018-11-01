#include <stdint.h>
#include <string.h> // CBC mode, for memset
#include "aes.h"
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdint.h>
#include "aes.c"
#include <conio.h>
#include <omp.h>
using namespace std;
//считываение файла по названию
//считываание в вектор

#define CBC 1
#define CTR 1
#define ECB 1

vector<char> readFile(string, ifstream &); 


static void printCiper(vector<unsigned char> & str, int size)
{

#if defined(AES256)
	uint8_t len = 32;
#elif defined(AES192)
	uint8_t len = 24;
#elif defined(AES128)
	uint8_t len = 16;
#endif

	unsigned char i;
	for (i = 0; i < size; ++i)
		//printf("%.2x", str[i]);
		std::cout << std::hex << str[i];
	printf("\n");
}

uint8_t* decrypt(uint8_t *input) {
#if defined(AES256)
	uint8_t key[] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
		0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };
	uint8_t in[] = { 0xf3, 0xee, 0xd1, 0xbd, 0xb5, 0xd2, 0xa0, 0x3c, 0x06, 0x4b, 0x5a, 0x7e, 0x3d, 0xb1, 0x81, 0xf8 };
#elif defined(AES192)
	uint8_t key[] = { 0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5,
		0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b };
	uint8_t in[] = { 0xbd, 0x33, 0x4f, 0x1d, 0x6e, 0x45, 0xf2, 0x5f, 0xf7, 0x12, 0xa2, 0x14, 0x57, 0x1f, 0xa5, 0xcc };
#elif defined(AES128)
	uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	uint8_t in[] = { 0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97 };
#endif

	uint8_t out[] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };
	struct AES_ctx ctx;

	AES_init_ctx(&ctx, key);
	AES_ECB_decrypt(&ctx, input);

//	printCiper(input);

	return input;
}

static void phex(uint8_t* str)
{

#if defined(AES256)
	uint8_t len = 32;
#elif defined(AES192)
	uint8_t len = 24;
#elif defined(AES128)
	uint8_t len = 16;
#endif

	unsigned char i;
	for (i = 0; i < /*len*/16; ++i)
		printf("%.2x", str[i]);
	printf("\n");
}
const size_t block_size = 16;
int main() {
 

	string filename = "nonCipherText.txt";
	ifstream text;
	uint8_t key[16] = { (uint8_t)0x2b, (uint8_t)0x7e, (uint8_t)0x15, (uint8_t)0x16, (uint8_t)0x28, (uint8_t)0xae, (uint8_t)0xd2, (uint8_t)0xa6, (uint8_t)0xab, (uint8_t)0xf7, (uint8_t)0x15, (uint8_t)0x88, (uint8_t)0x09, (uint8_t)0xcf, (uint8_t)0x4f, (uint8_t)0x3c };

	 
	std::ifstream f(filename, std::ios::binary);
	std::vector<unsigned char> vec(std::istreambuf_iterator<char>{f}, {});
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, key);
	cout << "Plain" << endl;
	printCiper(vec, block_size * 10);

#pragma omp parallel num_threads(2)
	{
#pragma omp for 
		for (int i = 0; i < vec.size(); i += block_size) {
			AES_ECB_encrypt(&ctx, (unsigned char*)&vec[i]);
		}
		cout << "Encrypted" << endl;
#pragma omp single	
		printCiper(vec, block_size * 10);

#pragma omp for
		for (int i = 0; i < vec.size(); i += block_size) {
			AES_ECB_decrypt(&ctx, (unsigned char*)&vec[i]);
		}

		cout << "Decrypted" << endl;
#pragma omp single
		printCiper(vec, block_size * 10);
	}


	system("pause");
	return 0;
}
//
//	
//	int size = vec.size();
//
//	char *info = (char*)malloc(20000);
//
////#pragma omp parallel num_threads(2) 
////	{
//	printf("Input text:\n");
//	for (int i = 0; i < size; i++) {
//		printf("%c", vec[i]);
//		info[i] = vec[i];
//		}
////	}
//
//	int i = 0;
//	int curAmount = 0;
//
//	printf("\n\n");
//	printf("Encrypted: \n");
//	int indForDec = 0;
//	uint8_t *forDecrypt = (uint8_t*)malloc(sizeof(uint8_t) * size);
//
//	while (i < size) {
//		
//		curAmount = 0;
//		char *curStr = (char*)malloc(129);
//		uint8_t *forEncrypt = (uint8_t*)malloc(129);
//
////#pragma omp parallel num_threads(2) 
////		{
////#pragma omp for  schedule(static)
//			for (int j = 0; j < 129; j++) {
//				if (i < size && vec[i] != '\0') {
//					curStr[curAmount] = info[i];
//					forDecrypt[i] = info[i];
//					i++;
//					curAmount++;
//				}
//				else {
//					curStr[i] = '\0';
//					forDecrypt[i] = '\0';
//					curAmount++;
//					break;
//				}
//			}
//		//}
//		
//		realloc(curStr, sizeof(char)*(curAmount+1));
//
//		forEncrypt = convertCharToUI_8(curStr, curAmount);
//		struct AES_ctx ctx;
//		AES_init_ctx(&ctx, key);
//
////#pragma omp parallel num_threads(2) 
////		{
////#pragma omp for schedule(dynamic)
//			for (int g = 0; g < 4; ++g) {
//				AES_ECB_encrypt(&ctx, forEncrypt /*+ (g * 16)*/);
		//	printCiper(forEncrypt/* + (g * 16)*/);
//			}
//	//	}
//
//		int tmpfl = 0;
//		int startInd = indForDec;
//		for (indForDec; indForDec < startInd + curAmount; indForDec++) {
//			forDecrypt[indForDec] = forEncrypt[tmpfl];
//			tmpfl++;
//		}
//		indForDec = startInd + curAmount;
//		i++;	
//	}
//	text.close();
//
//	//Decrypt
//	i = 0;
//	printf("ECB decrypt: \n");
//	while (i < indForDec) {
//		curAmount = 0;
//		uint8_t *tmp = (uint8_t*)malloc(129);
///*
//#pragma omp parallel num_threads(2) 
//		{
//#pragma omp for schedule(dynamic)*/
//			for (int j = 0; j < 128; j++) {
//				if (j < indForDec) {
//					tmp[j] = forDecrypt[i];
//					i++;
//					curAmount++;
//				}
//				else {
//					tmp[i] = '\0';
//					curAmount++;
//					break;
//				}
//			}
//		//}
//		
//		realloc(tmp, sizeof(char)*(curAmount + 1));
//
//		struct AES_ctx ctx_d;
//		AES_init_ctx(&ctx_d, key);
//		
////#pragma omp parallel num_threads(2)
////		{
////#pragma omp for schedule(dynamic)
//			for (int ggg = 0; ggg < 4; ggg++) {
//				AES_ECB_decrypt(&ctx_d, tmp);
//				char *decRes = new char[curAmount];
//				decRes = convertUI_8_ToInt(tmp, curAmount);
//				printCiper(tmp);
//			}
//	//	}
//		i++;
//	}
//
//	_getche();
//	system("pause");
//	return 0;
//}



vector<char> readFile(string strFile,ifstream &iFile) {

	vector<char> vecNames;
	char symb;
	iFile.open(strFile.c_str());

	while (iFile >> symb) {
		vecNames.push_back(symb);
	}

	iFile.close(); 

	return vecNames;
}