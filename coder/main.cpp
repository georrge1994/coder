// test2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include "ReadAndWrite.cpp"
#include "coder.cpp"

int _tmain(int argc, char *argv[])
{
	cout << "coder11  " << endl;
	cout << "mode:	" << argv[1] << endl;
	cout << "input file with data:	" << argv[2] << endl;
	cout << "output file with coding data:	" << argv[3] << endl;
	unsigned int *memory_0xA00 = (unsigned int *)calloc(246, sizeof(*memory_0xA00));
	unsigned int *memory_0x600;
	ChannelCoding CC;
	CC.accumulatorA = 0;
	CC.accumulatorB = 0;
	int in_bit, out_bit;
	//switch(atoi(argv[1])){
	switch (3 ){
	case(2) :
		CC.mode = 2;
		CC.brc = 2;
		in_bit = 240;
		out_bit = 492;
		break;
	case(3) :
		CC.mode = 3;
		CC.brc = 3;
		in_bit = 240;
		out_bit = 738;
		break;
	case(4) :
		CC.mode = 4;
		CC.brc = 4;
		in_bit = 240;
		out_bit = 984;
		break;
	}
	memory_0x600 = (unsigned int *)calloc(out_bit, sizeof(*memory_0x600));
	WriteRead WR;								// object of class "write and read" 
	WR.readFile(memory_0xA00, argv[2], in_bit);	// read source bits from "memory"
	CC.sub_5379(memory_0xA00, memory_0x600);
	WR.writeInFile(memory_0x600, argv[3], out_bit);
	free(memory_0x600);
	return 0;
}

