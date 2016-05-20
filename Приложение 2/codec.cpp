#include "codec.h"

int coder(int mode, unsigned int *memory_0xA00, unsigned int *memory_0x600, unsigned int length)
	{
		int accumulatorA = 0;			
		int accumulatorB = 0;
		int brc = mode;				
		int ar0 = 0;
		int regT = 0;
		int count_bits = 0, i = 0, j = 0;

		int tableRepliesHalf[8][2] =   {{ 42330, 27030 },
										{ 42330, 27030 },
										{ 23205, 27030 },
										{ 23205, 27030 },
										{ 23205, 38505 },
										{ 23205, 38505 },
										{ 42330, 38505 },
										{ 42330, 38505 } };

		int tableRepliesThree[8][3] = { { 42330, 27030, 42330, },
										{ 42330, 27030, 23205 },
										{ 23205, 27030, 42330 },
										{ 23205, 27030, 23205 },
										{ 23205, 38505, 23205 },
										{ 23205, 38505, 42330 },
										{ 42330, 38505, 23205 },
										{ 42330, 38505, 42330 } };

		int tableRepliesQuater[8][4] = {{ 42330, 42330, 26214, 27030 },
										{ 23205, 42330, 39321, 27030 },
										{ 42330, 23205, 39321, 27030 },
										{ 23205, 23205, 26214, 27030 },
										{ 23205, 23205, 39321, 38505 },
										{ 42330, 23205, 26214, 38505 },
										{ 23205, 42330, 26214, 38505 },
										{ 42330, 42330, 39321, 38505 } };
		
		switch (mode){
		case(2) :
			for (i = 0; i < length + 6; i++){
				accumulatorA = accumulatorA << 1;
				accumulatorA = accumulatorA & 127;
				accumulatorA = accumulatorA | memory_0xA00[i];
				ar0 = (accumulatorA >> 4) & 7;	
				regT = accumulatorA & 15;		
				for (j = 0; j<brc; j++){
					accumulatorB = tableRepliesHalf[ar0][j] >> regT;	
					memory_0x600[count_bits] = accumulatorB & 1;
					count_bits++;
				}
			}
			break;
		case(3) :
			for (i = 0; i < length + 6; i++){
				accumulatorA = accumulatorA << 1;
				accumulatorA = accumulatorA & 127;
				accumulatorA = accumulatorA | memory_0xA00[i];
				ar0 = (accumulatorA >> 4) & 7;	
				regT = accumulatorA & 15;		
				for (j = 0; j<brc; j++){
					accumulatorB = tableRepliesThree[ar0][j] >> regT;	
					memory_0x600[count_bits] = accumulatorB & 1;
					count_bits++;
				}
			}
			break;
		case(4) :
			for (i = 0; i < length + 6; i++){
				accumulatorA = accumulatorA << 1;
				accumulatorA = accumulatorA & 127;
				accumulatorA = accumulatorA | memory_0xA00[i];
				ar0 = (accumulatorA >> 4) & 7;	
				regT = accumulatorA & 15;		
				for (j = 0; j<brc; j++){
					accumulatorB = tableRepliesQuater[ar0][j] >> regT;	
					memory_0x600[count_bits] = accumulatorB & 1;
					count_bits++;
				}
			}
			break;
		}
		return count_bits;
	}

