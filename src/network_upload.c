#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "network_upload.h"
#include "list.h"
#include "check_common.h"
#include "cjson_common.h"
#include "struct_to_cjson.h"
#include "http_client.h"

typedef struct {
    char pc_http_url[256];
    void* upload_info_list;
    pthread_mutex_t upload_mutex;
    pthread_t pthread_upload;
}NetworkUploadMng;

static NetworkUploadMng kNetworkUploadMng = {.upload_mutex = PTHREAD_MUTEX_INITIALIZER};

static int NetworkUploadInfoToStr(NetworkUpload* info, char* body, int body_size, char* uri, int uri_size) {
    CHECK_POINTER(info, return -1);

    cJSON* json = NULL;
    switch(info->type) {
    case NETWORK_UPLOAD_ALARM:
        snprintf(uri, uri_size, "/pc_api/alarm_info");
        CHECK_LT(StructToCjsonAlarmInfo(&info->data.alarm_info, &json), 0, return -1);
        break;
    case NETWORK_UPLOAD_PERIPHERAL:
        snprintf(uri, uri_size, "/pc_api/peripheral_info");
        CHECK_LT(StructToCjsonPeripheralInfo(&info->data.peripherail_info, &json), 0, return -1);
        break;
    case NETWORK_UPLOAD_DOWNLOAD:{
        snprintf(uri, uri_size, "/pc_api/other_info");

        json = cJSON_CreateObject();
        CHECK_POINTER(json, return -1);
        CJSON_SET_STRING(json, "type", "download", cJSON_free(json);return -1);
        cJSON* data = cJSON_CreateObject();
        CHECK_POINTER(json, cJSON_free(json);return -1);
        CJSON_SET_STRING(json, "state", info->data.download_state, cJSON_free(json);return -1);
        CHECK_BOOL(cJSON_AddItemToObject(json, "data", data), cJSON_free(data);cJSON_free(json);return -1);
        break;
    }
    case NETWORK_UPLOAD_UPGRADE:{
        snprintf(uri, uri_size, "/pc_api/other_info");

        json = cJSON_CreateObject();
        CHECK_POINTER(json, return -1);
        CJSON_SET_STRING(json, "type", "upgrade", cJSON_free(json);return -1);
        cJSON* data = cJSON_CreateObject();
        CHECK_POINTER(json, cJSON_free(json);return -1);
        CJSON_SET_STRING(json, "state", info->data.upgrade_state, cJSON_free(json);return -1);
        CHECK_BOOL(cJSON_AddItemToObject(json, "data", data), cJSON_free(data);cJSON_free(json);return -1);
        break;

    }
    case NETWORK_UPLOAD_LASER_RANGING:{
        snprintf(uri, uri_size, "/pc_api/other_info");

        json = cJSON_CreateObject();
        CHECK_POINTER(json, return -1);
        CJSON_SET_STRING(json, "type", "laser_ranging", cJSON_free(json);return -1);
        cJSON* data = cJSON_CreateObject();
        CHECK_POINTER(json, cJSON_free(json);return -1);
        CJSON_SET_NUMBER(json, "distance", info->data.distence, cJSON_free(json);return -1);
        CHECK_BOOL(cJSON_AddItemToObject(json, "data", data), cJSON_free(data);cJSON_free(json);return -1);
        break;

    }
    default:
        LOG_ERR("not support upload type, %d", info->type);
        return -1;
    }

    char* buff = cJSON_PrintUnformatted(json);
    CHECK_POINTER(buff, cJSON_free(json);return -1);

    snprintf(body, body_size, "%s", buff);
    free(buff);
    cJSON_free(json);
    return 0;
}

static void *NetworkUploadProc(void *arg) {
    CHECK_POINTER(kNetworkUploadMng.upload_info_list, return NULL);

    while(1) {
        usleep(10*1000);

        pthread_mutex_lock(&kNetworkUploadMng.upload_mutex);
        if (ListSize(kNetworkUploadMng.upload_info_list) <= 0) {
            pthread_mutex_unlock(&kNetworkUploadMng.upload_mutex);
            continue;
        }

        NetworkUpload info;
        if (ListPop(kNetworkUploadMng.upload_info_list, &info, sizeof(NetworkUpload)) < 0) {
            pthread_mutex_unlock(&kNetworkUploadMng.upload_mutex);
            continue;
        }
        pthread_mutex_unlock(&kNetworkUploadMng.upload_mutex);

        if (strlen(kNetworkUploadMng.pc_http_url) <= 0) {
            continue;
        }

        char uri[64] = {0};
        char body[1024*8] = {0};
        if (NetworkUploadInfoToStr(&info, body, sizeof(body), uri, sizeof(uri)) < 0) {
            continue;
        }

        char res[1024*8] = {0};
        char url[512] = {0};
        snprintf(url, sizeof(url), "%s%s", kNetworkUploadMng.pc_http_url, uri);
        if (HttpRequest("POST", url, body, res, sizeof(res), 60*1000) < 0) {
            LOG_ERR("Network upload fail ! url:%s, boby:%s, res:%s", url, body, res);
        }
    }
}

int NetworkUploadInit() {
    kNetworkUploadMng.upload_info_list = ListCreate();
    CHECK_POINTER(kNetworkUploadMng.upload_info_list, return -1);

    pthread_create(&kNetworkUploadMng.pthread_upload, NULL, NetworkUploadProc, NULL);
    return 0;
}

int NetworkUploadUnInit() {
    if (kNetworkUploadMng.pthread_upload) {
        pthread_cancel(kNetworkUploadMng.pthread_upload);
        pthread_join(kNetworkUploadMng.pthread_upload, NULL);
    }

    ListDestory(kNetworkUploadMng.upload_info_list);
    return 0;
}

void NetworkUploadSetPcInfo(char* pc_addr) {
    CHECK_LT(sizeof(kNetworkUploadMng.pc_http_url), strlen(pc_addr), return);
    memcpy(kNetworkUploadMng.pc_http_url, pc_addr, strlen(pc_addr));
}

int NetworkUploadMassage(NetworkUpload* msg) {
    pthread_mutex_lock(&kNetworkUploadMng.upload_mutex);
    ListPush(kNetworkUploadMng.upload_info_list, &msg, sizeof(NetworkUpload));
    pthread_mutex_unlock(&kNetworkUploadMng.upload_mutex);
    return 0;
}