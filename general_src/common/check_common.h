#ifndef __CHECK_COMMON_H__
#define __CHECK_COMMON_H__

#include "log.h"

#define CHECK_POINTER(x, ret)   \
    if (x == NULL) {    \
        LOG_ERR("%s is NULL", #x); \
        return ret; \
    }   
    
#define CHECK_POINTER_GO(x, e)   \
    if (x == NULL) {    \
        LOG_ERR("%s is NULL", #x); \
        goto e; \
    }   
    
#define CHECK_BOOL(x, ret)   \
    if (!x) {    \
        LOG_ERR("%s is false", #x); \
        return ret; \
    }

#define CHECK_BOOL_GO(x, e)   \
    if (!x) {    \
        LOG_ERR("%s is false", #x); \
        goto e; \
    }

#define CHECK_LT(x, y, ret)   \
    if (x < y) {    \
        LOG_ERR("%s:%d < %s:%d", #x, x, #y, y);   \
        return ret; \
    }

#define CHECK_LT_GO(x, y, e)   \
    if (x < y) {    \
        LOG_ERR("%s:%d < %s:%d", #x, x, #y, y);   \
        goto e; \
    }

#define CHECK_GE(x, y, ret)   \
    if (x >= y) {    \
        LOG_ERR("%s:%d >= %s:%d", #x, x, #y, y);   \
        return ret; \
    }

#define CHECK_GE_GO(x, y, e)   \
    if (x >= y) {    \
        LOG_ERR("%s:%d >= %s:%d", #x, x, #y, y);   \
        goto e; \
    }

#define CHECK_EQ(x, y, ret)   \
    if (x != y) {    \
        LOG_ERR("%s:%d != %s:%d", #x, x, #y, y);   \
        return ret; \
    }

#define CHECK_EQ_GO(x, y, e)   \
    if (x != y) {    \
        LOG_ERR("%s:%d != %s:%d", #x, x, #y, y);   \
        goto e; \
    }

#endif