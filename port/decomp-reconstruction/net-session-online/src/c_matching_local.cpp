#include "net-session-online/c_matching_local.hpp"

#include "net-session-online/member_mask.hpp"

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
