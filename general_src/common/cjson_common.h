#ifndef __CJSON_COMMON_H__
#define __CJSON_COMMON_H__

#include "cJSON.h"

#define CJSON_GET_STRING(json, key, val, size, e) \
    do {    \
        cJSON* root = cJSON_GetObjectItem(json, key); \
        if (root == NULL || !cJSON_IsString(root)) {    \
            goto e; \
        }   \
        snprintf(val, size, "%s", cJSON_GetStringValue(root));  \
    }while(0);

#define CJSON_GET_NUMBER(json, key, val, size, e) \
    do {    \
        cJSON* root = cJSON_GetObjectItem(json, key); \
        if (root == NULL || !cJSON_IsNumber(root)) {    \
            goto e; \
        }   \
        val = cJSON_GetNumberValue(root); \
    }while(0);

#define CJSON_SET_STRING(json, key, val, e)   \
    do {    \
        cJSON* root = cJSON_CreateString(val);  \
        if (root == NULL) { \
            goto e; \
        }   \
        if (!cJSON_AddItemToObject(json, key, root)) {  \
            cJSON_free(root);   \
            goto e; \
        }   \
    } while (0);

#define CJSON_SET_NUMBER(json, key, val, e)   \
    do {    \
        cJSON* root = cJSON_CreateNumber(val);  \
        if (root == NULL) { \
            goto e; \
        }   \
        if (!cJSON_AddItemToObject(json, key, root)) {  \
            cJSON_free(root);   \
            goto e; \
        }   \
    } while (0);

#endif