// tMatchingPeer: one room server seen by CMatchingLocal (the value type of its server map).
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

#include <cstdint>
#include <string>

#include "net-session-online/deps/matching_stand_ins.hpp"

class tMatchingPeer {
public:
    // 0x8081c4 _ZN13tMatchingPeerC1Ev / C2Ev
    tMatchingPeer() = default;
    // 0x808208 _ZN13tMatchingPeerC1ERKS_ / C2ERKS_
    tMatchingPeer(const tMatchingPeer& other);
    // 0x807240 _ZN13tMatchingPeerD1Ev / D2Ev
    ~tMatchingPeer() = default;

    tMatchingPeer& operator=(const tMatchingPeer&) = default;

    std::int32_t memberId = 0;                 // +0x00
    CNetworkId address;                        // +0x04
    std::int32_t lastSeenMs = 0;               // +0x20
    std::string name;                          // +0x24
    std::int32_t rank = 0;                     // +0x3c
    std::int32_t timeOffset = 0;               // +0x40
    CRoomAttributes roomAttributes;            // +0x48
};
