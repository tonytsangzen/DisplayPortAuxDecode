#include <stdio.h>
#include "manchester.h"
#include "utils.h"
#include "bit_stream.h"
#include "aux_hal.h"

#ifndef PICO_BUILD 

int main(int argc, char* argv[]){
	/*aux msg*/
#if 0
	char plat[20] = {0};
	char raw[160];
	char en[320] = {0};
	char de[160] = {0};
	char squash[20] = {0};
	do{

		int rawLen = sizeof(raw);
		int squashLen = sizeof(raw);
		int enLen = sizeof(en);
		int deLen = sizeof(de);

		int len = GenRand(1, 64);
		for(int i = 0; i < (len+7)/8; i++)
			plat[i] = GenRand(0,255);

		
		DumpBit("plat", plat, len);
		rawLen = BitStreamSpread(plat, len, raw);
		DumpHex("raw", raw, rawLen);

		enLen = ManchesterEncode(raw, rawLen, en);
		DumpHex("Encode", en, enLen);
		deLen = ManchesterDecode(en, enLen, de);
		DumpHex("Decode", de, deLen);

		squashLen = BitStreamSquash(de, deLen, squash);
		DumpBit("squash", squash, squashLen);
		
		if(CompareBit(squash, plat, len) != 0){
			printf("error!\n");
			break;
		}
		printf("\n");
	}while(1);
#endif

	int cnt = 0;


	do{
		char tx[20] = {0};
		char rx[20] = {0};
		int txLen = GenRand(1, 20);

		for(int i = 0; i < txLen; i++)
			tx[i] = GenRand(0,255);


		DumpHex("send", tx, txLen);
		aux_send(tx, txLen);
		int rxLen = aux_recv(rx, 20);
		DumpHex("recv", rx, rxLen);
		if(CompareHex(tx, rx, txLen) != 0){
			printf("error!\n");
			break;
		}
		printf("test loop %d\n", cnt++);	

	}while(1);
}
#endif
