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

	int count_noise, dynamic_noise, dynamic_step,// ������� ����������� ���, ���-�� ����������� ��� (�� �������� ����), ��� ��������
		count_dist, dist;						// ��������� ����� ������������ ��������� ������� � ������� ���� ���������
	bool can_break = true;						// ����� ������� ����
	int i, j;


	dynamic_noise = k_noise;
	count_noise = dynamic_noise;				// ��������� ������������� ��������
	if (noise > 0){
		dynamic_step = k_noise / noise;			// ��� = ������������ ���-�� ����������� ��� ������ / �������.

		dist = CountRow * 120 / (noise);		// ��������� ������������ �������� ����������
		if (dist > nois_dist)					// ���� �������� ���������� ������� ������, ����� ����������� ����������, ����� 
			dist = nois_dist;					// ��������
		count_dist = dist;						// ��������� ������������� �������� ��������� ����� ������
	}
	else
	{
		can_break = false;
		dynamic_step = 0;

		dist = CountRow * 120;
		count_dist = dist;
	}


	for (i = 0; i < CountRow; i++){				// ���� �� ������� ������
		for (j = 0; j < 120; j++){				// ���� �� ����� � ������
			if (memory_0x600[120 * i + j] == 1)
				memory_0x600[120 * i + j] = 0xBF;// ������������ ����. ������� 100%
			else
				memory_0x600[120 * i + j] = 0x3F;// ������������ ����. ���� 100%
		}
	}

	int random_int = rand();
	int ri_120 = (random_int % 120);
	noise--;									// ���� ����������� ��������. "-", �.�. can_break = true �� ���������.
	for (i = 0; i < CountRow; i++){				// ���� �� ������� ������
		for (j = ri_120; j < 120; j++){			// ���� �� ����� � ������

			count_dist--;
			if (can_break){
				// ������ ���, ������� ��������������� ��������
				if (memory_0x600[120 * i + j] == 0xBF){
					memory_0x600[120 * i + j] = 0x15;	// 30% ��� ��� ����
				}
				else{
					memory_0x600[120 * i + j] = 0x95;	// 30% ��� ��� �������
				}
				if (count_noise <= 0){
					can_break = false;								// ������ �� ����������� �������
					dynamic_noise = dynamic_noise - dynamic_step;	// ��������� ���-�� ����������� ��� � ��������� ����������
					count_noise = dynamic_noise;					// ������ �������������� �������
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
