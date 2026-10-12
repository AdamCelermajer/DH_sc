// Out-of-line parts of the connection-manager stand-ins (include/net-transport/deps/connection_deps.hpp).
// These are test doubles, not reconstructions: they keep the connection manager linkable and
// testable until CMatching, CConnection, CEventQueueBase and CTransportManager are merged.
#include "net-transport/deps/connection_deps.hpp"

namespace {
CMatching g_matching;
}  // namespace

CMatching* CMatching::Get() {
    return &g_matching;
}

int CMatching::GetMemberIndex(int member_id) const {
    // Stand-in mapping: the member id is the slot index.
    return (member_id >= 0 && member_id < 32) ? member_id : -1;
}

void CMatching::SetServerForTest(bool server) {
    g_matching.server_ = server;
}

int CEventQueueBase::AddEvent(int event, void* data, int size) {
    (void)event;
    (void)data;
    (void)size;
    return 0;
}

int CEventQueueBase::Clear() {
    return 0;
}

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
    (void)packet_type;
    (void)data;
    (void)size;
    return 0;
}

int Send(CTransportManager& tm, std::intptr_t transport, int packet_type,
         const void* data, int size) {
    (void)tm;
    (void)transport;
    (void)packet_type;
    (void)data;
    (void)size;
    return 0;
}

}  // namespace transport_manager_calls
