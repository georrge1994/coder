#include "wt_function.h"

void wt_function(unsigned int *memory_0x600, int length){

	do{
		if (memory_0x600[length] == 1)
			memory_0x600[length] = 0xBF;
		else
			memory_0x600[length] = 0x3F;
		length--;
	} while (length >= 0);
}
