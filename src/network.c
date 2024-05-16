#include "network.h"
#include "common.h"
#include "http_server.h"
#include "cjson_common.h"
#include "check_common.h"

typedef struct 
{
    char pc_http_url[512];
    NetworkOperCb oper_cb;
}NetworkMng;
static NetworkMng kNetworkMng;

static int NetworkSetConfig(char* in, char* out, int out_size, char* res, int res_size) {
    printf("%s\n", in);
    snprintf(out, out_size, "%s", in);
    return 200;
}

static int NetworkGetConfig(char* in, char* out, int out_size, char* res, int res_size) {
    printf("%s\n", in);
    snprintf(out, out_size, "%s", in);
    return 200;
}

static int NetworkDevCtrl(char* in, char* out, int out_size, char* res, int res_size) {
    printf("%s\n", in);
    snprintf(out, out_size, "%s", in);
    return 200;
}

static int NetworkAlgorithmEnable(char* in, char* out, int out_size, char* res, int res_size) {
    printf("%s\n", in);
    snprintf(out, out_size, "%s", in);
    return 200;
}

static int NetworkUpgrade(char* in, char* out, int out_size, char* res, int res_size) {
    printf("%s\n", in);
    snprintf(out, out_size, "%s", in);
    return 200;
}

static int NetworkHttpServerInfo(char* in, char* out, int out_size, char* res, int res_size) {
    CHECK_POINTER(in, 500);
    CHECK_POINTER(out, 500);

    snprintf(res, res_size, "request json error");

    cJSON* json = cJSON_Parse(in);
    CHECK_POINTER(out, 400);

    CJSON_GET_STRING(json, "http_url", kNetworkMng.pc_http_url, sizeof(kNetworkMng.pc_http_url), end);
    cJSON_free(json);

    snprintf(res, res_size, "success");
    return 200;
end:

    cJSON_free(json);
    return 400;
}

typedef struct {
    char method[16];
    char url[256];
    HttpServerUrlProcCb cb;
}HttpServerUrlInfo;

static HttpServerUrlInfo kUrlInfo[] ={
    {.method = "POST", .url = "/dev_api/set_config", .cb = NetworkSetConfig},
    {.method = "POST", .url = "/dev_api/get_config", .cb = NetworkGetConfig},
    {.method = "POST", .url = "/dev_api/dev_ctrl", .cb = NetworkDevCtrl},
    {.method = "POST", .url = "/dev_api/algorithm_enable", .cb = NetworkAlgorithmEnable},
    {.method = "POST", .url = "/dev_api/Upgrade", .cb = NetworkUpgrade},
    {.method = "POST", .url = "/dev_api/http_svr_info", .cb = NetworkHttpServerInfo},
};

int NetworkInit(NetworkOperCb cb) {
    kNetworkMng.oper_cb = cb;
    
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

int NetworkRequest(const char* url, char* req_type, void* body, char* res, int res_size) {

}
