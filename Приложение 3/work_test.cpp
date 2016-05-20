#include "codec.h"
#include "wt_function.h"
#include <fstream>

int main(int argc, char *argv[])
{
	int in_bit = 240,				
		out_bit,					
		mode = atoi(argv[1]),
		count_bits_from_chanel;		

	switch (mode){
	case(2) :
		out_bit = 492;
		count_bits_from_chanel = 1440;
		break;
	case(3) :
		out_bit = 738;
		count_bits_from_chanel = 1680; 
		break; 
	case(4) :
		out_bit = 984;
		count_bits_from_chanel = 1920;
		break;
	}

	unsigned int memory[0xFFFF] = { 0 };
	int keyword = 0, shift = 0;				

	int i,
		error = 0,							
		count_tests = 1024,					
		average_error = 0,					
		count_error = 0;					

	int length_message;						

		for (int o = 0; o < count_tests; o++){
			for (i = 0; i< 240; i++){
				memory[i] = (keyword >> shift) & 1;
				shift++;
				if (shift>10)
					shift = 0;
			}

			length_message = coder(mode, &memory[0x0], &memory[0x1000], in_bit);		
			interliving(&memory[0xCB8], length_message);								

			wt_function(&memory[0xCB8], count_bits_from_chanel);

			deinterliving(&memory[0xCB8], 1920);										
			Viterbi(&memory[0x1000], &memory[0x2000], mode, out_bit);					

			for (i = 0; i< 240; i++){
				if (memory[i] != memory[0x2000 + i])
					error++;
			}
			if (error > 0){
				cout << "Error! Number of mistakes = " << error << ". Bad keyword = " << keyword << endl;
				count_error++;
			}
			else{
				cout << "It all ok " << keyword << endl;
			}
			keyword++;
		}

	cout << "Count of error: " << count_error << " out of " << count_tests << " Corl!" << endl;
	getchar();
	return 0;
}

