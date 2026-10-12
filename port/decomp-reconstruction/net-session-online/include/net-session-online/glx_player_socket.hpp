// GLXPlayerSocket: the socket wrapper of the GLX player services. It collects the HTTP
// reply in content_, strips and decodes the headers and chunked body, and reports the
// send progress. The socket I/O itself (Run and the platform socket that the factory
// creates in the original) is not reconstructed.
//
// Layout (original 32-bit offsets, kept as comments; confirmed against the ARM ctor):
//   +0x00 vptr
//   +0x04 state (int): 0 idle, 1 started, 6 finished, 7 error, 8 cancelled, others in progress
//   +0x08 socket handle (int, -1 until a connection is made)
//   +0x0c not decoded (zeroed by the ctor and by Start)
//   +0x10 port (int, ctor argument 2)
//   +0x14 host name (owned copy of ctor argument 1)
//   +0x18 observer (GLXPlayerSocketObserver*, ctor argument 3)
//   +0x1c receive buffer, 0x800 bytes (zeroed by Start)
//   +0x81c receive buffer length (int, zeroed by Start)
//   +0x820 socket type (int)
//   +0x824 accept address (owned string)
//   +0x828 accept port (int)
//   +0x82c received content (std::string; the STL private layout is not modelled)
//   +0x84c, +0x854, +0x858, +0x85c, +0x860, +0x864, +0x868 not decoded (zeroed by the ctor)
//   +0x850 send percentage (uint; values above 100 read as 0)
#pragma once

#include <cstdint>
#include <memory>
#include <string>

// The callback target of a socket. The observer methods of the original are not recovered.
class GLXPlayerSocketObserver {
public:
    virtual ~GLXPlayerSocketObserver() = default;
};

class GLXPlayerSocket {
public:
    // 0x8302f0 and 0x83039c: GLXPlayerSocket(char*, int, GLXPlayerSocketObserver*)
    GLXPlayerSocket(const char* host, std::int32_t port, GLXPlayerSocketObserver* observer);
    // 0x8301e4 (D1) and 0x83026c (D2). The deleting destructor 0x830250 is not ported.
    virtual ~GLXPlayerSocket() = default;

    // 0x8301ac _ZN15GLXPlayerSocket5StartEv, vtable +0x0c
    virtual void Start();
    // vtable +0x10: the transport step. The platform socket of the original provides it.
    virtual void Run() {}
    // 0x83007c _ZN15GLXPlayerSocket6CancelEv, vtable +0x14
    virtual void Cancel();
    // 0x8300bc _ZN15GLXPlayerSocket12IsInProgressEv, vtable +0x18
    virtual std::int32_t IsInProgress() const;
    // 0x8300e8 _ZN15GLXPlayerSocket15IsErrorOccurredEv, vtable +0x1c
    virtual bool IsErrorOccurred() const { return state_ == 7; }
    // vtable +0x58 and +0x74: the platform hooks the ping probe calls. Pure in the original
    // base. The port's defaults report failure (-1) for the send and no connection (0).
    virtual std::int32_t SendTo(const void* data, std::int32_t length, const char* address,
                                std::int32_t port) {
        (void)data; (void)length; (void)address; (void)port;
        return -1;
    }
    virtual std::int32_t Connect() { return 0; }
    // vtable +0x3c: called by Cancel. Pure in the original base; the port makes it a no-op.
    virtual void OnCancelled() {}

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
    // 0x8302d8 _ZN15GLXPlayerSocket11SetAcceptIPEPc
    // Copies the address into a new owned string. The original leaks the previous one,
    // and this port releases it.
    void SetAcceptIP(const char* address);

    // 0x8305bc _ZN15GLXPlayerSocket16RemoveHttpHeaderEv: drops everything up to and
    // including the first CRLF CRLF, if that sequence is found after position 0.
    void RemoveHttpHeader();
    // 0x830448 _ZN15GLXPlayerSocket15ParseHttpHeaderEPc: position of the lower-cased name
    // inside the lower-cased header block, or npos as a 32-bit value.
    std::uint32_t ParseHttpHeader(const char* name) const;
    // 0x830cb0 _ZN15GLXPlayerSocket19CalculateTotalLengthEv: the Content-Length header
    // value, 0 when there is none, -1 when the header block is not complete.
    std::int32_t CalculateTotalLength() const;
    // 0x83067c _ZN15GLXPlayerSocket18ParseChunkedContentEv: replaces content_ with the
    // de-chunked body. Returns 1 on success and 0 on a malformed chunk.
    std::int32_t ParseChunkedContent();

    // Not in the original: test seams.
    void SetStateForTest(std::int32_t state) { state_ = state; }
    void SetSendPercentageForTest(std::uint32_t percent) { sendPercentage_ = percent; }
    void SetContentForTest(const std::string& text) { content_ = text; }
    const std::string& ContentForTest() const { return content_; }

private:
    std::int32_t state_ = 0;                        // +0x04
    std::int32_t socket_ = -1;                      // +0x08
    std::int32_t field0c_ = 0;                      // +0x0c
    std::int32_t port_ = 0;                         // +0x10
    std::unique_ptr<char[]> host_;                  // +0x14
    GLXPlayerSocketObserver* observer_ = nullptr;   // +0x18
    char receiveBuffer_[0x800] = {};                // +0x1c
    std::int32_t receiveLength_ = 0;                // +0x81c
    std::int32_t socketType_ = 0;                   // +0x820
    std::unique_ptr<char[]> acceptIp_;              // +0x824
    std::int32_t acceptPort_ = 0;                   // +0x828
    std::string content_;                           // +0x82c
    std::int32_t field84c_ = 0;                     // +0x84c
    std::int32_t field854_ = 0;                     // +0x854
    std::int32_t field858_ = 0;                     // +0x858
    std::int32_t field85c_ = 0;                     // +0x85c (cleared by Cancel)
    std::int32_t field860_ = 0;                     // +0x860
    std::int32_t field864_ = 0;                     // +0x864
    std::int32_t field868_ = 0;                     // +0x868
    std::uint32_t sendPercentage_ = 0;              // +0x850
};

// 0x831528 _ZN22GLXPlayerSocketFactory9GetSocketEPciP23GLXPlayerSocketObserver
// The original builds a platform socket (CAndroidSocket, 0xcf4 bytes). The port builds
// the base class, which has no I/O.
class GLXPlayerSocketFactory {
public:
    static GLXPlayerSocket* GetSocket(const char* host, std::int32_t port,
                                      GLXPlayerSocketObserver* observer);
};
