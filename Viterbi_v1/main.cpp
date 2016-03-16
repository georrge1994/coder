/*
���������� ��������� ������� � ������� �������� ��� 1/2 � 1/4
��������:		���
������������:	������ �.�. � ��������� �.�.
2015 ���
*/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <cmath>
#include "WriteReadClass.cpp"
using  namespace std;

// ��������� ����������������� ���, �������� ������� ������
int callA(unsigned int *inData, int *k, int *ar3){

	int A, B;
	A = inData[*k];				// ���� � 1
	B = (A >> 6) & 2;					// �������� ���� �� ����� �1

	int byte_60 = A & 127;			// ������ ������� �����

	A = inData[*k + 1];				// ���� �2
	int byte_64 = B + (A >> 7);		// ���� ���������� ���

	int byte_61 = A & 127;			// ������ ���� 2

	ar3[3] = byte_61 + byte_60;		// ����� ������
	ar3[0] = ~ar3[3] + 1;			// �������� �����
	ar3[2] = byte_60 - byte_61;		// �������� ������
	ar3[1] = ~ar3[2] + 1;			// �������� ���������

	*k += 2;

	return byte_64;
}

int callA_3(unsigned int *inData, int *k, int *ar3){
	int A, B = 0;

	
	int byte_mark[3];
	A = inData[*k];
	B += (A >> 5) & 12;
	byte_mark[0] = A & 127;
	//cout << *k << "	" << A << "	" << B << "	" << byte_mark[0] << endl;
	*k += 1;

	A = inData[*k];
	B += (A >> 6) & 14;
	byte_mark[1] = A & 127;
	//cout << *k << "	" << A << "	" << B << "	" << byte_mark[1] << endl;
	*k += 1;

	A = inData[*k];
	B += (A >> 7) & 15;
	byte_mark[2] = A & 127;
	//cout << *k << "	" << A << "	" << B << "	" << byte_mark[2] << endl;
	int byte_64 = B;		// ������������������ ���������� ���

	ar3[0] = -byte_mark[0] - byte_mark[1] - byte_mark[2];
	ar3[1] = -byte_mark[0] - byte_mark[1] + byte_mark[2];
	ar3[2] = -byte_mark[0] + byte_mark[1] - byte_mark[2];
	ar3[3] = -byte_mark[0] + byte_mark[1] + byte_mark[2];
	ar3[4] = byte_mark[0] - byte_mark[1] - byte_mark[2];
	ar3[5] = byte_mark[0] - byte_mark[1] + byte_mark[2];
	ar3[6] = byte_mark[0] + byte_mark[1] - byte_mark[2];
	ar3[7] = byte_mark[0] + byte_mark[1] + byte_mark[2];
	/*for(int i =0; i< 8;  i++){
		cout<<"ar["<<i<<"] = "<<ar3[i]<<endl;
	}
	getchar();*/
	return byte_64;
}

