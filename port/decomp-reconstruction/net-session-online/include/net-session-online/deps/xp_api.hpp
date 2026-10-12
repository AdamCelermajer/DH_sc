// Stand-ins for the XP_API_* platform calls used by the lobby layer. Each one
// names the original call it replaces. The real implementations belong to the
// platform layer, which is outside this category.
#pragma once

#include <cstdio>

// XP_DEBUG_OUT(fmt, ...): debug trace. Prints to stderr in host builds.
template <typename... Args>
inline void XP_DEBUG_OUT(const char* fmt, Args... args) {
    std::fprintf(stderr, fmt, args...);
}
