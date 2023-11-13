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

#define internal __attribute__((visibility("hidden"))) static

#endif //LUNA_ENGINE_UTIL_H
