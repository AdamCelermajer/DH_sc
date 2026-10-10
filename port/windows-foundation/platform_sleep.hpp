#pragma once

#include <chrono>
#include <cstdint>
#include <thread>

namespace dh::foundation {

inline void platform_sleep_milliseconds(unsigned milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

} // namespace dh::foundation

// The shared Windows-era host uses GetTickCount for monotonic millisecond
// timestamps. Preserve its 32-bit wrap semantics on Linux without changing
// gameplay callers.
#if !defined(_WIN32)
inline std::uint32_t GetTickCount() noexcept {
    using namespace std::chrono;
    return static_cast<std::uint32_t>(duration_cast<milliseconds>(
        steady_clock::now().time_since_epoch()).count());
}
#endif
