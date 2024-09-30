#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "network.h"
#include "network_upload.h"
#include "network_download.h"
#include "http_server.h"
#include "cjson_common.h"
#include "check_common.h"
#include "struct_to_cjson.h"
#include "cjson_to_struct.h"
#include "list.h"

#define NETWORK_LIB_VERSION ("V1.0.1")
#define NETWORK_HTTP_PORT (8080)
#define NETWORK_DISCOVERY_PORT (6666)
#define NETWORK_MULTICAST_IP ("239.255.255.222")

typedef struct {
    char method[16];
    char url[256];
    HttpServerUrlProcCb cb;
}HttpServerUrlInfo;

typedef struct {
    int type;
    void* cb;
}NetworkOperationInfo;

typedef struct {
    // 配置
    NetworkSystemInfo system_info;
    NetworkCameraChipInfo camera_chip_info;
    NetworkPosition position;
    NetworkCalibrationGun calibration_gun;
    NetworkPtzInfo ptz_info;
    NetworkAreas areas;
    NetworkOtherConfig other_config;
    NetworkNetworkInfo network_info;
    NetworkAlgorithm algorithm;

    void* operation_list;
    pthread_mutex_t oper_mutex;

    char device_addr[16];

    pthread_t pthread_discovery_device;
    pthread_t pthread_upload;
}NetworkMng;
static NetworkMng kNetworkMng = {.oper_mutex = PTHREAD_MUTEX_INITIALIZER};

typedef int (*StructToCjsonNetworkCb)(void*, cJSON**);
typedef int (*CjsonToStructNetworkCb)(cJSON*, void*);

typedef struct {
    int type;
    char* name;
    void* st;
    int st_size;
    StructToCjsonNetworkCb s2j_cb;
    CjsonToStructNetworkCb j2s_cb;
}NetworkOperationSubInfo;

