// Free functions and CMatching members of the lib-online-glx slice that starts at 0x320e98.
// The rest of the slice is class code in its own files (online_game_state.cpp, c_online.cpp,
// cxplayer_manager.cpp); the blocked rows are listed in the ledger fragment.
#include <cstdint>

#include "net-session-online/deps/matching.hpp"

// 0x52f0e0 _Z26ALicenseCheck_GetUserAgentPc
// The original body is a bare return.
void ALicenseCheck_GetUserAgent(char* /*buffer*/) {}

// 0x7fe4a4 _ZN9CMatching25ReceiveGameBootInvitationEv
std::int32_t CMatching::ReceiveGameBootInvitation() const { return 0; }

// 0x7fe4ac _ZN9CMatching9GetRoomIdEv (64-bit result)
std::uint64_t CMatching::GetRoomId() const { return 0; }

// 0x7fe4b8 _ZN9CMatching14LimitRoomSlotsEj
std::int32_t CMatching::LimitRoomSlots(std::uint32_t /*slots*/) const { return 0; }

// 0x7fe4c0 _ZN9CMatching19SetMatchingProviderENS_18MATCHING_PROVIDERSE
void CMatching::SetMatchingProvider(MATCHING_PROVIDERS provider) {
    s_matchingProvider = static_cast<std::int32_t>(provider);
}

// 0x7fe544 _ZN9CMatching14IsMemberServerEi
// Vtable +0x74 is ServerMemberId() in the stand-in.
bool CMatching::IsMemberServer(std::int32_t memberId) const {
    return memberId >= 0 && memberId == ServerMemberId();
}

// 0x7fe59c _ZN9CMatching14GetMemberIndexEi
// Vtable +0x84 is MemberBit(). The original returns -1 for an empty mask and otherwise the
// position of the lowest set bit.
std::int32_t CMatching::GetMemberIndex(std::int32_t memberId) const {
    std::uint32_t mask = MemberBit(memberId);
    if (mask == 0) {
        return -1;
    }
    std::int32_t index = 0;
    while ((mask & 1u) == 0) {
        mask >>= 1;
        ++index;
    }
    return index;
}

// 0x7fe628 _ZN9CMatching15sReadPacketDataEiiR12NetBitStream
// Partial: forwards to vtable +0x10 (vtReadPacketData, name not identified). The original
// dereferences the instance without a check; this port returns 0 when none is set.
std::int32_t CMatching::sReadPacketData(std::int32_t a1, std::int32_t a2, NetBitStream& stream) {
    CMatching* matching = Get();
    return matching != nullptr ? matching->vtReadPacketData(a1, a2, stream) : 0;
}

// 0x7fe66c _ZN9CMatching16sWritePacketDataEiiR12NetBitStream
// Partial: forwards to vtable +0x0c (vtWritePacketData). Same null handling as above.
std::int32_t CMatching::sWritePacketData(std::int32_t a1, std::int32_t a2, NetBitStream& stream) {
    CMatching* matching = Get();
    return matching != nullptr ? matching->vtWritePacketData(a1, a2, stream) : 0;
}

// 0x7fe6b0 _ZN9CMatching26sProcessAcknowledgedPacketEii
// Partial: forwards to vtable +0x14 with (a1, a2). The Hex-Rays signature has a third argument
// that the body never forwards; the mangled name has two.
std::int32_t CMatching::sProcessAcknowledgedPacket(std::int32_t a1, std::int32_t a2) {
    CMatching* matching = Get();
    return matching != nullptr ? matching->vtProcessAcknowledgedPacket(a1, a2) : 0;
}

// 0x7fe6ec _ZN9CMatching18sProcessLostPacketEii
// Partial: forwards to vtable +0x18 (vtProcessLostPacket). Same note as sProcessAcknowledgedPacket.
std::int32_t CMatching::sProcessLostPacket(std::int32_t a1, std::int32_t a2) {
    CMatching* matching = Get();
    return matching != nullptr ? matching->vtProcessLostPacket(a1, a2) : 0;
}
