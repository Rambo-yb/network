#ifndef __CHECK_COMMON_H__
#define __CHECK_COMMON_H__

#include "log.h"

#define CHECK_POINTER(x, opr)   \
    if (x == NULL) {    \
        LOG_ERR("%s is NULL", #x); \
        opr; \
    } 
    
#define CHECK_BOOL(x, opr)   \
    if (!x) {    \
        LOG_ERR("%s is false", #x); \
        opr; \
    }

#define CHECK_LT(x, y, opr)   \
    if (x < y) {    \
        LOG_ERR("%s:%d < %s:%d", #x, x, #y, y);   \
        opr; \
    }

#define CHECK_GE(x, y, opr)   \
    if (x >= y) {    \
        LOG_ERR("%s:%d >= %s:%d", #x, x, #y, y);   \
        opr; \
    }

#define CHECK_EQ(x, y, opr)   \
    if (x != y) {    \
        LOG_ERR("%s:%d != %s:%d", #x, x, #y, y);   \
        opr; \
    }

#endif