#define NETWORK_CONFIG_INFO_INIT(t, nm, st_space, st_name)  \
    {.type = t, .name = nm, .st = &st_space, .st_size = sizeof(st_space), .s2j_cb = StructToCjson##st_name, .j2s_cb = CjsonToStruct##st_name}

static NetworkOperationSubInfo kNetworkOperationSubInfo[] = {
    NETWORK_CONFIG_INFO_INIT(NETWORK_CONFIG_SYSTEM_INFO, "system_info", kNetworkMng.system_info, NetworkSystemInfo),
    NETWORK_CONFIG_INFO_INIT(NETWORK_CONFIG_CAMERA_CHIP_INFO, "camera_chip_info", kNetworkMng.camera_chip_info, NetworkCameraChipInfo),
    NETWORK_CONFIG_INFO_INIT(NETWORK_CONFIG_POSITION, "position", kNetworkMng.position, NetworkPosition),
    NETWORK_CONFIG_INFO_INIT(NETWORK_CONFIG_CALIBRATION_GUN, "calibration_gun", kNetworkMng.calibration_gun, NetworkCalibrationGun),
    NETWORK_CONFIG_INFO_INIT(NETWORK_CONFIG_PTZ_INFO, "ptz_info", kNetworkMng.ptz_info, NetworkPtzInfo),
    NETWORK_CONFIG_INFO_INIT(NETWORK_CONFIG_AREAS, "areas", kNetworkMng.areas, NetworkAreas),
    NETWORK_CONFIG_INFO_INIT(NETWORK_CONFIG_OTHER_INFO, "other_info", kNetworkMng.other_config, NetworkOtherConfig),
    NETWORK_CONFIG_INFO_INIT(NETWORK_CONFIG_NETWORK, "network", kNetworkMng.network_info, NetworkNetworkInfo),
    NETWORK_CONFIG_INFO_INIT(NETWORK_CONFIG_ALGORITHM, "algorithm", kNetworkMng.algorithm, NetworkAlgorithm),

    {.type = NETWORK_SYSTEM_REBOOT, .name = "device_reboot"},
    {.type = NETWORK_SYSTEM_RESET, .name = "device_reset"},
    {.type = NETWORK_SYSTEM_FORMAT, .name = "device_format"},
    {.type = NETWORK_SYSTEM_DEVICE_INFO, .name = "device_info", .s2j_cb = StructToCjsonNetworkDeviceInfo},
    {.type = NETWORK_SYSTEM_SET_TIME, .name = "set_time"},
    {.type = NETWORK_SYSTEM_GET_RTSP_URL, .name = "get_rtsp_url"},

    {.type = NETWORK_CONTORL_SNAP, .name = "snap"},
    {.type = NETWORK_CONTORL_RECORD, .name = "record"},
    {.type = NETWORK_CONTORL_BAD_PIX, .name = "bad_pix"},
    {.type = NETWORK_CONTORL_SHUTTER_CALIBRATION, .name = "shutter_calibration"},
    {.type = NETWORK_CONTORL_LASER_RANGING, .name = "laser_ranging"},
    {.type = NETWORK_CONTORL_PTZ, .name = "ptz_ctrl"},
    {.type = NETWORK_CONTORL_TRACKING, .name = "tracking_object"},
};

static int NetworkGetOperationSubByName(char* name) {
    int i = 0;
    for (; i < sizeof(kNetworkOperationSubInfo) / sizeof(NetworkOperationSubInfo); i++) {
        if (strcmp(name, kNetworkOperationSubInfo[i].name) == 0) {
            break;
        }
    }
    return (i >= sizeof(kNetworkOperationSubInfo) / sizeof(NetworkOperationSubInfo)) ? -1 : i;
}

static void* NetworkGetOperationCb(int type) {
    NetworkOperationInfo* oper_info = NULL;
    int list_size = ListSize(kNetworkMng.operation_list);
    for (int i = 0; i < list_size; i++) {
        oper_info = ListGet(kNetworkMng.operation_list, i);
        if (oper_info->type == type) {
            break;
        }
        oper_info = NULL;
    }

    return oper_info;
}

enum {
    NETWORK_REPLAY_JSON,
    NETWORK_REPLAY_STR,
};

static void NetworkReplay(void* c, int code, int type, void* body, char* msg) {
    cJSON* json = cJSON_CreateObject();
    CHECK_POINTER(json, goto end);

    CJSON_SET_NUMBER(json, "code", code, goto end);
    CJSON_SET_STRING(json, "message", msg, goto end);

    if (type == NETWORK_REPLAY_JSON) {
        CHECK_BOOL(cJSON_AddItemToObject(json, "data", body), goto end);
    } else if (type == NETWORK_REPLAY_STR) {
        CJSON_SET_STRING(json, "data", body, goto end);
    }

    char* replay = cJSON_PrintUnformatted(json);
    CHECK_POINTER(replay, goto end);

    HttpServerReplay(c, code, "", replay);

    free(replay);
    cJSON_free(json);
    return ;
end:

    if (json != NULL) {
        cJSON_free(json);
    }

    HttpServerReplay(c, 500, "", "{\"code\":500, \"massage\":\"json abnormal\", \"data\":\"\"}");
}

static void NetworkGetAbility(void* c, void* data) {
    CHECK_POINTER(c, return);
    CHECK_POINTER(data, return);

    char msg[1024] = {0};
    snprintf(msg, sizeof(msg), "http server abnormal");

    cJSON* json = cJSON_CreateObject();
    CHECK_POINTER(json, goto end);

    int i = 0;
    pthread_mutex_lock(&kNetworkMng.oper_mutex);
    NetworkOperationInfo* oper_info = NetworkGetOperationCb(NETWORK_OPERATION_GET_ABILITY);
    if (oper_info == NULL || oper_info->cb == NULL) {
        snprintf(msg, sizeof(msg), "http server not support operation");
        LOG_ERR("http server not support operation");
        goto end_unlock;
    }

    NetworkSupportFunction func;
    memset(&func, 0, sizeof(NetworkSupportFunction));
    NetworkOperationGetAbilityCb cb = oper_info->cb;
    int ret = cb(&func);
    pthread_mutex_unlock(&kNetworkMng.oper_mutex);

    cJSON* conf_json = NULL;
    CHECK_LT(StructToCjsonNetworkSupportFunction(&func, &conf_json), 0, goto end);
    CHECK_BOOL(cJSON_AddItemToObject(json, "support_function", conf_json), cJSON_free(conf_json);goto end);

    NetworkReplay(c, 200, NETWORK_REPLAY_JSON, json, "success");

    cJSON_free(json);
    return ;
end_unlock:
    pthread_mutex_unlock(&kNetworkMng.oper_mutex);
end:
    if(json != NULL) {
        cJSON_free(json);
    }
    NetworkReplay(c, 500, NETWORK_REPLAY_STR, "", msg);
}

static void NetworkSystemRequest(void* c, void* data) {
    CHECK_POINTER(c, return);
    CHECK_POINTER(data, return);

    char msg[1024] = {0};
    snprintf(msg, sizeof(msg), "http server abnormal");

    char query[1024] = {0};
    HttpServerGetQuery(data, query, sizeof(query));

    char type[64] = {0};
    sscanf(query, "type=%64[^&]", type);
    

    int index = NetworkGetOperationSubByName(type);
    if (index < 0) {
        snprintf(msg, sizeof(msg), "ctrl request not support");
        LOG_ERR("ctrl request not support");
        goto end;
    }

    NetworkSystem network_sys;
    memset(&network_sys, 0, sizeof(NetworkSystem));
    network_sys.type = kNetworkOperationSubInfo[index].type;
    if (network_sys.type == NETWORK_SYSTEM_SET_TIME) {
        sscanf(query, "%*[^&],time=%d", &network_sys.in.time);
    }

    pthread_mutex_lock(&kNetworkMng.oper_mutex);
    NetworkOperationInfo* oper_info = NetworkGetOperationCb(NETWORK_OPERATION_SYSTEM_REQUEST);
    if (oper_info == NULL || oper_info->cb == NULL) {
        snprintf(msg, sizeof(msg), "http server not support operation");
        LOG_ERR("http server not support operation");
        goto end_unlock;
    }

    NetworkOperationSystemCb cb = oper_info->cb;
    int ret = cb(&network_sys);
    CHECK_LT(ret, 0, goto end_unlock);
    pthread_mutex_unlock(&kNetworkMng.oper_mutex);

    if (network_sys.type == NETWORK_SYSTEM_GET_RTSP_URL) {
        cJSON* json = cJSON_CreateObject();
        CHECK_POINTER(json, goto end);

        CJSON_SET_STRING(json, "rtsp_url", network_sys.out.rtsp_url, cJSON_free(json);goto end);
        NetworkReplay(c, 200, NETWORK_REPLAY_JSON, json, "success");
        cJSON_free(json);
    } else if (network_sys.type == NETWORK_SYSTEM_DEVICE_INFO) {
        cJSON* json = cJSON_CreateObject();
        CHECK_POINTER(json, goto end);

        cJSON* json_ = NULL;
        ret = kNetworkOperationSubInfo[index].s2j_cb(&network_sys.out.device_info, &json_);
        CHECK_LT(ret, 0, cJSON_free(json);goto end);

        CHECK_BOOL(cJSON_AddItemToObject(json, "device_info", json_), cJSON_free(json_);cJSON_free(json);goto end);
        NetworkReplay(c, 200, NETWORK_REPLAY_JSON, json, "success");
        cJSON_free(json);
    } else {
        NetworkReplay(c, 200, NETWORK_REPLAY_STR, "", "success");
    }
    
    return ;
end_unlock:
    pthread_mutex_unlock(&kNetworkMng.oper_mutex);
end:
    NetworkReplay(c, 500, NETWORK_REPLAY_STR, "", msg);
}

static void NetworkSetConfig(void* c, void* data) {
    CHECK_POINTER(c, return);
    CHECK_POINTER(data, return);

    char msg[1024] = {0};
    snprintf(msg, sizeof(msg), "request json error");

    char body[1024*4] = {0};
    HttpServerGetBody(data, body, sizeof(body));

    cJSON* json = cJSON_Parse(body);
    CHECK_POINTER(json, goto end);

    char conf_name[64] = {0};
    CJSON_GET_STRING(json, "conf_name", conf_name, sizeof(conf_name), goto end);

    cJSON* conf_json = cJSON_GetObjectItemCaseSensitive(json, "config_info");
    CHECK_POINTER(conf_json, goto end);
    
    int index = NetworkGetOperationSubByName(conf_name);
    if (index < 0) {
        snprintf(msg, sizeof(msg), "ctrl request not support");
        LOG_ERR("ctrl request not support");
        goto end;
    }

    pthread_mutex_lock(&kNetworkMng.oper_mutex);
    NetworkOperationInfo* oper_info = NetworkGetOperationCb(NETWORK_OPERATION_SET_CONFIG);
    if (oper_info == NULL || oper_info->cb == NULL) {
        snprintf(msg, sizeof(msg), "http server not support operation");
        LOG_ERR("http server not support operation");
        goto end_unlock;
    }

    memset(kNetworkOperationSubInfo[index].st, 0, kNetworkOperationSubInfo[index].st_size);
    int ret = kNetworkOperationSubInfo[index].j2s_cb(conf_json, kNetworkOperationSubInfo[index].st);
    CHECK_LT(ret, 0, goto end_unlock);

    NetworkOperationSetConfigCb cb = oper_info->cb;
    ret = cb(kNetworkOperationSubInfo[index].type, kNetworkOperationSubInfo[index].st, kNetworkOperationSubInfo[index].st_size);
    CHECK_LT(ret, 0, goto end_unlock);

    pthread_mutex_unlock(&kNetworkMng.oper_mutex);

    NetworkReplay(c, 200, NETWORK_REPLAY_STR, "", "success");
    cJSON_free(json);
    return ;
end_unlock:
    pthread_mutex_unlock(&kNetworkMng.oper_mutex);
end:
    if(json != NULL) {
        cJSON_free(json);
    }
    NetworkReplay(c, 500, NETWORK_REPLAY_STR, "", msg);
}

static void NetworkGetConfig(void* c, void* data) {
    CHECK_POINTER(c, return);
    CHECK_POINTER(data, return);

    char msg[1024] = {0};
    snprintf(msg, sizeof(msg), "request json error");

    char body[1024*4] = {0};
    HttpServerGetBody(data, body, sizeof(body));

    cJSON* json = cJSON_Parse(body);
    CHECK_POINTER(json, goto end);

    char conf_name[64] = {0};
    CJSON_GET_STRING(json, "conf_name", conf_name, sizeof(conf_name), goto end);

    cJSON* conf_json = NULL;
    int index = NetworkGetOperationSubByName(conf_name);
    if (index < 0) {
        snprintf(msg, sizeof(msg), "ctrl request not support");
        LOG_ERR("ctrl request not support");
        goto end;
    }

    pthread_mutex_lock(&kNetworkMng.oper_mutex);
    NetworkOperationInfo* oper_info = NetworkGetOperationCb(NETWORK_OPERATION_GET_CONFIG);
    if (oper_info == NULL || oper_info->cb == NULL) {
        snprintf(msg, sizeof(msg), "http server not support operation");
        LOG_ERR("http server not support operation");
        goto end_unlock;
    }

    NetworkOperationGetConfigCb cb = oper_info->cb;
    memset(kNetworkOperationSubInfo[index].st, 0, kNetworkOperationSubInfo[index].st_size);
    int ret = cb(kNetworkOperationSubInfo[index].type, kNetworkOperationSubInfo[index].st, kNetworkOperationSubInfo[index].st_size);
    CHECK_LT(ret, 0, goto end_unlock);

    ret = kNetworkOperationSubInfo[index].s2j_cb(kNetworkOperationSubInfo[index].st, &conf_json);
    CHECK_LT(ret, 0, goto end_unlock);

    pthread_mutex_unlock(&kNetworkMng.oper_mutex);

    CHECK_BOOL(cJSON_AddItemToObject(json, "config_info", conf_json), cJSON_free(conf_json);goto end);

    NetworkReplay(c, 200, NETWORK_REPLAY_JSON, json, "success");

    cJSON_free(json);
    return ;
end_unlock:
    pthread_mutex_unlock(&kNetworkMng.oper_mutex);
end:
    if(json != NULL) {
        cJSON_free(json);
    }
    NetworkReplay(c, 500, NETWORK_REPLAY_STR, "", msg);
}

static void NetworkGetSubParam(int type, char* in, void* out) {
    NetworkContorl* ctrl = (NetworkContorl*)out;
    switch (type)
    {
    case NETWORK_CONTORL_RECORD:
        sscanf(in, "%*[^&]&state=%d", &ctrl->in.recode_state);
        break;
    case NETWORK_CONTORL_BAD_PIX: 
        sscanf(in, "%*[^&]&oper=%d", &ctrl->in.bad_pix_operation);
        break;
    case NETWORK_CONTORL_PTZ:
        sscanf(in, "%*[^&]&mode=%d", &ctrl->in.ptz_ctrl.mode);
        break;
    case NETWORK_CONTORL_LASER_RANGING: 
        sscanf(in, "%*[^&]&mode=%d&state=%d", &ctrl->in.laser_ranging.mode, &ctrl->in.laser_ranging.state);
        break;
    default:
        break;
    }
}

static void NetworkControlRequest(void* c, void* data) {
    CHECK_POINTER(c, return);
    CHECK_POINTER(data, return);

    char msg[1024] = {0};
    snprintf(msg, sizeof(msg), "http server abnormal");

    char method[8] = {0};
    HttpServerGetMethod(data, method, sizeof(method));
    
    NetworkContorl ctrl;
    memset(&ctrl, 0, sizeof(NetworkContorl));
    if (strcmp(method, "POST") == 0) {
        char body[1024*4] = {0};
        HttpServerGetBody(data, body, sizeof(body));

        cJSON* json = cJSON_Parse(body);
        CHECK_POINTER(json, goto end);

        CJSON_GET_NUMBER_LIST(json, "tracking_object", ctrl.in.tracking_object.tracking_object, ctrl.in.tracking_object.tracking_num, NET_TRACKING_OBJECT_MAX_NUM, goto end);
        ctrl.type = NETWORK_CONTORL_TRACKING;
    } else if (strcmp(method, "GET") == 0) {
        char query[1024] = {0};
        HttpServerGetQuery(data, query, sizeof(query));

        char type[64] = {0};
        sscanf(query, "type=%64[^&]", type);

        int index = NetworkGetOperationSubByName(type);
        if (index < 0) {
            snprintf(msg, sizeof(msg), "ctrl request not support");
            LOG_ERR("ctrl request not support");
            goto end;
        }

        ctrl.type = kNetworkOperationSubInfo[index].type;
        NetworkGetSubParam(kNetworkOperationSubInfo[index].type, query, &ctrl);
    }

    pthread_mutex_lock(&kNetworkMng.oper_mutex);
    NetworkOperationInfo* oper_info = NetworkGetOperationCb(NETWORK_OPERATION_CONTORL_REQUEST);
    if (oper_info == NULL || oper_info->cb == NULL) {
        snprintf(msg, sizeof(msg), "http server not support operation");
        LOG_ERR("http server not support operation");
        goto end_unlock;
    }

    NetworkOperationControlCb cb = oper_info->cb;
    int ret = cb(&ctrl);
    CHECK_LT(ret, 0, goto end_unlock);
    pthread_mutex_unlock(&kNetworkMng.oper_mutex);

    if (ctrl.type == NETWORK_CONTORL_BAD_PIX && ctrl.in.bad_pix_operation == NETWORK_BAD_PIX_OPERATION_GET) {
        cJSON* json = cJSON_CreateObject();
        CHECK_POINTER(json, goto end);

        CJSON_SET_NUMBER(json, "bad_pix_num", ctrl.out.bad_pix_num, cJSON_free(json);goto end);
        NetworkReplay(c, 200, NETWORK_REPLAY_JSON, json, "success");
        cJSON_free(json);
    } else if (ctrl.type == NETWORK_CONTORL_LASER_RANGING && ctrl.in.laser_ranging.mode == NETWORK_LASER_RANGING_SINGLE) {
        cJSON* json = cJSON_CreateObject();
        CHECK_POINTER(json, goto end);

        CJSON_SET_NUMBER(json, "distance", ctrl.out.distence, cJSON_free(json);goto end);
        NetworkReplay(c, 200, NETWORK_REPLAY_JSON, json, "success");
        cJSON_free(json);
    } else {
        NetworkReplay(c, 200, NETWORK_REPLAY_STR, "", "success");
    }

    return ;
end_unlock:
    pthread_mutex_unlock(&kNetworkMng.oper_mutex);
end:
    NetworkReplay(c, 500, NETWORK_REPLAY_STR, "", msg);
}

static void NetworkDownload(void* c, void* data) {
    CHECK_POINTER(c, return);
    CHECK_POINTER(data, return);

    char msg[1024] = {0};
    snprintf(msg, sizeof(msg), "request json error");

    char body[1024*4] = {0};
    HttpServerGetBody(data, body, sizeof(body));

    cJSON* json = cJSON_Parse(body);
    CHECK_POINTER(json, goto end);

    NetworkDownloadInfo download_info;
    CJSON_GET_STRING(json, "type", download_info.type, sizeof(download_info.type), goto end);
    CJSON_GET_STRING(json, "url", download_info.url, sizeof(download_info.url), goto end);
    CJSON_GET_NUMBER(json, "size", download_info.size, sizeof(download_info.size), goto end);
    CJSON_GET_STRING(json, "md5", download_info.md5, sizeof(download_info.md5), goto end);
    NetworkDownloadFile(&download_info);

    NetworkReplay(c, 200, NETWORK_REPLAY_STR, "", "success");

    cJSON_free(json);
    return ;
end:
    if(json != NULL) {
        cJSON_free(json);
    }
    NetworkReplay(c, 400, NETWORK_REPLAY_STR, "", msg);
}

static void NetworkPcInfo(void* c, void* data) {
    CHECK_POINTER(c, return);
    CHECK_POINTER(data, return);

    char msg[1024] = {0};
    snprintf(msg, sizeof(msg), "request json error");

    char body[1024*4] = {0};
    HttpServerGetBody(data, body, sizeof(body));

    cJSON* json = cJSON_Parse(body);
    CHECK_POINTER(json, goto end);

    char svr_url[256] = {0};
    CJSON_GET_STRING(json, "http_svr_url", svr_url, sizeof(svr_url), goto end);
    NetworkUploadSetPcInfo(svr_url);

    NetworkReplay(c, 200, NETWORK_REPLAY_STR, "", "success");
    cJSON_free(json);
    return ;
end:
    if(json != NULL) {
        cJSON_free(json);
    }
    NetworkReplay(c, 400, NETWORK_REPLAY_STR, "", msg);
}

static HttpServerUrlInfo kUrlInfo[] ={
    {.method = "GET", .url = "/dev_api/get_ability", .cb = NetworkGetAbility},
    {.method = "GET", .url = "/dev_api/system_request", .cb = NetworkSystemRequest},
    {.method = "POST", .url = "/dev_api/set_config", .cb = NetworkSetConfig},
    {.method = "POST", .url = "/dev_api/get_config", .cb = NetworkGetConfig},
    {.method = "GET", .url = "/dev_api/control_request", .cb = NetworkControlRequest},
    {.method = "POST", .url = "/dev_api/control_request", .cb = NetworkControlRequest},
    {.method = "POST", .url = "/dev_api/download", .cb = NetworkDownload},
    {.method = "POST", .url = "/dev_api/pc_info", .cb = NetworkPcInfo},
};

static void *NetworkDiscoveryDeviceProc(void *arg) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    CHECK_LT(sockfd, 0, return NULL);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(NETWORK_DISCOVERY_PORT);
    CHECK_LT(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)), 0, close(sockfd);return NULL);

    struct ip_mreq mreq;
    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(NETWORK_MULTICAST_IP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    CHECK_LT(setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)), 0, close(sockfd);return NULL);

    while (1) {
        struct sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(client_addr));
        socklen_t addr_len = sizeof(struct sockaddr_in);
        
        char buffer[1024] = {0};
        int ret = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        CHECK_LT(ret, 0, continue);
        CHECK_EQ(strcmp(buffer, "discovery cdjp device"), 0, continue);

        cJSON* json =  cJSON_CreateObject();
        CHECK_POINTER(json, goto end);
        CJSON_SET_STRING(json, "device_addr", kNetworkMng.device_addr, goto end);
        CJSON_SET_NUMBER(json, "http_port", NETWORK_HTTP_PORT, goto end);

        char* buff = cJSON_PrintUnformatted(json);
        CHECK_POINTER(buff, goto end);

        sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&client_addr, addr_len);
        free(buff);
        
    end:
        cJSON_free(json);
    }

    close(sockfd);
}

