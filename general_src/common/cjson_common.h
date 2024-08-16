#ifndef __CJSON_COMMON_H__
#define __CJSON_COMMON_H__

#include "check_common.h"
#include "cJSON.h"

#define CJSON_GET_STRING(json, key, val, size, e) \
    do {    \
        cJSON* root = cJSON_GetObjectItemCaseSensitive(json, key); \
        CHECK_POINTER(root, e); \
        CHECK_BOOL(cJSON_IsString(root), e); \
        snprintf(val, size, "%s", cJSON_GetStringValue(root));  \
    }while(0);

#define CJSON_GET_NUMBER(json, key, val, size, e) \
    do {    \
        cJSON* root = cJSON_GetObjectItemCaseSensitive(json, key); \
        CHECK_POINTER(root, e); \
        CHECK_BOOL(cJSON_IsNumber(root), e); \
        val = cJSON_GetNumberValue(root); \
    }while(0);

#define CJSON_GET_NUMBER_LIST(json, key, val, size, list_size, e)   \
    do {    \
        cJSON* root = cJSON_GetObjectItemCaseSensitive(json, key);  \
        CHECK_POINTER(root, e); \
        CHECK_BOOL(cJSON_IsArray(root), e); \
        size = cJSON_GetArraySize(root);    \
        for(int ii = 0; ii < size && ii < list_size; ii++) {    \
            cJSON* item = cJSON_GetArrayItem(root, ii); \
            CHECK_POINTER(item, e); \
            CHECK_BOOL(cJSON_IsNumber(item), e);    \
            val[ii] = cJSON_GetNumberValue(item);   \
        }   \
    }while(0);

#define CJSON_SET_STRING(json, key, val, e)   \
    do {    \
        cJSON* root = cJSON_CreateString(val);  \
        CHECK_POINTER(root, e); \
        if (!cJSON_AddItemToObject(json, key, root)) {  \
            cJSON_free(root);   \
            e; \
        }   \
    } while (0);

#define CJSON_SET_NUMBER(json, key, val, e)   \
    do {    \
        cJSON* root = cJSON_CreateNumber(val);  \
        CHECK_POINTER(root, e); \
        if (!cJSON_AddItemToObject(json, key, root)) {  \
            cJSON_free(root);   \
            e; \
        }   \
    } while (0);

#endif