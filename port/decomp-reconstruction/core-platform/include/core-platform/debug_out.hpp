// Debug text output: _DEBUG_OUT formats a line, prints it and forwards it to
// appDebugLog.
#pragma once

// 0x324114 _Z10_DEBUG_OUTPKcz. The text is limited to a 1 KiB buffer.
void _DEBUG_OUT(const char* format, ...);

// 0x533560 appDebugLog(char const*, char const*). The original writes the tag
// and text to Android logcat at info priority. Partial: it writes to stderr here.
void appDebugLog(const char* tag, const char* text);
