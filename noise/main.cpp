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
#include <ctime>
using namespace std;

class WriteRead
{
public:
	int readFile(int **memory_0xA00, char fileName[], int CountRow)
	{
		FILE *fp;
		// read file
		if ((fp = fopen(fileName, "r")) == NULL) {
			printf("ќшибка открыти€ файла.\n");
			exit(1);
		}
		for (int i = 0; i< CountRow; i++){
			for (int j = 0; j < 120; j++){
				fscanf(fp, "%i", &memory_0xA00[i][j]); /* чтение из файла */
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
			printf("ќшибка открыти€ файла.\n");
			exit(1);
		}
		for (int i = 0; i< CountRow; i++){
			for (int j = 0; j < 120; j++){
				fprintf(fp, "%x ", memory_0xA00[i][j]);
			}
			fprintf(fp, "\n");
		}
		fclose(fp); // закрываем файл
	}
};

/* ‘ункци€ имитирующа€ шум. ¬ходные параметры:
	noise - кол-во зашумленных участков в посылке
	k_noise - максимальное кол-во испорченных бит подр€д в шум. участке
	nois_dist - минимальн€ дистанци€ между шумами
	 
	 »де€ проста: создать несколько участков зашумлени€, с разным кол-вом испорченных бит.
*/
void noise(int noise, int k_noise, int nois_dist, int mode, int **memory_0x600){

	int CountRow = 12;
	switch (mode)
	{
	case(2) :
		CountRow = 12;
		break;
	case(3) :
		CountRow = 14;
		break;
	case(4) :
		CountRow = 16;
		break;
	default:
		break;
	}

	int count_noise, dynamic_noise, dynamic_step,// счетчик испорченных бит, кол-во испорченных бит (по убыванию идет), шаг динамики
		count_dist, dist;					// дистанци€ между испорченными кусками посылки и счетчик этой дистанции
	bool can_break = true;					// можно портить биты



	dynamic_noise = k_noise;
	count_noise = dynamic_noise;			// начальна€ инициализаци€ счетчика
	if (noise > 0){
		dynamic_step = k_noise / noise;		// шаг = максимальное кол-во испорченных бит подр€д / посылок.

		dist = CountRow * 120 / (noise);		// вычисл€ем максимальное возможно рассто€ние
		if (dist > nois_dist)					// если заданное рассто€ние слишком велико, берем максимально допустимое, иначе 
			dist = nois_dist;					// заданное
		count_dist = dist;						// начальна€ инициализаци€ счетчика дистанции между шумами
	}
	else
	{
		can_break = false;
		dynamic_step = 0;

		dist = CountRow * 120;
		count_dist = dist;
	}
		
	noise--;								// учет зашумленных участков. "-", т.к. can_break = true по умолчанию.
	cout << "\ndynamic_step	"<<dynamic_step << endl;
	cout << "dist	" << dist << endl;
	cout << endl << "dynamic_noise	" << dynamic_noise << endl;
	for (int i = 0; i < CountRow; i++){		// цикл по строкам данных
		for (int j = 0; j < 120; j++){		// цикл по битам в строке
			count_dist--;
			if (can_break){
				// портим бит, заносим инвертированное значение
				if (memory_0x600[i][j] == 1)
					memory_0x600[i][j] = 0x15;	// 30% что это нуль
 				else
					memory_0x600[i][j] = 0x95;	// 30% что это единица

				if (count_noise <= 0){
					can_break = false;								// запрет на повреждение посылки
					dynamic_noise = dynamic_noise - dynamic_step;	// уменьшели кол-во испорченных бит в следующем зашумлении
					count_noise = dynamic_noise;					// занова инициализируем счетчик
					cout << "dynamic_noise	" << dynamic_noise << endl;
				}
				else{
					count_noise--;
				}
			}
			else
			{
				if (memory_0x600[i][j] == 1)
					memory_0x600[i][j] = 0xBF;	// максимальные веса. единица 100%
				else
					memory_0x600[i][j] = 0x3F;	// максимальные веса. нуль 100%

				if (count_dist <= 0 && noise > 0){
					count_dist = dist;
					can_break = true;
					noise--;
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	cout << "noise  " << endl;
	cout << "mode:	" << argv[1] << endl;
	cout << "file with encoded-interlived data:	" << argv[2] << endl;
	cout << "output file encoded-deinterlived data:	" << argv[3] << endl;
	int  i, mode = atoi(argv[1]), N = 12;									// 480, 720, 960
	int **memory_0x600;
	// noise - кол-во зашумлений в посылке
	// k_noise - максимальное кол-во испорченных бит подр€д
	// nois_dist - минимальн€ дистанци€ между шумами
	int nois = atoi(argv[4]), noise_k = atoi(argv[5]), nois_dist = atoi(argv[6]);
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
	memory_0x600 = (int **)calloc(N, sizeof(*memory_0x600));  //arr теперь указывает на массив указателей на int
	for (i = 0; i < N; ++i)
		memory_0x600[i] = (int*)calloc(120, sizeof(*memory_0x600[i]));
	WriteRead wr;
	wr.readFile(memory_0x600, argv[2], N);
	noise(nois, noise_k, nois_dist, mode, memory_0x600);
	//noise(0, 25, 120, mode, memory_0x600);
	wr.writeInFile(memory_0x600, argv[3], N);

	//после завершени€ работы с массивом, необходимо освободить всю выделенную пам€ть
	for (i = 0; i < N ; i++)
		free(memory_0x600[i]);
	free(memory_0x600);
	return 0;
}

