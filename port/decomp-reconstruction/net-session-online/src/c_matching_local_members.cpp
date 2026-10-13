#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/member_mask.hpp"

// 0x805c04 _ZNK14CMatchingLocal14GetMemberCountEv
std::int32_t CMatchingLocal::GetMemberCount() const {
    return CConnectionManager::GetInstance()->GetConnectionCount(false) + 1;
}

// 0x805c1c _ZN14CMatchingLocal8IsInRoomEi
// Partial. Exact: for the local member id the answer is the no-argument IsInRoom() (vtable +0x64, which is
// CMatchingLocal::IsInRoom); for another id, a connected member is in the room. The original's last clause
// is not reproduced: (memberId_ < 0 or memberId_ != serverMemberId_) && IsInRoom() && the member record found
// by CMatching::GetMemberIndex has its member-id value (element +0x150) >= 0. Those member records are not
// declared in this class (layout request), so the result is false there.
bool CMatchingLocal::IsInRoom(std::int32_t memberId) const {
    if (memberId_ == memberId) {
        return IsInRoom();  // vtable +0x64
    }
    if (CConnectionManager::GetInstance()->IsConnected(memberId)) {
        return true;
    }
    return false;  // member-table clause not reproduced (see above)
}

// 0x805d38 _ZN14CMatchingLocal10KickMemberEi
std::int32_t CMatchingLocal::KickMember(std::int32_t memberId) {
    CConnectionManager::GetInstance()->Disconnect(memberId, true);
    return 0;
}

// 0x80675c _ZN14CMatchingLocal19GenerateNewMemberIdEv
// Partial. The room mask is CMatching::GetRoomMask() (base, 0x8000dc, itself partial). The counter is
// memberIdCounter_, the value of the room-info int (+0x4bb8 object, value at +0x4bd8). The original writes it
// through NetStruct SetValue, which also sets the member's change flag when the value changes; that flag is
// not reproduced because the counter is a plain field here.
std::int32_t CMatchingLocal::GenerateNewMemberId() {
    const std::uint32_t roomMask = CMatching::GetRoomMask();  // 0x8000dc
    std::int32_t id = 0;
    do {
        const std::int32_t counter = memberIdCounter_;
        // The 32-bit original wraps 16 * counter; the unsigned multiply keeps that wrap defined here.
        const auto scaled = static_cast<std::int32_t>(16u * static_cast<std::uint32_t>(counter));
        id = counter / 17 + 1 + scaled % 272;
        memberIdCounter_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(counter) + 1u);
    } while ((static_cast<std::uint32_t>(GetMemberMask(id)) & roomMask) != 0);  // vtable +0x84 = GetMemberMask
    return id;
}
