#ifndef __CHECK_COMMON_H__
#define __CHECK_COMMON_H__

#include "log.h"

#define CHECK_POINTER(x, ret)   \
    if (x == NULL) {    \
        LOG_ERR("%s is NULL\n", #x); \
        return ret; \
    }   
    
#define CHECK_POINTER_GO(x, e)   \
    if (x == NULL) {    \
        LOG_ERR("%s is NULL\n", #x); \
        goto e; \
    }   
    
#define CHECK_BOOL(x, ret)   \
    if (!x) {    \
        return ret; \
    }

#define CHECK_BOOL_GO(x, e)   \
    if (!x) {    \
        goto e; \
    }

#endif