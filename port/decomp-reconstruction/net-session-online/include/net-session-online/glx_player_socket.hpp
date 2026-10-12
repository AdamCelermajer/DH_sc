// GLXPlayerSocket: the socket wrapper of the GLX player services. It collects the HTTP
// reply in content_, strips and decodes the headers and chunked body, and reports the
// send progress. Run is the connection state machine. Its transport calls (open, connect,
// poll, send, receive, close) are virtual hooks; the platform socket that the factory
// creates in the original is not reconstructed, so the base hooks report failure.
//
// Layout (original 32-bit offsets, kept as comments; confirmed against the ARM ctor and
// the Run pseudocode):
//   +0x00 vptr
//   +0x04 state (int): 0 idle, 1 started, 2 connecting, 3 sending, 4 receiving,
//         5 response complete, 6 finished, 7 error, 8 cancelled
//   +0x08 socket handle (int, -1 until a connection is made)
//   +0x0c connect handle (int; set by Run when the transport opens the connection)
//   +0x10 port (int, ctor argument 2)
//   +0x14 host name (owned copy of ctor argument 1)
//   +0x18 observer (GLXPlayerSocketObserver*, ctor argument 3)
//   +0x1c receive buffer, 0x800 bytes (zeroed by Start and by Run before each receive)
//   +0x81c receive buffer length (int, zeroed by Start and when a send completes)
//   +0x820 socket type (int)
//   +0x824 accept address (owned string)
//   +0x828 accept port (int)
//   +0x82c received content (std::string; the STL private layout is not modelled)
//   +0x83c end of the response data (pointer, written by the platform receive path)
//   +0x840 start of the response data (pointer, written by the platform receive path)
//   +0x844 flag (byte): cleared when a send completes; read outside Run
//   +0x845 header pending (byte): the HTTP header is still in content_
//   +0x846 content length known (byte)
//   +0x848 expected content length (int, from Content-Length)
//   +0x84c bytes sent so far (int)
//   +0x850 send percentage (uint; values above 100 read as 0)
//   +0x854 send start time (uint32 seconds)
//   +0x858 receive start time (uint32 seconds)
//   +0x85c connecting flag (int; 1 while the transport is still connecting; Cancel clears it)
//   +0x860 connect result (int; cleared when a connect attempt times out)
//   +0x868 connect start time (uint32 seconds; written by the platform open path)
#pragma once

#include <cstdint>
#include <memory>
#include <string>

// The callback target of a socket. The HTTP object is the observer of the GLX sockets.
// Slot names come from Run's call sites, in vtable order (the base dtor takes slots 0 and
// 1 here, so the byte offsets of the original are not the port's offsets).
class GLXPlayerSocketObserver {
public:
    virtual ~GLXPlayerSocketObserver() = default;

    // Original slot +0x00: the bytes to send (the request or the post body).
    virtual const char* RequestData() const { return nullptr; }
    // Original slot +0x04: the number of bytes RequestData holds.
    virtual std::uint32_t RequestLength() const { return 0; }
    // Original slot +0x08: the response once the connection is closed (data, length).
    virtual void OnResponse(const char* data, std::int32_t length) { (void)data; (void)length; }
    // Original slot +0x0c: count of response bytes available, or 0 after a receive error.
    virtual void OnReceived(std::int32_t count) { (void)count; }
};

class GLXPlayerSocket {
public:
    // 0x8302f0 and 0x83039c: GLXPlayerSocket(char*, int, GLXPlayerSocketObserver*)
    GLXPlayerSocket(const char* host, std::int32_t port, GLXPlayerSocketObserver* observer);
    // 0x8301e4 (D1) and 0x83026c (D2). The deleting destructor 0x830250 is not ported.
    virtual ~GLXPlayerSocket() = default;

    // 0x8301ac _ZN15GLXPlayerSocket5StartEv, vtable +0x0c
    virtual void Start();
    // 0x830f20 _ZN15GLXPlayerSocket3RunEv, vtable +0x10. The connection state machine.
    virtual void Run();
    // 0x83007c _ZN15GLXPlayerSocket6CancelEv, vtable +0x14
    virtual void Cancel();
    // 0x8300bc _ZN15GLXPlayerSocket12IsInProgressEv, vtable +0x18
    virtual std::int32_t IsInProgress() const;
    // 0x8300e8 _ZN15GLXPlayerSocket15IsErrorOccurredEv, vtable +0x1c
    virtual bool IsErrorOccurred() const { return state_ == 7; }

