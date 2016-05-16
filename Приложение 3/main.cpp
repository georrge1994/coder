#include "codec.h"
#include "noise_function.h"
#include <fstream>

int main(int argc, char *argv[])
{
	int in_bit = 240,				// ����� ����������� ���������
		out_bit,					// ����� ��������������� ��������� (��� �������� ��. ��������)
		mode = atoi(argv[1]),
		count_bits_from_chanel;		// ���-�� ��� �������� � ������

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

	// ������
	unsigned int memory[0xFFFF] = { 0 };

	// ���������� ��� ��������� ����. ������.
	int keyword = 0, shift = 0;				

	// ���������� ��� �����
	int i,
		error = 0,							// ���-�� ������ � count_tests
		count_tests = 1024,					// ���-�� ������ �� ������
		average_error = 0,					// ������� �������������� ������ �� N ������� (count_tests/error)
		count_error = 492;					// ������������ ���-�� ������ �������� � ����

	int length_message;						// ����� ��������������� ���������

	// ��������� ���� 
	int nois = 2,							// ����� �����������
		noise_k,							// ���-�� ���������� � ������� 
		nois_dist;							// ���������� ��������� ����� ������ 

	// write in file
	ofstream fout("result_of_the_test.txt");

	for (int noise_test = 0; noise_test < 492; noise_test++){
		for (int o = 0; o < count_tests; o++){

			// �������� ������-��������� ������������������
			for (i = 0; i< 240; i++){
				memory[i] = (keyword >> shift) & 1;
				shift++;
				if (shift>10)
					shift = 0;
			}

			length_message = coder(mode, &memory[0x0], &memory[0x1000], in_bit);		// encoder
			interliving(&memory[0xCB8], length_message);								// function interliving

			noise_k = noise_test;														// ���-�� ���������� � ������� 
			nois_dist = rand() % 80;													// ���������� ��������� ����� ������ 
			noise_function(nois, noise_k, nois_dist, mode, &memory[0xCB8]);				// (���� ���������� ��������� � �������� ������ - 0,0,120,..,..)

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