int callA_4(unsigned int *inData, int *k, int *ar3){
	int A, B = 0;

	int byte_mark[4];

	/*cout<<"B0	"<<B<<endl;*/
	A = inData[*k];

	/*int num=8, i;
	for(i=sizeof(A)*8-1; i>=0; --i){
	printf("%d", A&(1<<i)?1:0);
	}*/

	B += (A >> 4) & 8;
	byte_mark[0] = A & 127;
	//cout << *k << "	" << A << "	" << B << "	" << byte_mark[0] << endl;
	*k += 1;
	/*cout<<"B1	"<<B<<endl;*/
	A = inData[*k];
	/*    for(i=sizeof(A)*8-1; i>=0; --i){
	printf("%d", A&(1<<i)?1:0);
	}*/
	B += (A >> 5) & 12;
	byte_mark[1] = A & 127;
	//cout << *k << "	" << A << "	" << B << "	" << byte_mark[1] << endl;
	*k += 1;
	/*cout<<"B2	"<<B<<endl;*/
	A = inData[*k];
	/*    for(i=sizeof(A)*8-1; i>=0; --i){
	printf("%d", A&(1<<i)?1:0);
	}*/
	B += (A >> 6) & 14;
	byte_mark[2] = A & 127;
	//cout << *k << "	" << A << "	" << B << "	" << byte_mark[2] << endl;
	*k += 1;
	/*cout<<"B3	"<<B<<endl;*/
	A = inData[*k];
	/*    for(i=sizeof(A)*8-1; i>=0; --i){
	printf("%d", A&(1<<i)?1:0);
	}*/
	B += (A >> 7) & 15;
	byte_mark[3] = A & 127;
	//cout << *k << "	" << A << "	" << B << "	" << byte_mark[2] << endl;
	*k += 1;
	/*	cout<<"B4	"<<B<<endl;*/



	int byte_64 = B;		// ������������������ ���������� ���

	ar3[15] = byte_mark[0] + byte_mark[1] + byte_mark[2] + byte_mark[3];
	ar3[0] = ~ar3[15] + 1;

	ar3[14] = byte_mark[0] + byte_mark[1] + byte_mark[2] - byte_mark[3];
	ar3[1] = ~ar3[14] + 1;

	ar3[3] = ar3[1] + byte_mark[2] + byte_mark[2];
	ar3[12] = ~ar3[3] + 1;

	ar3[13] = ar3[12] + byte_mark[3] + byte_mark[3];
	ar3[2] = ~ar3[13] + 1;

	ar3[11] = byte_mark[0] - byte_mark[1] + byte_mark[2] + byte_mark[3];
	ar3[4] = ~ar3[11] + 1;

	ar3[10] = byte_mark[0] - byte_mark[1] + byte_mark[2] - byte_mark[3];
	ar3[5] = ~ar3[10] + 1;

	ar3[7] = ar3[5] + byte_mark[2] + byte_mark[2];
	ar3[8] = ~ar3[7] + 1;

	ar3[9] = ar3[8] + byte_mark[3] + byte_mark[3];
	ar3[6] = ~ar3[9] + 1;

	/*for (int i = 0; i< 16; i++){
		cout << "ar[" << i << "] = " << ar3[i] << endl;
	}
	getchar();*/
	return byte_64;
}

// �������� �����������
int correction(int *state, int size){
	int B = 0, c;
	for (int i = 0; i < size; i++){
		if (B < state[i]){
			B = state[i];
			c = 0x800;
		}
		else{
			c = 0x801;
		}
	}
	//cout<<"B	"<<B<<"		"<<B-c<<endl;
	B = B - c;
	if (B<0)
		return 0;
	//cout<<"id"<<endl;
	for (int i = 0; i < size; i++){
		state[i] = state[i] + 0xC000;
		//		cout<<"+0xC000		"<<state[i]<<endl;
		state[i] = state[i] & 0xFFFF;
		//		cout<<"0xFFFF	"<<state[i]<<endl;
	}
	return 0;
}

