#include <stdlib.h>
#include <string.h>
#include "http_client.h"
#include "check_common.h"
#include "curl/curl.h"

typedef struct {
    int size;
    char* buf;
}HttpClientResponse;

static int HttpClientResponseCb(void* ptr, int size, int nmemb, void* userp) {
    HttpClientResponse* resp = (HttpClientResponse*)userp;
    char* p = (char*) realloc(resp->buf, resp->size + size*nmemb + 1);
    CHECK_POINTER(p, 0);

    resp->buf = p;
    memset(resp->buf + resp->size, 0, size*nmemb + 1);
    memcpy(resp->buf + resp->size, ptr, size*nmemb);
    resp->size = resp->size + size*nmemb + 1;

    return size*nmemb;
}

int HttpClientRequest(const char* method, const char* url, const char* body, char* res, int res_size, int timeout){
    CHECK_POINTER(url, -1);
    CHECK_POINTER(res, -1);

    int ret = -1;

    CURL *curl = curl_easy_init();
    CHECK_POINTER(curl, -1);

    HttpClientResponse resp;
    memset(&resp, 0, sizeof(HttpClientResponse));
    resp.buf = (char*)calloc(1, sizeof(char));
    CHECK_POINTER_GO(resp.buf, end);

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "http");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout);

    struct curl_slist *headers = NULL;
    if (strcmp(method, "POST") == 0) {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpClientResponseCb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&resp);

    CURLcode curl_res = curl_easy_perform(curl);
    CHECK_LT_GO(curl_res, CURLE_OK, end);
    
    snprintf(res, res_size, "%s", resp.buf);

    ret = 0;
end:
    if (headers != NULL) {
        curl_slist_free_all(headers);
    }

    if (resp.buf != NULL) {
        free(resp.buf);
    }

    curl_easy_cleanup(curl);
    return ret;
}