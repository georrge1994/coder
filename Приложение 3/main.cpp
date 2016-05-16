#include "codec.h"
#include "noise_function.h"
#include <fstream>

int main(int argc, char *argv[])
{
	int in_bit = 240,				// длина кодируемого сообщения
		out_bit,					// длина закодированного сообщения (для декодера вх. параметр)
		mode = atoi(argv[1]),
		count_bits_from_chanel;		// кол-во бит принятых с канала

	// this is switch-help for in_bits=240
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

	// память
	unsigned int memory[0xFFFF] = { 0 };

	// переменные для генерации случ. послед.
	int keyword = 0, shift = 0;				

	// переменные для теста
	int i,
		error = 0,							// кол-во ошибок в count_tests
		count_tests = 1024,					// кол-во тестов на ошибку
		average_error = 0,					// среднее арихметическое ошибок на N посылок (count_tests/error)
		count_error = 492;					// максимальное кол-во ошибок вносимых в тест

	int length_message;						// длина закодированного сообщения

	// параметры шума 
	int nois = 2,							// режим кодирования
		noise_k,							// кол-во зашумлений в посылке 
		nois_dist;							// минимальня дистанция между шумами 

	// write in file
	ofstream fout("result_of_the_test.txt");

	for (int noise_test = 0; noise_test < 492; noise_test++){
		for (int o = 0; o < count_tests; o++){

			// создание псевдо-случайной последовательности
			for (i = 0; i< 240; i++){
				memory[i] = (keyword >> shift) & 1;
				shift++;
				if (shift>10)
					shift = 0;
			}

			length_message = coder(mode, &memory[0x0], &memory[0x1000], in_bit);		// encoder
			interliving(&memory[0xCB8], length_message);								// function interliving

			noise_k = noise_test;														// кол-во зашумлений в посылке 
			nois_dist = rand() % 80;													// минимальня дистанция между шумами 
			noise_function(nois, noise_k, nois_dist, mode, &memory[0xCB8]);				// (если необходимо запустить в холостом режиме - 0,0,120,..,..)

			deinterliving(&memory[0xCB8], 1920);										// function deinterliving(240 -> 1440; 1680; 1920)
			Viterbi(&memory[0x1000], &memory[0x2000], mode, out_bit);					// Viterbi decoder (240 -> 492; 738; 984)

			for (i = 0; i< 240; i++){
				if (memory[i] != memory[0x2000 + i])
					error++;
			}
			/*if (error > 0){
				cout << "Error! Number of mistakes = " << error << ". Bad keyword = " << keyword << endl;
				count_error++;
			}
			else{
				cout << "It all ok " << keyword << endl;
			}*/
			keyword++;
		}
		average_error = error / 1024;													
		fout << average_error << "	" << noise_test << endl;							// result write in file
		error = 0;
		cout << noise_test << endl;														// cout number of group test 
	}
	fout.close(); 
	/*cout << "Count of error: " << count_error << " out of " << count_tests << " Corl!" << endl;
	getchar();*/

	return 0;
}

