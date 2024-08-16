#include <stdlib.h>
#include <string.h>
#include "http_client.h"
#include "check_common.h"
#include "curl/curl.h"

typedef struct {
    int size;
    char* buf;
}HttpResponse;

static int HttpResponseCb(void* ptr, int size, int nmemb, void* userp) {
    HttpResponse* resp = (HttpResponse*)userp;
    char* p = (char*) realloc(resp->buf, resp->size + size*nmemb + 1);
    CHECK_POINTER(p, 0);

    resp->buf = p;
    memset(resp->buf + resp->size, 0, size*nmemb + 1);
    memcpy(resp->buf + resp->size, ptr, size*nmemb);
    resp->size = resp->size + size*nmemb + 1;

    return size*nmemb;
}

int HttpRequest(const char* method, const char* url, const char* body, char* res, int res_size, int timeout){
    CHECK_POINTER(url, return -1);
    CHECK_POINTER(res, return -1);

    int ret = -1;

    CURL *curl = curl_easy_init();
    CHECK_POINTER(curl, return -1);

    HttpResponse resp;
    memset(&resp, 0, sizeof(HttpResponse));
    resp.buf = (char*)calloc(1, sizeof(char));
    CHECK_POINTER(resp.buf, goto end);

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout);

    struct curl_slist *headers = NULL;
    if (strcmp(method, "POST") == 0) {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpResponseCb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&resp);

    CURLcode curl_res = curl_easy_perform(curl);
    CHECK_LT(curl_res, CURLE_OK, goto end);
    
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

typedef struct {
    CURL* curl;
    struct curl_slist* headers;
    curl_mime* mime;
}FormDataReqInfo;

void* HttpFormDataRequestInit(const char* url, int timeout) {
    FormDataReqInfo* info = (FormDataReqInfo*)malloc(sizeof(FormDataReqInfo));
    CHECK_POINTER(info, NULL);
    memset(info, 0, sizeof(FormDataReqInfo));

    info->curl = curl_easy_init();
    CHECK_POINTER(info->curl, goto end);

    info->mime = curl_mime_init(info->curl);
    CHECK_POINTER(info->mime, goto end);

    curl_easy_setopt(info->curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(info->curl, CURLOPT_URL, url);
    curl_easy_setopt(info->curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(info->curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    curl_easy_setopt(info->curl, CURLOPT_TIMEOUT_MS, timeout);

    return info;
end:
    if (info->curl) {
        curl_easy_cleanup(info->curl);
    }

    free(info);
    return NULL;
}


void HttpFormDataRequestHeader(void* handle, const char* header) {
    FormDataReqInfo* info = handle;
    info->headers = curl_slist_append(info->headers, header);
}

void HttpFormDataRequestBody(void* handle, char* type, const char* key, const char* val, int len) {
    FormDataReqInfo* info = handle;
    curl_mimepart* part = curl_mime_addpart(info->mime);
    curl_mime_name(part, key);
    if (strcmp(type, "file") == 0) {
        curl_mime_filedata(part, val);
    } else if (strcmp(type, "text") == 0) {
        curl_mime_data(part, val, CURL_ZERO_TERMINATED);
    } else {
        curl_mime_data(part, val, len);
    }
}

int HttpFormDataExec(void* handle, char* res, int res_size){
    FormDataReqInfo* info = handle;

    HttpResponse resp;
    memset(&resp, 0, sizeof(HttpResponse));
    resp.buf = (char*)calloc(1, sizeof(char));
    CHECK_POINTER(resp.buf, return -1);

    curl_easy_setopt(info->curl, CURLOPT_HTTPHEADER, info->headers);
    curl_easy_setopt(info->curl, CURLOPT_MIMEPOST, info->mime);
    curl_easy_setopt(info->curl, CURLOPT_WRITEFUNCTION, HttpResponseCb);
    curl_easy_setopt(info->curl, CURLOPT_WRITEDATA, (void*)&resp);
    
    CURLcode curl_res = curl_easy_perform(info->curl);

    snprintf(res, res_size, "%s", resp.buf);
    if (resp.buf != NULL) {
        free(resp.buf);
    }
    
    return curl_res == CURLE_OK ? 0 : -1;
}

void HttpFormDataRequestUnInit(void* handle){
    FormDataReqInfo* info = handle;
    
    if (info->mime) {
        curl_mime_free(info->mime);
    }
    if (info->mime) {
        curl_slist_free_all(info->headers);
    }
    if (info->curl) {
        curl_easy_cleanup(info->curl);
    }
    free(info);
}