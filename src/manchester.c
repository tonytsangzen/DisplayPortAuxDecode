#include <stdint.h>
#include <stdio.h>

#if 0
int ManchesterEncode(char* in, int inBitLen,  char* out, int* pOutBitLen){

	uint8_t tmp = 0;
	int  outBitLen = 0; 
	int  outByteLen = 0;

	int i;
	for(i = 0; i < inBitLen; i++){
	
		uint8_t mask = 0x80 >> (i%8);

		if(in[i/8]&mask){
			tmp |= 0x2;	
		}else{
			tmp |= 0x1;
		}

		outBitLen+=2;

		if(outBitLen % 8 == 0){
			out[outByteLen] = tmp;
			outByteLen++;
			tmp = 0;
		}else{
			tmp <<= 2;
		}
	}

	if(i % 4)
		out[outByteLen] = tmp << (6 - (i%4)*2);

	if(pOutBitLen)
		*pOutBitLen = outBitLen;
	return outBitLen;
}


int ManchesterDecode(char* in, int inBitLen, char* out, int* pOutBitLen){
	int preBit;

	uint8_t  tmp = 0;
	int  outBitLen = 0;
	int	 outByteLen = 0;

	int i;
	for(i = 0; i < inBitLen; i+=2){
		uint8_t mask = 0xc >> (i%8);
		int bit = (in[i/8] >> (6 - i%8))&0x3;

		switch(bit){
			case 0:
			case 3:
				printf("ilg code\n");
				break;
			case 2:
				tmp |= 0x1;
			case 1:
				outBitLen++;
				if(outBitLen % 8 == 0){
					out[outByteLen] = tmp;
					outByteLen++;;
					tmp = 0;
				}else{
					tmp <<= 1;
				};
			break;
		}
	}

	if((i/2) % 8)
		out[outByteLen] = tmp << (7 - (i/2)%8);

	if(pOutBitLen)
		*pOutBitLen = outBitLen;

	return outBitLen;
}
#endif

int ManchesterEncode(const char* in, int inBitLen,  char* out){

	int i = 0;
	for( ;i < inBitLen; i++){
		if(in[i]){
			*out++ = 0xff;
			*out++ = 0x0;
		}else{
			*out++ = 0x0;
			*out++ = 0xff;
		}
	}

	return i*2;
}


int ManchesterDecode(const char* in, int inBitLen, char* out){
	int i = 0;

	uint16_t *pIn = (uint16_t*) in;

	for( ;i < inBitLen/2; i++){
		if(pIn[i] == 0xFF){
			*out++ = 0xff;
		}else if(pIn[i] == 0xFF00){
			*out++ = 0x0;
		}else{
			printf("miss match!\n");
			break;
		}
	}

	return i;
}	
