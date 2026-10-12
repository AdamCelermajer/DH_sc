// Stand-in for CTransportManager, the registry that owns the transports. The class
// is reconstructed in a later session; the transports only need these calls, which
// are no-ops here (the callers' own bodies are complete).
#pragma once

#include <cstdint>

#include "net-transport/net_network_id.hpp"
#include "net-transport/transport.hpp"

class CTransportManager {
public:
    // 0x81b4c4 _ZN17CTransportManager12AddTransportEP10CTransport (not reconstructed)
    void AddTransport(CTransport* transport) { (void)transport; }
    // 0x8500524 _ZN17CTransportManager14AcceptCallbackE15tTRANSPORT_TYPEP10CTransport (not reconstructed)
    void AcceptCallback(int transport_type, CTransport* transport) {
        (void)transport_type;
        (void)transport;
    }
    // _ZN17CTransportManager16ReceiverCallbackEP10CTransportR10CNetworkIdPci (not reconstructed)
    void ReceiverCallback(CTransport* transport, CNetworkId& from, const char* data, int size) {
        (void)transport;
        (void)from;
        (void)data;
        (void)size;
    }
};

// 0x8497664 _Z15GetTransportMgrv
inline CTransportManager& GetTransportMgr() {
    static CTransportManager manager;
    return manager;
}
