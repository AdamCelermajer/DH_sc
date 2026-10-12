// Stand-ins for the XP_API_* / LC_API_* platform layer (owned by another category).
// The debug trace (XP_DEBUG_OUT) is not reproduced.
#pragma once

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>

// XP_API_GET_TIME: the millisecond clock truncated to 32 bits.
inline std::int32_t XP_API_GET_TIME() {
    auto now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<std::int32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}

// XP_API_GET_RANDOM: a random 32-bit value. Stand-in: the C library generator.
inline std::int32_t XP_API_GET_RANDOM(void*) {
    return static_cast<std::int32_t>(std::rand());
}

// XP_API_STRLEN: the length as a 16-bit value, as the packet fields take it.
inline std::int16_t XP_API_STRLEN(const char* text) {
    return static_cast<std::int16_t>(std::strlen(text));
}

// LC_API_FREE_0 and CustomFree: the original's allocator frees.
inline void LC_API_FREE_0(void* p) { std::free(p); }
inline void CustomFree(void* p) { std::free(p); }
