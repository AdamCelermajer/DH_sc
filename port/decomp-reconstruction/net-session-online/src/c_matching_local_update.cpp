#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/deps/matching_queue.hpp"
#include "net-session-online/deps/matching_stand_ins.hpp"

// 0x80890c _ZN14CMatchingLocal6UpdateEv
// Partial. Reproduced: the not-initialized return, the server change when there are no
// connections (flags, change-server message, event 0x80000b), and the final server-response step. Not reproduced: the connected branch that walks the 32 member
// records (NetStruct array), the pending-room polling and reconnect loop (the branch that
// calls PurgeServers and SendClientRequest), the member-id list handling, and the consumption of
// events 0x600001 and 0x40000e. The base CMatching::Update() is not in this port.
std::int32_t CMatchingLocal::Update() {
    if (!initialized_) {
        return -1;
    }

    const bool hasConnections = GetConnectionMgr().GetConnectionCount(false) > 0;
    if (!hasConnections && memberId_ >= 0 && memberId_ != serverMemberId_) {
        // Server change: clear the room flags, announce the change, post 0x80000b.
        serverMemberId_ = memberId_;
        roomClosed_ = 0;
        roomHiddenFlag_ = 0;
        SendChangeServerMessage();
        MatchingInternalEventQueue().AddEvent(8388619, &serverMemberId_, sizeof serverMemberId_);  // 0x80000b
    }

    // Final step (reproduced): a pending room-hidden event with the local member as server gets
    // a server response. The response carries the closed flag.
    if (!MatchingInternalEventQueue().HasEventOccured(8388618, true)) {  // 0x80000a
        return 0;
    }
    if (memberId_ < 0 || memberId_ != serverMemberId_) {
        return 0;
    }
    if (IsRoomHidden() && !IsRoomClosed()) {
        return 0;
    }
    SendServerResponse(IsRoomClosed());
    return 0;
}
