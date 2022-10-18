#ifndef __BIT_STREAM_H__
#define __BIT_STREAM_H__

int BitStreamSpread(const char* in, int bitLen, char* out);
int BitStreamSquash(const char* in, int byteLen, char* out);
#endif
