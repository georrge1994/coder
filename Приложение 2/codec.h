#ifndef __CODEC__
#define __CODEC__
	#include <math.h>
	#include <stdlib.h>
	/*
	���������� �����:
		mode - ����� �����������;
		memory_0xA00 - ��������� �� ������;
		memory_0x600 - ��������� �� ������� ������, ���� ����� ������� ���������;
		length	- ����� ����������� ���������;
	*/
	int coder(int mode, unsigned int *memory_0xA00, unsigned int *memory_0x600, unsigned int length);
	
	/*
	������� ����������� � ���������� �����������:
		memory_0x600 - ��������� �� ������� ������(�������);
		length	- ����� ���������, ���� �������� ����� ��������/������������� �����������.
	*/
	void interliving(unsigned int *memory_0x600, int length);
	void deinterliving(unsigned int *memory_0x600, int length);

	/*
	�������-�������:
		inData - ��������� �� �������������� ������;
		outDate - ��������� �� �������������� ������;
		mode - ����� �������������;
		length - ����� ������������� ���������.
	*/
	void Viterbi(unsigned int *inData, unsigned int *outDate, int mode, int length);
#endif