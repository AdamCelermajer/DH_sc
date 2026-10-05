#pragma once
#include <algorithm>
#include <chrono>
#include <cstdint>

namespace dh2::android_ui {
// The source input API takes whole milliseconds. Retain the unused fraction
// across rendered frames so a display's refresh rate does not change time.
class MenuFrameClock {
public:
    void reset() { remainder_=0; }
    std::int32_t advance(std::chrono::nanoseconds elapsed) {
        const auto bounded=std::clamp<std::int64_t>(elapsed.count(),0,100000000);
        const auto total=bounded+remainder_;
        remainder_=total%1000000;
        return static_cast<std::int32_t>(total/1000000);
    }
private:
    std::int64_t remainder_=0;
};
}
