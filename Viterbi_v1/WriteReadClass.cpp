#include <fstream>
#include <iostream>
using namespace std;
class WriteRead
{
public:
	int readFile(unsigned int *memory, char * fileName, int sizeOfArray)
	{

		FILE *fl = fopen(fileName, "rb");
		// read file
		for (int i = 0; i< sizeOfArray; i++){
			if (feof(fl)){
				break;
			}
			fscanf(fl, "%x", &memory[i]);
		}
		fclose(fl);

		return *memory;

	}

	void writeInFile(unsigned int *memory, string fileName, int sizeOfArray)
	{
		// write in file
		ofstream fout(fileName);
		int count = 1;
		for (int i = 0; i< sizeOfArray - 6; i++){
			printf("%x\n", memory[i]);
			fout << memory[i] << " ";
			if (count % 120 == 0)
				fout << endl;
			count++;
		}
		fout.close(); // закрываем файл
	}
};