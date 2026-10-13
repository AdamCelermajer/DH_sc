// LCXPlayerSocket: the lobby-side socket wrapper. The original LCX class has its own
// layout (the ctor, Start and Cancel bodies below are the source); it is not the GLX
// class with a different name, so the offsets differ from glx_player_socket.hpp.
//
// Layout (original 32-bit offsets, kept as comments; confirmed against the ARM ctor and
// the Hex-Rays bodies):
//   +0x00 vptr                                  +0x04 state (XSocketState, see below)
//   +0x08 socket handle (int, -1 until connected) +0x0c server address (type not identified)
//   +0x10 server port (int)                     +0x14 server name (owned copy of ctor argument 1)
//   +0x18 observer                              +0x1c receive buffer, 0x800 bytes
//   +0x81c receive length (int)                 +0x820 socket type (int)
//   +0x824 accept address (owned string)        +0x828 accept port (int)
//   +0x82c content (std::string, 0x18 bytes in the original)
//   +0x845 header pending, +0x846 length known, +0x848 total length (the members are below)
//   +0x84c sent length, +0x850 send start time, +0x854 receive start time
//   +0x858 non-blocking connect state, +0x85c gethostbyname state
//   +0x860 connect start time, +0x864 gethostbyname start time
#pragma once

#include <cstdint>
#include <memory>
#include <string>

// The callback target of a socket (LCXPlayerHttp in the lobby). Run calls the four slots below in
// this order: slot 0 GetRequestData, slot 1 GetRequestLength, slot 2 SetResponseData, slot 3
// SetResponseLen (the Hex-Rays calls index the observer vtable at +0x00, +0x04, +0x08, +0x0c; the
// LCXPlayerHttp vtable in the dump has the same four entries first). The defaults are empty.
class LCXPlayerSocketObserver {
public:
    virtual ~LCXPlayerSocketObserver() = default;
    virtual const char* GetRequestData() const { return nullptr; }                       // slot 0
    virtual std::int32_t GetRequestLength() const { return 0; }                          // slot 1
    virtual void SetResponseData(const char* /*data*/, std::int32_t /*length*/) {}      // slot 2
    virtual void SetResponseLen(std::int32_t /*length*/) {}                              // slot 3
};

class LCXPlayerSocket {
public:
    // The XSocketState enumerators of the original. Values are read from the compares in
    // Cancel (0x8a01b4) and Start (0x8a02c4) in the ARM listing.
    enum XSocketState : std::int32_t {
        kStateNone = 0,              // XSOCKET_STATE_NONE
        kStateStartConnect = 1,      // XSOCKET_STATE_START_CONNECT
        kStateWaitConnecting = 2,    // XSOCKET_STATE_WAIT_CONNECTING (Run's switch: case 1, Connect)
        kStateSendRequest = 3,       // XSOCKET_STATE_SEND_REQUEST (case 2, Select(1))
        kStateWaitResponse = 4,      // XSOCKET_STATE_WAIT_RESPONSE (case 3, Select(0))
        kStateCloseConnection = 5,   // XSOCKET_STATE_CLOSE_CONNECTION (case 4, close hook)
        kStateRequestComplete = 6,   // XSOCKET_STATE_REQUEST_COMPLETE
        kStateError = 7,             // XSOCKET_STATE_ERROR
        kStateCancelled = 8,         // XSOCKET_STATE_CANCELLED
    };

    // 0x8a0314 and 0x8a03bc _ZN15LCXPlayerSocketC1EPciP23LCXPlayerSocketObserver.
    // The original does not initialise the state word; the port starts at kStateNone.
    LCXPlayerSocket(const char* serverName, std::int32_t port, LCXPlayerSocketObserver* observer);
    // 0x8a0464 (D1). D2 (0x8a051c) has the same body; D0 (0x8a0500) is the deleting variant.
    virtual ~LCXPlayerSocket() = default;

    // 0x8a02c4 _ZN15LCXPlayerSocket5StartEv, vtable +0x0c
    virtual void Start();
    // 0x8a1c1c _ZN15LCXPlayerSocket3RunEv, vtable +0x10: one step of the XSocketState machine
    // (see the cpp). The I/O it needs is the virtual set below, so the base class fails.
    virtual void Run();
    // 0x8a01b4 _ZN15LCXPlayerSocket6CancelEv, vtable +0x14
    virtual void Cancel();
    // 0x8a01f4 _ZN15LCXPlayerSocket12IsInProgressEv, vtable +0x18
    virtual bool IsInProgress() const;
    // 0x8a0220 _ZN15LCXPlayerSocket15IsErrorOccurredEv, vtable +0x1c
    virtual bool IsErrorOccurred() const { return state_ == kStateError; }
    // vtable +0x3c: the close hook, called by Cancel and by Run's error paths. LCAndroidSocket's
    // override at this slot is CloseSocket. Pure in the original base; the port makes it a no-op.
    virtual void OnCancelled() {}

