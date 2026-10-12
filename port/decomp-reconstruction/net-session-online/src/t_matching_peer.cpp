#include "net-session-online/t_matching_peer.hpp"

// 0x808208 _ZN13tMatchingPeerC1ERKS_
// Member-wise copy in the original's field order. The original copies the 28-byte address
// and the time as raw words; the port copies the typed fields.
tMatchingPeer::tMatchingPeer(const tMatchingPeer& other)
    : memberId(other.memberId),
      address(other.address),
      lastSeenMs(other.lastSeenMs),
      name(other.name),
      rank(other.rank),
      timeOffset(other.timeOffset),
      roomAttributes(other.roomAttributes) {}
