// deinterliving.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
//#include <tchar.h>
using namespace std;

class WriteRead
{
public:
	int readFile(int **memory_0xA00, string fileName, int CountRow)
	{
		// read file
		ifstream fin(fileName);
		for (int i = 7; i< 7+CountRow; i++){
			for (int j = 0; j < 120; j++){
				fin >> memory_0xA00[i][j];
				if (fin.eof()){
					break;
				}
			}
		}
		fin.close(); // закрываем файл
		
		return **memory_0xA00;

	}

	void writeInFile(int **memory_0xA00, string fileName, int CountRow)
	{
		// write in file
		ofstream fout(fileName);
		for (int i = 0; i< 7+CountRow; i++){
			for (int j = 0; j < 120; j++){
				fout << memory_0xA00[i][j]<<" ";
			}
			fout << endl;
		}
		fout.close(); // закрываем файл
	}
};


void interliving(int mode, int **memory_0x600){
	int CountRow = 19;
	switch (mode)
	{
	case(2) :
		CountRow = 19;
		break;
	case(3) :
		CountRow = 21;
		break;
	case(4) :
		CountRow = 23;
		break;
	default:
		break;
	}

	int i,			// номер плавающей строки
		row = 0;	// номер текущей строки(максимум их 7+х)
	do{
		i = row;
		for (int k = 0; k < 120; k++){ // номер элемента в массиве
			if (i - row == 8){
				i = row;
			}
			memory_0x600[row][k] = memory_0x600[i][k];
			i++;
		}
		row++;
	} while (row < CountRow - 7); // обработать нужно только семь нулевых + x новых, 7 конечных обрабатывать не нужно(поэтому CountRow-7)
	
	int index = 0;				  // индекс в массиве
	for (int i = 0; i < CountRow - 7; i++){	// цикл по строкам данных
		for (int j = 0; j < 120; j++){		// цикл по битам в строке
			memory_0x600[CountRow-1][j] = memory_0x600[i][index]; // записываем новую последовательность бит в неиспользуемую строку
			index = index + 10;				// каждый 10-ый элемент
			if (index > 119)				// "обнуляем" при необходимости
				index = index - 119;		// увеличивая индекс на 1
		}
		index = 0;
		for (int j = 0; j < 120; j++){
			memory_0x600[i][j] = memory_0x600[CountRow-1][j]; // переносим новую строку на место старой
		}
	}
}

int main(int argc, char *argv[])
{
	cout << "interliving" << endl;
	cout << "mode:	" << argv[1] << endl;
	cout << "input file with coded data:	" << argv[2] << endl;
	cout << "output file with interlived coded data:	" << argv[3] << endl;
	int  i,mode, N=5;									// 480, 720, 960
	int **memory_0x600;
	mode = atoi(argv[1]);
	switch (mode)
	{
	case(2) :
		N = 5;
		break;
	case(3) :
		N = 7;
		break;
	case(4) :
		N = 9;
		break;
	default:
		//cout << "uncorret mode" << endl;
		getchar();
		return -1;
	}
	memory_0x600 = (int **)calloc(N+14, sizeof(*memory_0x600));  //arr теперь указывает на массив указателей на int
	for (i = 0; i < N + 14; ++i)
		memory_0x600[i] = (int*)calloc(120, sizeof(*memory_0x600[i]));
	WriteRead wr;
	cout << "teset16";
	wr.readFile(memory_0x600, argv[2], N);
	interliving(mode, memory_0x600);		
	for (i = 0; i < N + 14; i++){	// цикл по строкам данных
		for (int j = 0; j < 120; j++){
			cout << memory_0x600[i][j];
		}
		cout << endl;
	}
	getchar();
	wr.writeInFile(memory_0x600, argv[3], N);

	//после завершения работы с массивом, необходимо освободить всю выделенную память
	for (i = 0; i < N + 14; i++)
		free(memory_0x600[i]);
	free(memory_0x600);
	return 0;
}