    // Platform I/O that Run calls. The names are those of LCAndroidSocket's overrides in the binary
    // (vtable dump, shifted to this class's vptr: GetHostByName +0x20, CreateSocket +0x28,
    // GetLastError +0x2c, Connect +0x34, Select +0x40, Send +0x44, Recv +0x48). The base has no
    // socket, so the defaults report failure: no address, no connection, -1 for the byte counts.
    virtual const void* GetHostByName(const char* /*serverName*/) { return nullptr; }
    virtual bool CreateSocket() { return false; }
    virtual std::int32_t GetLastError() { return 0; }
    virtual bool Connect() { return false; }
    // flag 1 asks for write readiness (SEND_REQUEST), flag 0 for read readiness (WAIT_RESPONSE).
    virtual std::int32_t Select(std::int32_t /*flag*/) { return -1; }
    virtual std::int32_t Send(const char* /*data*/, std::int32_t /*length*/) { return -1; }
    virtual std::int32_t Recv(char* /*buffer*/, std::int32_t /*length*/) { return -1; }

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
    // 0x8a02fc _ZN15LCXPlayerSocket11SetAcceptIPEPc
    // The copy follows LC_API_STRNEW (0x89d31c, lc_api_net.hpp): null gives null. The previous
    // address is released (the original leaks it).
    void SetAcceptIP(const char* address);

    // 0x8a05b8 _ZN15LCXPlayerSocket15ParseHttpHeaderEPc: position of the lower-cased name
    // in the lower-cased header block (content up to the first CRLF CRLF), or -1.
    std::int32_t ParseHttpHeader(const char* name) const;
    // 0x8a090c _ZN15LCXPlayerSocket16RemoveHttpHeaderEv: drops the header block and the
    // CRLF CRLF after it, when the terminator is found after position 0.
    void RemoveHttpHeader();
    // 0x8a1534 _ZN15LCXPlayerSocket20CalculateTotalLengthEv: the Content-Length value in
    // the header block; 0 when the header has none, -1 when the block is not complete.
    std::int32_t CalculateTotalLength() const;
    // 0x8a0af0 _ZN15LCXPlayerSocket19ParseChunkedContentEv: replaces content_ with the
    // de-chunked body. Returns 1 on success and 0 on a malformed chunk. Partial, see the cpp.
    std::int32_t ParseChunkedContent();

    // Not in the original: test seams.
    void SetStateForTest(std::int32_t state) { state_ = state; }
    std::int32_t StateForTest() const { return state_; }
    void SetContentForTest(const std::string& text) { content_ = text; }
    const std::string& ContentForTest() const { return content_; }

private:
    std::int32_t state_ = kStateNone;               // +0x04
    std::int32_t socket_ = -1;                      // +0x08
    const void* serverAddress_ = nullptr;           // +0x0c
    std::int32_t serverPort_ = 0;                   // +0x10
    std::unique_ptr<char[]> serverName_;            // +0x14
    LCXPlayerSocketObserver* observer_ = nullptr;   // +0x18
    char receiveBuffer_[0x800] = {};                // +0x1c
    std::int32_t receiveLength_ = 0;                // +0x81c
    std::int32_t socketType_ = 0;                   // +0x820
    std::unique_ptr<char[]> acceptIp_;              // +0x824
    std::int32_t acceptPort_ = 0;                   // +0x828
    std::string content_;                           // +0x82c
    bool receiveHttpHeader_ = false;                // +0x845 header still to parse (m_bReceiveHttpHeader)
    bool lengthKnown_ = false;                      // +0x846 Content-Length was read (m_bLengthKnown)
    std::int32_t totalLength_ = 0;                  // +0x848 (m_total_length)
    std::int32_t sentLength_ = 0;                   // +0x84c
    std::int32_t sendStartTime_ = 0;                // +0x850
    std::int32_t recvStartTime_ = 0;                // +0x854
    std::int32_t nonBlockConnectState_ = 0;         // +0x858
    std::int32_t gethostbynameState_ = 0;           // +0x85c
    std::int32_t connectStartTime_ = 0;             // +0x860
    std::int32_t gethostbynameStartTime_ = 0;       // +0x864
};

// 0x89e57c _ZN22LCXPlayerSocketFactory9GetSocketEPciP23LCXPlayerSocketObserver
// Partial: the original builds LCAndroidSocket (0xcf0 bytes). The port builds the base class,
// which has no I/O.
class LCXPlayerSocketFactory {
public:
    static LCXPlayerSocket* GetSocket(const char* serverName, std::int32_t port,
                                      LCXPlayerSocketObserver* observer);
};
