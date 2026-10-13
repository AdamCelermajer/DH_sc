// Stand-ins for the calls CConnectionManager makes on CTransportManager. CTransportManager is
// owned by the transport-manager slice; only the calls the connection manager uses are declared
// here. Bodies are no-op stand-ins and must be replaced when CTransportManager is merged.
//
// CMatching and CEventQueueBase are not stand-ins any more: they come from net-session-online
// (net-session-online/deps/matching.hpp and net-session-online/c_event_queue_base.hpp).
#pragma once

#include <cstdint>

#include "net-transport/connection.hpp"
#include "net-transport/net_bit_stream.hpp"
#include "net-transport/net_network_id.hpp"

class CTransportManager;  // full class: net-transport/deps/transport_manager.hpp

// Calls CConnectionManager makes on CTransportManager. The class itself is in the
// transport-manager slice; until it is merged these are no-op stand-ins that return 0.
namespace transport_manager_calls {
void Update(CTransportManager& tm);
void Initialize(CTransportManager& tm);
void Terminate(CTransportManager& tm);
void InitializeTransport(CTransportManager& tm, std::intptr_t transport);
void Disconnect(CTransportManager& tm, CNetworkId& peer);
// CTransportManager::Send with a network id: (transport, to, packet type, data, size).
int Send(CTransportManager& tm, std::intptr_t transport, const CNetworkId& to,
         int packet_type, const void* data, int size);
// CTransportManager::Send without a network id: (transport, packet type, data, size).
int Send(CTransportManager& tm, std::intptr_t transport, int packet_type,
         const void* data, int size);
}  // namespace transport_manager_calls
