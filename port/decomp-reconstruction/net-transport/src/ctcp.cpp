#include "net-transport/ctcp.hpp"

#include <cstring>

#include "net-transport/deps/transport_manager.hpp"

bool CTcp::s_initialized_ = false;

// 0x82a36c _ZN4CTcpD1Ev
// 0x82a3c8 _ZN4CTcpD2Ev
CTcp::~CTcp() {
    CloseSocket();
}

// 0x82a228 _ZN4CTcp7ConnectER10CNetworkId
// Connects to the address0 of peer; fails when the socket is closed or the id has no address0.
int CTcp::Connect(CNetworkId& peer) {
    std::uint16_t port = 0;
    std::uint32_t address = 0;
    if (!socket_.IsValid() || !peer.GetAddress0(port, address)) {
        return -1;
    }
    in_addr to{};
    to.s_addr = address;
    return socket_.Connect(to, port);
}

// 0x82a040 _ZN4CTcp11IsConnectedEv
bool CTcp::IsConnected() const {
    return socket_.IsConnected();
}

// 0x82a008 _ZN4CTcp13IsConnectedToER10CNetworkId
// Compares the peer's address with the given id. Partial: the decompile shows the call but
// not the returned value; the comparison result is returned here.
bool CTcp::IsConnectedTo(const CNetworkId& peer) {
    return GetPeerNetworkId().IsEqual(peer);
}

// 0x829fc4 _ZN4CTcp17CanDisconnectFromER10CNetworkId
// Partial: the decompile is a tail call into IsConnectedTo; the result is assumed to pass through.
bool CTcp::CanDisconnectFrom(CNetworkId& peer) {
    return IsConnectedTo(peer);
}

// 0x829fd8 _ZN4CTcp7GetTypeEv
int CTcp::GetType() const {
    return 2;
}

// 0x829fe0 _ZN4CTcp18TerminateTransportEv
int CTcp::TerminateTransport() {
    s_initialized_ = false;
    return 0;
}

// 0x82a048 _ZN4CTcp16GetPeerNetworkIdEv
CNetworkId CTcp::GetPeerNetworkId() {
    in_addr peer{};
    peer.s_addr = socket_.GetPeerAddress();
    std::uint16_t port = socket_.GetPeerPort();
    CNetworkId id;
    id.Reset();
    id.SetAddress0(port, peer.s_addr);
    return id;
}

// 0x82a090 _ZN4CTcp17GetLocalNetworkIdEv
CNetworkId CTcp::GetLocalNetworkId() {
    std::uint32_t address = socket_.GetLocalAddress();
    std::uint16_t port = socket_.GetLocalPort();
    CNetworkId id;
    id.Reset();
    id.SetAddress0(port, address);
    return id;
}

// 0x82a0d8 _ZN4CTcp14ReceivePacketsEv
// Reads up to 21 packets of at most 4096 bytes and hands each to the transport manager.
int CTcp::ReceivePackets() {
    CNetworkId from;
    char buffer[0x1000];
    int remaining = 0x14;
    for (;;) {
        from.Reset();
        int got = static_cast<int>(socket_.Receive(buffer, sizeof(buffer)));
        if (got < 1) {
            break;
        }
        from = GetPeerNetworkId();
        GetTransportMgr().ReceiverCallback(this, from, buffer, got);
        if (remaining == 0) {
            break;
        }
        --remaining;
    }
    return 0;
}

// 0x82a1dc _ZN4CTcp4SendER10CNetworkIdPvi
std::int64_t CTcp::Send(CNetworkId& peer, const void* data, int len) {
    if (!socket_.IsValid() || !IsConnectedTo(peer)) {
        return -1;
    }
    return socket_.Send(data, len);
}

// 0x82a5dc _ZN4CTcp7ReceiveEv
int CTcp::Receive() {
    if (socket_.IsValid()) {
        if (socket_.IsListening()) {
            return AcceptConnections();
        }
        return ReceivePackets();
    }
    return 0;
}

// 0x82a284 _ZN4CTcp11CloseSocketEv
int CTcp::CloseSocket() {
    if (socket_.IsValid()) {
        socket_.Close();
    }
    return 0;
}

// 0x82a2a4 _ZN4CTcp10OpenSocketEtb
// Partial: the original's debug print of the local address is dropped.
int CTcp::OpenSocket(std::uint16_t port, bool listen) {
    if (!socket_.IsValid()) {
        if (socket_.OpenTCP(port, 6) < 0) {
            return -1;
        }
        if (listen && socket_.IsValid() && socket_.Listen() < 0) {
            return -1;
        }
    }
    socket_.GetLocalAddress();
    socket_.GetLocalPort();
    return 0;
}

// 0x82a408 _ZN4CTcp12CreateByPortEtj
CTcp* CTcp::CreateByPort(std::uint16_t port, unsigned int flags) {
    CTcp* transport = new CTcp();
    transport->create_flags_ = flags;
    transport->OpenSocket(port, (flags & 1) != 0);
    return transport;
}

// 0x82a4b8 _ZN4CTcp16CreateBySocketIdEij
// The accepted handle is adopted with options 4 (TCP_NODELAY), as CSocketAbstract::Assign does.
CTcp* CTcp::CreateBySocketId(int handle, unsigned int flags) {
    CTcp* transport = new CTcp();
    transport->create_flags_ = flags;
    transport->socket_.Assign(handle, 4);
    return transport;
}

// 0x82a540 _ZN4CTcp17AcceptConnectionsEv
// Partial: the accepted handle is wrapped in a new CTcp and handed to the transport manager.
// The original also builds a CNetworkId for the peer that it never uses; it is not built here.
int CTcp::AcceptConnections() {
    if (socket_.IsValid() && socket_.IsListening()) {
        in_addr from{};
        std::uint16_t from_port = 0;
        int handle = socket_.Accept(from, from_port);
        if (handle > 0) {
            CTcp* accepted = CreateBySocketId(handle, 0);
            GetTransportMgr().AcceptCallback(2, accepted);
        }
    }
    return 0;
}

// 0x82a464 _ZN4CTcp19InitializeTransportEv
// Registers one listening TCP transport on any port, once (the original's CreateByPort(0, 5)).
int CTcp::InitializeTransport() {
    if (!s_initialized_) {
        CTcp* listener = CreateByPort(0, 5);
        GetTransportMgr().AddTransport(listener);
    }
    s_initialized_ = true;
    return 0;
}
