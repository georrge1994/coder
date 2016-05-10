#include "stdafx.h"

class ChannelCoding
{
public:
	unsigned int accumulatorA;
	unsigned int mode;
	unsigned int accumulatorB;
	unsigned int *memory_0x600[1000];
	int brc;
public:
	void sub_5379(unsigned int *memory_0xA00, unsigned int *memory_0x600)
	{
		sub_5b0d(memory_0xA00, memory_0x600);			// half(quater) coding
		//	sub_5c1b(memory_0x600);							// delete some bits
	}

private:
	void sub_5b0d(unsigned int *memory_0xA00, unsigned int *memory_0x600)
	{
		int ar0 = 0;
		int regT = 0;
		// режим 1/2
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
		// режим 1/4
		int tableRepliesQuater[8][4] = {{ 42330, 42330, 26214, 27030 },
										{ 23205, 42330, 39321, 27030 },
										{ 42330, 23205, 39321, 27030 },
										{ 23205, 23205, 26214, 27030 },
										{ 23205, 23205, 39321, 38505 },
										{ 42330, 23205, 26214, 38505 },
										{ 23205, 42330, 26214, 38505 },
										{ 42330, 42330, 39321, 38505 } };
		int count = 0, i = 0, j = 0;
		switch (mode){
		case(2) :
			for (i = 0; i<246; i++){
				accumulatorA = accumulatorA << 1;
				accumulatorA = accumulatorA & 127;
				accumulatorA = accumulatorA | memory_0xA00[i];
				ar0 = (accumulatorA >> 4) & 7;	// старшие три бита
				regT = accumulatorA & 15;		// младшие четыре бита - сдвиг в числе
				for (j = 0; j<brc; j++){
					accumulatorB = tableRepliesHalf[ar0][j] >> regT;	// <- отличие здесь!
					memory_0x600[count] = accumulatorB & 1;
					count++;
				}
			}
			break;
		case(3) :
			for (i = 0; i<246; i++){
				accumulatorA = accumulatorA << 1;
				accumulatorA = accumulatorA & 127;
				accumulatorA = accumulatorA | memory_0xA00[i];
				ar0 = (accumulatorA >> 4) & 7;	// старшие три бита
				regT = accumulatorA & 15;		// младшие четыре бита - сдвиг в числе
				for (j = 0; j<brc; j++){
					accumulatorB = tableRepliesThree[ar0][j] >> regT;	// <- отличие здесь!
					memory_0x600[count] = accumulatorB & 1;
					count++;
				}
			}
			break;
		case(4) :
			for (i = 0; i<246; i++){
				accumulatorA = accumulatorA << 1;
				accumulatorA = accumulatorA & 127;
				accumulatorA = accumulatorA | memory_0xA00[i];
				ar0 = (accumulatorA >> 4) & 7;	// старшие три бита
				regT = accumulatorA & 15;		// младшие четыре бита - сдвиг в числе
				for (j = 0; j<brc; j++){
					accumulatorB = tableRepliesQuater[ar0][j] >> regT;	// <- отличие здесь!
					memory_0x600[count] = accumulatorB & 1;
					count++;
				}
			}
			break;
		}
	}


	void sub_5c1b(unsigned int *memory_0x600)
	{
		int step = 0, k = 0, N;
		if (mode == 0){
			N = 492;
		}
		else{
			N = 984;
		}
		for (int i = 0; i < N; i++)
		{
			if (step == 0){
				cout << i << "	" << memory_0x600[i] << endl;
				if (mode == 0){
					step = 41;
				}
				else{
					step = 81;
					i++;
				}
			}
			else{
				memory_0x600[k] = memory_0x600[i];
				k++;
			}
			step--;
		}
	}
};
