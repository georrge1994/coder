#include "codec.h"
/*
	Сверточный кодер:
	mode - режим кодирования;
	memory_0xA00 - указатель на данные;
	memory_0x600 - указатель на область памяти, куда будет записан результат;
	length	- длина кодируемого сообщения;
*/
int coder(int mode, unsigned int *memory_0xA00, unsigned int *memory_0x600, unsigned int length)
	{
		int accumulatorA = 0;			
		int accumulatorB = 0;
		int brc = mode;					// count of cycle
		int ar0 = 0;
		int regT = 0;
		int count_bits = 0, i = 0, j = 0;

		// for mode 1/2
		int tableRepliesHalf[8][2] =   {{ 42330, 27030 },
										{ 42330, 27030 },
										{ 23205, 27030 },
										{ 23205, 27030 },
										{ 23205, 38505 },
										{ 23205, 38505 },
										{ 42330, 38505 },
										{ 42330, 38505 } };
		// for mode 1/3
		int tableRepliesThree[8][3] = { { 42330, 27030, 42330, },
										{ 42330, 27030, 23205 },
										{ 23205, 27030, 42330 },
										{ 23205, 27030, 23205 },
										{ 23205, 38505, 23205 },
										{ 23205, 38505, 42330 },
										{ 42330, 38505, 23205 },
										{ 42330, 38505, 42330 } };
		// for mode 1/4
		int tableRepliesQuater[8][4] = {{ 42330, 42330, 26214, 27030 },
										{ 23205, 42330, 39321, 27030 },
										{ 42330, 23205, 39321, 27030 },
										{ 23205, 23205, 26214, 27030 },
										{ 23205, 23205, 39321, 38505 },
										{ 42330, 23205, 26214, 38505 },
										{ 23205, 42330, 26214, 38505 },
										{ 42330, 42330, 39321, 38505 } };
		
		switch (mode){
		case(2) :
			for (i = 0; i < length + 6; i++){
				accumulatorA = accumulatorA << 1;
				accumulatorA = accumulatorA & 127;
				accumulatorA = accumulatorA | memory_0xA00[i];
				ar0 = (accumulatorA >> 4) & 7;	// old 3 bits
				regT = accumulatorA & 15;		// younger 4 bits
				for (j = 0; j<brc; j++){
					accumulatorB = tableRepliesHalf[ar0][j] >> regT;	// <- distinction here
					memory_0x600[count_bits] = accumulatorB & 1;
					count_bits++;
				}
			}
			break;
		case(3) :
			for (i = 0; i < length + 6; i++){
				accumulatorA = accumulatorA << 1;
				accumulatorA = accumulatorA & 127;
				accumulatorA = accumulatorA | memory_0xA00[i];
				ar0 = (accumulatorA >> 4) & 7;	// old 3 bits
				regT = accumulatorA & 15;		// younger 4 bits
				for (j = 0; j<brc; j++){
					accumulatorB = tableRepliesThree[ar0][j] >> regT;	// <- distinction here
					memory_0x600[count_bits] = accumulatorB & 1;
					count_bits++;
				}
			}
			break;
		case(4) :
			for (i = 0; i < length + 6; i++){
				accumulatorA = accumulatorA << 1;
				accumulatorA = accumulatorA & 127;
				accumulatorA = accumulatorA | memory_0xA00[i];
				ar0 = (accumulatorA >> 4) & 7;	// old 3 bits
				regT = accumulatorA & 15;		// younger 4 bits
				for (j = 0; j<brc; j++){
					accumulatorB = tableRepliesQuater[ar0][j] >> regT;	// <- distinction here
					memory_0x600[count_bits] = accumulatorB & 1;
					count_bits++;
				}
			}
			break;
		}
		return count_bits;
	}

