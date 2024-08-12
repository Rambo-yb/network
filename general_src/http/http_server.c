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
            if (info != NULL 
                && mg_match(hm->uri, mg_str(info->url), NULL) 
                && mg_match(hm->method, mg_str(info->method), NULL)) {
                LOG_INFO("method:%.*s, uri:%.*s, query:%.*s, proto:%.*s, body:%.*s", 
                    hm->method.len, hm->method.buf, hm->uri.len, hm->uri.buf, 
                    hm->query.len, hm->query.buf, hm->proto.len, hm->proto.buf, hm->body.len, hm->body.buf);
                info->cb(c, ev_data);
                
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

void HttpServerGetUri(void* data, char* uri, int size) {
    CHECK_POINTER(data, (void)0);
    CHECK_POINTER(uri, (void)0);
    struct mg_http_message *hm = (struct mg_http_message *)data;
    snprintf(uri, size, "%.*s", hm->uri.len, hm->uri.buf);
}

void HttpServerGetHead(void* data, char* head, int size) {
    CHECK_POINTER(data, (void)0);
    CHECK_POINTER(head, (void)0);
    struct mg_http_message *hm = (struct mg_http_message *)data;
    snprintf(head, size, "%.*s", hm->head.len, hm->head.buf);
}

void HttpServerGetBody(void* data, char* body, int size) {
    CHECK_POINTER(data, (void)0);
    CHECK_POINTER(body, (void)0);
    struct mg_http_message *hm = (struct mg_http_message *)data;
    snprintf(body, size, "%.*s", hm->body.len, hm->body.buf);
}

void HttpServerGetQuery(void* data, char* query, int size) {
    CHECK_POINTER(data, (void)0);
    CHECK_POINTER(query, (void)0);
    struct mg_http_message *hm = (struct mg_http_message *)data;
    snprintf(query, size, "%.*s", hm->query.len, hm->query.buf);
}

void HttpServerReplay(void* c, int code, char* header, char* body){
    mg_http_reply(c, code, header, body);
}