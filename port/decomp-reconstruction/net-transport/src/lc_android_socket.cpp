#include "net-transport/lc_android_socket.hpp"

#include <chrono>
#include <cstring>
#include <new>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
using NativeHandle = SOCKET;
using SockLen = int;
constexpr int kInProgressError = WSAEWOULDBLOCK;
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <csignal>
using NativeHandle = int;
using SockLen = socklen_t;
constexpr int kInProgressError = EINPROGRESS;
#endif

namespace {

// The ready bitmap covers fds 0..1023 (the original's 0x80-byte fd_set).
constexpr int kFdBitmapBits = 1024;
// GetHostByName rejects names longer than this (LC_API_STRLEN > 1024).
constexpr std::size_t kHostNameLimit = 1024;
// Cache of resolved hosts (l_hostLC[4] / l_countLC in the original).
hostent* l_hostLC[4] = {nullptr, nullptr, nullptr, nullptr};
int l_countLC = 0;

NativeHandle ToNative(int fd) { return static_cast<NativeHandle>(fd); }

// XP-style millisecond clock truncated to 32 bits (LC_API_GET_TIME).
int NowMs() {
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch());
    return static_cast<int>(static_cast<std::uint32_t>(ms.count()));
}

// Returns the handle as int, or -1 when socket() failed.
int OpenNative(int family, int type, int protocol) {
    NativeHandle h = ::socket(family, type, protocol);
#ifdef _WIN32
    return h == INVALID_SOCKET ? -1 : static_cast<int>(h);
#else
    return h >= 0 ? static_cast<int>(h) : -1;
#endif
}

void CloseNative(int fd) {
#ifdef _WIN32
    ::closesocket(ToNative(fd));
#else
    ::close(fd);
#endif
}

int LastSocketError() {
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

// O_NONBLOCK on or off (F_GETFL / F_SETFL, or ioctlsocket FIONBIO on Winsock).
bool SetBlockingMode(int fd, bool nonblocking) {
#ifdef _WIN32
    u_long mode = nonblocking ? 1u : 0u;
    return ::ioctlsocket(ToNative(fd), FIONBIO, &mode) == 0;
#else
    int flags = ::fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        return false;
    }
    flags = nonblocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    return ::fcntl(fd, F_SETFL, flags) >= 0;
#endif
}

// strdup-style copy with new[]; released with delete[].
char* NewCopy(const char* text) {
    std::size_t n = std::strlen(text);
    char* copy = new char[n + 1];
    std::memcpy(copy, text, n + 1);
    return copy;
}

// Copies at most size - 1 characters and always terminates (strlcpy-style, size from the caller).
void CopyBounded(char* dst, const char* src, std::size_t size) {
    std::size_t n = std::strlen(src);
    if (n >= size) {
        n = size - 1;
    }
    std::memcpy(dst, src, n);
    dst[n] = '\0';
}

// Copy of the first address of a resolved host, h_length raw bytes (new[]; released with delete[]).
char* NewAddressCopy(const hostent* host) {
    std::size_t length = static_cast<std::size_t>(host->h_length);
    char* copy = new char[length];
    std::memcpy(copy, host->h_addr_list[0], length);
    return copy;
}

// 0x89f8a0 threadGetHostByNameProcessLC (the binary's copy lives in core-platform, with its own
// cache; this one fills the cache that GetHostIP and GetHostByName read). The busy flag is set
// while the lookup runs and cleared after a successful lookup that is cached. A failed lookup
// leaves the flag set, so GetHostByName starts no new lookup (as the original). The cache is
// filled without a lock, as in the original.
void ThreadGetHostByNameProcessLC(LCHostNameRequest* request) {
    *request->flag_address = 1;
    hostent* found = ::gethostbyname(request->server_name);
    if (found == nullptr) {
        return;
    }
    if (l_countLC <= 3 && found->h_addr_list[0] != nullptr) {
        hostent* entry = new hostent{};
        entry->h_length = found->h_length;
        entry->h_name = NewCopy(request->server_name);
        entry->h_addr_list = new char*[1]{};
        entry->h_addr_list[0] = NewAddressCopy(found);
        l_hostLC[l_countLC] = entry;
        ++l_countLC;
    }
    *request->flag_address = 0;
}

}  // namespace

