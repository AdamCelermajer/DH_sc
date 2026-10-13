#include "net-session-online/c_matching_local.hpp"

#include "net-session-online/member_mask.hpp"
#include "net-session-online/deps/matching_queue.hpp"
#include "net-transport/net_bit_stream.hpp"

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

// The matching object the static packet callbacks use (CMatching::s_Matching in the original).
CMatchingLocal* CMatchingLocal::s_Matching = nullptr;

// 0x807e44 _ZN14CMatchingLocalC2Ev
// Partial: the original runs Reset() and constructs the NetStruct members (room info, member
// and server-backup records) before the counters are final. Reset() is not in this port
// yet, and the NetStruct members are not modelled, so only the counters and containers are set.
CMatchingLocal::CMatchingLocal() {
    nextPeerKey_ = 1;          // +0x3634
    memberId_ = -1;            // +0x3638
    serverMemberId_ = -2;      // +0x363c
    roomOperationPending_ = false;  // +0x3630
}

// 0x807f80 _ZN14CMatchingLocalD1Ev
// Partial: the original destroys the NetStruct members and clears the server map before the
// base destructor. Here the members release themselves, so the body is empty.
CMatchingLocal::~CMatchingLocal() = default;
