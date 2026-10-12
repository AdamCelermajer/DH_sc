#include "net-session-online/lcx_player_socket.hpp"

#include <cstring>

// 0x8a02fc _ZN15LCXPlayerSocket11SetAcceptIPEPc
// Partial: see the header. A null argument is copied as an empty string here.
void LCXPlayerSocket::SetAcceptIP(const char* address) {
    const char* text = address != nullptr ? address : "";
    const std::size_t length = std::strlen(text);
    acceptIp_.reset(new char[length + 1]);
    std::memcpy(acceptIp_.get(), text, length + 1);
}
