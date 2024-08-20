#ifndef __NETWORK_UPLOAD_H__
#define __NETWORK_UPLOAD_H__

#include "network_common.h"

int NetworkUploadInit();

int NetworkUploadUnInit();

void NetworkUploadSetPcInfo(char* pc_addr);

int NetworkUploadMassage(NetworkUpload* msg);

#endif