// 0x89feec _ZN15LCAndroidSocket7StartupEv
// Partial: LC_DEBUG_OUT dropped. Sets SIGPIPE to ignore (sigaction) and clears the four cache
// slots without freeing them or resetting l_countLC, as the original does. The Winsock
// initialisation is added for _WIN32 builds.
void LCAndroidSocket::Startup() {
#ifdef _WIN32
    WSADATA data{};
    ::WSAStartup(MAKEWORD(2, 2), &data);
#else
    struct sigaction sa{};
    sa.sa_handler = SIG_IGN;
    ::sigaction(SIGPIPE, &sa, nullptr);
#endif
    for (auto& slot : l_hostLC) {
        slot = nullptr;
    }
}

// 0x89fc78 _ZN15LCAndroidSocket7ClearupEv
// Partial: LC_DEBUG_OUT dropped. Frees every cache entry (name, address, address list,
// entry). Null slots are skipped; the original dereferences them. WSACleanup is added for
// _WIN32 builds.
void LCAndroidSocket::Clearup() {
    for (int i = 0; i < l_countLC; ++i) {
        hostent* entry = l_hostLC[i];
        if (entry == nullptr) {
            continue;
        }
        delete[] entry->h_name;
        if (entry->h_addr_list != nullptr) {
            delete[] entry->h_addr_list[0];
            delete[] entry->h_addr_list;
        }
        delete entry;
        l_hostLC[i] = nullptr;
    }
    l_countLC = 0;
#ifdef _WIN32
    ::WSACleanup();
#endif
}

// 0x89e5b0 _ZN15LCAndroidSocket4InitEv
// The original body is empty.
void LCAndroidSocket::Init() {}

// 0x89e5b4 _ZNK15LCAndroidSocket12IsReadyForRWEv
// Partial: fds outside the 1024-bit bitmap report false (the original indexes without a check).
bool LCAndroidSocket::IsReadyForRW() const {
    if (fd_ < 0 || fd_ >= kFdBitmapBits) {
        return false;
    }
    return ((ready_bits_[fd_ / 32] >> (fd_ % 32)) & 1u) != 0;
}

// 0x89e5dc _ZN15LCAndroidSocket13SendBroadcastEPKcii
// Partial: LC_DEBUG_OUT logging dropped. Sends to 255.255.255.255 on the given port and returns
// the sendto() result.
int LCAndroidSocket::SendBroadcast(const char* buffer, int size, int port) {
    sockaddr_in to{};
    to.sin_family = AF_INET;
    to.sin_addr.s_addr = INADDR_BROADCAST;
    to.sin_port = htons(static_cast<std::uint16_t>(port));
    return static_cast<int>(
        ::sendto(ToNative(fd_), buffer, size, 0, reinterpret_cast<sockaddr*>(&to), sizeof(to)));
}

// 0x89e6d0 _ZN15LCAndroidSocket12SetBroadcastEv
// Partial: the "last error" log on failure is dropped.
void LCAndroidSocket::SetBroadcast() {
    int one = 1;
    ::setsockopt(ToNative(fd_), SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char*>(&one), sizeof(one));
}

// 0x89e734 _ZN15LCAndroidSocket6ListenEi
// Partial: the "Listen failed!" log is dropped.
void LCAndroidSocket::Listen(int count) {
    (void)::listen(ToNative(fd_), count);
}

// 0x89e760 _ZN15LCAndroidSocket13GetSocketPortEv
// The getsockname() result is ignored, as in the original; a failed call yields port 0.
std::uint16_t LCAndroidSocket::GetSocketPort() const {
    sockaddr_in addr{};
    SockLen len = sizeof(addr);
    ::getsockname(ToNative(fd_), reinterpret_cast<sockaddr*>(&addr), &len);
    return ntohs(addr.sin_port);
}

