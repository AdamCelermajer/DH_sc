// CAndroidSocket: BSD-socket wrapper from libDungeonHunter2.so (Android build).
// Reconstructed from the pseudocode. The Winsock path (under _WIN32) and the
// POSIX path share one implementation through small helpers in the .cpp.
//
// The original derives from GLXPlayerSocket (not reconstructed in this category,
// see net-session-online). Its virtual slots are called here as ordinary member
// functions; the vtable is not reproduced.
#pragma once

#include <cstddef>
#include <cstdint>

struct hostent;

class CAndroidSocket {
public:
    // GLXPlayerSocket::XSocketSelectFlag
    enum XSocketSelectFlag : int { kSelectRead = 0, kSelectWrite = 1 };

    // State stored in conn_state_ (+0x85c).
    enum ConnectState : int {
        kConnectIdle = 0,
        kConnectInProgress = 1,
        kConnectDone = 2,
        kConnectFailed = 3,
    };

    static constexpr int kStatusError = 7;          // status_ value after a failure
    static constexpr int kConnectTimeoutMs = 30000; // 0x7530; the wait continues while elapsed <= this
    static constexpr int kLookupTimeoutMs = 9999;

    // 0x84232c / 0x842430 CAndroidSocket(char*, int, GLXPlayerSocketObserver*)
    CAndroidSocket(const char* host, int port, void* observer);
    // 0x842260 / 0x8422d4 ~CAndroidSocket()
    ~CAndroidSocket();
    CAndroidSocket(const CAndroidSocket&) = delete;
    CAndroidSocket& operator=(const CAndroidSocket&) = delete;

    static void Startup();                          // 0x842208 _ZN14CAndroidSocket7StartupEv
    static void Clearup();                          // 0x84210c _ZN14CAndroidSocket7ClearupEv

    void Init();                                    // 0x840880 _ZN14CAndroidSocket4InitEv
    bool IsReadyForRW() const;                      // 0x840884
    std::ptrdiff_t SendBroadcast(const char* data, int length, int port); // 0x8408ac
    int SetBroadcast();                             // 0x8409a0, setsockopt result
    int Listen(int backlog);                        // 0x840a04
    std::uint16_t GetSocketPort() const;            // 0x840a30 (host byte order)
    const char* GetSocketAddr() const;              // 0x840ab4
    std::ptrdiff_t RecvFromUnkownIP(char* buffer, int length, char** ip_out, int* port_out); // 0x840b4c
    void GetHostName(char** out) const;             // 0x840c60
    std::ptrdiff_t RecvFrom(char* buffer, int length); // 0x840cc8
    std::ptrdiff_t SendTo(const char* data, int length, const char* host, int port); // 0x840de0
    bool CreateIcmpSocket();                        // 0x840ed0
    bool CreateUdpSocket();                         // 0x840f04
    void CreateTcpSocket();                         // 0x840f38
    bool CreateSocket();                            // 0x840f68
    int Bind(const char* host, std::uint16_t port); // 0x840fc8, returns port + retries
    std::ptrdiff_t Recv(char* buffer, int length);  // 0x8410d8
    std::ptrdiff_t Send(const char* data, int length); // 0x8410f8
    int Select(XSocketSelectFlag flag);             // 0x8411e8
    bool CloseSocket();                             // 0x8412b4
    int ConnectTo(const char* host, int port);      // 0x8412f0, 1 when done
    int Connect();                                  // 0x84152c, 1 when done
    int ConnectByName(const char* name, std::uint16_t port); // 0x841790, 1 when done
    bool SetNonBlocking();                          // 0x841ad0
    int GetLastError() const;                       // 0x841b98
    hostent* GetHostByName(const char* name);       // 0x841ca8
    hostent* GetHostIP(const char* name);           // 0x841e4c
    int GetLocalIP(char* out);                      // 0x841fbc, 1 on success
    int Accept();                                   // 0x842360 (accepted fd or -1)

private:
    int FinishConnect();                            // state-1 path shared by Connect/ConnectByName

    int status_ = 0;                                // +0x04 (7 = error)
    int fd_ = -1;                                   // +0x08 native socket handle
    hostent* host_ = nullptr;                       // +0x0c resolved host (set by ConnectByName)
    std::uint16_t port_ = 0;                        // +0x10 port, host byte order
    int conn_state_ = kConnectIdle;                 // +0x85c
    int worker_state_ = 0;                          // +0x860 1 = lookup busy, disables cache
    int connect_start_ms_ = 0;                      // +0x864
    int lookup_start_ms_ = 0;                       // +0x868
    std::uint32_t ready_bits_[32] = {};             // +0x86c fd bitmap (fd_set in the original)
    int socket_kind_ = 0;                           // +0x820 1 = TCP, 2 = UDP/ICMP
};
