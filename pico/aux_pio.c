#include "aux.pio.h"
#include "pico/stdlib.h"

void pio_aux_set_level(PIO pio, uint sm, uint32_t level) {
    pio_sm_put_blocking(pio, sm, level);
}

void aux_tx(PIO pio, int sm, uint8_t *data, int len){
	static uint32_t en[16];

	memset(en, 0xFF, sizeof(en));
	en[0] = 0xAAAAAAAA;
	en[1] = 0x0FAAAAAA;
	int i = 0;
	for(; i <  len * 8; i++){
		int in_mask = 0x80 >> i%8;
		int out_mask = 0x3 << (i%16)*2;
		int out_0 = 0x2 << (i%16)*2;
		int out_1 = 0x1 << (i%16)*2;

		en[2 + i/16] &= ~out_mask;

		if(data[i/8]&in_mask){
			en[2 + i/16] |= out_1; 
		}else{
			en[2 + i/16] |= out_0;
		}
	}

	for(; i < len*8 + 2; i++){
		en[2 + i/16] |= 0x3 << (i%16)*2;
	}

	for(; i < len * 8 + 4; i++){
		en[2 + i/16] &= ~(0x3 << (i%16)*2);
	}

	for(i = 0; i < (len + 1) / 2 + 4; i++)
		pio_sm_put_blocking(pio, sm, en[i]); //16 bit pre charge  pattern

}

int aux_rx(PIO pio, int sm,  uint8_t* data){
	int fifoLen = pio_sm_get_rx_fifo_level(pio, sm); 	

	uint32_t rx[8];

	for(int i = 0; i < fifoLen; ){
		rx[i] = ~pio_sm_get_blocking(pio, sm);
		if(i == 0 && rx[0] == 0xFFFFFFFF){
			fifoLen--;
			continue;
		}
		i++;
	}

	int byteLen = fifoLen * 4;
	while((rx[fifoLen - 1] >> 24) == 0xFF){
		rx[fifoLen - 1] <<= 8;
		byteLen--;
	}

	uint8_t *p = rx;
	for(int i = 0; i < byteLen; i++){
		data[i] = p[i/4*4 + 3 - i%4];	
	}
	return byteLen;
}

void PioAuxProgramInit(void){

    PIO pio = pio0;
    int sm_tx = 0;
    int sm_rx = 1;

    uint offset_tx = pio_add_program(pio, &aux_tx_program);
    uint offset_rx = pio_add_program(pio, &aux_rx_program);

    printf("Loaded tx program at %d\n", offset_tx);
    printf("Loaded rx program at %d\n", offset_rx);

    aux_tx_program_init(pio, sm_tx, offset_tx, 25);
    aux_rx_program_init(pio, sm_rx, offset_rx, 24);

	uint8_t txBuf[23];
	uint8_t rxBuf[23];
	while(1){
	
		int len = GenRand(2, 23);
		for(int i = 0; i <  len; i++)
			txBuf[i] = GenRand(0, 255);

		DumpHex("TX", txBuf, len);
		aux_tx(pio, sm_tx, txBuf, len);
		sleep_ms(1);
		len = aux_rx(pio, sm_rx, rxBuf);
		DumpHex("RX", rxBuf, len);

		if(CompareHex(txBuf, rxBuf, len)){
			printf("error!\n");
			while(1);
		}
		sleep_ms(1);
	}
}
