// Track B batch e1-net-session-online-CMatchingBluetooth-0.
//
// Only CMatchingBluetooth::GenerateBluetoothDeviceName is written here (partial). The other
// functions of this class are not rebuilt in this batch: their bodies use the 43 KB object
// layout (the peer map, the CBluetoothNetRoomInfo member, the device name string, the state
// bytes), which is not declared for this class, and the base class calls (CMatching::Initialize,
// CMatching::m_eventQueue). They are listed as skipped in the ledger with the reason for each.

#include "net-session-online/c_matching_bluetooth.hpp"

#include <cstdio>
#include <string>

#include "net-transport/transport_manager.hpp"  // GetTransportMgr, CTransportManager

// 0x803cbc CMatchingBluetooth::GenerateBluetoothDeviceName(tMatchingPeer)
// Partial. Written: the "|m=<member id>" field (tMatchingPeer +0x00), the "|i_<n>=<value>" fields for the
// active int attributes 1..7 in that order (room attributes at +0x48, the CRoomAttributes copy in the original).
// Not written (the output differs when these apply):
//   - the peer name append. The original appends a string read from tMatchingPeer +0x34 and +0x38. The
//     port's tMatchingPeer::name is at +0x24 in its layout comment, and the STL fields at +0x34/+0x38 are
//     not identified, so nothing is appended.
//   - the "|b_<n>=" fields for the bin attributes 3 and 4. They need CRoomAttributes::GetMaxBufferSize
//     (table CSWTCH_280), which is not recovered. The original builds "|b_<n>=" and the bin bytes into a
//     buffer of GetMaxBufferSize + 5 bytes and appends its strlen.
//   - the final append. Hex-Rays shows it as (first "|", last ""), an empty range, so nothing is
//     written for it here.
std::string CMatchingBluetooth::GenerateBluetoothDeviceName(const tMatchingPeer& peer) {
    std::string out;
    char field[64];

    std::snprintf(field, sizeof field, "|m=%d", static_cast<int>(peer.memberId));
    out += field;

    // The original loop: for i = 1 .. 7, skip inactive attributes, then append "|i_<i>=<value>".
    for (int i = 1; i < CRoomAttributes::kIntAttributeCount; ++i) {
        if (!peer.roomAttributes.IsAttributeActiveInt(i)) {
            continue;
        }
        std::snprintf(field, sizeof field, "|i_%d=%d", i, peer.roomAttributes.GetAttributeInt(i));
        out += field;
    }
    return out;
}

// 0x801c58 _ZN18CMatchingBluetooth18SetServerBroadcastEv
// The listen id of all transports (GetListenNetworkId(0)) is passed to EnableBroadcast, which starts the broadcast
// of the transport whose local id equals it. The object is not read, and the result (EnableBroadcast's leftover
// r0) is not returned.
void CMatchingBluetooth::SetServerBroadcast() {
    CTransportManager* manager = GetTransportMgr();
    CNetworkId listen = GetTransportMgr()->GetListenNetworkId(0);
    manager->EnableBroadcast(listen);
}
