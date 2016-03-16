#define _CRT_SECURE_NO_WARNINGS
// deinterliving.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
using namespace std;

class WriteRead
{
public:
	int readFile(int **memory_0xA00, char fileName[], int CountRow)
	{
		FILE *fp;
		// read file
		if ((fp = fopen(fileName, "r")) == NULL) {
			printf("Ошибка открытия файла.\n");
			exit(1);
		}
		for (int i = 0; i< CountRow; i++){
			for (int j = 0; j < 120; j++){
				fscanf(fp, "%x", &memory_0xA00[i][j]); /* чтение из файла */
			}
		}
		fclose(fp); // закрываем файл
		return **memory_0xA00;

	}

	void writeInFile(int **memory_0xA00, char fileName[], int CountRow)
	{
		FILE *fp;
		// read file
		if ((fp = fopen(fileName, "w")) == NULL) {
			printf("Ошибка открытия файла.\n");
			exit(1);
		}
		for (int i = 7; i< CountRow; i++){
			for (int j = 0; j < 120; j++){
				fprintf(fp, "%x ", memory_0xA00[i][j]);
			}
			fprintf(fp, "\n");
		}
		fclose(fp); // закрываем файл
	}
};


void deinterliving(int mode, int **memory_0x600){
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
	
	int index = 0;				  // индекс в массиве
	for (int i = 0; i < CountRow - 7; i++){	// цикл по строкам данных
		for (int j = 0; j < 120; j++){		// цикл по битам в строке

			memory_0x600[CountRow - 1][index] = memory_0x600[i][j]; // записываем новую последовательность бит в неиспользуемую строку
			index = index + 10;				// каждый 10-ый элемент
			if (index > 119)				// "обнуляем" при необходимости
				index = index - 119;		// увеличивая индекс на 1
		}
		index = 0;
		for (int j = 0; j < 120; j++){
			memory_0x600[i][j] = memory_0x600[CountRow - 1][j]; // переносим новую строку на место старой
		}
	}
	
	int i,					// номер плавающей строки
	row = CountRow - 8;		// номер текущей строки(максимум их 7+х)
	do{
		i = row;
		for (int k = 0; k < 120; k++){ // номер элемента в массиве
			if (i - row == 8){
				i = row;
			}
			memory_0x600[i][k] = memory_0x600[row][k];
			i++;
		}
		row--;
	} while (row >= 0); // обработать нужно только семь нулевых + x новых, 7 конечных обрабатывать не нужно(поэтому CountRow-7)
	
}

int main(int argc, char *argv[])
{
	cout << "deinterliving  " << endl;
	cout << "mode:	" << argv[1] << endl;
	cout << "file with encoded-interlived data:	" << argv[2] << endl;
	cout << "output file encoded-deinterlived data:	" << argv[3] << endl;
	int  i, mode = atoi(argv[1]), N = 12;									// 480, 720, 960
	int **memory_0x600;
	switch (mode)
	{
	case(2) :
		N = 12;
		break;
	case(3) :
		N = 14;
		break;
	case(4) :
		N = 16;
		break;
	default:
		break;
	}
	memory_0x600 = (int **)calloc(N + 7, sizeof(*memory_0x600));  //arr теперь указывает на массив указателей на int
	for (i = 0; i < N + 7; ++i)
		memory_0x600[i] = (int*)calloc(120, sizeof(*memory_0x600[i]));
	WriteRead wr;
	cout << "test16";
	wr.readFile(memory_0x600, argv[2], N);
	deinterliving(mode, memory_0x600);
	wr.writeInFile(memory_0x600, argv[3], N);

	//после завершения работы с массивом, необходимо освободить всю выделенную память
	for (i = 0; i < N + 7; i++)
		free(memory_0x600[i]);
	free(memory_0x600);
	return 0;
}

