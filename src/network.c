#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "network.h"
#include "http_server.h"
#include "http_client.h"
#include "cjson_common.h"
#include "check_common.h"
#include "struct_to_cjson.h"
#include "cjson_to_struct.h"

typedef struct {
    char method[16];
    char url[256];
    HttpServerUrlProcCb cb;
}HttpServerUrlInfo;

typedef struct {
    Areas areas;
    AlgorithmEnable algorithm_enable;
    PtzCtrl ptz_ctrl;
    ChipCtrl chip_ctrl;
    OtherCtrl other_ctrl;
    PeripheralInfo peripheral_info;
    AlarmInfo alarm_info;

    pthread_mutex_t mutex;
    char pc_http_url[256];
    NetworkOperFunc func;
}NetworkMng;
static NetworkMng kNetworkMng = {.mutex = PTHREAD_MUTEX_INITIALIZER};

typedef int (*StructToCjsonCb)(void*, cJSON**);
typedef int (*CjsonToStructCb)(cJSON*, void*);

typedef struct 
{
    char* sub_key;
    void* st;
    int st_size;
    StructToCjsonCb s2j_cb;
    CjsonToStructCb j2s_cb;
}NetworkSubInfo;

static NetworkSubInfo kNetworkSubInfo[] = {
    {.sub_key = "areas", .st = &kNetworkMng.areas, .st_size = sizeof(kNetworkMng.areas), .s2j_cb = StructToCjsonAreas, .j2s_cb = CjsonToStructAreas},
    {.sub_key = "algorithm_enable", .st = &kNetworkMng.algorithm_enable, .st_size = sizeof(kNetworkMng.algorithm_enable), .s2j_cb = StructToCjsonAlgorithmEnable, .j2s_cb = CjsonToStructAlgorithmEnable},
    {.sub_key = "ptz_ctrl", .st = &kNetworkMng.ptz_ctrl, .st_size = sizeof(kNetworkMng.ptz_ctrl), .s2j_cb = StructToCjsonPtzCtrl, .j2s_cb = CjsonToStructPtzCtrl},
    {.sub_key = "chip_ctrl", .st = &kNetworkMng.chip_ctrl, .st_size = sizeof(kNetworkMng.chip_ctrl), .s2j_cb = StructToCjsonChipCtrl, .j2s_cb = CjsonToStructChipCtrl},
    {.sub_key = "other_ctrl", .st = &kNetworkMng.other_ctrl, .st_size = sizeof(kNetworkMng.other_ctrl), .s2j_cb = StructToCjsonOtherCtrl, .j2s_cb = CjsonToStructOtherCtrl},
    {.sub_key = "peripheral_info", .st = &kNetworkMng.peripheral_info, .st_size = sizeof(kNetworkMng.peripheral_info), .s2j_cb = StructToCjsonPeripheralInfo, .j2s_cb = CjsonToStructPeripheralInfo},
    {.sub_key = "alarm_info", .st = &kNetworkMng.alarm_info, .st_size = sizeof(kNetworkMng.alarm_info), .s2j_cb = StructToCjsonAlarmInfo, .j2s_cb = CjsonToStructAlarmInfo},
};

static int NetworkSetConfig(char* in, char* out, int out_size, char* res, int res_size) {
    CHECK_POINTER(in, 500);
    CHECK_POINTER(out, 500);
    snprintf(res, res_size, "request json error");

    cJSON* json = cJSON_Parse(in);
    CHECK_POINTER(json, 400);

    char conf_name[64] = {0};
    CJSON_GET_STRING(json, "conf_name", conf_name, sizeof(conf_name), end);

    cJSON* conf_json = cJSON_GetObjectItemCaseSensitive(json, conf_name);
    CHECK_POINTER_GO(conf_json, end);
    
    int i = 0;
    pthread_mutex_lock(&kNetworkMng.mutex);
    for (; i < sizeof(kNetworkSubInfo) / sizeof(NetworkSubInfo); i++) {
        if (strcmp(conf_name, kNetworkSubInfo[i].sub_key) == 0) {
            memset(kNetworkSubInfo[i].st, 0, kNetworkSubInfo[i].st_size);
            int ret = kNetworkSubInfo[i].j2s_cb(conf_json, kNetworkSubInfo[i].st);
            CHECK_LT_GO(ret, 0, end_unlock);

            ret = kNetworkMng.func.set_config_cb(conf_name, kNetworkSubInfo[i].st, NULL, res, res_size);
            CHECK_LT_GO(ret, 0, end_unlock);

            break;
        }
    }
    pthread_mutex_unlock(&kNetworkMng.mutex);
    
    if (i >= sizeof(kNetworkSubInfo) / sizeof(NetworkSubInfo)) {
        snprintf(res, res_size, "ctrl request not support");
        LOG_ERR("ctrl request not support\n");
        goto end;
    }

    snprintf(res, res_size, "success");
    cJSON_free(json);
    return 200;
end_unlock:
    pthread_mutex_unlock(&kNetworkMng.mutex);
end:
    cJSON_free(json);
    return 400;
}

