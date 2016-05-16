#ifndef __CODEC__
#define __CODEC__
	#include <math.h>
	#include <stdlib.h>
	/*
	Сверточный кодер:
		mode - режим кодирования;
		memory_0xA00 - указатель на данные;
		memory_0x600 - указатель на область памяти, куда будет записан результат;
		length	- длина кодируемого сообщения;
	*/
	int coder(int mode, unsigned int *memory_0xA00, unsigned int *memory_0x600, unsigned int length);
	
	/*
	Функции чередования и ликвидации чередования:
		memory_0x600 - указатель на участок памяти(массива);
		length	- длина сообщения, надо которыми нужно провести/ликвидировать чередование.
	*/
	void interliving(unsigned int *memory_0x600, int length);
	void deinterliving(unsigned int *memory_0x600, int length);

	/*
	Декодер-Витерби:
		inData - указатель на закодированные данные;
		outDate - указатель на декодированные данные;
		mode - режим декодирования;
		length - длина декодируемого сообщения.
	*/
	void Viterbi(unsigned int *inData, unsigned int *outDate, int mode, int length);
#endif