/*	
	Функции чередования и ликвидации чередования:
	memory_0x600 - указатель на участок памяти(массива);
	length	- длина сообщения, надо которыми нужно провести/ликвидировать чередование.	
*/
void interliving(unsigned int *memory_0x600, int length){
	int CountRow = length / 120 + 15;
	int i,			// номер плавающей строки
		row = 0;	// номер текущей строки(максимум их 7+х)
	do{
		i = row;
		for (int k = 0; k < 120; k++){ // номер элемента в массиве
			if (i - row == 8){
				i = row;
			}
			memory_0x600[120 * row + k] = memory_0x600[120 * i + k];
			i++;
		}
		row++;
	} while (row < CountRow - 7); // обработать нужно только семь нулевых + x новых, 7 конечных обрабатывать не нужно(поэтому CountRow-7)

	int index = 0;				  // индекс в массиве
	for (int i = 0; i < CountRow - 7; i++){	// цикл по строкам данных
		for (int j = 0; j < 120; j++){		// цикл по битам в строке
			memory_0x600[120 * (CountRow - 1) + j] = memory_0x600[120 * i + index]; // записываем новую последовательность бит в неиспользуемую строку
			index = index + 10;				// каждый 10-ый элемент
			if (index > 119)				// "обнуляем" при необходимости
				index = index - 119;		// увеличивая индекс на 1
		}
		index = 0;
		for (int j = 0; j < 120; j++){
			memory_0x600[120 * i + j] = memory_0x600[120 * (CountRow - 1) + j]; // переносим новую строку на место старой
		}
	}
}
void deinterliving(unsigned int *memory_0x600, int length){
	int CountRow = length / 120 + 7;		// 7- это пространство необходимое для "распаковки"
	int index = 0;							// индекс в массиве
	for (int i = 0; i < CountRow - 7; i++){	// цикл по строкам данных
		for (int j = 0; j < 120; j++){		// цикл по битам в строке

			memory_0x600[120 * (CountRow - 1) + index] = memory_0x600[120 * i + j]; // записываем новую последовательность бит в неиспользуемую строку
			index = index + 10;				// каждый 10-ый элемент
			if (index > 119)				// "обнуляем" при необходимости
				index = index - 119;		// увеличивая индекс на 1
		}
		index = 0;
		for (int j = 0; j < 120; j++){
			memory_0x600[120 * i + j] = memory_0x600[120 * (CountRow - 1) + j]; // переносим новую строку на место старой
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
			memory_0x600[120 * i + k] = memory_0x600[120 * row + k];
			i++;
		}
		row--;
	} while (row >= 0); // обработать нужно только семь нулевых + x новых, 7 конечных обрабатывать не нужно(поэтому CountRow-7)	
}

int callA(unsigned int *inData, int *k, int *ar3);
int callA_3(unsigned int *inData, int *k, int *ar3);
int callA_4(unsigned int *inData, int *k, int *ar3);

