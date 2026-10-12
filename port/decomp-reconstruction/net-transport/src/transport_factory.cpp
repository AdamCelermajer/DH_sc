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
// Partial: type 1 (UDP) and type 2 (TCP) are initialised as in the original. Types 3 and 4
// (Bluetooth) call CBluetoothUnreliable::InitializeTransport and CBluetoothReliable::InitializeTransport,
// which are not reconstructed yet, so those cases only return 0.
int CTransportFactory::InitializeTransport(tTRANSPORT_TYPE type) {
    switch (type - 1) {
        case 0:
            CUdp::InitializeTransport();
            break;
        case 1:
            CTcp::InitializeTransport();
            break;
        default:
            break;
    }
    return 0;
}

// 0x8272c4 _ZN17CTransportFactory16ConnectTransportER10CNetworkId
// The argument is the peer id; its presence flags select the transports (bit 1 = TCP,
// bit 2 = Bluetooth unreliable, bit 3 = Bluetooth reliable). Partial: only the TCP branch
// (flag 2) is reconstructed. The Bluetooth branches need CBluetoothUnreliable::Create,
// CBluetoothReliable::Create and the host-name lookup on CTransportManagerImpl, which are not
// in the port yet. The Hex-Rays pseudocode for those branches also loses std::string arguments.
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
