#include "net-session-online/glx_player_socket.hpp"

#include <cstring>

// 0x83006c _ZN15GLXPlayerSocket17GetSendPercentageEv
std::uint32_t GLXPlayerSocket::GetSendPercentage() const {
    return sendPercentage_ > 100 ? 0 : sendPercentage_;
}

// 0x8302d8 _ZN15GLXPlayerSocket11SetAcceptIPEPc
// The original calls XP_API_STRNEW (operator new[]) on the argument, so the argument must
// not be null. The port copies a null argument as an empty string.
void GLXPlayerSocket::SetAcceptIP(const char* address) {
    const char* text = address != nullptr ? address : "";
    const std::size_t length = std::strlen(text);
    acceptIp_.reset(new char[length + 1]);
    std::memcpy(acceptIp_.get(), text, length + 1);
}
