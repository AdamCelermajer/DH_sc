#include "net-session-online/glx_player_http.hpp"

#include <cstring>

#include "net-session-online/deps/glx_stand_ins.hpp"

// 0x82cedc _ZN13GLXPlayerHttp17GetSendPercentageEv
std::uint32_t GLXPlayerHttp::GetSendPercentage() const {
    if (socket_ == nullptr) {
        return 0;
    }
    return socket_->GetSendPercentage();
}

// 0x82d220 _ZN13GLXPlayerHttp16GetRequestLengthEv
// The GET path is XP_API_STRLEN on the inline buffer. The POST path returns the stored length.
std::uint32_t GLXPlayerHttp::GetRequestLength() const {
    if (sendByPost_) {
        return static_cast<std::uint32_t>(postLength_);
    }
    return static_cast<std::uint32_t>(std::strlen(request_));
}

void GLXPlayerHttp::SetRequestForTest(const char* text) {
    std::strncpy(request_, text, sizeof(request_) - 1);
    request_[sizeof(request_) - 1] = '\0';
}