/*
	Декодер-Витерби:
	inData - указатель на закодированные данные;
	outDate - указатель на декодированные данные;
	mode - режим декодирования;
	length - длина декодируемого сообщения.
*/
void Viterbi(unsigned int *inData, unsigned int *outDate, int mode, int length){
	int byte6E = -1;							// число для функции корректироки
	int bias[32] = { 3, 1, 3, 1, 0, 2, 0, 2, 0, 2, 0, 2, 3, 1, 3, 1, 2, 0, 2, 0, 1, 3, 1, 3, 1, 3, 1, 3, 2, 0, 2, 0 };		// переходы между состояниями
	int bias_3[32] = { 7, 3, 6, 2, 0, 4, 1, 5, 0, 4, 1, 5, 7, 3, 6, 2, 5, 1, 4, 0, 2, 6, 3, 7, 2, 6, 3, 7, 5, 1, 4, 0 };	// для 1/3
	int bias_4[32] = { 15, 9, 5, 3, 2, 4, 8, 14, 2, 4, 8, 14, 15, 9, 5, 3, 12, 10, 6, 0, 1, 7, 11, 13, 1, 7, 11, 13, 12, 10, 6, 0 };	// для 1/4
	length /= mode;								// тело цикла прямой проходки должно быть равно кол-ву 
	// закодированных бит (кол-во полученных / режим кодирования)
	// установка числа корректировки в соответствии с режимом
	switch (mode){
	case(2) :
		byte6E = 63;
		break;
	case(3) :
		byte6E = 63;
		for (int i = 0; i < 32; i++){
			bias[i] = bias_3[i];
		}
		break;
	case(4) :
		byte6E = 31;
		for (int i = 0; i < 32; i++){
			bias[i] = bias_4[i];
		}
		break;
	}

	int byte_68 = 0;
	unsigned int *TRN = (unsigned int*)calloc(length * 4, sizeof(int));	// регистры trn
	int state[128];								// адреса под решетку Витерби (старые + новые)

	// инициализация начальных состояний
	state[0] = 0;								// первое состояние уникально
	for (int j = 1; j<64; j++)					// инициализация остальных состояний
		state[j] = -1000;

	int B, T, ar2,
		byte_64,								// полученная пара бит
		k = 0;									// номер байта, передоваемый в callA
	int x1, x2, y1, x_cnt, y_cnt, z_cnt;		// служебные переменные(описание при объявлении, см. ниже)
	int ar3[16];								// массив мягких оценок

	/*ПРЯМАЯ ПРОХОДКА begin:*/
	z_cnt = 0;									// счетчик для TRN
	for (int ar0 = 0; ar0 < length; ar0++){			// цикл №1
		switch (mode){
		case(2) :
			byte_64 = callA(inData, &k, ar3);	// вызов аккумулятора		
			break;
		case(3) :
			byte_64 = callA_3(inData, &k, ar3);	// вызов аккумулятора
			break;
		case(4) :
			byte_64 = callA_4(inData, &k, ar3);	// вызов аккумулятора
			break;
		}

		if (ar0 % 2 == 0){						// для правильной записи состояний по нужным адреса, необходимо реализовать циклическую адресацию.
			x1 = 64; x2 = 96; y1 = 0;			// для этого введены x1,x2, который изменяются в соответствии с номером прохода(четные - первые 64 адреса,
		}
		else{									// нечетные - с 65 по 128). Далее они используются для корректной ЗАПИСИ изменившихся состояний в необходимые 
			x1 = 0; x2 = 32; y1 = 64;			// адреса. y1 - число для корректного ЧТЕНИЯ состояний.
		}

		y_cnt = 0;								// счетчик для переходов от состояния к состоянию при ЧТЕНИИ
		x_cnt = 0;								// счетчик для переходов от состояния к состоянию при ЗАПИСИ

		// вычисление метрик для пары бит
		for (int ar1 = 0; ar1<4; ar1++){		// цикл №2
			for (int brc = 0; brc<8; brc++){	// цикл №3

				B = byte_64 ^ bias[x_cnt];		// вычисляем расстояние Хэммингa
				T = ar3[B];						// выбираем оценку 

				// реализация "бабочки"
				if (state[y1 + y_cnt] + T > state[y1 + y_cnt + 1] - T) {
					state[x1 + x_cnt] = (state[y1 + y_cnt] + T);
					TRN[z_cnt] = TRN[z_cnt] << 1;	// дополнение метрики
					TRN[z_cnt] = TRN[z_cnt] & 0xFFFF;
				}
				else {
					state[x1 + x_cnt] = (state[y1 + y_cnt + 1] - T);
					TRN[z_cnt] = (TRN[z_cnt] << 1) + 1;// дополнение метрики
					TRN[z_cnt] = TRN[z_cnt] & 0xFFFF;
				}

				if (state[y1 + y_cnt] - T > state[y1 + y_cnt + 1] + T) {
					state[x2 + x_cnt] = state[y1 + y_cnt] - T;
					TRN[z_cnt] = TRN[z_cnt] << 1;	// дополнение метрики
					TRN[z_cnt] = TRN[z_cnt] & 0xFFFF;
				}
				else {
					state[x2 + x_cnt] = (state[y1 + y_cnt + 1] + T);
					TRN[z_cnt] = (TRN[z_cnt] << 1) + 1;// дополнение метрики	
					TRN[z_cnt] = TRN[z_cnt] & 0xFFFF;
				}
				y_cnt += 2;						// инкримент сч. чтение(описание см.выше, при инициализации)
				x_cnt++;						// инкримент сч. записи(описание см.выше, при инициализации)
			}
			z_cnt++;							// инкримент сч. TRN(описание см.выше, при инициализации)
		}
		y_cnt = 0;								// обнуление сч. чтение
		x_cnt = 0;								// обнуление сч. запись
	}

	int A = 0;									// указывает на нулевое состояние

	// обратная проходка								 
	int	ar4 = 0x0,								// переменная для сохранения магических чисел
		byte_66 = 0,							// служебная переменная для определения номера TRN
		ar7 = 983,								// указатель на конец TRN массива
		mask = 0,								// маска состояния(см. ниже)
		TC = 0;									// флаг перехода

	// массив магических чисел для 1/2
	int jump[64] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48,
		50, 52, 54, 56, 58, 60, 62, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35,
		37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63 };

	for (int brc = length - 1; brc>-1; brc--){			// цикл №3

		B = ar4;								// текущее маг. число

		if (B - 0x20 >= 0)						// нахождение декодированного бита
			outDate[brc] = 1;
		else
			outDate[brc] = 0;

		// определние номера TRN в следующей пачке TRN регистров(0-3)	
		B = (A >> 3) & 3;
		byte_66 = B;
		B = B ^ 3;
		ar7 = ar7 - B;

		// определение состояния, из которого был осуществлен переход в текущее состояние
		A = jump[ar4];							// новое магическое число
		T = A & 0xF;							// указатель на состояние из которого был осуществлен переход	
		mask = pow(2.0, (0xF - T));				// маска для определения состояние из которого был осуществлен переход
		TC = (TRN[ar7] & mask) >> (0xF - T);	// определение флага перехода(если 0 - если переход был осуществлен из четного,1 - если из нечетного)

		// корректировка смещения в таблице магических чисел
		if (ar4 - 0x20 >= 0){
			if (TC == 0)
				A = A - 1;
		}
		else{
			if (TC == 1)
				A = A + 1;
		}
		ar7 = ar7 + ~byte_66 + 1;				// перешли к новой четверке TRN регистров
		ar4 = A;								// сохранение тещкуего м.ч. в ar4
		ar7--;
	}
}

