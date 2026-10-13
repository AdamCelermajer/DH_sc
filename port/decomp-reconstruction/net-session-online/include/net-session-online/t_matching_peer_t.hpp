// tMatchingPeerT: the body of tMatchingPeer, one room server seen by the matching layer (the value
// type of the std::map<unsigned long long, tMatchingPeer> of CMatchingLocal at +0x4644 and of
// CMatchingGLLive at +0x39e8).
//
// It is a template over the room attributes type only because the port has two CRoomAttributes
// definitions that cannot be included in one translation unit: the stand-in in
// deps/matching_stand_ins.hpp (used by CMatchingLocal) and the real class in c_room_attributes.hpp
// (used by CMatchingBase and CMatchingGLLive). t_matching_peer.hpp names the stand-in instance
// tMatchingPeer; c_matching_gl_live.hpp names the real instance tMatchingPeer. When the room layer
// is unified the template collapses to one class.
//
// The two CRoomAttributes classes have the same name, so their instantiations would get the same
// symbol names and the linker could merge members of different layouts. The Variant parameter keeps
// them apart: 0 for the stand-in (t_matching_peer.hpp), 1 for the real class (c_matching_gl_live.hpp).
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x00 member id (int)
//   +0x04 peer address (CNetworkId, 28 bytes)
//   +0x20 last-seen time in ms (int), compared with the online clock by PurgeServers
//   +0x24 transport name (std::string here; the original uses the STL private layout)
//   +0x3c rank value (int), compared with CMatchingLocal::joinThreshold_ by JoinRoomInternal
//   +0x40 time offset (int), set from the online clock minus the poll time
//   +0x48 room attributes
#pragma once

#include <cstdint>
#include <string>

#include "net-session-online/deps/network_id.hpp"

template <class RoomAttributes, int Variant = 0>
class tMatchingPeerT {
public:
    // 0x8081c4 _ZN13tMatchingPeerC1Ev / C2Ev
    tMatchingPeerT() = default;
    // 0x808208 _ZN13tMatchingPeerC1ERKS_ / C2ERKS_: member-wise copy in the original's field order.
    // The original copies the 28-byte address and the time as raw words; the port copies the typed fields.
    tMatchingPeerT(const tMatchingPeerT& other)
        : memberId(other.memberId),
          address(other.address),
          lastSeenMs(other.lastSeenMs),
          name(other.name),
          rank(other.rank),
          timeOffset(other.timeOffset),
          roomAttributes(other.roomAttributes) {}
    // 0x807240 _ZN13tMatchingPeerD1Ev / D2Ev
    ~tMatchingPeerT() = default;

    tMatchingPeerT& operator=(const tMatchingPeerT&) = default;

    std::int32_t memberId = 0;                 // +0x00
    CNetworkId address;                        // +0x04
    std::int32_t lastSeenMs = 0;               // +0x20
    std::string name;                          // +0x24
    std::int32_t rank = 0;                     // +0x3c
    std::int32_t timeOffset = 0;               // +0x40
    RoomAttributes roomAttributes;             // +0x48
};
