#include <stdio.h>
#include "manchester.h"
#include "utils.h"
#include "bit_stream.h"
#include "aux_hal.h"

#include "pico/stdlib.h"
#include "hardware/clocks.h"

int main(int argc, char* argv[]){
	set_sys_clock_khz(128000, true);

	stdio_init_all();
	sleep_ms(2000);
	printf("clock:%d\n", clock_get_hz (clk_sys));

	PioAuxProgramInit();



	do{
	#if 0
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
		#ifdef PICO_BUILD
		#endif
	#endif

	}while(1);
}
