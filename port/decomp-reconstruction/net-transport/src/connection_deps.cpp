// Out-of-line parts of the CTransportManager stand-ins (include/net-transport/deps/connection_deps.hpp).
// These are test doubles, not reconstructions: they keep the connection manager linkable and
// testable until CTransportManager is merged. CMatching and CEventQueueBase are defined once, in
// net-session-online.
#include "net-transport/deps/connection_deps.hpp"

#include "net-transport/deps/connection_env.hpp"  // SentPacketLog

namespace transport_manager_calls {

void Update(CTransportManager& tm) {
    (void)tm;
}

void Initialize(CTransportManager& tm) {
    (void)tm;
}

void Terminate(CTransportManager& tm) {
    (void)tm;
}

void InitializeTransport(CTransportManager& tm, std::intptr_t transport) {
    (void)tm;
    (void)transport;
}

void Disconnect(CTransportManager& tm, CNetworkId& peer) {
    (void)tm;
    (void)peer;
}

int Send(CTransportManager& tm, std::intptr_t transport, const CNetworkId& to,
         int packet_type, const void* data, int size) {
    (void)tm;
    (void)transport;
    (void)to;
    RecordSentPacket(packet_type, false, data, size);
    return 0;
}

int Send(CTransportManager& tm, std::intptr_t transport, int packet_type,
         const void* data, int size) {
    (void)tm;
    (void)transport;
    RecordSentPacket(packet_type, true, data, size);
    return 0;
}

}  // namespace transport_manager_calls
