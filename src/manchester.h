#ifndef __MANCHESTER_H__
#define __MANCHESTER_H__

int ManchesterEncode(const char* in, int inBitLen,  char* out);
int ManchesterDecode(const char* in, int inBitLen, char* out);

#endif 
