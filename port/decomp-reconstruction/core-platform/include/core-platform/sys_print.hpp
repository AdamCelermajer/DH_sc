// sys::print family: timestamped console output.
#pragma once

namespace sys {
void printNoFormat(const char* text);       // 0x317848
void print(const char* fmt, ...);           // 0x31784c: "[HH:MM:SS] " + message
void println(const char* fmt, ...);         // 0x317914: as print plus newline
}  // namespace sys
