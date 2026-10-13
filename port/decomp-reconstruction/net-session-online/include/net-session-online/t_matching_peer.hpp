// tMatchingPeer: one room server seen by CMatchingLocal (the value type of its server map).
// The body is tMatchingPeerT (t_matching_peer_t.hpp); this header instantiates it with the
// CRoomAttributes stand-in of deps/matching_stand_ins.hpp, which CMatchingLocal uses.
// CMatchingGLLive instantiates the same template with the real CRoomAttributes.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x00 member id (int)
//   +0x04 peer address (CNetworkId, 28 bytes)
//   +0x20 last-seen time in ms (int), compared with the online clock by PurgeServers
//   +0x24 transport name (std::string here; the original uses the STL private layout)
//   +0x3c rank value (int), compared with CMatchingLocal::joinThreshold_ by JoinRoomInternal
//   +0x40 time offset (int), set from the online clock minus the poll time
//   +0x48 room attributes (CRoomAttributes)
#pragma once

#include "net-session-online/deps/matching_stand_ins.hpp"
#include "net-session-online/t_matching_peer_t.hpp"

using tMatchingPeer = tMatchingPeerT<CRoomAttributes, 0>;
