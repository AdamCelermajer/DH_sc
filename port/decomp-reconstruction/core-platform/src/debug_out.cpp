#include "core-platform/debug_out.hpp"

#include <cstdarg>
#include <cstdio>

#if defined(__ANDROID__)
#include <android/log.h>
#endif

// 0x324114 _Z10_DEBUG_OUTPKcz
// The original calls vsprintf into a 1024-byte stack buffer; vsnprintf keeps the
// same buffer size without overrunning it.
void _DEBUG_OUT(const char* format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    std::vsnprintf(buffer, sizeof buffer, format, args);
    va_end(args);
    std::printf("%s", buffer);
    appDebugLog("_DEBUG_OUT", buffer);
}

// 0x533560 appDebugLog
// Partial: the original is a single __android_log_write(ANDROID_LOG_INFO, tag,
// text) call (the listing's _android_log_write(4, ...); 4 is ANDROID_LOG_INFO),
// and the Android build makes that call. Other targets have no logcat, so they
// write to stderr. The original returns the write's int; this version returns void.
void appDebugLog(const char* tag, const char* text) {
#if defined(__ANDROID__)
    __android_log_write(ANDROID_LOG_INFO, tag, text);
#else
    std::fprintf(stderr, "%s: %s\n", tag, text);
#endif
}
