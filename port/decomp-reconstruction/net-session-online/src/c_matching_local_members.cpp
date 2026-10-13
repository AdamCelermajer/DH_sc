#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/member_mask.hpp"

// 0x805c04 _ZNK14CMatchingLocal14GetMemberCountEv
std::int32_t CMatchingLocal::GetMemberCount() const {
    return CConnectionManager::GetInstance()->GetConnectionCount(false) + 1;
}

// 0x805c1c _ZN14CMatchingLocal8IsInRoomEi
// Partial: the original asks the no-argument IsInRoom() (vtable +0x64, not in this port) for
// the local member, and reads the member-table entry from CMatching::GetMemberIndex (base class,
// not in this port). Both are stood in as "not in the room". The connection check is exact.
bool CMatchingLocal::IsInRoom(std::int32_t memberId) const {
    if (memberId_ == memberId) {
        return false;  // IsInRoom() of the local member: not in this port
    }
    if (CConnectionManager::GetInstance()->IsConnected(memberId)) {
        return true;
    }
    // Member-table check (entry +0x150 >= 0) is not reproduced.
    return false;
}

// 0x805d38 _ZN14CMatchingLocal10KickMemberEi
std::int32_t CMatchingLocal::KickMember(std::int32_t memberId) {
    CConnectionManager::GetInstance()->Disconnect(memberId, true);
    return 0;
}

// 0x80675c _ZN14CMatchingLocal19GenerateNewMemberIdEv
// Partial: the room mask comes from the base CMatching::GetRoomMask() (not in this port) and is
// taken as 0 here, so the loop ends on the first id. The counter is kept in memberIdCounter_ in
// place of the NetStruct member value (its dirty-flag write is not reproduced).
std::int32_t CMatchingLocal::GenerateNewMemberId() {
    const std::int32_t roomMask = 0;  // CMatching::GetRoomMask(this): base class, not in this port
    std::int32_t id = 0;
    do {
        const std::int32_t counter = memberIdCounter_;
        id = counter / 17 + 1 + 16 * counter % 272;
        memberIdCounter_ = counter + 1;
    } while ((GetMemberMask(id) & roomMask) != 0);
    return id;
}
