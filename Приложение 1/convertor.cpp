#include "convertor.h"

void convert(char * IDA_file, char * CCS_file)
{
	int bit = 0x0;
	FILE *IDA = fopen(IDA_file, "rb");
	FILE *CCS = fopen(CCS_file, "wb");
	
	// header
	fprintf(CCS, "1651 1 0 0 ffff\n");

	// body
	do{
		fscanf(IDA, "%x", &bit);
		fprintf(CCS, "0x%x\n", bit);
	} while (!feof(IDA));

	fclose(CCS);
	fclose(IDA);
}

int main(){
	printf("Converting starting...\n");
	convert("IDA.txt", "CCS.dat");
	printf("Proccess was finished!\n");
	getchar();
	return 0;
}