#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "bit_stream.h"

//#define SPI_SIMULATION
#define SPI_FREQ_HZ		8000000
#define SPI_JETTER_PPM	20

#ifdef SPI_SIMULATION
char spi_fifo[1024 * 1024 * 8];
int spi_fifo_w = 1024*1024 + 4;
int spi_fifo_r = 0;
#if 0
if(c){
	if(last_ch)
		printf("▔");
	else
		printf("|▔");
}else{
	if(last_ch)
		printf("|▁");
	else
		printf("▁");
}

last_ch = c;	
fflush(stdout);
#endif

static void fifo_write(char ch){
	spi_fifo[spi_fifo_w%sizeof(spi_fifo)] = ch;
	spi_fifo_w++;
}

static char fifo_read(void){
	char ch = spi_fifo[spi_fifo_r%sizeof(spi_fifo)];
	spi_fifo_r++;
	return ch;
}

char pipe_jetter_rw(char ch){
	char temp[8];
	static jetter_add_point = 0;
	int jetter = 0;
	jetter_add_point++;
	if(jetter_add_point % 5){
		if(ch){
			jetter = GenRand(0, 2) - 1;
		}
	}

	for(int i = 0; i <8 + jetter; i++){
		fifo_write(ch);
	}

	for(int i = 0; i < 8; i++){
		temp[i] = fifo_read();
	}

	BitStreamSquash(temp, 8, &ch);
	return ch;
}

int sim_transfer(const char* tx, char* rx, int len){
	int i;
	//if(tx)
	//DumpHex("SPI", tx, len);

	for(i = 0; i <  len; i++){
		char ch = 0;
		if(tx)
			ch = *tx++;

		ch = pipe_jetter_rw(ch);

		if(rx)
			*rx++ = ch;
	}

	return i;
}
#endif

int spi_transfer(const char* tx,  char* rx, int len){
#ifdef SPI_SIMULATION
	return sim_transfer(tx, rx, len);
#else
	return 0;
#endif
}
