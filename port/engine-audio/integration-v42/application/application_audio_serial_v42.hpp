#pragma once
#include <cstdint>
namespace dh2::android_audio {
// Protected by registry mutex; reservation is lifetime authority only.
class ApplicationAudioSerialV42 {
    std::uint64_t current_{},next_{};
public:
    std::uint64_t current()const noexcept{return current_;}
    std::uint64_t reserve()noexcept {
        if(current_)return current_;
        if(next_==0x7fffffffffffffffULL)return 0;
        current_=++next_;return current_;
    }
    bool matches(std::uint64_t expected)const noexcept{return expected&&expected==current_;}
    void clear()noexcept {current_=0;}
};
}
