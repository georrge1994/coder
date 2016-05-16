#ifndef __NOISE_F__
#define __NOISE_F__

#include <iostream>
using namespace std;
	/*
	Функция имитирующая шум. Входные параметры:
	noise - кол-во зашумленных участков в посылке
	k_noise - максимальное кол-во испорченных бит подряд в шум. участке
	nois_dist - минимальня дистанция между шумами
	*/
	void noise_function(int noise, int k_noise, int nois_dist, int mode, unsigned int *memory_0x600);

#endif