// Внутренние функции для выделение последовательноти бит, создание массива оценок (для 3 режимов)
int callA(unsigned int *inData, int *k, int *ar3){

	int A, B;
	A = inData[*k];				// байт № 1
	B = (A >> 6) & 2;					// значение бита из байта №1

	int byte_60 = A & 127;			// оценка первого байта

	A = inData[*k + 1];				// байт №2
	int byte_64 = B + (A >> 7);		// пара полученных бит

	int byte_61 = A & 127;			// оценка байт 2

	ar3[3] = byte_61 + byte_60;		// сумма оценок
	ar3[0] = ~ar3[3] + 1;			// инверсия суммы
	ar3[2] = byte_60 - byte_61;		// разность оценок
	ar3[1] = ~ar3[2] + 1;			// инверсия разностей

	*k += 2;

	return byte_64;
}

int callA_3(unsigned int *inData, int *k, int *ar3){
	int A, B = 0;


	int byte_mark[3];
	A = inData[*k];
	B += (A >> 5) & 12;
	byte_mark[0] = A & 127;
	*k += 1;

	A = inData[*k];
	B += (A >> 6) & 14;
	byte_mark[1] = A & 127;
	*k += 1;

	A = inData[*k];
	B += (A >> 7) & 15;
	byte_mark[2] = A & 127;
	*k += 1;

	int byte_64 = B;		// последовательность полученных бит

	ar3[0] = -byte_mark[0] - byte_mark[1] - byte_mark[2];
	ar3[1] = -byte_mark[0] - byte_mark[1] + byte_mark[2];
	ar3[2] = -byte_mark[0] + byte_mark[1] - byte_mark[2];
	ar3[3] = -byte_mark[0] + byte_mark[1] + byte_mark[2];
	ar3[4] = byte_mark[0] - byte_mark[1] - byte_mark[2];
	ar3[5] = byte_mark[0] - byte_mark[1] + byte_mark[2];
	ar3[6] = byte_mark[0] + byte_mark[1] - byte_mark[2];
	ar3[7] = byte_mark[0] + byte_mark[1] + byte_mark[2];

	return byte_64;
}

int callA_4(unsigned int *inData, int *k, int *ar3){
	int A, B = 0;

	int byte_mark[4];

	A = inData[*k];
	B += (A >> 4) & 8;
	byte_mark[0] = A & 127;
	*k += 1;

	A = inData[*k];
	B += (A >> 5) & 12;
	byte_mark[1] = A & 127;
	*k += 1;

	A = inData[*k];
	B += (A >> 6) & 14;
	byte_mark[2] = A & 127;
	*k += 1;

	A = inData[*k];
	B += (A >> 7) & 15;
	byte_mark[3] = A & 127;
	*k += 1;

	int byte_64 = B;		// последовательность полученных бит

	ar3[15] = byte_mark[0] + byte_mark[1] + byte_mark[2] + byte_mark[3];
	ar3[0] = ~ar3[15] + 1;

	ar3[14] = byte_mark[0] + byte_mark[1] + byte_mark[2] - byte_mark[3];
	ar3[1] = ~ar3[14] + 1;

	ar3[3] = ar3[1] + byte_mark[2] + byte_mark[2];
	ar3[12] = ~ar3[3] + 1;

	ar3[13] = ar3[12] + byte_mark[3] + byte_mark[3];
	ar3[2] = ~ar3[13] + 1;

	ar3[11] = byte_mark[0] - byte_mark[1] + byte_mark[2] + byte_mark[3];
	ar3[4] = ~ar3[11] + 1;

	ar3[10] = byte_mark[0] - byte_mark[1] + byte_mark[2] - byte_mark[3];
	ar3[5] = ~ar3[10] + 1;

	ar3[7] = ar3[5] + byte_mark[2] + byte_mark[2];
	ar3[8] = ~ar3[7] + 1;

	ar3[9] = ar3[8] + byte_mark[3] + byte_mark[3];
	ar3[6] = ~ar3[9] + 1;

	return byte_64;
}