#include "net-transport/csocket.hpp"

#include <cstring>

using net_platform::NativeSocket;

namespace {

sockaddr_in MakeAddress(const in_addr& addr, std::uint16_t port) {
    sockaddr_in sa{};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr = addr;
    return sa;
}

}  // namespace

// 0x827bb0 _ZN7CSocketC1Ev
// 0x827b5c _ZN7CSocketC2Ev
CSocket::CSocket() {
    handle_ = -1;
    valid_ = false;
    connected_ = false;
}

// 0x827f70 _ZN7CSocketD2Ev
// 0x827f3c _ZN7CSocketD1Ev
CSocket::~CSocket() {
    Close();
}

// 0x827c04 _ZN7CSocket11IsConnectedEv
// Writable with a zero timeout means connected (the original selects on the write set).
bool CSocket::IsConnected() const {
    timeval tv{};
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    fd_set read_set;
    fd_set write_set;
    FD_ZERO(&read_set);
    FD_SET(static_cast<NativeSocket>(handle_), &read_set);
    FD_ZERO(&write_set);
    FD_SET(static_cast<NativeSocket>(handle_), &write_set);
    int ready = ::select(handle_ + 1, &read_set, &write_set, nullptr, &tv);
    if (ready < 1) {
        return false;
    }
    return FD_ISSET(static_cast<NativeSocket>(handle_), &write_set);
}

// 0x827cdc _ZN7CSocket13DataAvailableEi
bool CSocket::DataAvailable(int timeout_us) {
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(static_cast<NativeSocket>(handle_), &read_set);
    timeval tv{};
    tv.tv_sec = timeout_us / 1000000;
    tv.tv_usec = timeout_us % 1000000;
    int ready = ::select(handle_ + 1, &read_set, nullptr, nullptr, &tv);
    if (ready < 1) {
        return false;
    }
    return FD_ISSET(static_cast<NativeSocket>(handle_), &read_set);
}

// 0x827ee0 _ZN7CSocket5CloseEv
int CSocket::Close() {
    int result = 0;
    valid_ = false;
    listening_ = false;
    if (handle_ >= 0) {
        result = net_platform::CloseSocket(static_cast<NativeSocket>(handle_));
        if (result < 0) {
            GetSocketError();
            handle_ = -1;
            return result;
        }
    }
    handle_ = -1;
    return result;
}

// 0x827fa4 _ZN7CSocket12GetLocalPortEv
std::uint16_t CSocket::GetLocalPort() {
    sockaddr_in sa{};
    socklen_t len = sizeof(sa);
    if (::getsockname(static_cast<NativeSocket>(handle_), reinterpret_cast<sockaddr*>(&sa), &len) < 0) {
        GetSocketError();
        return 0;
    }
    return ntohs(sa.sin_port);
}

// 0x828038 _ZN7CSocket10WouldBlockEv
bool CSocket::WouldBlock() const {
    return net_platform::LastError() == net_platform::kErrWouldBlock;
}

// 0x828054 _ZN7CSocket14GetSocketErrorEv
int CSocket::GetSocketError() const {
    return net_platform::LastError();
}

// 0x828064 _ZN7CSocket14GetPeerAddressEv
// On failure the address recorded by Accept is returned.
std::uint32_t CSocket::GetPeerAddress() {
    sockaddr_in sa{};
    socklen_t len = sizeof(sa);
    if (::getpeername(static_cast<NativeSocket>(handle_), reinterpret_cast<sockaddr*>(&sa), &len) < 0) {
        GetSocketError();
        return peer_address_;
    }
    return static_cast<std::uint32_t>(sa.sin_addr.s_addr);
}

// 0x82810c _ZN7CSocket11GetPeerPortEv
// On failure the port recorded by Accept is returned.
std::uint16_t CSocket::GetPeerPort() {
    sockaddr_in sa{};
    socklen_t len = sizeof(sa);
    if (::getpeername(static_cast<NativeSocket>(handle_), reinterpret_cast<sockaddr*>(&sa), &len) < 0) {
        return peer_port_;
    }
    return ntohs(sa.sin_port);
}

// 0x8281a8 _ZN7CSocket11ReceiveFromER7in_addrRtPci
// Partial: the decompiled error path calls the error hook and returns the negative count.
std::int64_t CSocket::ReceiveFrom(in_addr& from, std::uint16_t& from_port, char* buf, int len) {
    if (!valid_) {
        return -1;
    }
    from.s_addr = 0;
    from_port = 0;
    sockaddr_in sa{};
    socklen_t slen = sizeof(sa);
    int got = static_cast<int>(::recvfrom(static_cast<NativeSocket>(handle_), buf, len, 0,
                                          reinterpret_cast<sockaddr*>(&sa), &slen));
    if (got < 0) {
        if (WouldBlock()) {
            return 0;
        }
        GetSocketError();
        return got;
    }
    if (got == 0) {
        from.s_addr = 0;
        from_port = 0;
    } else {
        from_port = ntohs(sa.sin_port);
        from = sa.sin_addr;
    }
    return got;
}

