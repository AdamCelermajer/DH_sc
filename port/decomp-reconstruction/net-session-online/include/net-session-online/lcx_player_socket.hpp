// LCXPlayerSocket: the lobby-side socket wrapper. Its layout and accessors match
// GLXPlayerSocket (see glx_player_socket.hpp); the original keeps a separate class for
// the lobby services, so this one does too.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x04 state (int; 7 means an error occurred)   +0x08 socket handle (int)
//   +0x820 socket type (int)                       +0x824 accept address (owned string)
//   +0x828 accept port (int)
#pragma once

#include <cstdint>
#include <memory>

class LCXPlayerSocket {
public:
    LCXPlayerSocket() = default;
    virtual ~LCXPlayerSocket() = default;  // 0x8a0500 is queued

    // 0x8a0184 _ZN15LCXPlayerSocket11GetAcceptIPEv
    const char* GetAcceptIP() const { return acceptIp_.get(); }
    // 0x8a018c _ZN15LCXPlayerSocket13GetAcceptPortEv
    std::int32_t GetAcceptPort() const { return acceptPort_; }
    // 0x8a0194 _ZN15LCXPlayerSocket13SetAcceptPortEi
    void SetAcceptPort(std::int32_t port) { acceptPort_ = port; }
    // 0x8a019c _ZN15LCXPlayerSocket9SetSocketEi
    void SetSocket(std::int32_t handle) { socket_ = handle; }
    // 0x8a01a4 _ZN15LCXPlayerSocket13GetSocketTypeEv
    std::int32_t GetSocketType() const { return socketType_; }
    // 0x8a01ac _ZN15LCXPlayerSocket13SetSocketTypeEi
    void SetSocketType(std::int32_t type) { socketType_ = type; }
    // 0x8a0220 _ZN15LCXPlayerSocket15IsErrorOccurredEv
    bool IsErrorOccurred() const { return state_ == 7; }
    // 0x8a02fc _ZN15LCXPlayerSocket11SetAcceptIPEPc
    // Partial: the original copies with LC_API_STRNEW (0x89d31c, queued). This copy uses
    // the same new[] logic, and the previous address is released (the original leaks it).
    void SetAcceptIP(const char* address);

    // Not in the original: test seams.
    void SetStateForTest(std::int32_t state) { state_ = state; }

private:
    std::int32_t state_ = 0;            // +0x04
    std::int32_t socket_ = 0;           // +0x08
    std::int32_t socketType_ = 0;       // +0x820
    std::unique_ptr<char[]> acceptIp_;  // +0x824
    std::int32_t acceptPort_ = 0;       // +0x828
};
