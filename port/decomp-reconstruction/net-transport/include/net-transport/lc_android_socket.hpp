// LCAndroidSocket: BSD-socket wrapper of the lobby-side socket in libDungeonHunter2.so
// (Android build). Same socket layer as CAndroidSocket, but a separate class with its own
// non-blocking connect state machine and a process-wide host cache (l_hostLC / l_countLC).
//
// The original derives from LCXPlayerSocket (net-session-online, not part of this class's
// category). Its virtual slots are called here as ordinary member functions (slot 8 =
// GetHostByName, 11 = GetLastError, 12 = SetNonBlocking, 15 = CloseSocket, 16 = Select), and
// the base members are held directly by this class. The vtable is not reproduced.
//
// Layout (32-bit offsets from the original, kept as comments):
//   +0x04 state_ (7 = error)     +0x08 fd_ (socket handle, -1 when closed)
//   +0x820 socket_type_ (1 TCP, 2 UDP)     +0x868 ready_bits_ (m_set, 0x80 bytes; ARM listing of Select)
// Other member offsets were not recovered from the pseudocode, so they are not given.
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <thread>

struct hostent;

// Request block for the host lookup thread (m_getHostByNameThreadParam): the busy flag
// that the worker clears, and the host name (copied, at most 0x3ff characters).
struct LCHostNameRequest {
    int* flag_address = nullptr;   // flagAddress
    char server_name[0x400] = {};  // serverName
};

class LCAndroidSocket {
public:
    // LCXPlayerSocket::XSocketSelectFlag. Only the write value (1) is visible in the
    // pseudocode; the read value (0) follows CAndroidSocket.
    enum XSocketSelectFlag : int { kSelectRead = 0, kSelectWrite = 1 };

    // LCXPlayerSocket::XSocketType (TCP and UDP values confirmed by the ARM listing).
    enum XSocketType : int { kTypeNone = 0, kTypeTcp = 1, kTypeUdp = 2 };

    static constexpr int kStateError = 7;              // XSOCKET_STATE_ERROR
    static constexpr int kConnectTimeoutMs = 0x7530;   // 30000
    static constexpr int kLookupTimeoutMs = 0x270F;    // 9999

    // 0x8a0020 _ZN15LCAndroidSocketC1EPciP23LCXPlayerSocketObserver (C2 at 0x8a013c)
    LCAndroidSocket(const char* server_name, int server_port, void* observer);
    // 0x89ff54 _ZN15LCAndroidSocketD1Ev (D2 at 0x89ffc8)
    ~LCAndroidSocket();
    LCAndroidSocket(const LCAndroidSocket&) = delete;
    LCAndroidSocket& operator=(const LCAndroidSocket&) = delete;

    static void Startup();                              // 0x89feec _ZN15LCAndroidSocket7StartupEv
    static void Clearup();                              // 0x89fc78 _ZN15LCAndroidSocket7ClearupEv

    void Init();                                        // 0x89e5b0 _ZN15LCAndroidSocket4InitEv
    bool IsReadyForRW() const;                          // 0x89e5b4
    int SendBroadcast(const char* buffer, int size, int port);  // 0x89e5dc
    void SetBroadcast();                                // 0x89e6d0
    void Listen(int count);                             // 0x89e734
    std::uint16_t GetSocketPort() const;                // 0x89e760 (host byte order)
    const char* GetSocketAddr() const;                  // 0x89e7e8
    int RecvFromUnkownIP(char* buffer, int size, char** ip, int* port);  // 0x89e880
    void* GetHostName(char** name);                     // 0x89e998 (always returns nullptr)
    int RecvFrom(char* buffer, int size);               // 0x89ea00
    int SendTo(const char* buffer, int size, const char* ip, int port);  // 0x89eb1c
    bool CreateUdpSocket();                             // 0x89ec0c
    bool CreateTcpSocket();                             // 0x89ec40
    bool CreateSocket();                                // 0x89ec70
    int Bind(char* ip, std::uint16_t port);             // 0x89ece4 (port + retries)
    int Recv(char* buffer, int size);                   // 0x89edfc
    int Send(char* data, int size);                     // 0x89ee4c
    int Select(XSocketSelectFlag flag);                 // 0x89ef3c
    bool CloseSocket();                                 // 0x89f004
    bool ConnectTo(char* ip, int port);                 // 0x89f054 (true when connected)
    bool Connect();                                     // 0x89f290 (true when connected)
    bool ConnectByName(char* server_name);              // 0x89f4fc (true when connected)
    bool SetNonBlocking();                              // 0x89f7c8
    int GetLastError() const;                           // 0x89f890
    static hostent* GetHostIP(char* server_name);       // 0x89f9a0
    hostent* GetHostByName(char* server_name);          // 0x89fb04
    static void TransToIp(char*& name);                 // 0x89fc1c
    bool GetLocalIP(char* szLocalIP);                   // 0x89fd84
    LCAndroidSocket* Accept();                          // 0x8a0068 (accepted socket object)

private:
    // Socket state shared by the connect paths (state 1 = in progress, 2 = done, 3 = failed).
    bool FinishConnectProgress(bool wait_timeout);      // state-1 path of Connect and ConnectByName
    void FailConnect();                                 // close, state 3, error

    int state_ = 0;                                     // +0x04
    int fd_ = -1;                                       // +0x08
    std::unique_ptr<char[]> server_name_;               // +0x14 (owned copy of the constructor's name)
    void* observer_ = nullptr;                          // +0x18 (observer; its type is not in deps)
    int socket_type_ = kTypeNone;                       // +0x820
    std::uint32_t ready_bits_[32] = {};                 // +0x868 m_set (fd_set; 1024-bit bitmap)
    int non_block_connect_state_ = 0;                   // m_nonBlockConnectState
    int connect_time_start_ = 0;                        // m_connectTimeStart (ms)
    hostent* server_addr_ = nullptr;                    // m_server_addr
    int server_port_ = 0;                               // m_server_port
    int gethostbyname_time_start_ = 0;                  // m_gethostbynametimeStart (ms)
    int gethostbyname_state_ = 0;                       // m_gethostbynameState (1 = lookup busy)
    LCHostNameRequest host_request_{};                  // m_getHostByNameThreadParam
    std::thread host_thread_;                           // m_getHostByNameThreadId
    std::string accept_ip_;                             // accept IP (base class, +0x824 there)
    int accept_port_ = 0;                               // accept port (base class, +0x828 there)
};