    // Transport hooks used by Run. The original calls them through its platform socket
    // (vtable slots +0x20..+0x48 of the GLXPlayerSocket object). The base versions report
    // failure, so a port without a transport never leaves the first states.
    // vtable +0x20: opens the connection to the host; nonzero is the connect handle.
    virtual std::int32_t OpenConnection(const char* host) { (void)host; return 0; }
    // vtable +0x28: starts the connect; nonzero when it started.
    virtual std::int32_t BeginConnect() { return 0; }
    // vtable +0x2c: the last transport error code.
    virtual std::int32_t LastError() const { return 0; }
    // vtable +0x34: nonzero once the connection is up.
    virtual std::int32_t IsConnected() const { return 0; }
    // vtable +0x3c: closes the transport. Cancel calls it, and so do the Run error paths.
    // The name comes from Cancel; LCXPlayerSocket uses the same hook.
    virtual void OnCancelled() {}
    // vtable +0x40: readiness. wantWrite true asks whether a send is possible, false whether
    // data can be read. Returns >0 ready, 0 not ready and <0 on error.
    virtual std::int32_t Poll(bool wantWrite) { (void)wantWrite; return -1; }
    // vtable +0x44: sends up to length bytes; returns the count sent or <0 on error.
    virtual std::int32_t SendBytes(const char* data, std::int32_t length) {
        (void)data; (void)length;
        return -1;
    }
    // vtable +0x48: receives up to length bytes into buffer; returns the count, 0 when the
    // peer closed the connection, or <0 on error.
    virtual std::int32_t RecvBytes(char* buffer, std::int32_t length) {
        (void)buffer; (void)length;
        return -1;
    }
    // vtable +0x58 and +0x74: the platform hooks the ping probe calls. Pure in the original
    // base. The port's defaults report failure (-1) for the send and no connection (0).
    virtual std::int32_t SendTo(const void* data, std::int32_t length, const char* address,
                                std::int32_t port) {
        (void)data; (void)length; (void)address; (void)port;
        return -1;
    }
    virtual std::int32_t Connect() { return 0; }

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
    // 0x83067c _ZN15GLXPlayerSocket19ParseChunkedContentEv: replaces content_ with the
    // de-chunked body. Returns 1 on success and 0 on a malformed chunk.
    std::int32_t ParseChunkedContent();

    // Not in the original: test seams.
    void SetStateForTest(std::int32_t state) { state_ = state; }
    void SetSendPercentageForTest(std::uint32_t percent) { sendPercentage_ = percent; }
    void SetContentForTest(const std::string& text) { content_ = text; }
    const std::string& ContentForTest() const { return content_; }

protected:
    // Error path shared by the Run states: LastError, the close hook, state 7.
    void FailAndClose();
    // Length of the response span (responseEnd_ - responseStart_), 0 when either is unset.
    std::int32_t ResponseSpan() const;

private:
    std::int32_t state_ = 0;                        // +0x04
    std::int32_t socket_ = -1;                      // +0x08
    std::int32_t connectHandle_ = 0;                // +0x0c
    std::int32_t port_ = 0;                         // +0x10
    std::unique_ptr<char[]> host_;                  // +0x14
    GLXPlayerSocketObserver* observer_ = nullptr;   // +0x18
    char receiveBuffer_[0x800] = {};                // +0x1c
    std::int32_t receiveLength_ = 0;                // +0x81c
    std::int32_t socketType_ = 0;                   // +0x820
    std::unique_ptr<char[]> acceptIp_;              // +0x824
    std::int32_t acceptPort_ = 0;                   // +0x828
    std::string content_;                           // +0x82c
    const char* responseEnd_ = nullptr;             // +0x83c
    const char* responseStart_ = nullptr;           // +0x840
    bool responseFlag844_ = false;                  // +0x844
    bool headerPending_ = false;                    // +0x845
    bool contentLengthKnown_ = false;               // +0x846
    std::int32_t expectedLength_ = 0;               // +0x848
    std::int32_t sentBytes_ = 0;                    // +0x84c
    std::uint32_t sendPercentage_ = 0;              // +0x850
    std::uint32_t sendStartTime_ = 0;               // +0x854
    std::uint32_t receiveStartTime_ = 0;            // +0x858
    std::int32_t connecting_ = 0;                   // +0x85c
    std::int32_t connectResult_ = 0;                // +0x860
    std::uint32_t connectStartTime_ = 0;            // +0x868
};

// 0x831528 _ZN22GLXPlayerSocketFactory9GetSocketEPciP23GLXPlayerSocketObserver
// The original builds a platform socket (CAndroidSocket, 0xcf4 bytes). The port builds
// the base class, which has no I/O.
class GLXPlayerSocketFactory {
public:
    static GLXPlayerSocket* GetSocket(const char* host, std::int32_t port,
                                      GLXPlayerSocketObserver* observer);
};
