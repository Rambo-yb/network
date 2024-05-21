#include <string.h>
#include <stdlib.h>
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
    char pc_http_url[256];
    NetworkOperFunc func;
}NetworkMng;
static NetworkMng kNetworkMng;

static int NetworkSetConfig(char* in, char* out, int out_size, char* res, int res_size) {
    CHECK_POINTER(in, 500);
    CHECK_POINTER(out, 500);

    snprintf(res, res_size, "request json error");

    cJSON* json = cJSON_Parse(in);
    CHECK_POINTER(json, 400);

    char conf_name[64] = {0};
    CJSON_GET_STRING(json, "conf_name", conf_name, sizeof(conf_name), end);

    cJSON* conf_json = cJSON_GetObjectItem(json, conf_name);
    CHECK_POINTER_GO(conf_json, end);
    
    Areas areas;
    memset(&areas, 0, sizeof(Areas));
    int ret = CjsonToStructAreas(conf_json, &areas);
    CHECK_LT_GO(ret, 0, end);

    ret = kNetworkMng.func.set_config_cb(conf_name, &areas, NULL, res, res_size);
    CHECK_LT_GO(ret, 0, end);

    snprintf(res, res_size, "success");
    cJSON_free(json);
    return 200;
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
    
    Areas areas;
    memset(&areas, 0, sizeof(Areas));
    int ret = kNetworkMng.func.get_config_cb(conf_name, "unused", &areas, res, res_size);
    CHECK_LT_GO(ret, 0, end);

    snprintf(res, res_size, "respect json error");

    cJSON* conf_json = NULL;
    ret = StructToCjsonAreas(&areas, &conf_json);
    CHECK_LT_GO(ret, 0, end);

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
end:

    cJSON_free(json);
    return 400;
}

static int NetworkDevCtrl(char* in, char* out, int out_size, char* res, int res_size) {
    CHECK_POINTER(in, 500);
    CHECK_POINTER(out, 500);

    snprintf(res, res_size, "request json error");

    cJSON* json = cJSON_Parse(in);
    CHECK_POINTER(json, 400);

    char ctrl_mode[64] = {0};
    CJSON_GET_STRING(json, "ctrl_mode", ctrl_mode, sizeof(ctrl_mode), end);

    cJSON* conf_json = cJSON_GetObjectItem(json, ctrl_mode);
    CHECK_POINTER_GO(conf_json, end);
    
    void* dev_ctrl = NULL;
    PtzCtrl ptz_ctrl;
    ChipCtrl chip_ctrl;
    OtherCtrl other_ctrl;
    if (strcmp(ctrl_mode, "ptz_ctrl") == 0) {
        memset(&ptz_ctrl, 0, sizeof(PtzCtrl));
        int ret = CjsonToStructPtzCtrl(conf_json, &ptz_ctrl);
        CHECK_LT_GO(ret, 0, end);
        dev_ctrl = &ptz_ctrl;
    } else if (strcmp(ctrl_mode, "chip_ctrl") == 0) {
        memset(&chip_ctrl, 0, sizeof(ChipCtrl));
        int ret = CjsonToStructChipCtrl(conf_json, &chip_ctrl);
        CHECK_LT_GO(ret, 0, end);
        dev_ctrl = &chip_ctrl;
    } else if (strcmp(ctrl_mode, "other_ctrl") == 0) {
        memset(&other_ctrl, 0, sizeof(OtherCtrl));
        int ret = CjsonToStructOtherCtrl(conf_json, &other_ctrl);
        CHECK_LT_GO(ret, 0, end);
        dev_ctrl = &other_ctrl;
    } else {
        snprintf(res, res_size, "ctrl request not support");
        goto end;
    }

    int ret = kNetworkMng.func.dev_ctrl_cb(ctrl_mode, &dev_ctrl, NULL, res, res_size);
    CHECK_LT_GO(ret, 0, end);

    snprintf(res, res_size, "success");
    cJSON_free(json);
    return 200;
end:

    cJSON_free(json);
    return 400;
}

static int NetworkAlgorithemEnable(char* in, char* out, int out_size, char* res, int res_size) {
    CHECK_POINTER(in, 500);
    CHECK_POINTER(out, 500);

    snprintf(res, res_size, "request json error");

    cJSON* json = cJSON_Parse(in);
    CHECK_POINTER(json, 400);
    
    AlgorithemEnable algorithem_enable;
    memset(&algorithem_enable, 0, sizeof(AlgorithemEnable));
    int ret = CjsonToStructAlgorithemEnable(json, &algorithem_enable);
    CHECK_LT_GO(ret, 0, end);

    ret = kNetworkMng.func.algorithm_enable_cb("algorithem_enable", &algorithem_enable, NULL, res, res_size);
    CHECK_LT_GO(ret, 0, end);

    snprintf(res, res_size, "success");
    cJSON_free(json);
    return 200;
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
    {.method = "POST", .url = "/dev_api/dev_ctrl", .cb = NetworkDevCtrl},
    {.method = "POST", .url = "/dev_api/algorithm_enable", .cb = NetworkAlgorithemEnable},
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
    char buff[512] = {0};
    char* json_str = NULL;
    if (url == NULL) {
        snprintf(buff, sizeof(buff), "%s/pc_api/%s", kNetworkMng.pc_http_url, req_key);

        cJSON* json;
        if(strcmp(req_key, "peripheral_info") == 0) {
            ret = StructToCjsonPeripheralInfo((PeripheralInfo*)body, &json);
            CHECK_LT(ret, 0, -1);
        } else if (strcmp(req_key, "alarm_info") == 0) {
            ret = StructToCjsonAlarmInfo((AlarmInfo*)body, &json);
            CHECK_LT(ret, 0, -1);
        }

        json_str = cJSON_PrintUnformatted(json);
        if (json_str == NULL) {
            cJSON_free(json);
            return -1;
        }
    } else {
        snprintf(buff, sizeof(buff), "%s", url);
        json_str = (char*)body;
    }

    ret = HttpClientRequest(buff, json_str, res, res_size, timeout);

    if (url == NULL && json_str != NULL) {
        free(json_str);
    }

    return ret;
}
