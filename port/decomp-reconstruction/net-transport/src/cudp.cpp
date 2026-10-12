#include "net-transport/cudp.hpp"

#include "net-transport/deps/transport_manager.hpp"

bool CUdp::s_initialized_ = false;

// 0x827504 _ZN4CUdp11IsConnectedEv
bool CUdp::IsConnected() const {
    return true;
}

// 0x82750c _ZN4CUdp17CanDisconnectFromER10CNetworkId
bool CUdp::CanDisconnectFrom(CNetworkId& peer) const {
    (void)peer;
    return false;
}

// 0x827514 _ZN4CUdp7GetTypeEv
int CUdp::GetType() const {
    return 1;
}

// 0x82751c _ZN4CUdp18TerminateTransportEv
int CUdp::TerminateTransport() {
    s_initialized_ = false;
    return 0;
}

// 0x827544 _ZN4CUdp13IsConnectedToER10CNetworkId
// Returns the inverse of the field2 presence bit (flags & 4), as the original computes it.
bool CUdp::IsConnectedTo(const CNetworkId& peer) const {
    return (peer.Flags() & 4) == 0;
}

// 0x827554 _ZN4CUdp16GetPeerNetworkIdEv
CNetworkId CUdp::GetPeerNetworkId() const {
    CNetworkId id;
    id.Reset();
    return id;
}

// 0x827568 _ZN4CUdp17GetLocalNetworkIdEv
CNetworkId CUdp::GetLocalNetworkId() {
    std::uint32_t address = socket_.GetLocalAddress();
    std::uint16_t port = socket_.GetLocalPort();
    CNetworkId id;
    id.Reset();
    id.SetAddress1(port, address);
    return id;
}

// 0x82770c _ZN4CUdp4SendER10CNetworkIdPvi
// Sends only when the socket is open and the peer id carries an address1 (flag bit 0).
std::int64_t CUdp::Send(CNetworkId& peer, const void* data, int len) {
    std::uint16_t port = 0;
    std::uint32_t address = 0;
    if (!socket_.IsValid() || !peer.GetAddress1(port, address)) {
        return -1;
    }
    in_addr to{};
    to.s_addr = address;
    return socket_.SendTo(to, port, data, len);
}

// 0x827764 _ZN4CUdp11CloseSocketEv
int CUdp::CloseSocket() {
    if (!socket_.IsValid()) {
        return 0;
    }
    return socket_.Close();
}

// 0x827780 _ZN4CUdp10OpenSocketEtb
// Opens the socket only when it is not open yet; the local address is looked up afterwards
// as the original does (its debug print is dropped). Partial for that reason.
int CUdp::OpenSocket(std::uint16_t port, bool broadcast) {
    if (!socket_.IsValid()) {
        unsigned int options = broadcast ? 3u : 2u;
        int result = socket_.OpenUDP(port, options);
        if (result < 0) {
            return result;
        }
    }
    socket_.GetLocalAddress();
    socket_.GetLocalPort();
    return 0;
}

// 0x827804 _ZN4CUdpD1Ev
// 0x827860 _ZN4CUdpD2Ev
CUdp::~CUdp() {
    CloseSocket();
}

// 0x8278a0 _ZN4CUdp12CreateByPortEtj
CUdp* CUdp::CreateByPort(std::uint16_t port, unsigned int flags) {
    CUdp* transport = new CUdp();
    transport->create_flags_ = flags;
    transport->OpenSocket(port, ((flags >> 1) & 1) != 0);
    return transport;
}

// 0x8278fc _ZN4CUdp19InitializeTransportEv
// Registers the two UDP transports (a broadcast one on port 0x1a78 and a plain one on any
// port) with the transport manager, once.
int CUdp::InitializeTransport() {
    if (!s_initialized_) {
        CUdp* first = CreateByPort(0x1a78, 4);
        GetTransportMgr().AddTransport(first);
        CUdp* second = CreateByPort(0, 3);
        GetTransportMgr().AddTransport(second);
    }
    s_initialized_ = true;
    return 0;
}
