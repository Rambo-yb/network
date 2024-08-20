#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "network_download.h"
#include "network_common.h"
#include "network_upload.h"
#include "check_common.h"
#include "list.h"

#define NETWORK_DOWNLOAD_FILE_PATH "/data/download"

typedef int (*NetworkOperationUpgradeCb)(const char* );

typedef struct {
    void* download_list;
    pthread_mutex_t mutex;
    pthread_t pthread_download;
    NetworkOperationUpgradeCb upgrade_cb;
}NetworkDownloadMng;
static NetworkDownloadMng kNetworkDownloadMng = {.mutex = PTHREAD_MUTEX_INITIALIZER};

static int NetworkDownloadGetMd5(char* file_path, char* md5, int size) {
    char cmd[256] = {0};
    snprintf(cmd, sizeof(cmd), "md5sum %s", file_path);

    FILE* fp = popen(cmd, "r");
    CHECK_POINTER(fp, return -1);

    char result[64] = {0};
    if (fgets(result, sizeof(result), fp) != NULL) {
        sscanf(result, "%32s", md5);
    }

    pclose(fp);
    return 0;
}

static int NetworkDownloadGetFileLength(char * file_path) {
    FILE* fp = fopen(file_path, "rb");
    CHECK_POINTER(fp, return -1);

    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    fclose(fp);
    return file_size;
}

static int NetworkDownloadCheck(NetworkDownloadInfo info, char* path, int size) {
    char filename[256] = {0};
    if (strrchr(info.url, '/') != NULL) {
        snprintf(filename, sizeof(filename), "%s", strrchr(info.url, '/') + 1);
    } else {
        snprintf(filename, sizeof(filename), "%s", info.url);
    }

    snprintf(path, size, "%s/%s", NETWORK_DOWNLOAD_FILE_PATH, filename);
    CHECK_EQ(access(path, F_OK), 0, return -1);
    
    CHECK_EQ(NetworkDownloadGetFileLength(path), info.size, return -1);
    
    char md5[64] = {0};
    NetworkDownloadGetMd5(path, md5, sizeof(md5));
    LOG_INFO("loc md5:%s, pc md5:%s", md5, info.md5);
    CHECK_EQ(strcmp(md5, info.md5), 0, return -1);

    return 0;
}

static void NetworkDownloadUpload(char* state) {
    NetworkUpload upload;
    memset(&upload, 0, sizeof(NetworkUpload));
    upload.type = NETWORK_UPLOAD_DOWNLOAD;
    snprintf(upload.data.download_state, sizeof(upload.data.download_state), "%s", state);
    NetworkUploadMassage(&upload);
}

static void* NetworkDownloadProc(void* arg) {
    CHECK_POINTER(kNetworkDownloadMng.download_list, return NULL);
    
    while (1) {
        sleep(1);

        pthread_mutex_lock(&kNetworkDownloadMng.mutex);
        if (ListSize(kNetworkDownloadMng.download_list) <= 0) {
            pthread_mutex_unlock(&kNetworkDownloadMng.mutex);
            continue;
        }

        NetworkDownloadInfo info;
        if (ListPop(kNetworkDownloadMng.download_list, &info, sizeof(NetworkDownloadInfo)) < 0) {
            pthread_mutex_unlock(&kNetworkDownloadMng.mutex);
            continue;
        }
        pthread_mutex_unlock(&kNetworkDownloadMng.mutex);

        char cmd[1024*2] = {0};
        snprintf(cmd, sizeof(cmd), "mkdir -p %s;cd %s;wget -t 3 -c -T 600 -w 60 %s", NETWORK_DOWNLOAD_FILE_PATH, NETWORK_DOWNLOAD_FILE_PATH, info.url);
        LOG_INFO("download cmd:%s", cmd);
        NetworkDownloadUpload(NETWORK_UPLOAD_DOWNLOADING);

        system(cmd);
        LOG_INFO("download end");

        char path[256] = {0};
        int ret = NetworkDownloadCheck(info, path, sizeof(path));
        LOG_INFO("check download %s", (ret == 0 ? "success" : "fail"));
        NetworkDownloadUpload((ret == 0 ? NETWORK_UPLOAD_DOWNLOAD_SUCCESS : NETWORK_UPLOAD_DOWNLOAD_FAIL));

        if (ret == 0 && strcmp(info.type, "upgrade") == 0 && kNetworkDownloadMng.upgrade_cb) {
            kNetworkDownloadMng.upgrade_cb(path);
        }
    }
}

int NetworkDownloadInit() {
    kNetworkDownloadMng.download_list = ListCreate();
    CHECK_POINTER(kNetworkDownloadMng.download_list, return -1);

    pthread_create(&kNetworkDownloadMng.pthread_download, NULL, NetworkDownloadProc, NULL);
    return 0;
}

int NetworkDownloadUnInit() {
    if (kNetworkDownloadMng.pthread_download) {
        pthread_cancel(kNetworkDownloadMng.pthread_download);
        pthread_join(kNetworkDownloadMng.pthread_download, NULL);
    }

    ListDestory(kNetworkDownloadMng.download_list);
    return 0;

}

void NetworkDownloadFile(NetworkDownloadInfo* info) {
    pthread_mutex_lock(&kNetworkDownloadMng.mutex);
    ListPush(kNetworkDownloadMng.download_list, &info, sizeof(NetworkDownloadInfo));
    pthread_mutex_unlock(&kNetworkDownloadMng.mutex);
}

int NetworkDownloadUpgradeCb(void* cb) {
    kNetworkDownloadMng.upgrade_cb = (NetworkOperationUpgradeCb)cb;
}