// Call shims for CTransport virtual slots that the transport base does not declare
// (transport.hpp names only +0x08, +0x14, +0x30 and +0x34). The slots belong to the
// transport slice; each shim keeps the byte offset of the original call and returns a
// neutral value, so the transport manager compiles on its own. Replace each shim with the
// real virtual call when CTransport declares the slot.
#pragma once

#include <cstdint>

#include "net-transport/net_network_id.hpp"
#include "net-transport/transport.hpp"

namespace transport_slots {
// +0x20: transport type (tTRANSPORT_TYPE). Stand-in: type 0.
inline int Type(CTransport* transport) {
    (void)transport;
    return 0;
}
// +0x24: connected state, read as the result of IsConnectedTo. Stand-in: not connected.
inline int ConnectedState(CTransport* transport) {
    (void)transport;
    return 0;
}
// +0x28: IsConnectedTo(peer). Stand-in: never connected.
inline bool IsConnectedTo(CTransport* transport, const CNetworkId& peer) {
    (void)transport;
    (void)peer;
    return false;
}
// +0x18: writes the transport's local network id. Stand-in: an empty id.
inline void GetLocalId(CTransport* transport, CNetworkId& out) {
    (void)transport;
    out.Reset();
}
// +0x2c: CanDisconnectFrom(peer). Stand-in: never.
inline bool CanDisconnectFrom(CTransport* transport, const CNetworkId& peer) {
    (void)transport;
    (void)peer;
    return false;
}
// +0x10: Receive pass. Stand-in: nothing received.
inline int Receive(CTransport* transport) {
    (void)transport;
    return 0;
}
// +0x0c: Send(peer, data, size). Stand-in: nothing sent.
inline int Send(CTransport* transport, const CNetworkId& peer, const void* data, int size) {
    (void)transport;
    (void)peer;
    (void)data;
    (void)size;
    return 0;
}
// Object flag at +0x04 of the transport (bit 0 = listening). Stand-in: not listening.
inline bool IsListening(CTransport* transport) {
    (void)transport;
    return false;
}
}  // namespace transport_slots