static int NetworkGetConfig(char* in, char* out, int out_size, char* res, int res_size) {
    CHECK_POINTER(in, 500);
    CHECK_POINTER(out, 500);

    snprintf(res, res_size, "request json error");

    cJSON* json = cJSON_Parse(in);
    CHECK_POINTER(json, 400);

    char conf_name[64] = {0};
    CJSON_GET_STRING(json, "conf_name", conf_name, sizeof(conf_name), end);
    
    snprintf(res, res_size, "respect json error");

    cJSON* conf_json = NULL;
    int i = 0;
    pthread_mutex_lock(&kNetworkMng.mutex);
    for (; i < sizeof(kNetworkSubInfo) / sizeof(NetworkSubInfo); i++) {
        if (strcmp(conf_name, kNetworkSubInfo[i].sub_key) == 0) {
            memset(kNetworkSubInfo[i].st, 0, kNetworkSubInfo[i].st_size);

            int ret = kNetworkMng.func.get_config_cb(conf_name, "unused", kNetworkSubInfo[i].st, res, res_size);
            CHECK_LT_GO(ret, 0, end_unlock);

            ret = kNetworkSubInfo[i].s2j_cb(kNetworkSubInfo[i].st, &conf_json);
            CHECK_LT_GO(ret, 0, end_unlock);

            break;
        }
    }
    pthread_mutex_unlock(&kNetworkMng.mutex);
    
    if (i >= sizeof(kNetworkSubInfo) / sizeof(NetworkSubInfo)) {
        snprintf(res, res_size, "ctrl request not support");
        LOG_ERR("ctrl request not support\n");
        goto end;
    }

    if(!cJSON_AddItemToObject(json, conf_name, conf_json)) {
        cJSON_free(conf_json);
        goto end;
    }

    char* buff = cJSON_PrintUnformatted(json);
    CHECK_POINTER_GO(buff, end);

    snprintf(out, out_size, "%s", buff);
    snprintf(res, res_size, "success");

    free(buff);
    cJSON_free(json);
    return 200;
end_unlock:
    pthread_mutex_unlock(&kNetworkMng.mutex);
end:
    cJSON_free(json);
    return 400;
}

static int NetworkUpgrade(char* in, char* out, int out_size, char* res, int res_size) {
    CHECK_POINTER(in, 500);
    CHECK_POINTER(out, 500);

    snprintf(res, res_size, "request json error");

    cJSON* json = cJSON_Parse(in);
    CHECK_POINTER(json, 400);

    char url[512] = {0};
    CJSON_GET_STRING(json, "upgrade_url", url, sizeof(url), end);

    int ret = kNetworkMng.func.upgrade_cb("string", url, NULL, res, res_size);
    CHECK_LT_GO(ret, 0, end);

    snprintf(res, res_size, "success");
    cJSON_free(json);
    return 200;
end:

    cJSON_free(json);
    return 400;
}

static int NetworkHttpServerInfo(char* in, char* out, int out_size, char* res, int res_size) {
    CHECK_POINTER(in, 500);
    CHECK_POINTER(out, 500);

    snprintf(res, res_size, "request json error");

    cJSON* json = cJSON_Parse(in);
    CHECK_POINTER(json, 400);

    CJSON_GET_STRING(json, "http_url", kNetworkMng.pc_http_url, sizeof(kNetworkMng.pc_http_url), end);

    snprintf(res, res_size, "success");
    cJSON_free(json);
    return 200;
end:

    cJSON_free(json);
    return 400;
}

static HttpServerUrlInfo kUrlInfo[] ={
    {.method = "POST", .url = "/dev_api/set_config", .cb = NetworkSetConfig},
    {.method = "POST", .url = "/dev_api/get_config", .cb = NetworkGetConfig},
    {.method = "POST", .url = "/dev_api/upgrade", .cb = NetworkUpgrade},
    {.method = "POST", .url = "/dev_api/http_svr_info", .cb = NetworkHttpServerInfo},
};

int NetworkInit(NetworkOperFunc* func) {
    memcpy(&kNetworkMng.func, func, sizeof(NetworkOperFunc));

    HttpServerInit(":8080");
    for(int i = 0; i < sizeof(kUrlInfo) / sizeof(HttpServerUrlInfo); i++) {
        HttpServerUrlRegister(kUrlInfo[i].method, kUrlInfo[i].url, kUrlInfo[i].cb);
        LOG_INFO("server listen url, method:%s, url:%s\n", kUrlInfo[i].method, kUrlInfo[i].url);
    }

    return 0;
}

void NetworkUnInit() {
    HttpServerUnInit();
}

int NetworkRequest(const char* url, const char* req_key, void* body, char* res, int res_size, int timeout) {
    int ret = 0;
    char fall_url[512] = {0};
    char* json_str = NULL;
    if (url == NULL) {
        snprintf(fall_url, sizeof(fall_url), "%s/pc_api/%s", kNetworkMng.pc_http_url, req_key);

        int i = 0;
        cJSON* json = NULL;
        for (; i < sizeof(kNetworkSubInfo) / sizeof(NetworkSubInfo); i++) {
            if (strcmp(req_key, kNetworkSubInfo[i].sub_key) == 0) {
                ret = kNetworkSubInfo[i].s2j_cb(body, &json);
                CHECK_LT(ret, 0, -1);

                break;
            }
        }
        CHECK_GE(i, sizeof(kNetworkSubInfo) / sizeof(NetworkSubInfo), -1);

        json_str = cJSON_PrintUnformatted(json);
        if (json_str == NULL) {
            cJSON_free(json);
            return -1;
        }
    } else {
        snprintf(fall_url, sizeof(fall_url), "%s", url);
        json_str = (char*)body;
    }

    char method[16] = {0};
    snprintf(method, sizeof(method), "%s", body == NULL ? "GET" : "POST");
    ret = HttpClientRequest(method, fall_url, json_str, res, res_size, timeout);

    if (url == NULL && json_str != NULL) {
        free(json_str);
    }

    return ret;
}
