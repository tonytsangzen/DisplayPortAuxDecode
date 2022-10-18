#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void DumpHex(const char* lable, char* buf, int len){
	printf("%-8s:", lable);
	for(int i = 0; i < len; i++){
		printf("%02x ", (unsigned char)buf[i]);

		if(i && (i%64 == 0))
			printf("\n%-8s:", "");
	}
	printf("\n");
}

void DumpBit(const char* lable, char* buf, int len){
	printf("%-8s:", lable);
	for(int i = 0; i < len;){
		if(buf[i/8]&(0x80>>(i%8)))
			printf("1");
		else
			printf("0");

		i++;

		if(i%8 == 0)
			printf(" ");

		if(i && !(i % 64))
			printf("\n%-8s:", "");
	}
	printf("\n");

}

int CompareBit(const char* a, const char* b, int len){

	int bytes = len / 8;

	int i;
	for(i = 0; i < bytes; i++){
		if(a[i] != b[i])
			return a[i] - b[i] ;
	}
	if(len % 8){
		uint8_t mask = ~((0x1 << (8-len%8)) - 1);
		return (a[i] & mask) - (b[i] & mask);
	}else{
		return 0;
	}
}

int CompareHex(const char* a, const char* b, int len){

	int bytes = len / 8;

	int i;
	for(i = 0; i < len; i++){
		if(a[i] != b[i])
			return a[i] - b[i] ;
	}
	return 0;
}

int GenRand(int min, int max){
	
	static long seed = 0;
	if(!seed){
		struct timeval ts;
		gettimeofday(&ts, NULL);
		seed = ts.tv_usec;
		srand(seed);
	}

	float r = rand();

	return  min + r*(max-min)/RAND_MAX;
}
