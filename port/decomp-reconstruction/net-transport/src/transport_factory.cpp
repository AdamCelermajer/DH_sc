#include "net-transport/transport_factory.hpp"

#include "net-transport/bluetooth_transport.hpp"
#include "net-transport/ctcp.hpp"
#include "net-transport/cudp.hpp"
#include "net-transport/deps/transport_manager.hpp"

// 0x827204 _ZN17CTransportFactory18TerminateTransportEv
// The original chains the four TerminateTransport calls through their return values; the
// static callees ignore their argument, so the chain is written as four calls.
int CTransportFactory::TerminateTransport() {
    CTcp::TerminateTransport();
    CUdp::TerminateTransport();
    CBluetoothReliable::TerminateTransport();
    CBluetoothUnreliable::TerminateTransport();
    return 0;
}

// 0x827220 _ZN17CTransportFactory19InitializeTransportE15tTRANSPORT_TYPE
// Dispatch on type - 1: 0 UDP, 1 TCP, 2 Bluetooth unreliable (type 3), 3 Bluetooth reliable
// (type 4). Other values do nothing. Every case returns 0, as in the original. The Bluetooth
// InitializeTransport bodies are partial in the port (their own rows in bluetooth_transport.cpp).
int CTransportFactory::InitializeTransport(tTRANSPORT_TYPE type) {
    switch (type - 1) {
        case 0:
            CUdp::InitializeTransport();
            break;
        case 1:
            CTcp::InitializeTransport();
            break;
        case 2:
            CBluetoothUnreliable::InitializeTransport();
            break;
        case 3:
            CBluetoothReliable::InitializeTransport();
            break;
        default:
            break;
    }
    return 0;
}

// 0x8272c4 _ZN17CTransportFactory16ConnectTransportER10CNetworkId
// The argument is the peer id; its presence flags select the transports (bit 1 = TCP,
// bit 2 = Bluetooth unreliable, bit 3 = Bluetooth reliable). Partial: only the TCP branch
// (flag 2) is reconstructed. The Bluetooth branches (flags 4 and 8) are not written. They call
// the manager's GetHostname (CTransportManagerImpl) on a reset CNetworkId, then CBluetoothUnreliable::Create or
// CBluetoothReliable::Create (std::string first, std::string second, int channel). The Hex-Rays
// pseudocode loses the std::string temporaries and the channel argument, so the argument roles
// are not recovered; the branches stay out until the ARM of those call sites is checked.
int CTransportFactory::ConnectTransport(CNetworkId& peer) {
    int result = 0;
    std::uint32_t flags = peer.Flags();
    if ((flags & 2u) != 0) {
        CTcp* tcp = CTcp::CreateByPort(0, flags);
        if (tcp == nullptr) {
            return -1;
        }
        GetTransportMgr().AddTransport(tcp);
        result = tcp->Connect(peer);
    }
    return result;
}
