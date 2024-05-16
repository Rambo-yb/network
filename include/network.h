#ifndef __NETWORK_H__
#define __NETWORK_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*NetworkOperCb)(char* oper_type, void* in, void* out, char* res, int res_size);

int NetworkInit(NetworkOperCb cb);

void NetworkUnInit();

int NetworkRequest(const char* url, char* req_type, void* body, char* res, int res_size);

#ifdef __cplusplus
};
#endif

#endif