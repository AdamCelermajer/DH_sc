#include "core-platform/sys_print.hpp"

#include <cstdarg>
#include <cstdio>
#include <ctime>

namespace {
// Formats the message into a 1 KiB buffer (vsnprintf size 0x400 in the original).
void FormatMessage(char* buffer, unsigned size, const char* fmt, va_list args) {
    std::vsnprintf(buffer, size, fmt, args);
}

// Writes "[HH:MM:SS] " as the 32-byte timestamp prefix.
void PrintTimestamp() {
    char stamp[32];
    std::time_t now = std::time(nullptr);
    const std::tm* tm = std::localtime(&now);
    std::strftime(stamp, sizeof stamp, "[%H:%M:%S] ", tm);
    sys::printNoFormat(stamp);
}
}  // namespace

namespace sys {

// 0x317848 sys::printNoFormat(char const*)
// The original passes the text as the printf format string; this uses "%s" so
// that a '%' in a message is printed literally.
void printNoFormat(const char* text) { std::printf("%s", text); }

// 0x31784c sys::print(char const*, ...)
// Partial: the message is formatted with "%s" in printNoFormat (see above), so
// output differs from the original for messages containing '%'.
void print(const char* fmt, ...) {
    char message[1024];
    va_list args;
    va_start(args, fmt);
    FormatMessage(message, sizeof message, fmt, args);
    va_end(args);
    PrintTimestamp();
    printNoFormat(message);
}

// 0x317914 sys::println(char const*, ...)
// Partial, same caveat as print().
void println(const char* fmt, ...) {
    char message[1024];
    va_list args;
    va_start(args, fmt);
    FormatMessage(message, sizeof message, fmt, args);
    va_end(args);
    PrintTimestamp();
    printNoFormat(message);
    printNoFormat("\n");
}

}  // namespace sys