// 0x89e7e8 _ZN15LCAndroidSocket13GetSocketAddrEv
// First IPv4 address of this machine's host name, in inet_ntoa's static buffer (as the original).
const char* LCAndroidSocket::GetSocketAddr() const {
    char name[256];
    std::memset(name, 0, sizeof(name));
    if (::gethostname(name, sizeof(name)) != 0) {
        return nullptr;
    }
    hostent* host = ::gethostbyname(name);
    if (host == nullptr || host->h_addr_list[0] == nullptr) {
        return nullptr;
    }
    in_addr addr{};
    std::memcpy(&addr, host->h_addr_list[0], sizeof(addr));
    return ::inet_ntoa(addr);
}

// 0x89e880 _ZN15LCAndroidSocket16RecvFromUnkownIPEPciPPcPi
// Partial: the error and debug logs are dropped. On success *ip receives a new[] copy of the
// sender address (release with delete[]) and *port the sender port in host byte order.
int LCAndroidSocket::RecvFromUnkownIP(char* buffer, int size, char** ip, int* port) {
    sockaddr_in from{};
    from.sin_family = AF_INET;
    SockLen len = sizeof(from);
    int n = static_cast<int>(
        ::recvfrom(ToNative(fd_), buffer, size, 0, reinterpret_cast<sockaddr*>(&from), &len));
    if (n > 0) {
        *ip = NewCopy(::inet_ntoa(from.sin_addr));
        *port = ntohs(from.sin_port);
    }
    return n;
}

// 0x89e998 _ZN15LCAndroidSocket11GetHostNameEPPc
// Stores a new[] copy of the host name in *name. The original always returns nullptr.
void* LCAndroidSocket::GetHostName(char** name) {
    char host[256];
    std::memset(host, 0, sizeof(host));
    ::gethostname(host, 255);
    *name = NewCopy(host);
    return nullptr;
}

// 0x89ea00 _ZN15LCAndroidSocket8RecvFromEPci
// Partial: the sender is received into a local address, but the only use of the sender
// (the debug log) is dropped, so it is not kept.
int LCAndroidSocket::RecvFrom(char* buffer, int size) {
    sockaddr_in from{};
    from.sin_family = AF_INET;
    SockLen len = sizeof(from);
    return static_cast<int>(
        ::recvfrom(ToNative(fd_), buffer, size, 0, reinterpret_cast<sockaddr*>(&from), &len));
}

// 0x89eb1c _ZN15LCAndroidSocket6SendToEPKciS1_i
// Partial: the debug log is dropped. Returns 0 without sending when ip is null.
int LCAndroidSocket::SendTo(const char* buffer, int size, const char* ip, int port) {
    if (ip == nullptr) {
        return 0;
    }
    sockaddr_in to{};
    to.sin_family = AF_INET;
    to.sin_addr.s_addr = ::inet_addr(ip);
    to.sin_port = htons(static_cast<std::uint16_t>(port));
    return static_cast<int>(
        ::sendto(ToNative(fd_), buffer, size, 0, reinterpret_cast<sockaddr*>(&to), sizeof(to)));
}

// 0x89ec0c _ZN15LCAndroidSocket15CreateUdpSocketEv
// Opens an AF_INET SOCK_DGRAM IPPROTO_UDP socket. The type is set only on success.
bool LCAndroidSocket::CreateUdpSocket() {
    fd_ = OpenNative(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd_ < 0) {
        return false;
    }
    socket_type_ = kTypeUdp;
    return true;
}

