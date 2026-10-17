#pragma once

#include <cstdarg>
#include <cstdio>

#define ANDROID_LOG_UNKNOWN 0
#define ANDROID_LOG_DEFAULT 1
#define ANDROID_LOG_VERBOSE 2
#define ANDROID_LOG_DEBUG 3
#define ANDROID_LOG_INFO 4
#define ANDROID_LOG_WARN 5
#define ANDROID_LOG_ERROR 6
#define ANDROID_LOG_FATAL 7
#define ANDROID_LOG_SILENT 8

inline int __android_log_vprint(int priority, const char* tag,
                                const char* format, std::va_list args) {
    const char* level = priority >= ANDROID_LOG_FATAL ? "F" :
                        priority >= ANDROID_LOG_ERROR ? "E" :
                        priority >= ANDROID_LOG_WARN ? "W" :
                        priority >= ANDROID_LOG_INFO ? "I" : "D";
    std::fprintf(stderr, "[%s/%s] ", level, tag ? tag : "android");
    const int written = std::vfprintf(stderr, format, args);
    std::fputc('\n', stderr);
    return written;
}

inline int __android_log_print(int priority, const char* tag,
                               const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    const int written = __android_log_vprint(priority, tag, format, args);
    va_end(args);
    return written;
}

inline int __android_log_write(int priority, const char* tag, const char* text) {
    return __android_log_print(priority, tag, "%s", text ? text : "");
}
