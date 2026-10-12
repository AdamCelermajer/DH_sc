// Track B batch e1-net-session-online-lib_online_glx-0: lib-online-glx platform clock wrapper.
//
// XP_API_GET_TIME is the only body in this batch that has a Hex-Rays pseudocode. The
// FUN_008b1704 queue row is not a function entry (see the ledger note), so it is skipped.
//
// Not declared in xp_api.hpp or deps/glx_stand_ins.hpp on purpose. glx_stand_ins.hpp holds an
// inline stub `std::uint32_t XP_API_GET_TIME() { return 0; }` that several GLX sources include
// together with xp_api.hpp. A second declaration with a different return type would not
// compile there. Integration request: replace that stub with `int XP_API_GET_TIME();` declared
// in xp_api.hpp, so the GLX sources call this body. Until then the inline stub still wins in
// those translation units.
#include <cstdint>

#include "net-session-online/deps/time_of_day.hpp"

// 0x82b138 XP_API_GET_TIME
// int XP_API_GET_TIME()
// {
//     struct timeval v1;
//     gettimeofday(&v1, nullptr);
//     return v1.tv_usec / 1000 + 1000 * v1.tv_sec;
// }
// The ARM listing agrees: gettimeofday(sp, 0), then tv_usec / 1000 by a multiply-high
// (0x10624DD3 >> 6), then 1000 * tv_sec added in 32-bit registers. The fields are 32-bit on
// the device, so the sum wraps modulo 2^32 and is returned as a signed int. Timeval is the
// 32-bit stand-in from deps/time_of_day.hpp, and GetTimeOfDay fills it from the host clock.
int XP_API_GET_TIME() {
    Timeval now{};                                              // +0x00 tv_sec, +0x04 tv_usec
    GetTimeOfDay(&now);
    const std::uint32_t msPart = static_cast<std::uint32_t>(now.tv_usec / 1000);
    const std::uint32_t secPart = static_cast<std::uint32_t>(now.tv_sec) * 1000u;
    return static_cast<int>(msPart + secPart);                  // 32-bit wrap as on ARM
}
