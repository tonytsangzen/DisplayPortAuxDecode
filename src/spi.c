#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "bit_stream.h"

#define SPI_SIMULATION
#define SPI_FREQ_HZ		8000000
#define SPI_JETTER_PPM	20

#ifdef SPI_SIMULATION
char spi_pipe[1024 * 8 + 3];

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


char pipe_rw(char ch){
	char c = spi_pipe[sizeof(spi_pipe) - 1];
	for(int i = sizeof(spi_pipe) - 1 ; i > 0; i--)
		spi_pipe[i] = spi_pipe[i - 1];
	spi_pipe[0] = ch;
	return c;
}

char pipe_jetter_rw(char ch){
	int jetter = GenRand(0, 2) - 1;
	char temp[8];
	static char pre = 0;

	memset(temp, ch, sizeof(temp));
	switch(jetter){
		case -1:
			temp[0] = pre;
			break;
		case +1:
			pipe_rw(ch);
			break;
		default:
			break;
	}

	pre = ch;
	for(int i = 0; i <  8; i++){
		temp[i] = pipe_rw(temp[i]);
	}
	BitStreamSquash(temp, 8, &ch);
	return ch;
}

int sim_transfer(const char* tx, char* rx, int len){
	int i;
	char ch = 0;

	for(i = 0; i <  len; i++){
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
