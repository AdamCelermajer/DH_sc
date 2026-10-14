#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/deps/matching_queue.hpp"
#include "net-session-online/c_room_attributes.hpp"  // CRoomAttributes (real)
#include "net-session-online/c_room_search_filter.hpp"  // CRoomSearchFilter (real)
#include "net-session-online/deps/platform_stand_ins.hpp"
#include "net-session-online/deps/stream_bytes.hpp"
#include "net-transport/connection_manager.hpp"  // CConnectionManager (real)

// 0x80890c _ZN14CMatchingLocal6UpdateEv
// Partial. The original's control flow is kept in its order (the Hex-Rays labels in brackets):
//   - CMatching::Update() runs first; its result is not used.
//   - not initialized: -1.
//   - branch choice: with connections, the server branch is taken when the local id is the server id.
//     Without connections and with a local id >= 0, a server change is announced (room flags cleared,
//     change message, event 0x80000b) and the server branch is taken. A negative local id goes to [LABEL_7].
//   - the server branch skips [LABEL_7] and [LABEL_8] (goto LABEL_30 in the original).
//   - [LABEL_7] the pending-room block: poll timer, event 0x80000f, SendClientRequest, PurgeServers.
//   - [LABEL_31] the output-queue event 0x40000e: when its member id is the local id, 0x40000a is posted.
//   - the final step: a pending 0x80000a with this member as the server gets a server response.
// Not reproduced, because the NetStruct arrays and flags of the original are not in the CMatchingLocal
// layout yet (see the layout requests in the batch ledger):
//   - the server branch body (the original's lines 300-399): the walk over the 32 ServerBackupNetStruct
//     elements (count at +0x7ee4, stride 344) that set each member slot to -3, or to the connected id list
//     from GetConnMemberIdList.
//   - [LABEL_8], for a local id > 0: IsConnected on the server id, and the reconnect walk over the same
//     elements that announces a server change when the local id is found there.
//   - [LABEL_30]: event 0x600001 on the connection manager's queue (private to CConnectionManager). Its
//     server sub-branch (IsRoomClosed, IsRoomHidden, SendServerResponse(false)) and its member record update
//     (MemberInfoNetStruct array, stride 408 at +0x4be0, event 0x800003) are skipped too.
//   - the peer loop that the virtual IsInRoom() (vtable +0x64) gates: it walks the 408-byte member array
//     and connects each member through its CNetworkId member.
//   - the NetStruct change notifications of the room flags.
// CORRECTION of this comment's opening claim ("because the NetStruct arrays and flags of the original are
// not in the CMatchingLocal layout yet"): the arrays ARE in the layout now -- roomInfo_ (+0x4A20),
// localMembers_ (+0x4BE0, 32 x 408) and serverBackup_ (+0x7EE8, 32 x 344), all real NetStruct records, and
// the two room flags are roomInfo_'s bool members (the server change below writes those, so the
// notifications DO run). This row stays partial for the five body gaps listed above, which are still not
// ported: Update is a 400-instruction state machine and the walks were not re-derived in this pass.
std::int32_t CMatchingLocal::Update() {
    CMatching::Update();  // 0x8018fc, the base update (ProcessEvents, room mask dirty). Result unused.
    if (!initialized_) {
        return -1;
    }

    // The branch choice of the original (its v6 and the server-id test before the server branch).
    bool serverBranch = false;
    const bool hasConnections = CConnectionManager::GetInstance()->GetConnectionCount(false) > 0;
    if (hasConnections) {
        serverBranch = memberId_ >= 0 && memberId_ == serverMemberId_;
    } else if (memberId_ >= 0) {
        if (memberId_ != serverMemberId_) {
            // Server change: clear the room flags, announce the change, post 0x80000b.
            serverMemberId_ = memberId_;
            roomInfo_->setRoomClosed(false);
            roomInfo_->setRoomHidden(false);
            SendChangeServerMessage();
            MatchingInternalEventQueue().AddEvent(8388619, &serverMemberId_, sizeof serverMemberId_);  // 0x80000b
        }
        serverBranch = true;  // here the local id equals the server id
    }

    if (!serverBranch) {
        // [LABEL_7] Pending-room block: runs only while a room operation is pending (+0x3630).
        if (roomOperationPending_) {
            // Poll timer. The original divides the elapsed milliseconds by 1000 with a multiply by
            // 274877907 and a shift of 38. The elapsed value is unsigned, as the original casts it.
            const std::uint32_t elapsedMs = static_cast<std::uint32_t>(OnlineNowMs()) -
                                            static_cast<std::uint32_t>(serverPollTime_);
            const std::int32_t elapsedSeconds =
                static_cast<std::int32_t>((static_cast<std::uint64_t>(elapsedMs) * 274877907ULL) >> 38);
            // The allowance is the server count plus 4, capped at 15.
            std::int32_t allowance = static_cast<std::int32_t>(servers_.size()) + 4;
            if (allowance >= 15) {
                allowance = 15;
            }
            bool sendRequest = true;
            if (allowance >= elapsedSeconds) {
                if (serverPollTime_ != 0) {
                    sendRequest = false;  // the original jumps to PurgeServers
                }
            } else if (serverPollTime_ != 0 && servers_.empty()) {
                MatchingInternalEventQueue().AddEvent(8388623, nullptr, 0);  // 0x80000f
            }
            if (sendRequest) {
                if (SendClientRequest() > 0) {
                    serverPollTime_ = OnlineNowMs();
                }
            }
            PurgeServers();  // [LABEL_62]
        }
        // [LABEL_8] (local id > 0: server reconnect walk) not reproduced; see the comment above.
    }

    // [LABEL_30] event 0x600001 not reproduced; see the comment above.

    // [LABEL_31] The output-queue event 0x40000e carries a member id in its first word. When it is the
    // local id, the output queue gets 0x40000a. The original reads 8 bytes; the second word is not used.
    if (MatchingOutputEventQueue().HasEventOccured(0x40000e, false)) {
        std::int32_t payload[2] = {0, 0};
        MatchingOutputEventQueue().GetEventData(0x40000e, payload, sizeof payload);
        if (payload[0] == LocalMemberId()) {
            MatchingOutputEventQueue().AddEvent(0x40000a, nullptr, 0);
        }
    }

    // The IsInRoom() peer loop is not reproduced; see the comment above.

    // Final step (reproduced): a pending 0x80000a with the local member as the server gets a server response.
    // The response carries the closed flag. The original's hold-back test calls vtable +0x58 (IsRoomHidden)
    // and +0x54 (IsRoomClosed).
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
