#include <stdio.h>
#include <stdint.h>

int BitStreamSpread(const char* in, int bitLen, char* out){

	int i = 0;
	for(; i < bitLen; i++){
		uint8_t mask = 0x80 >> (i%8) ;	

		if(in[i/8] & mask){
			out[i] = 0xFF;
		}else{
			out[i] = 0x0;
		}
	}

	return i;
}

int BitStreamSquash(const char* in, int byteLen, char* out){

	int len = 0;
	for(;len < (byteLen + 7)/8; len++){
		out[len] = 0;
	}	

	for(int i = 0;i < byteLen; i++){
		uint8_t mask = 0x80 >> (i%8) ;	
		if(in[i] != 0){
			out[i/8] |= mask;
		}
	}
	
	return len;
}

