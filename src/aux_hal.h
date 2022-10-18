#ifndef __AUX_HAL_H__
#define __AUX_HAL_H__

int aux_send(const char* msg, int len);
int aux_recv(char* msg, int len);
#endif
