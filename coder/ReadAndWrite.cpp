
#include "stdafx.h"
#include <fstream>

class WriteRead
{
public:
	int readFile(unsigned int *memory_0xA00, string fileName, int sizeOfArray)
	{
		// create array of zeros, size=240 

		for (int i = 0; i< sizeOfArray + 6; i++){
			memory_0xA00[i] = 0;
		}

		// read file
		ifstream fin(fileName);
		for (int i = 0; i< sizeOfArray; i++){
			if (fin.eof()){
				break;
			}
			fin >> memory_0xA00[i];
		}
		fin.close(); // закрываем файл

		return *memory_0xA00;
	}

	void writeInFile(unsigned int *memory_0xA00, string fileName, int sizeOfArray)
	{
		// write in file
		ofstream fout(fileName);
		int count = 1;
		for (int i = 0; i< sizeOfArray; i++){
			fout << memory_0xA00[i] << " ";
			if (count % 120 == 0)
				fout << endl;
			count++;
		}
		fout.close(); // закрываем файл
	}
};