int NetworkInit(char* addr) {
    CHECK_POINTER(addr, return -1);

    log_init("/oem/logs/network.log", 512*1024, 3, 3);
    snprintf(kNetworkMng.device_addr, sizeof(kNetworkMng.device_addr), "%s", addr);

    kNetworkMng.operation_list = ListCreate();
    CHECK_POINTER(kNetworkMng.operation_list, return -1);

    char http_server[32] = {0};
    snprintf(http_server, sizeof(http_server), ":%d", NETWORK_HTTP_PORT);
    HttpServerInit(http_server);
    for(int i = 0; i < sizeof(kUrlInfo) / sizeof(HttpServerUrlInfo); i++) {
        HttpServerUrlRegister(kUrlInfo[i].method, kUrlInfo[i].url, kUrlInfo[i].cb);
        LOG_INFO("server listen url, method:%s, uri:%s", kUrlInfo[i].method, kUrlInfo[i].url);
    }

    NetworkUploadInit();
    NetworkDownloadInit();

    pthread_create(&kNetworkMng.pthread_discovery_device, NULL, NetworkDiscoveryDeviceProc, NULL);

    LOG_INFO("network init success! compile time:%s %s, ver:%s", __DATE__, __TIME__, NETWORK_LIB_VERSION);
    return 0;
}

void NetworkUnInit() {
    if (kNetworkMng.pthread_discovery_device) {
        pthread_cancel(kNetworkMng.pthread_discovery_device);
        pthread_join(kNetworkMng.pthread_discovery_device, NULL);
    }

    NetworkDownloadUnInit();
    NetworkUploadUnInit();

    HttpServerUnInit();
    ListDestory(kNetworkMng.operation_list);
}

void NetworkOperationRegister(NetworkOperationType type, void* cb) {
    NetworkOperationInfo info = {.type = type, .cb = cb};

    if (type == NETWORK_OPERATION_UPGRADE) {
        NetworkDownloadUpgradeCb(cb);
        return ;
    }

    pthread_mutex_lock(&kNetworkMng.oper_mutex);
    ListPush(kNetworkMng.operation_list, &info, sizeof(NetworkOperationInfo));
    pthread_mutex_unlock(&kNetworkMng.oper_mutex);
}

int NetworkUploadInfo(NetworkUpload* upload_info) {
    NetworkUploadMassage(upload_info);
    return 0;
}
