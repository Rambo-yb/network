#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#define LOG_ERR(format, ...) \
    printf("\e[0;31m[%s:%d %s]"format, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
    
#define LOG_WRN(format, ...) \
    printf("\e[0;33m[%s:%d %s]"format, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
    
#define LOG_INFO(format, ...) \
    printf("\e[0m[%s:%d %s]"format, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#endif