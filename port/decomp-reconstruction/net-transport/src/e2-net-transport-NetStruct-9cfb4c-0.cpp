// NetStruct functions of batch e2-net-transport-NetStruct-9cfb4c-0: the packet history
// (AddPacketHistory and the lost/acknowledged handlers), the serialize entry points and the
// deleting destructor. The class and the packet history map are declared in
// include/net-transport/net_struct.hpp. Functions without a body are not reconstructed yet: the
// comment above each one names what is missing.
#include "net-transport/net_struct.hpp"

// 0x8139bc _ZN9NetStructD0Ev
// skipped: deleting destructor (D0). The compiler generates it for `delete`; the original sets the
// vptr, runs the complete-object destructor (D1, 0x81395c, partial in tb_transport-NetStruct-0.cpp)
// and calls CustomFree(this). The packet history map is destroyed with the object.

// 0x814774 _ZN9NetStruct17ProcessLostPacketEii
// partial: the history lookup, the early exits and the erase of the packet's entry are written.
// The member loop is not: for each member bit set in the packet's mask, the original calls
// NetStructMember::AreChangesAcknowledgedBy(member, bit) and, when that is false,
// SetSentFlag(bit, false, stamp). The bit is CMatching::MemberBit(memberId) (vtable +0x84), which
// the net-transport CMatching stand-in (deps/connection_deps.hpp) does not declare. Those calls are
// skipped, so the members keep their sent flags.
void NetStruct::ProcessLostPacket(int memberId, int packetId) {
    if (packetHistory_.empty()) {
        return;
    }
    // operator[] creates the entry for memberId, as the original does.
    auto& byPacket = packetHistory_[memberId];
    const auto entry = byPacket.find(packetId);
    if (entry == byPacket.end()) {
        return;
    }
    // The original keeps the packet's stamp and mask for the member loop, then erases the entry.
    byPacket.erase(entry);
}

// 0x8149d4 _ZN9NetStruct16AddPacketHistoryEiiy
// stream_flags is the caller's NetBitStream overflow flags. The pseudocode passes it but the body
// never reads it, so it is kept only for the signature.
// The return value is an int in the pseudocode, while the first branch returns the 64-bit mask;
// the result is the mask truncated to int. The second branch returns NetStruct::Resend() (0x81347c,
// partial in tb_transport-NetStruct-0.cpp, not reconstructed yet). SerializeInternal ignores the
// result.
int NetStruct::AddPacketHistory(int memberId, int packetId, int stream_flags, std::uint64_t changeMask) {
    (void)stream_flags;
    auto& byPacket = packetHistory_[memberId];  // operator[] creates the entry, as the original does
    if (byPacket.size() <= 0x40u) {
        // Read the change counter before the entry is found (no side effects in between).
        const std::uint64_t stamp = s_changeCounter;
        tPacketHistory& entry = byPacket[packetId];  // find-or-insert, zero-initialised when new
        entry.changeStamp = stamp;
        entry.changeMask = changeMask;
        return static_cast<int>(changeMask);
    }
    // Over 64 packets for this member: drop the whole history for it, then resend.
    byPacket.clear();
    return Resend();
}

// 0x814b0c _ZN9NetStruct17SerializeInternalEbR12NetBitStreamii
// Body not written. Early exit (known): when conditional is false, the original writes a 0 bit to the
// stream and returns 0. The conditional path needs the virtual HasDataToSend(memberId) (vtable +0x28),
// which the header declares non-virtual, and the member loop needs CMatching::MemberBit (vtable +0x84),
// which the CMatching stand-in does not declare. The loops call NetStructMember::Serialize(stream, bit)
// and SetSentFlag(bit, true, ~0). The packet history is then written with AddPacketHistory, and the
// return value (0, 1 or 2) depends on the result of the mask comparison.

// 0x814cd4 _ZN9NetStruct20ConditionalSerializeEbR12NetBitStreamii
// partial: a tail jump to SerializeInternal (0x814b0c, body not written). Forwarded as the original does.
int NetStruct::ConditionalSerialize(bool conditional, NetBitStream& stream, int memberId, int packetId) {
    return SerializeInternal(conditional, stream, memberId, packetId);
}

// 0x814cd8 _ZN9NetStruct17SerializeDisabledER12NetBitStream
// partial: forwards to SerializeInternal(false, stream, -1, -1) (0x814b0c, body not written).
int NetStruct::SerializeDisabled(NetBitStream& stream) {
    return SerializeInternal(false, stream, -1, -1);
}

// 0x814d00 _ZN9NetStruct9SerializeER12NetBitStreamii
// partial: forwards to SerializeInternal(true, stream, memberId, packetId) (0x814b0c, body not written).
int NetStruct::Serialize(NetBitStream& stream, int memberId, int packetId) {
    return SerializeInternal(true, stream, memberId, packetId);
}

// 0x814d28 _ZN9NetStruct25ProcessAcknowledgedPacketEii
// partial: same as ProcessLostPacket. The history lookup, the early exits and the erase are written.
// The member loop is not: for each member bit set in the packet's mask the original calls
// NetStructMember::AcknowledgeChange(bit, stamp), with bit = CMatching::MemberBit(memberId)
// (vtable +0x84), which the CMatching stand-in does not declare. The loop is skipped.
void NetStruct::ProcessAcknowledgedPacket(int memberId, int packetId) {
    if (packetHistory_.empty()) {
        return;
    }
    // operator[] creates the entry for memberId, as the original does.
    auto& byPacket = packetHistory_[memberId];
    const auto entry = byPacket.find(packetId);
    if (entry == byPacket.end()) {
        return;
    }
    // The original keeps the packet's stamp and mask for the member loop, then erases the entry.
    byPacket.erase(entry);
}
