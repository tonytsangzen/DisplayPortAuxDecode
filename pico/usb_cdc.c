#include <stdint.h>
#include <stdio.h>
static char fifo[512] = {'C', 'D', 'C'};
static int fifo_write = 1;
static int fifo_read = 0;

int _putchar (int ch){
	fifo[fifo_write%sizeof(fifo)] = ch;
	fifo_write++;
	return ch;
}

void usb_cdc_process(uint8_t itf)
{
	int len  = fifo_write - fifo_read;
	int n =  tud_cdc_n_write_available(0);

	//tud_cdc_n_write(0, "cdc\r\n", 5);

	if(len > n)
		len = n;

	if(len > 0){
		tud_cdc_n_write(0, &fifo[fifo_read%sizeof(fifo)], len);
		//fifo_read += len;
		if(fifo_read > sizeof(fifo) && fifo_write > sizeof(fifo)){
			fifo_read -= sizeof(fifo);
			fifo_write -= sizeof(fifo);
		}
	}
}

void usb_cdc_loop(void){
	tud_task();	
	if(tud_cdc_n_connected(0))
		usb_cdc_process(0);
}
