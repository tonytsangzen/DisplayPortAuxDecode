#ifndef __UTILS_H__
#define __UTILS_H__

void DumpHex(const char* lable, char* buf,  int len); 
void DumpBit(const char* lable, char* buf,  int len); 
int CompareBit(const char* a, const char*b, int le);
int CompareHex(const char* a, const char* b, int len);
int GenRand(int min, int max);
#endif