void interliving(unsigned int *memory_0x600, int length){
	int CountRow = length / 120 + 15;
	int i,			
		row = 0;	
	do{
		i = row;
		for (int k = 0; k < 120; k++){ 
			if (i - row == 8){
				i = row;
			}
			memory_0x600[120 * row + k] = memory_0x600[120 * i + k];
			i++;
		}
		row++;
	} while (row < CountRow - 7); 

	int index = 0;				  
	for (int i = 0; i < CountRow - 7; i++){	
		for (int j = 0; j < 120; j++){		
			memory_0x600[120 * (CountRow - 1) + j] = memory_0x600[120 * i + index];
			index = index + 10;				
			if (index > 119)				
				index = index - 119;		
		}
		index = 0;
		for (int j = 0; j < 120; j++){
			memory_0x600[120 * i + j] = memory_0x600[120 * (CountRow - 1) + j]; 
		}
	}
}
void deinterliving(unsigned int *memory_0x600, int length){
	int CountRow = length / 120 + 7;		
	int index = 0;							
	for (int i = 0; i < CountRow - 7; i++){	
		for (int j = 0; j < 120; j++){		

			memory_0x600[120 * (CountRow - 1) + index] = memory_0x600[120 * i + j]; 
			index = index + 10;				
			if (index > 119)				
				index = index - 119;	
		}
		index = 0;
		for (int j = 0; j < 120; j++){
			memory_0x600[120 * i + j] = memory_0x600[120 * (CountRow - 1) + j]; 
		}
	}

	int i,					
		row = CountRow - 8;		
	do{
		i = row;
		for (int k = 0; k < 120; k++){ 
			if (i - row == 8){
				i = row;
			}
			memory_0x600[120 * i + k] = memory_0x600[120 * row + k];
			i++;
		}
		row--;
	} while (row >= 0); 
}

int callA(unsigned int *inData, int *k, int *ar3);
int callA_3(unsigned int *inData, int *k, int *ar3);
int callA_4(unsigned int *inData, int *k, int *ar3);

void Viterbi(unsigned int *inData, unsigned int *outDate, int mode, int length){
	int byte6E = -1;							
	int bias[32] = { 3, 1, 3, 1, 0, 2, 0, 2, 0, 2, 0, 2, 3, 1, 3, 1, 2, 0, 2, 0, 1, 3, 1, 3, 1, 3, 1, 3, 2, 0, 2, 0 };		
	int bias_3[32] = { 7, 3, 6, 2, 0, 4, 1, 5, 0, 4, 1, 5, 7, 3, 6, 2, 5, 1, 4, 0, 2, 6, 3, 7, 2, 6, 3, 7, 5, 1, 4, 0 };	
	int bias_4[32] = { 15, 9, 5, 3, 2, 4, 8, 14, 2, 4, 8, 14, 15, 9, 5, 3, 12, 10, 6, 0, 1, 7, 11, 13, 1, 7, 11, 13, 12, 10, 6, 0 };	
	length /= mode;								

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
	unsigned int *TRN = (unsigned int*)calloc(length * 4, sizeof(int));	
	int state[128];								

	state[0] = 0;								
	for (int j = 1; j<64; j++)					
		state[j] = -1000;

	int B, T, ar2,
		byte_64,								
		k = 0;									
	int x1, x2, y1, x_cnt, y_cnt, z_cnt;		
	int ar3[16];								

	z_cnt = 0;									
	for (int ar0 = 0; ar0 < length; ar0++){			
		switch (mode){
		case(2) :
			byte_64 = callA(inData, &k, ar3);		
			break;
		case(3) :
			byte_64 = callA_3(inData, &k, ar3);	
			break;
		case(4) :
			byte_64 = callA_4(inData, &k, ar3);	
			break;
		}

		if (ar0 % 2 == 0){						
			x1 = 64; x2 = 96; y1 = 0;			
		}
		else{									
			x1 = 0; x2 = 32; y1 = 64;			
		}

		y_cnt = 0;								
		x_cnt = 0;								

		for (int ar1 = 0; ar1<4; ar1++){		
			for (int brc = 0; brc<8; brc++){	

				B = byte_64 ^ bias[x_cnt];		
				T = ar3[B];						

				if (state[y1 + y_cnt] + T > state[y1 + y_cnt + 1] - T) {
					state[x1 + x_cnt] = (state[y1 + y_cnt] + T);
					TRN[z_cnt] = TRN[z_cnt] << 1;	
					TRN[z_cnt] = TRN[z_cnt] & 0xFFFF;
				}
				else {
					state[x1 + x_cnt] = (state[y1 + y_cnt + 1] - T);
					TRN[z_cnt] = (TRN[z_cnt] << 1) + 1;
					TRN[z_cnt] = TRN[z_cnt] & 0xFFFF;
				}

				if (state[y1 + y_cnt] - T > state[y1 + y_cnt + 1] + T) {
					state[x2 + x_cnt] = state[y1 + y_cnt] - T;
					TRN[z_cnt] = TRN[z_cnt] << 1;	
					TRN[z_cnt] = TRN[z_cnt] & 0xFFFF;
				}
				else {
					state[x2 + x_cnt] = (state[y1 + y_cnt + 1] + T);
					TRN[z_cnt] = (TRN[z_cnt] << 1) + 1;
					TRN[z_cnt] = TRN[z_cnt] & 0xFFFF;
				}
				y_cnt += 2;						
				x_cnt++;						
			}
			z_cnt++;							
		}
		y_cnt = 0;								
		x_cnt = 0;								
	}

	int A = 0;														 
	int	ar4 = 0x0,								
		byte_66 = 0,							
		ar7 = 983,								
		mask = 0,								
		TC = 0;									
	int jump[64] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48,
		50, 52, 54, 56, 58, 60, 62, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35,
		37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63 };

	for (int brc = length - 1; brc>-1; brc--){			
		B = ar4;								

		if (B - 0x20 >= 0)						
			outDate[brc] = 1;
		else
			outDate[brc] = 0;

		B = (A >> 3) & 3;
		byte_66 = B;
		B = B ^ 3;
		ar7 = ar7 - B;

		
		A = jump[ar4];							
		T = A & 0xF;							
		mask = pow(2.0, (0xF - T));				
		TC = (TRN[ar7] & mask) >> (0xF - T);	

		if (ar4 - 0x20 >= 0){
			if (TC == 0)
				A = A - 1;
		}
		else{
			if (TC == 1)
				A = A + 1;
		}
		ar7 = ar7 + ~byte_66 + 1;				
		ar4 = A;								
		ar7--;
	}
}

