// Platform layer for the socket wrappers: POSIX sockets, or Winsock under _WIN32.
// Only the calls the wrappers need are here: close, last error, and the error
// codes the original compares against (EWOULDBLOCK, EISCONN, EINPROGRESS).
#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <cerrno>
#endif

namespace net_platform {

#ifdef _WIN32
using NativeSocket = SOCKET;
constexpr NativeSocket kInvalidSocket = INVALID_SOCKET;
constexpr int kErrWouldBlock = WSAEWOULDBLOCK;
constexpr int kErrIsConnected = WSAEISCONN;
constexpr int kErrInProgress = WSAEINPROGRESS;
inline int CloseSocket(NativeSocket s) { return closesocket(s); }
inline int LastError() { return WSAGetLastError(); }
// Equivalent of fcntl(F_SETFL, flags | O_NONBLOCK) on the handle.
inline int SetNonBlocking(NativeSocket s) { u_long mode = 1; return ioctlsocket(s, FIONBIO, &mode); }
#else
using NativeSocket = int;
constexpr NativeSocket kInvalidSocket = -1;
constexpr int kErrWouldBlock = EWOULDBLOCK;
constexpr int kErrIsConnected = EISCONN;
constexpr int kErrInProgress = EINPROGRESS;
inline int CloseSocket(NativeSocket s) { return ::close(s); }
inline int LastError() { return errno; }
inline int SetNonBlocking(NativeSocket s) {
    int flags = ::fcntl(s, F_GETFL, 0);
    return ::fcntl(s, F_SETFL, flags | O_NONBLOCK);
}
#endif

}  // namespace net_platform
