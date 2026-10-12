// CTransport: base of the concrete transports (CTcp, CUdp, the Bluetooth pair).
// Reconstructed from libDungeonHunter2.so pseudocode. Only the slots whose
// bodies were recovered are named here; the other vtable slots are pure in the
// original and are declared by the derived classes.
#pragma once

#include "net-transport/net_network_id.hpp"

class CTransport {
public:
    virtual ~CTransport() = default;             // 0x8274e8 _ZN10CTransportD1Ev
    virtual int Connect(CNetworkId& peer);       // +0x08, 0x8274ec _ZN10CTransport7ConnectER10CNetworkId
    virtual int Disconnect();                    // +0x14, 0x8274f4 _ZN10CTransport10DisconnectEv
    virtual void StartBroadcast();               // +0x30, 0x8274fc _ZN10CTransport14StartBroadcastEv
    virtual void StopBroadcast();                // +0x34, 0x827500 _ZN10CTransport13StopBroadcastEv
};