// 0x8282ac _ZN7CSocket7ReceiveEPci
std::int64_t CSocket::Receive(char* buf, int len) {
    if (!valid_ || !IsConnected()) {
        return -1;
    }
    connected_ = true;
    if (!DataAvailable(0)) {
        return 0;
    }
    int got = static_cast<int>(::recv(static_cast<NativeSocket>(handle_), buf, len, 0));
    if (got < 0) {
        if (WouldBlock()) {
            return 0;
        }
        GetSocketError();
    }
    return got;
}

// 0x828380 _ZN7CSocket6SendToERK7in_addrtPvi
// Partial: the decompile shows no return value; the sendto() result is returned here.
std::int64_t CSocket::SendTo(const in_addr& to, std::uint16_t port, const void* data, int len) {
    if (!valid_) {
        return -1;
    }
    sockaddr_in sa = MakeAddress(to, port);
    int sent = static_cast<int>(::sendto(static_cast<NativeSocket>(handle_), static_cast<const char*>(data),
                                         len, 0, reinterpret_cast<const sockaddr*>(&sa), sizeof(sa)));
    if (sent < 0) {
        GetSocketError();
    }
    return sent;
}

// 0x82844c _ZN7CSocket4SendEPvi
std::int64_t CSocket::Send(const void* data, int len) {
    if (!valid_ || !IsConnected()) {
        return -1;
    }
    connected_ = true;
    int sent = static_cast<int>(::send(static_cast<NativeSocket>(handle_), static_cast<const char*>(data), len, 0));
    if (sent < 0) {
        GetSocketError();
    }
    return sent;
}

// 0x8284c8 _ZN7CSocket16SetSocketOptionsEj
// Bit 1: SO_REUSEADDR. Bit 0: SO_BROADCAST. Bit 2: TCP_NODELAY. Each failure logs and stops.
void CSocket::SetSocketOptions(unsigned int options) {
    int on = 1;
    if ((options & 2) != 0) {
        if (::setsockopt(static_cast<NativeSocket>(handle_), SOL_SOCKET, SO_REUSEADDR,
                         reinterpret_cast<const char*>(&on), sizeof(on)) < 0) {
            GetSocketError();
            return;
        }
    }
    if ((options & 1) != 0) {
        if (::setsockopt(static_cast<NativeSocket>(handle_), SOL_SOCKET, SO_BROADCAST,
                         reinterpret_cast<const char*>(&on), sizeof(on)) < 0) {
            GetSocketError();
            return;
        }
    }
    if ((options & 4) != 0) {
        if (::setsockopt(static_cast<NativeSocket>(handle_), IPPROTO_TCP, TCP_NODELAY,
                         reinterpret_cast<const char*>(&on), sizeof(on)) < 0) {
            GetSocketError();
        }
    }
}

// 0x8285ec _ZN7CSocket6AcceptER7in_addrRt
// Partial: the decompiled body sets O_NONBLOCK on the listening handle and records the
// peer, but never stores the accepted handle. The new handle is returned here so the
// caller can keep it; the original's use of it is not recovered.
int CSocket::Accept(in_addr& from, std::uint16_t& from_port) {
    if (!DataAvailable(0)) {
        return -1;
    }
    sockaddr_in sa{};
    socklen_t len = sizeof(sa);
    int accepted = static_cast<int>(::accept(static_cast<NativeSocket>(handle_),
                                             reinterpret_cast<sockaddr*>(&sa), &len));
    if (accepted < 0) {
        GetSocketError();
        return -1;
    }
    net_platform::SetNonBlocking(static_cast<NativeSocket>(handle_));
    from = sa.sin_addr;
    from_port = ntohs(sa.sin_port);
    peer_address_ = static_cast<std::uint32_t>(sa.sin_addr.s_addr);
    peer_port_ = from_port;
    return accepted;
}

// 0x8286e4 _ZN7CSocket6ListenEv
int CSocket::Listen() {
    if (listening_) {
        return 0;
    }
    if (::listen(static_cast<NativeSocket>(handle_), 10) != 0) {
        GetSocketError();
    }
    listening_ = true;
    return 0;
}

// 0x828738 _ZN7CSocket7ConnectERK7in_addrt
// Returns 0 when connected or the attempt would block, 1 when the connect is in progress
// (the connected flag is set), and -1 after a hard failure (the socket is closed).
int CSocket::Connect(const in_addr& peer, std::uint16_t port) {
    sockaddr_in sa = MakeAddress(peer, port);
    int result = ::connect(static_cast<NativeSocket>(handle_), reinterpret_cast<const sockaddr*>(&sa), sizeof(sa));
    if (result < 0 && !WouldBlock() && GetSocketError() != net_platform::kErrIsConnected) {
        if (GetSocketError() == net_platform::kErrInProgress) {
            connected_ = true;
            return 1;
        }
        GetSocketError();
        Close();
        return -1;
    }
    return 0;
}
