#ifndef __LOGGER_H__
#define __LOGGER_H__
#include "common.h"


void print_hex_string(unsigned char *buf, int len, const char *str);

int print_ts_string(struct timeval ts, const char *str);




















#define INFO_OUTPUT         3
#define WARNING_OUTPUT      2
#define DEBUG_OUTPUT        1
#define ERROR_OUTPUT        0

#define DEBUG
#define DEBUG_LEVEL         INFO_OUTPUT

#define PRINT(info, ...) do{ \
    printf("[Info] (%s:%d->%s):" info"", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
}while(0)

#define INFO_PRINT(info, ...) do{ \
    if(DEBUG_LEVEL>=INFO_OUTPUT){ \
        printf("[Info] (%s:%d->%s):" info"", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
    } \
}while(0)

#define WARNING_PRINT(info, ...) do{ \
    if(DEBUG_LEVEL>=WARNING_OUTPUT){ \
        printf("[Warning] (%s:%d->%s):" info"", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
    } \
}while(0)

#define DEBUG_PRINT(info,...) do{ \
    if(DEBUG_LEVEL>=DEBUG_OUTPUT){ \
        printf("[Debug] (%s:%d->%s):" info"",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__); \
    } \
}while(0)

#define ERROR_PRINT(info, ...) do{ if(DEBUG_LEVEL>=ERROR_OUTPUT){ printf("[Error] (%s:%d->%s):" info"", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); } }while(0)

// #define debugsit    printf("[debug][%s:%d]:", __FUNCTION__, __LINE__);
#define debugsit    std::cout << "[debug][" << __FUNCTION__ << ":" << __LINE__ << "]:" << std::endl;

#endif /* __LOGGER_H__ */
