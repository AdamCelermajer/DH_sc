#include "net-session-online/lcx_player_http.hpp"

#include <cstring>

// 0x89d484 _ZN13LCXPlayerHttp16GetRequestLengthEv
// The GET path is LC_API_STRLEN (a strlen wrapper) on the inline buffer.
std::uint32_t LCXPlayerHttp::GetRequestLength() const {
    if (sendByPost_) {
        return static_cast<std::uint32_t>(postLength_);
    }
    return static_cast<std::uint32_t>(std::strlen(request_));
}

void LCXPlayerHttp::SetRequestForTest(const char* text) {
    std::strncpy(request_, text, sizeof(request_) - 1);
    request_[sizeof(request_) - 1] = '\0';
}
