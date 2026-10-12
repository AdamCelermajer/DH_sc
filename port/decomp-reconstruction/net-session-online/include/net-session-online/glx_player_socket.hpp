// GLXPlayerSocket: the socket wrapper of the GLX player services. The accept address
// and port are set by the server, the type selects the transport, and the send
// percentage reports progress. Only the accessors are reconstructed. The socket
// creation and I/O are not (they are net-transport code).
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x04 state (int; 7 means an error occurred)
//   +0x08 socket handle (int)
//   +0x820 socket type (int)
//   +0x824 accept address (owned string)
//   +0x828 accept port (int)
//   +0x850 send percentage (uint; values above 100 read as 0)
#pragma once

#include <cstdint>
#include <memory>

class GLXPlayerSocket {
public:
    GLXPlayerSocket() = default;
    virtual ~GLXPlayerSocket() = default;  // 0x830250 is queued

    // 0x83003c _ZN15GLXPlayerSocket11GetAcceptIPEv
    const char* GetAcceptIP() const { return acceptIp_.get(); }
    // 0x830044 _ZN15GLXPlayerSocket13GetAcceptPortEv
    std::int32_t GetAcceptPort() const { return acceptPort_; }
    // 0x83004c _ZN15GLXPlayerSocket13SetAcceptPortEi
    void SetAcceptPort(std::int32_t port) { acceptPort_ = port; }
    // 0x830054 _ZN15GLXPlayerSocket9SetSocketEi
    void SetSocket(std::int32_t handle) { socket_ = handle; }
    // 0x83005c _ZN15GLXPlayerSocket13GetSocketTypeEv
    std::int32_t GetSocketType() const { return socketType_; }
    // 0x830064 _ZN15GLXPlayerSocket13SetSocketTypeEi
    void SetSocketType(std::int32_t type) { socketType_ = type; }
    // 0x83006c _ZN15GLXPlayerSocket17GetSendPercentageEv: values above 100 read as 0.
    std::uint32_t GetSendPercentage() const;
    // 0x8300e8 _ZN15GLXPlayerSocket15IsErrorOccurredEv
    bool IsErrorOccurred() const { return state_ == 7; }
    // 0x8302d8 _ZN15GLXPlayerSocket11SetAcceptIPEPc
    // Copies the address into a new owned string. The original leaks the previous one,
    // and this port releases it.
    void SetAcceptIP(const char* address);

    // Not in the original: test seams.
    void SetStateForTest(std::int32_t state) { state_ = state; }
    void SetSendPercentageForTest(std::uint32_t percent) { sendPercentage_ = percent; }

private:
    std::int32_t state_ = 0;                 // +0x04
    std::int32_t socket_ = 0;                // +0x08
    std::int32_t socketType_ = 0;            // +0x820
    std::unique_ptr<char[]> acceptIp_;       // +0x824
    std::int32_t acceptPort_ = 0;            // +0x828
    std::uint32_t sendPercentage_ = 0;       // +0x850
};
