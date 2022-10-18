#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "spi.h"
#include "utils.h"
#include "manchester.h"
#include "bit_stream.h"

static const char SYNC_HEAD[2] = {0};
static const char SYNC_END[8] = {0xfF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};

int aux_send(const char* msg, int len){

	int l = 0;
	char encode[320] = {0};
	char raw[160] = {0};

	//start transfer
	l = BitStreamSpread(SYNC_HEAD, sizeof(SYNC_HEAD) * 8, raw);
	//16 precharge plus  
	l = ManchesterEncode(raw, l, encode);
	//sync head
	l = ManchesterEncode(raw, l, encode + l);
	//sync end
	memcpy(encode + l, SYNC_END, sizeof(SYNC_END));
	l += sizeof(SYNC_END);

	spi_transfer(encode, 0, l);

	//data 
	l =  BitStreamSpread(msg, len * 8, raw);
	l = ManchesterEncode(raw, l, encode);

	//data end
	memcpy(encode + l, SYNC_END, sizeof(SYNC_END));
	l += sizeof(SYNC_END);

	spi_transfer(encode, 0, l);

	return len;
}

int BitStreamAlign(char* in, int len, char* out){
	static char init = 0;
	static int clock = 0;
	int l = 0;

	for(int i = 0; i < len; i++){
		char ch = in[i];
		clock++;
		/*clock recovery*/
		if(ch != init){
			clock = 1;
			init = ch;
		}
		if((clock + 4 ) % 8 == 0){
			out[l] = ch;
			l++;
		}
	}
	return l;
}

int aux_recv(char* msg, int len){

	int state = 0;

	char chs[8];
	char temp[sizeof(chs) * 8];
	char align[sizeof(chs) * 8];

	int percharge = 0;
	int	 recvLen = 0;
	char recv[160] = {0};

	char init = 0;

	while(1){
		spi_transfer(0, chs, sizeof(chs));	
		int len = BitStreamSpread(chs, sizeof(chs) * 8, temp);
		len = BitStreamAlign(temp,  len, align);

		for(int i = 0; i < len; i++){
		char ch = align[i];
		switch(state){
			case 0:
				percharge = 0;
				if(!ch)
					state = 1;
				break;
			case 1:
				if(ch)
					state = 2;
				else
					state = 0;
				break;
			case 2:
				if(!ch){
					state = 1;
					percharge++;	
				}else{
					if(percharge >= 16)
						state = 3;
					else
						state = 1;
				}
				break;
			//sync
			case 3:
				if(ch)
					state = 4;
				else
					state = 0;
				break;
			case 4:
				if(ch)
					state = 5;
				else
					state = 0;
				break;
			case 5:
				if(ch)
					state = 6;
				else
					state = 0;
				break;
			case 6:
				if(ch)
					state = 0;
				else
					state = 7;
				break;
			case 7:
				if(ch)
					state = 0;
				else
					state = 8;
				break;
			case 8:
				if(ch)
					state = 0;
				else
					state = 9;
				break;
			case 9:
				if(ch)
					state = 0;
				else{
					state = 10;
				}
				break;
			//sync end
			case 10:
				if(ch)
					state = 11;
				else
					state = 12;
				break;
			case 11:
				if(!ch){
					recv[recvLen++] = 0xFF; 
					state = 10;
				}else{
					state = 13;
				}
				break;
			case 12:
				if(ch){
					recv[recvLen++] = 0x0; 
					state = 10;
				}else{
					state = 0;
				}
				break;
			//stop
			case 13:
				if(ch)
					state = 14;
				else
					state = 0;
				break;
			case 14:
				if(ch)
					state = 15;
				else
					state = 0;
				break;
			case 15:
				if(ch)
					state = 0;
				else
					state = 16;
				break;
			case 16:
				if(ch)
					state = 0;
				else
					state = 17;
				break;
			case 17:
				if(ch)
					state = 0;
				else
					state = 18;
				break;
			case 18:
				if(ch)
					state = 0;
				else{
					return BitStreamSquash(recv, recvLen, msg);
				}
				break;
			default:
				state = 0;
				break;
		}
		}
	}
}
