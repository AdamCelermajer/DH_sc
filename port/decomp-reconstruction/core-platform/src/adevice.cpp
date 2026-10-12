#include "core-platform/adevice.hpp"

#include "core-platform/time_util.hpp"

// 0x52eea8 ADevice::currentTime()
// Partial: the original reads gettimeofday and returns sec * 1000 + usec / 1000 in the
// low word. TimeNow() gives the same millisecond count on every platform, so it is used.
std::int64_t ADevice::currentTime() { return static_cast<std::int64_t>(TimeNow()); }
