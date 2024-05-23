#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "http_server.h"
#include "mongoose.h"
#include "list.h"
#include "cjson_common.h"

typedef struct 
{
    char method[16];
    char url[256];
    HttpServerUrlProcCb cb;
}HttpServerUrlInfo;

typedef struct {
    pthread_mutex_t mutex;
    pthread_t pthread_id;
    void* url_info_list;
    struct mg_mgr mgr;
    struct mg_connection *con;
}HttpServerMng;
static HttpServerMng kHttpServerMng = {.mutex = PTHREAD_MUTEX_INITIALIZER};

static void cb(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        int i = 0;
        struct mg_http_message *hm = ev_data;
        int list_size = ListSize(kHttpServerMng.url_info_list);
        pthread_mutex_lock(&kHttpServerMng.mutex);
        for(; i < list_size; i++) {
            HttpServerUrlInfo* info = ListGet(kHttpServerMng.url_info_list, i);
            if (info == NULL) {
                mg_http_reply(c, 500, "", "{\"code\":500, \"massage\":\"json abnormal\", \"data\":\"\"}");
                break;
            }

            if (mg_match(hm->uri, mg_str(info->url), NULL) 
                && mg_match(hm->method, mg_str(info->method), NULL)) {
                char out[1024*10] = {0};
                char res[1024] = {0};
                int code = info->cb(hm->body.buf, out, sizeof(out), res, sizeof(res));

                cJSON* json = cJSON_CreateObject();
                if (json != NULL) {
                    CJSON_SET_NUMBER(json, "code", code, end);
                    CJSON_SET_STRING(json, "message", res, end);
                    if (strlen(out) > 0) {
                        cJSON* out_json = cJSON_Parse(out);
                        CHECK_POINTER_GO(out_json, end);
                        CHECK_BOOL_GO(cJSON_AddItemToObject(json, "data", out_json), end);
                    } else {
                        CJSON_SET_STRING(json, "data", out, end);
                    }

                    char* replay = cJSON_PrintUnformatted(json);
                    if (replay != NULL) {
                        mg_http_reply(c, code, "", replay);
                        free(replay);
                    } else {
end:
                        mg_http_reply(c, 500, "", "{\"code\":500, \"massage\":\"json to string error\", \"data\":\"\"}");
                    }
                    cJSON_free(json);
                } else {
                    mg_http_reply(c, 500, "", "{\"code\":500, \"massage\":\"json abnormal\", \"data\":\"\"}");
                }

                break;
            }
        }
        pthread_mutex_unlock(&kHttpServerMng.mutex);

        if(i >= list_size) {
            mg_http_reply(c, 501, "", "{\"code\":501, \"massage\":\"request not support\", \"data\":\"\"}");
        }
    }
}

static void* HttpServerProc(void* arg) {
    mg_mgr_init(&kHttpServerMng.mgr);
    kHttpServerMng.con = mg_http_listen(&kHttpServerMng.mgr, arg, cb, &kHttpServerMng.mgr);

    while(1){
        mg_mgr_poll(&kHttpServerMng.mgr, 1000);
    }
}

int HttpServerInit(char *addr) {
    kHttpServerMng.url_info_list = ListCreate();
    if(kHttpServerMng.url_info_list == NULL) {
        return -1;
    }

    int ret = pthread_create(&kHttpServerMng.pthread_id, NULL, HttpServerProc, addr);
    return ret;
}

void HttpServerUnInit() {
    pthread_cancel(kHttpServerMng.pthread_id);
    pthread_join(kHttpServerMng.pthread_id, NULL);

    mg_mgr_free(&kHttpServerMng.mgr);
    ListDestory(kHttpServerMng.url_info_list);
}

void HttpServerUrlRegister(char* method, char* url, HttpServerUrlProcCb cb) {
    HttpServerUrlInfo info;
    snprintf(info.method, sizeof(info.method), "%s", method);
    snprintf(info.url, sizeof(info.url), "%s", url);
    info.cb = cb;

    pthread_mutex_lock(&kHttpServerMng.mutex);
    ListPush(kHttpServerMng.url_info_list, &info, sizeof(HttpServerUrlInfo));
    pthread_mutex_unlock(&kHttpServerMng.mutex);
}