void Viterbi(unsigned int *inData, unsigned int *outDate, int mode){
	int byte6E = -1;							// ����� ��� ������� ������������
	int bias[32] = { 3, 1, 3, 1, 0, 2, 0, 2, 0, 2, 0, 2, 3, 1, 3, 1, 2, 0, 2, 0, 1, 3, 1, 3, 1, 3, 1, 3, 2, 0, 2, 0 };					// �������� ����� �����������
	int bias_3[32] = { 7, 3, 2, 6, 1, 5, 4, 0, 1, 5, 4, 0, 7, 3, 2, 6, 1, 5, 4, 0, 7, 3, 2, 6, 7, 3, 2, 6, 1, 5, 4, 0 };	// ��� 1/3
	int bias_4[32] = { 15, 9, 5, 3, 2, 4, 8, 14, 2, 4, 8, 14, 15, 9, 5, 3, 12, 10, 6, 0, 1, 7, 11, 13, 1, 7, 11, 13, 12, 10, 6, 0 };	// ��� 1/4
	//+ ,+,+,-,-,-,-,-, -,-,C,-,  -,-,H,-,+,  -,-,-,-,O, +,-, -,-, -, -, H, -,-,-,
	// ��������� ����� ������������� � ������������ � �������
	switch (mode){
	case(2) :
		byte6E = 63;
		break;
	case(3) :
		byte6E = 63;
		for (int i = 0; i < 32; i++){
			bias[i] = bias_3[i];
		}
		break;
	case(4) :
		byte6E = 31;
		for (int i = 0; i < 32; i++){
			bias[i] = bias_4[i];
		}
		break;
	}

	int byte_68 = 0;
	int TRN[984] = { 0 };							// �������� trn
	int state[128];								// ������ ��� ���������(������ + �����)

	// ������������� ��������� ���������
	state[0] = 0;									// ������ ��������� ���������
	for (int j = 1; j<64; j++)						// ������������� ��������� ���������
		state[j] = -1000;

	int B, T, ar2,
		byte_64,								// ���������� ���� ���
		k = 0;									// ����� �����, ������������ � callA
	int x1, x2, y1, x_cnt, y_cnt, z_cnt;				// ��������� ����������(�������� ��� ����������, ��. ����)
	int ar3[16];								// ������ ������ ������

	/*������ �������� begin:*/
	z_cnt = 0;									// ������� ��� TRN
	for (int ar0 = 0; ar0<246; ar0++){				// ���� �1
		switch (mode){
		case(2) :
			byte_64 = callA(inData, &k, ar3);		// ����� ������������		
			break;
		case(3) :
			byte_64 = callA_3(inData, &k, ar3);	// ����� ������������
			break;
		case(4) :
			byte_64 = callA_4(inData, &k, ar3);	// ����� ������������
			break;
		}

		if (ar0 % 2 == 0){						// ��� ���������� ������ ��������� �� ������ ������, ���������� ����������� ����������� ���������.
			x1 = 64; x2 = 96; y1 = 0;			// ��� ����� ������� x1,x2, ������� ���������� � ������������ � ������� �������(������ - ������ 64 ������,
		}
		else{									// �������� - � 65 �� 128). ����� ��� ������������ ��� ���������� ������ ������������ ��������� � ����������� 
			x1 = 0; x2 = 32; y1 = 64;			// ������. y1 - ����� ��� ����������� ������ ���������.
		}

		y_cnt = 0;								// ������� ��� ��������� �� ��������� � ��������� ��� ������
		x_cnt = 0;								// ������� ��� ��������� �� ��������� � ��������� ��� ������

		// ���������� ������ ��� ���� ���
		for (int ar1 = 0; ar1<4; ar1++){			// ���� �2
			for (int brc = 0; brc<8; brc++){		// ���� �3

				B = byte_64 ^ bias[x_cnt];		// ��������� ���������� �������a
				T = ar3[B];						// �������� ������ 

				// ���������� "�������"
				if (state[y1 + y_cnt] + T > state[y1 + y_cnt + 1] - T) {
					//cout<<"state[y1+y_cnt] + T "<<state[y1+y_cnt] + T<<endl;
					//cout<<"state[y1+y_cnt + 1]-T "<<state[y1+y_cnt + 1]-T<<endl;
					state[x1 + x_cnt] = (state[y1 + y_cnt] + T);
					TRN[z_cnt] = TRN[z_cnt] << 1;	// ���������� �������
					TRN[z_cnt] = TRN[z_cnt] & 0xFFFF;
				}
				else {
					//cout<<"state[y1+y_cnt + 1]-T "<<state[y1+y_cnt + 1]-T<<endl;
					//cout<<"state[y1+y_cnt] + T "<<state[y1+y_cnt] + T<<endl;
					state[x1 + x_cnt] = (state[y1 + y_cnt + 1] - T);
					TRN[z_cnt] = (TRN[z_cnt] << 1) + 1;// ���������� �������
					TRN[z_cnt] = TRN[z_cnt] & 0xFFFF;
				}

				if (state[y1 + y_cnt] - T > state[y1 + y_cnt + 1] + T) {
					///cout<<"state[y1+y_cnt] - T "<<state[y1+y_cnt] - T<<endl;
					//cout<<"state[y1+y_cnt + 1]+T "<<state[y1+y_cnt + 1]+T<<endl;
					state[x2 + x_cnt] = state[y1 + y_cnt] - T;
					TRN[z_cnt] = TRN[z_cnt] << 1;	// ���������� �������
					TRN[z_cnt] = TRN[z_cnt] & 0xFFFF;
				}
				else {
					//cout<<"state[y1+y_cnt + 1]+T "<<state[y1+y_cnt + 1]+T<<endl;
					//cout<<"state[y1+y_cnt] - T "<<state[y1+y_cnt] - T <<endl;
					state[x2 + x_cnt] = (state[y1 + y_cnt + 1] + T);
					TRN[z_cnt] = (TRN[z_cnt] << 1) + 1;// ���������� �������	
					TRN[z_cnt] = TRN[z_cnt] & 0xFFFF;
				}
				y_cnt += 2;						// ��������� ��. ������(�������� ��.����, ��� �������������)
				x_cnt++;						// ��������� ��. ������(�������� ��.����, ��� �������������)
				//getchar();
			}
			//cout<<"TRN		 "<<TRN[z_cnt]<<endl;
			z_cnt++;							// ��������� ��. TRN(�������� ��.����, ��� �������������)
		}
		y_cnt = 0;								// ��������� ��. ������
		x_cnt = 0;								// ��������� ��. ������

		// �������� ������������� ������� ������� ���������
		if ((ar0 + 10) % byte6E == 0){
			//cout<<"g"<<endl;
			//byte_64 = correction(state, 128);	// ��������� ���������	
		}
	}
	/*������ �������� end.*/
	// write in file
	/*	FILE *fl = fopen("outbits.txt", "wb");
	for(int i = 0; i< 984; i++){
	fprintf(fl,"%x\n",TRN[i]);
	//fout << TRN[i] << endl;
	}
	fclose(fl);*/

	int A = 0;									// ��������� �� ������� ���������
	// ....
	// ���������� � �������� ��������. ��� 1/2 - ��� �� �����. ������� �����.
	// ....
	// �������� ��������								 
	int	ar4 = 0,								// ���������� ��� ���������� ���������� �����
		byte_66 = 0,							// ��������� ���������� ��� ����������� ������ TRN
		ar7 = 984,								// ��������� �� ����� TRN �������
		mask = 0,								// ����� ���������(��. ����)
		TC = 0;									// ���� ��������

	// ������ ���������� ����� ��� 1/2
	int jump[64] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48,
		50, 52, 54, 56, 58, 60, 62, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35,
		37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63 };

	for (int brc = 245; brc>-1; brc--){				// ���� �3
		B = ar4;								// ������� ���. �����
		//	cout<<"B			"<<B<<A<<ar4<<endl;
		if (B - 0x20 >0)							// ���������� ��������������� ����
			outDate[brc] = 1;
		else
			outDate[brc] = 0;
		//	cout<<brc<<" BIT:				"<<outDate[brc]<<endl;
		// ���������� ������ TRN � ��������� ����� TRN ���������(0-3)	
		B = (A >> 3) & 3;
		byte_66 = B;
		//	cout<<"byte_66			"<<B<<endl;
		B = ~(B ^ 3) + 1;
		//	cout<<"~(B ^ 3) + 1		"<<B<<endl;
		ar7 = ar7 + B;
		//	cout<<"ar7			"<<ar7<<endl;

		// ����������� ���������, �� �������� ��� ����������� ������� � ������� ���������
		A = jump[ar4];							// ����� ���������� �����
		//	cout<<"new magic number	"<<A<<endl;
		T = A & 0xF;							// ��������� �� ��������� �� �������� ��� ����������� �������	
		//	cout<<"T	"<<T<<endl;
		//	cout<<"15 - T	"<<15 - T<<endl;

		mask = pow(2.0, (15 - T));				// ����� ��� ����������� ��������� �� �������� ��� ����������� �������
		TC = (TRN[ar7] & mask) >> (15 - T);		// ����������� ����� ��������(���� 0 - ���� ������� ��� ����������� �� �������,1 - ���� �� ���������)
		//	cout<<"mask		"<<mask<<endl;
		//	cout<<"TC		"<<TC<<endl;																	
		// ������������� �������� � ������� ���������� �����
		//cout<<"beforeA		"<<A<<endl;
		if (ar4 - 0x20 > 0){
			if (TC == 0)
				A = A - 1;
		}
		else{
			if (TC == 1)
				A = A + 1;
		}
		//cout<<"aferA		"<<A<<endl;
		ar7 = ar7 + ~byte_66 + 1;				// ������� � ����� �������� TRN ���������
		ar4 = A;								// ���������� �������� �.�. � ar4
		ar7--;
		//getchar();
	}
	/*
	FILE *fl = fopen("outbits.txt", "wb");
	for(int brc = 0; brc< 246; brc++){
	fprintf(fl,"%x\n",outDate[brc]);
	//fout << TRN[i] << endl;
	}

	fclose(fl);*/
}