int callA(unsigned int *inData, int *k, int *ar3){

	int A, B;
	A = inData[*k];				
	B = (A >> 6) & 2;					

	int byte_60 = A & 127;			

	A = inData[*k + 1];				
	int byte_64 = B + (A >> 7);		

	int byte_61 = A & 127;			

	ar3[3] = byte_61 + byte_60;		
	ar3[0] = ~ar3[3] + 1;			
	ar3[2] = byte_60 - byte_61;		
	ar3[1] = ~ar3[2] + 1;			

	*k += 2;

	return byte_64;
}

int callA_3(unsigned int *inData, int *k, int *ar3){
	int A, B = 0;
	int byte_mark[3];
	
	A = inData[*k];
	B += (A >> 5) & 12;
	byte_mark[0] = A & 127;
	*k += 1;

	A = inData[*k];
	B += (A >> 6) & 14;
	byte_mark[1] = A & 127;
	*k += 1;

	A = inData[*k];
	B += (A >> 7) & 15;
	byte_mark[2] = A & 127;
	*k += 1;

	int byte_64 = B;	

	ar3[0] = -byte_mark[0] - byte_mark[1] - byte_mark[2];
	ar3[1] = -byte_mark[0] - byte_mark[1] + byte_mark[2];
	ar3[2] = -byte_mark[0] + byte_mark[1] - byte_mark[2];
	ar3[3] = -byte_mark[0] + byte_mark[1] + byte_mark[2];
	ar3[4] = byte_mark[0] - byte_mark[1] - byte_mark[2];
	ar3[5] = byte_mark[0] - byte_mark[1] + byte_mark[2];
	ar3[6] = byte_mark[0] + byte_mark[1] - byte_mark[2];
	ar3[7] = byte_mark[0] + byte_mark[1] + byte_mark[2];

	return byte_64;
}

int callA_4(unsigned int *inData, int *k, int *ar3){
	int A, B = 0;

	int byte_mark[4];

	A = inData[*k];
	B += (A >> 4) & 8;
	byte_mark[0] = A & 127;
	*k += 1;

	A = inData[*k];
	B += (A >> 5) & 12;
	byte_mark[1] = A & 127;
	*k += 1;

	A = inData[*k];
	B += (A >> 6) & 14;
	byte_mark[2] = A & 127;
	*k += 1;

	A = inData[*k];
	B += (A >> 7) & 15;
	byte_mark[3] = A & 127;
	*k += 1;

	int byte_64 = B;		

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

	return byte_64;
}