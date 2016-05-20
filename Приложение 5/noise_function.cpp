#include "noise_function.h"

void noise_function(int noise, int k_noise, int nois_dist, int mode, unsigned int *memory_0x600){

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
		count_dist, dist;						// дистанция между испорченными участками посылки и счетчик этой дистанции
	bool can_break = true;						// можно портить биты
	int i, j;


	dynamic_noise = k_noise;
	count_noise = dynamic_noise;				// начальная инициализация счетчика
	if (noise > 0){
		dynamic_step = k_noise / noise;			// шаг = максимальное кол-во испорченных бит подряд / посылок.

		dist = CountRow * 120 / (noise);		// вычисляем максимальное возможно расстояние
		if (dist > nois_dist)					// если заданное расстояние слишком велико, берем максимально допустимое, иначе 
			dist = nois_dist;					// заданное
		count_dist = dist;						// начальная инициализация счетчика дистанции между шумами
	}
	else
	{
		can_break = false;
		dynamic_step = 0;

		dist = CountRow * 120;
		count_dist = dist;
	}


	for (i = 0; i < CountRow; i++){				// цикл по строкам данных
		for (j = 0; j < 120; j++){				// цикл по битам в строке
			if (memory_0x600[120 * i + j] == 1)
				memory_0x600[120 * i + j] = 0xBF;// максимальные веса. единица 100%
			else
				memory_0x600[120 * i + j] = 0x3F;// максимальные веса. нуль 100%
		}
	}

	int random_int = rand();
	int ri_120 = (random_int % 120);
	noise--;									// учет зашумленных участков. "-", т.к. can_break = true по умолчанию.
	for (i = 0; i < CountRow; i++){				// цикл по строкам данных
		for (j = ri_120; j < 120; j++){			// цикл по битам в строке

			count_dist--;
			if (can_break){
				// портим бит, заносим инвертированное значение
				if (memory_0x600[120 * i + j] == 0xBF){
					memory_0x600[120 * i + j] = 0x15;	// 30% что это нуль
				}
				else{
					memory_0x600[120 * i + j] = 0x95;	// 30% что это единица
				}
				if (count_noise <= 0){
					can_break = false;								// запрет на повреждение посылки
					dynamic_noise = dynamic_noise - dynamic_step;	// уменьшели кол-во испорченных бит в следующем зашумлении
					count_noise = dynamic_noise;					// занова инициализируем счетчик
				}
				else{
					count_noise--;
				}
			}
			else
			{
				if (count_dist <= 0 && noise > 0){
					count_dist = dist;
					can_break = true;
					noise--;
				}
			}
		}
		ri_120 = 0;
	}
}