void Depunctur(unsigned int *memory_0x600, int mode){
	unsigned int *loc_memory;
	int step = 0, i = 0, k = 0, N;
	if (mode == 2){
		N = 492;
	}
	else{
		N = 984;
	}
	loc_memory = (unsigned int*)calloc(N, sizeof(int));
	for (k = 0; k < N; k++)
	{
		if (step == 0){
			if (mode == 2){
				step = 41;
				loc_memory[k] = 0x0;
			}
			else{
				step = 81;
				loc_memory[k] = 0x0;
				k++;
				loc_memory[k] = 0x0;
			}
		}
		else{
			loc_memory[k] = memory_0x600[i];
			i++;
		}
		step--;
	}

	for (i = 0; i<N; i++){
		memory_0x600[i] = loc_memory[i];
	}
}

int main(int argc, char *argv[]){
	cout.unsetf(ios::dec);
	cout.setf(ios::hex);

	cout << "Decoder-Viterbi " << endl;
	cout << "mode:	" << argv[1] << endl;
	cout << "input file with data:	" << argv[2] << endl;
	cout << "output file with coding data:	" << argv[3] << endl;

	int mode =3,//atoi(argv[1]),						// ����� ������(2 - 1/2, 3- 1/3, 4 - 1/4 )
		n_in,											// ������ �������� �������
		n_out;											// �������� �������
	switch (mode){
	case(2) :
		n_in = 492;										// ��� 1/2
		n_out = 246;
		break;
	case(3) :
		n_in = 738;										// ��� ��� ������ 1/3
		n_out = 246;
		break;
	case(4) :
		n_in = 984;										// ��� ��� ������ 1/4
		n_out = 246;
		break;
	default:
		cout << "uncorrect mode..." << endl;
		return -1;										// ���� ����� �� ���������� - �����
	}
	unsigned int *inData = (unsigned int*)calloc(n_in, sizeof(int));		// ����. ��. �������
	unsigned int *outDate = (unsigned int*)calloc(n_out, sizeof(int));		// ����. ���. �������

	WriteRead WR;										// object of class "write and read" 
	WR.readFile(inData, argv[2], n_in);			// read source bits from "memory"
	//Depunctur( inData, mode);
	Viterbi(inData, outDate, mode);
	WR.writeInFile(outDate, argv[3], n_out);

	free(inData);
	free(outDate);
	return 0;
}