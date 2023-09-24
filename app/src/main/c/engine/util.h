#ifndef LUNA_ENGINE_UTIL_H
#define LUNA_ENGINE_UTIL_H

#include <android/log.h>

#define LOG(level, ...) __android_log_print(level, "luna", __VA_ARGS__)
#define LOGI(...) LOG(ANDROID_LOG_INFO, __VA_ARGS__)
#define LOGW(...) LOG(ANDROID_LOG_WARN, __VA_ARGS__)
#define LOGE(...) LOG(ANDROID_LOG_ERROR, __VA_ARGS__)

#ifdef __DEBUG__
#   define LOGD(...) LOG(ANDROID_LOG_DEBUG, __VA_ARGS__)
#else
#   define LOGD(...)
#endif //__DEBUG__

#ifdef true
#   undef true
#endif

#ifdef false
#   undef false
#endif

#ifdef bool
#   undef bool
#endif

typedef enum: char {
    true = 1,
    TRUE = 1,
    false = 0,
    FALSE = 0
} bool;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

#endif //LUNA_ENGINE_UTIL_H