// 0x89ec40 _ZN15LCAndroidSocket15CreateTcpSocketEv
// Opens an AF_INET SOCK_STREAM IPPROTO_TCP socket. The type is set only on success.
bool LCAndroidSocket::CreateTcpSocket() {
    fd_ = OpenNative(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bool ok = fd_ >= 0;
    if (ok) {
        socket_type_ = kTypeTcp;
    }
    return ok;
}

// 0x89ec70 _ZN15LCAndroidSocket12CreateSocketEv
// Partial: the debug logs are dropped. Opens an AF_INET SOCK_STREAM socket with protocol 0.
bool LCAndroidSocket::CreateSocket() {
    fd_ = OpenNative(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) {
        state_ = kStateError;
        return false;
    }
    return true;
}

// 0x89ece4 _ZN15LCAndroidSocket4BindEPct
// Reconstructed, bugs included: inet_addr(ip) has no effect in the original, so the socket binds
// INADDR_ANY whatever the address. SO_REUSEADDR is set and its result ignored. When the port is
// taken, the port is incremented until a bind succeeds. The original has no exit when no
// port binds, and that is kept: the loop does not terminate in that case. The return value is
// retries + port, as the pseudocode's v6 + v4.
int LCAndroidSocket::Bind(char* ip, std::uint16_t port) {
    (void)ip;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0;
    int one = 1;
    ::setsockopt(ToNative(fd_), SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&one), sizeof(one));
    addr.sin_port = htons(port);
    int retries = 0;
    if (::bind(ToNative(fd_), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::uint16_t next = static_cast<std::uint16_t>(port + 1);
        do {
            addr.sin_port = htons(next);
            ++retries;
            next = static_cast<std::uint16_t>(next + 1);
        } while (::bind(ToNative(fd_), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0);
    }
    return retries + port;
}

// 0x89edfc _ZN15LCAndroidSocket4RecvEPci
// Partial: the timing debug log is dropped. Returns recv()'s result (-1 on error).
int LCAndroidSocket::Recv(char* data, int size) {
    return static_cast<int>(::recv(ToNative(fd_), data, size, 0));
}

// 0x89ee4c _ZN15LCAndroidSocket4SendEPci
// Partial: the timing and echo debug log is dropped. Returns send()'s result.
int LCAndroidSocket::Send(char* data, int size) {
    return static_cast<int>(::send(ToNative(fd_), data, size, 0));
}

// 0x89ef3c _ZN15LCAndroidSocket6SelectEN15LCXPlayerSocket17XSocketSelectFlagE
// Reconstructed. The socket's bit is set in the ready bitmap and select() runs on a local
// fd_set holding only that socket. The bitmap is rewritten from the result, so afterwards
// IsReadyForRW reports whether the socket was ready (the original's fd_set is updated by
// select() the same way). A socket outside the bitmap, or a socket that is closed, returns -1.
int LCAndroidSocket::Select(XSocketSelectFlag flag) {
    timeval tval{};
    if (fd_ < 0 || fd_ >= kFdBitmapBits) {
        return -1;
    }
    std::memset(ready_bits_, 0, sizeof(ready_bits_));
    ready_bits_[fd_ / 32] |= 1u << (fd_ % 32);

    fd_set set;
    FD_ZERO(&set);
    FD_SET(ToNative(fd_), &set);
    int result;
    if (flag == kSelectRead) {
        result = ::select(fd_ + 1, &set, nullptr, nullptr, &tval);
    } else if (flag == kSelectWrite) {
        result = ::select(fd_ + 1, nullptr, &set, nullptr, &tval);
    } else {
        return -1;
    }

    std::memset(ready_bits_, 0, sizeof(ready_bits_));
    if (FD_ISSET(ToNative(fd_), &set)) {
        ready_bits_[fd_ / 32] |= 1u << (fd_ % 32);
    }
    return result;
}

// 0x89f004 _ZN15LCAndroidSocket11CloseSocketEv
// Partial: the two debug logs are dropped. Closes the handle when open and marks it closed.
bool LCAndroidSocket::CloseSocket() {
    if (fd_ >= 0) {
        CloseNative(fd_);
        fd_ = -1;
    }
    return true;
}

// Shared failure tail of the connect paths: close, state 3 (failed), state error.
void LCAndroidSocket::FailConnect() {
    CloseSocket();
    non_block_connect_state_ = 3;
    state_ = kStateError;
}

// Progress of a non-blocking connect (state 1). Returns true when the connection completed
// (state 2). Otherwise it returns false, either still waiting or failed (and closed).
// wait_timeout is false for ConnectTo, which waits without a timeout.
bool LCAndroidSocket::FinishConnectProgress(bool wait_timeout) {
    int ready = Select(kSelectWrite);
    if (ready < 0) {
        FailConnect();
        return false;
    }
    if (ready != 0) {
        int error = 0;
        SockLen len = sizeof(error);
        if (::getsockopt(ToNative(fd_), SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&error), &len) >= 0 &&
            error == 0) {
            if (SetBlockingMode(fd_, false)) {
                non_block_connect_state_ = 2;
                return true;
            }
        }
        FailConnect();
        return false;
    }
    if (!wait_timeout) {
        return false;
    }
    if (static_cast<unsigned int>(NowMs() - connect_time_start_) <= static_cast<unsigned int>(kConnectTimeoutMs)) {
        return false;
    }
    FailConnect();
    return false;
}

// 0x89f054 _ZN15LCAndroidSocket9ConnectToEPci
// Partial: the debug log on failure is dropped. Starts a non-blocking connect to ip:port
// (state 1) and returns true only once it has completed, which is done by a later call.
bool LCAndroidSocket::ConnectTo(char* ip, int port) {
    if (non_block_connect_state_ != 0) {
        if (non_block_connect_state_ != 1) {
            return false;
        }
        return FinishConnectProgress(false);
    }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ::inet_addr(ip);
    addr.sin_port = htons(static_cast<std::uint16_t>(port));
    if (!SetNonBlocking()) {
        FailConnect();
        return false;
    }
    if (::connect(ToNative(fd_), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) >= 0 ||
        GetLastError() == kInProgressError) {
        non_block_connect_state_ = 1;
        return false;
    }
    FailConnect();
    return false;
}

// 0x89f290 _ZN15LCAndroidSocket7ConnectEv
// Partial: the debug log on failure is dropped. Connects to the resolved server address
// (m_server_addr) and port. Waiting for the connect uses a 30 s timeout. A null server address
// returns false here; the original would dereference it.
bool LCAndroidSocket::Connect() {
    if (non_block_connect_state_ != 0) {
        if (non_block_connect_state_ != 1) {
            return false;
        }
        return FinishConnectProgress(true);
    }
    if (server_addr_ == nullptr) {
        return false;
    }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    std::memcpy(&addr.sin_addr, server_addr_->h_addr_list[0], static_cast<std::size_t>(server_addr_->h_length));
    addr.sin_port = htons(static_cast<std::uint16_t>(server_port_));
    if (!SetNonBlocking()) {
        FailConnect();
        return false;
    }
    if (::connect(ToNative(fd_), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) >= 0 ||
        GetLastError() == kInProgressError) {
        non_block_connect_state_ = 1;
        connect_time_start_ = NowMs();
        return false;
    }
    FailConnect();
    return false;
}

// 0x89f4fc _ZN15LCAndroidSocket13ConnectByNameEPc
// Partial: the debug logs are dropped. Resolves server_name through GetHostByName, which
// returns null until the lookup finishes. The lookup is given 9999 ms. The connect is the same
// as Connect's. The port is the stored server_port_ (the argument of the original is ignored).
bool LCAndroidSocket::ConnectByName(char* server_name) {
    if (non_block_connect_state_ != 0) {
        if (non_block_connect_state_ != 1) {
            return false;
        }
        return FinishConnectProgress(true);
    }
    hostent* host = GetHostByName(server_name);
    server_addr_ = host;
    if (host == nullptr) {
        if (static_cast<unsigned int>(NowMs() - gethostbyname_time_start_) <=
            static_cast<unsigned int>(kLookupTimeoutMs)) {
            return false;
        }
        FailConnect();
        return false;
    }
    if (host->h_addr_list[0] == nullptr) {
        FailConnect();
        return false;
    }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    std::memcpy(&addr.sin_addr, host->h_addr_list[0], static_cast<std::size_t>(host->h_length));
    addr.sin_port = htons(static_cast<std::uint16_t>(server_port_));
    if (!SetNonBlocking()) {
        FailConnect();
        return false;
    }
    if (::connect(ToNative(fd_), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) >= 0 ||
        GetLastError() == kInProgressError) {
        non_block_connect_state_ = 1;
        connect_time_start_ = NowMs();
        return false;
    }
    FailConnect();
    return false;
}

// 0x89f7c8 _ZN15LCAndroidSocket14SetNonBlockingEv
// Partial: the error log is dropped. On failure the socket is closed and the state is set to
// error (the connect state is left alone, as the original does).
bool LCAndroidSocket::SetNonBlocking() {
    if (SetBlockingMode(fd_, true)) {
        return true;
    }
    CloseSocket();
    state_ = kStateError;
    return false;
}

// 0x89f890 _ZN15LCAndroidSocket12GetLastErrorEv
// errno, or WSAGetLastError on Winsock.
int LCAndroidSocket::GetLastError() const {
    return LastSocketError();
}

// 0x89f9a0 _ZN15LCAndroidSocket9GetHostIPEPc
// Partial: the debug log is dropped. A cache hit (same name) returns the cached entry. A miss
// is resolved with gethostbyname, returned as is, and cached while fewer than four entries
// exist. A cache entry copies the first address as h_length raw bytes. The decompiled copy
// reads that address as a C string. Null cache slots are skipped; the original dereferences
// them after Startup clears the slots.
hostent* LCAndroidSocket::GetHostIP(char* server_name) {
    for (int i = 0; i < l_countLC; ++i) {
        hostent* entry = l_hostLC[i];
        if (entry != nullptr && entry->h_name != nullptr && std::strcmp(server_name, entry->h_name) == 0) {
            return entry;
        }
    }
    hostent* result = ::gethostbyname(server_name);
    if (result != nullptr && l_countLC <= 3 && result->h_addr_list[0] != nullptr) {
        hostent* entry = new hostent{};
        entry->h_length = result->h_length;
        entry->h_name = NewCopy(server_name);
        entry->h_addr_list = new char*[1]{};
        entry->h_addr_list[0] = NewAddressCopy(result);
        l_hostLC[l_countLC] = entry;
        ++l_countLC;
    }
    return result;
}

// 0x89fb04 _ZN15LCAndroidSocket13GetHostByNameEPc
// Partial: the lookup runs on a std::thread (pthread_create in the original). The thread is
// joined before a new one replaces a finished one, which the original does not do. The worker
// caches the first address as raw bytes, not as a C string (see GetHostIP). Cache hits (same
// name) return the cached entry, as the original does. The first call returns null while the
// lookup runs; a later call returns the cached entry. The thread start failure returns null.
hostent* LCAndroidSocket::GetHostByName(char* server_name) {
    if (server_name == nullptr || std::strlen(server_name) > kHostNameLimit) {
        return nullptr;
    }
    for (int i = 0; i < l_countLC; ++i) {
        hostent* entry = l_hostLC[i];
        if (entry != nullptr && entry->h_name != nullptr && std::strcmp(server_name, entry->h_name) == 0) {
            return entry;
        }
    }
    if (gethostbyname_state_ == 0) {
        gethostbyname_time_start_ = NowMs();
        host_request_.flag_address = &gethostbyname_state_;
        CopyBounded(host_request_.server_name, server_name, sizeof(host_request_.server_name));
        try {
            if (host_thread_.joinable()) {
                host_thread_.join();
            }
            host_thread_ = std::thread(ThreadGetHostByNameProcessLC, &host_request_);
            gethostbyname_state_ = 1;
        } catch (...) {
            // pthread_create failed in the original: the state is left unchanged.
        }
    }
    return nullptr;
}

// 0x89fc1c _ZN15LCAndroidSocket9TransToIpERPc
// Partial: the debug log is dropped. The original stores inet_ntoa's static buffer in *name,
// so the caller must not free it. The name passed in is released first, as the original does.
void LCAndroidSocket::TransToIp(char*& name) {
    hostent* host = GetHostIP(name);
    if (name != nullptr) {
        delete[] name;
        name = nullptr;
    }
    if (host != nullptr && host->h_addr_list[0] != nullptr) {
        in_addr addr{};
        std::memcpy(&addr, host->h_addr_list[0], sizeof(addr));
        name = ::inet_ntoa(addr);
    }
}

// 0x89fd84 _ZN15LCAndroidSocket10GetLocalIPEPc
// Partial: the debug log is dropped. POSIX: walks the SIOCGIFCONF list and keeps each IPv4
// address that is up and is not 127.0.0.1; the last one wins, copied into szLocalIP with no
// size limit, as in the original. The walk stride is sizeof(ifreq), which is 32 bytes in the
// original and 40 bytes on 64-bit. The socket is closed on the failure path (the original
// leaks it). Winsock has no SIOCGIFCONF, so that path uses gethostbyname instead, without
// the up check (platform substitution).
bool LCAndroidSocket::GetLocalIP(char* szLocalIP) {
#ifdef _WIN32
    char name[256];
    std::memset(name, 0, sizeof(name));
    if (::gethostname(name, sizeof(name)) != 0) {
        return false;
    }
    hostent* host = ::gethostbyname(name);
    if (host == nullptr) {
        return false;
    }
    for (char** item = host->h_addr_list; item != nullptr && *item != nullptr; ++item) {
        in_addr addr{};
        std::memcpy(&addr, *item, sizeof(addr));
        const char* ip = ::inet_ntoa(addr);
        if (std::strcmp(ip, "127.0.0.1") != 0) {
            std::strcpy(szLocalIP, ip);
        }
    }
    return true;
#else
    int fd = OpenNative(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        return false;
    }
    char buffer[4000];
    ifconf ifc{};
    ifc.ifc_len = sizeof(buffer);
    ifc.ifc_buf = buffer;
    if (::ioctl(fd, SIOCGIFCONF, &ifc) < 0) {
        CloseNative(fd);
        return false;
    }
    char* item = buffer;
    while (item < buffer + ifc.ifc_len) {
        ifreq* request = reinterpret_cast<ifreq*>(item);
        item += sizeof(ifreq);
        const char* ip = ::inet_ntoa(reinterpret_cast<sockaddr_in*>(&request->ifr_addr)->sin_addr);
        if (request->ifr_addr.sa_family == AF_INET) {
            char* colon = std::strchr(request->ifr_name, ':');
            if (colon != nullptr) {
                *colon = '\0';
            }
            ::ioctl(fd, SIOCGIFFLAGS, request);
            if ((request->ifr_flags & IFF_UP) != 0 && std::strcmp(ip, "127.0.0.1") != 0) {
                std::strcpy(szLocalIP, ip);
            }
        }
    }
    CloseNative(fd);
    return true;
#endif
}

// 0x8a0068 _ZN15LCAndroidSocket6AcceptEv
// Partial: the accepted socket is built as an LCAndroidSocket, not an LCXPlayerSocket (the
// base is not reconstructed). The accept state (handle, IP, port, TCP type) is stored in this
// class's own members. The construction runs Startup again, as the original does.
LCAndroidSocket* LCAndroidSocket::Accept() {
    sockaddr_in addr{};
    SockLen len = sizeof(addr);
    int handle = static_cast<int>(::accept(ToNative(fd_), reinterpret_cast<sockaddr*>(&addr), &len));
    const char* ip = ::inet_ntoa(addr.sin_addr);
    std::uint16_t port = ntohs(addr.sin_port);
    auto* accepted = new LCAndroidSocket(nullptr, 0, nullptr);
    accepted->fd_ = handle;
    accepted->accept_ip_ = ip;
    accepted->accept_port_ = port;
    accepted->socket_type_ = kTypeTcp;
    return accepted;
}

// 0x8a0020 _ZN15LCAndroidSocketC1EPciP23LCXPlayerSocketObserver
// 0x8a013c _ZN15LCAndroidSocketC2EPciP23LCXPlayerSocketObserver is the same body (skipped).
// Partial: the LCXPlayerSocket base constructor (which keeps the host string and the observer)
// is not reconstructed. The port is taken to be stored by it, and the observer is ignored.
// Startup runs here, as in the original. LC_DEBUG_OUT is dropped.
LCAndroidSocket::LCAndroidSocket(const char* server_name, int server_port, void* observer)
    : server_port_(server_port) {
    (void)server_name;
    (void)observer;
    Startup();
}

// 0x89ff54 _ZN15LCAndroidSocketD1Ev
// 0x89ffc8 _ZN15LCAndroidSocketD2Ev is the same body (skipped).
// Partial: the LCXPlayerSocket base destructor is not run. The lookup thread is joined whenever
// one is joinable, then the socket is closed. The original joins only while its busy flag is set;
// a joinable std::thread must be joined here, because destroying it would terminate the program.
LCAndroidSocket::~LCAndroidSocket() {
    if (host_thread_.joinable()) {
        host_thread_.join();
    }
    CloseSocket();
}
