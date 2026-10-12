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

// 0x803cbc CMatchingBluetooth::GenerateBluetoothDeviceName(tMatchingPeer)
// Partial. Written: the "|m=<member id>" field, the "|i_<n>=<value>" fields for the active int
// attributes 1..7, in that order.
// Not written (the output differs when these apply):
//   - the peer name append. The original appends a string stored at tMatchingPeer +0x34..+0x38.
//     That does not match the port's tMatchingPeer::name (+0x24 in its layout comment), so the
//     field is not identified and nothing is appended.
//   - the "|b_<n>=" fields for the bin attributes 3 and 4. They need CRoomAttributes::GetMaxBufferSize
//     (table CSWTCH_280), which is not recovered.
//   - the final append. Hex-Rays shows it as (first "|", last ""), an empty range, so nothing is
//     written for it here.
std::string CMatchingBluetooth::GenerateBluetoothDeviceName(std::int32_t memberId,
                                                            const CRoomAttributes& attributes) {
    std::string out;
    char field[64];

    std::snprintf(field, sizeof field, "|m=%d", static_cast<int>(memberId));
    out += field;

    // The original loop: for i = 1 .. 7, skip inactive attributes, then append "|i_<i>=<value>".
    for (int i = 1; i < CRoomAttributes::kIntAttributeCount; ++i) {
        if (!attributes.IsAttributeActiveInt(i)) {
            continue;
        }
        std::snprintf(field, sizeof field, "|i_%d=%d", i, attributes.GetAttributeInt(i));
        out += field;
    }
    return out;
}
