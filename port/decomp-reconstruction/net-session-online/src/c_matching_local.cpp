#include "net-session-online/c_matching_local.hpp"

#include "net-session-online/member_mask.hpp"
#include "net-session-online/deps/matching_queue.hpp"
#include "net-session-online/deps/net_bit_stream.hpp"

// 0x801b14 _ZN14CMatchingLocal12IsRoomHiddenEv
bool CMatchingLocal::IsRoomHidden() const {
    if (roomHiddenFlag_ != 0) {
        return true;
    }
    return roomClosed_ != 0;
}

// 0x805664 _ZN14CMatchingLocal13GetMemberMaskEi
std::int32_t CMatchingLocal::GetMemberMask(std::int32_t memberId) const {
    return MemberMaskForId(memberId);
}

// 0x8056a0 _ZN14CMatchingLocal16GetTransportTypeE14tPROTOCOL_TYPE
std::int32_t CMatchingLocal::GetTransportType(std::uint32_t protocolType) {
    static const std::int32_t kTransportTypes[3] = {1, 2, 1};  // CSWTCH.616
    if (protocolType > 2) {
        return 0;
    }
    return kTransportTypes[protocolType];
}

// 0x801fb0 _ZN14CMatchingLocal14SendInvitationEPKcS1_b
std::int32_t CMatchingLocal::SendInvitation(const char* /*from*/, const char* /*to*/, bool /*flag*/) {
    MatchingInternalEventQueue().AddEvent(0x800015, nullptr, 0);
    return 0;
}

// 0x801fe8 _ZN14CMatchingLocal17ReceiveInvitationEv
std::int32_t CMatchingLocal::ReceiveInvitation() {
    MatchingInternalEventQueue().AddEvent(0x800017, nullptr, 0);
    return 0;
}

// 0x805998 _ZN14CMatchingLocal26ProcessChangeServerMessageEiR12NetBitStream
// The original returns the AddEvent result, which is nothing in the port's queue.
void CMatchingLocal::ProcessChangeServerMessage(std::int32_t /*from*/, NetBitStream& stream) {
    std::int32_t serverId = 0;
    stream.ReadByteArray(&serverId, sizeof serverId);
    serverMemberId_ = serverId;
    MatchingInternalEventQueue().AddEvent(0x80000b, &serverId, sizeof serverId);
}
