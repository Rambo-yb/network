#ifndef __NETWORK_DOWNLOAD_H__
#define __NETWORK_DOWNLOAD_H__

int NetworkDownloadInit();

int NetworkDownloadUnInit();

typedef struct {
    char type[16];
    char url[256];
    int size;
    char md5[64];
}NetworkDownloadInfo;

void NetworkDownloadFile(NetworkDownloadInfo* info);

int NetworkDownloadUpgradeCb(void* cb);

#endif