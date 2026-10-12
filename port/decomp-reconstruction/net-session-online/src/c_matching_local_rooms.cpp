#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/deps/matching_queue.hpp"

// Room flags: the original writes them through NetStruct members (value plus a dirty flag).
// The port writes the plain fields, so the dirty-flag bookkeeping is not reproduced.

// 0x806d64 _ZN14CMatchingLocal8HideRoomEv
std::int32_t CMatchingLocal::HideRoom() {
    roomHiddenFlag_ = 1;
    MatchingInternalEventQueue().AddEvent(8388618, nullptr, 0);  // 0x80000a
    return 0;
}

// 0x806e4c _ZN14CMatchingLocal9CloseRoomEv
std::int32_t CMatchingLocal::CloseRoom() {
    hiddenBeforeClose_ = IsRoomHidden();
    roomClosed_ = 1;
    roomHiddenFlag_ = 1;
    MatchingInternalEventQueue().AddEvent(8388618, nullptr, 0);  // 0x80000a
    return 0;
}

// 0x806fc8 _ZN14CMatchingLocal8ShowRoomEv
std::int32_t CMatchingLocal::ShowRoom() {
    if (!baseRoomClosed_) {
        roomHiddenFlag_ = 0;
        MatchingInternalEventQueue().AddEvent(8388618, nullptr, 0);  // 0x80000a
    }
    return 0;
}

// 0x8070bc _ZN14CMatchingLocal8OpenRoomEb
std::int32_t CMatchingLocal::OpenRoom(bool open) {
    if (!baseRoomClosed_) {
        roomClosed_ = 0;
        roomHiddenFlag_ = open ? 0 : 1;
        MatchingInternalEventQueue().AddEvent(8388618, nullptr, 0);  // 0x80000a
    }
    return 0;
}

// 0x80795c _ZN14CMatchingLocal9LeaveRoomEv
std::int32_t CMatchingLocal::LeaveRoom() {
    if (memberId_ >= 0 && memberId_ == serverMemberId_) {
        SendServerResponse(true);
    }
    GetConnectionMgr().DisconnectAll();
    Reset();
    MatchingInternalEventQueue().AddEvent(8388612, nullptr, 0);  // 0x800004
    return 0;
}

// 0x8079d4 _ZN14CMatchingLocal21CreateJoinRoomInternalEbR15CRoomAttributes
// Partial: the member-table writes of the original (the NetStruct member entry for the local
// member and its listen address, at member-table offsets +0x4d10 and +0x4d38 (both within the 408-byte entry of the member index)) are not
// reproduced because the member-info records are not in this port yet.
std::int32_t CMatchingLocal::CreateJoinRoomInternal(bool open, const CRoomAttributes& attributes) {
    if (!initialized_) {
        return -1;
    }
    Reset();
    roomOperationPending_ = true;
    const std::int32_t newId = GenerateNewMemberId();
    serverMemberId_ = newId;
    memberId_ = newId;
    hiddenBeforeClose_ = false;
    OpenRoom(open);
    MatchingInternalEventQueue().AddEvent(8388611, nullptr, 0);  // 0x800003
    roomAttributes_ = attributes;
    if (!IsRoomHidden()) {
        SendServerResponse(false);
    }
    return 0;
}

// 0x807bec _ZN14CMatchingLocal18SearchRoomInternalER16CRoomSearchFilterbh
// The bool and the byte are not used by the original body.
std::int32_t CMatchingLocal::SearchRoomInternal(const CRoomSearchFilter& filter, bool /*flag*/,
                                                std::uint8_t /*mode*/) {
    if (!initialized_) {
        return -1;
    }
    Reset();
    roomSearchFilter_ = filter;
    roomOperationPending_ = true;
    if (!servers_.empty()) {
        MatchingInternalEventQueue().AddEvent(8388622, nullptr, 0);  // 0x80000e
    }
    return 0;
}

// 0x808774 _ZN14CMatchingLocal16JoinRoomInternalEy
std::int32_t CMatchingLocal::JoinRoomInternal(std::uint64_t key) {
    serversMutex_.Lock();
    auto it = servers_.find(key);
    if (it == servers_.end()) {
        MatchingInternalEventQueue().AddEvent(8388620, nullptr, 0);  // 0x80000c
    } else if (it->second.rank < joinThreshold_) {
        serverMemberId_ = it->second.memberId;
        GetConnectionMgr().Connect(it->second.memberId, it->second.address);
        roomOperationPending_ = false;
    } else {
        MatchingInternalEventQueue().AddEvent(8388621, nullptr, 0);  // 0x80000d
    }
    serversMutex_.Unlock();
    return 0;
}

// 0x808694 _ZN14CMatchingLocal24GetSearchRoomAttributesEy
CRoomAttributes CMatchingLocal::GetSearchRoomAttributes(std::uint64_t key) const {
    auto it = servers_.find(key);
    if (it == servers_.end()) {
        return CRoomAttributes();
    }
    return it->second.roomAttributes;
}
