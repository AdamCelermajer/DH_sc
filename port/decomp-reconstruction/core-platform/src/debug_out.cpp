#include "core-platform/debug_out.hpp"

#include <cstdarg>
#include <cstdio>

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
// text) call. There is no logcat on the Windows target, so stderr is used.
void appDebugLog(const char* tag, const char* text) {
    std::fprintf(stderr, "%s: %s\n", tag, text);
}
