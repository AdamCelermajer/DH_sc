// Batch e1-net-session-online-CMatchingGLLiveLobbyObserver-0.
//
// Every body here is partial. The room list (+0x18..+0x20), the member list (+0x24..+0x2c,
// 84-byte entries of tMemberInfo) and the packet reads (DataPacket vtable slots 12, 20, 64,
// 80) are not modelled in the owner header, so each body that touches them keeps only the
// scalar stores it can make by name. Unmodelled parts are listed per function.
// Original field offsets are kept as comments; see c_matching_gl_live_observers.hpp for the layout.

#include <ctime>
#include <vector>

#include "net-session-online/c_matching_gl_live.hpp"
#include "net-session-online/c_matching_gl_live_observers.hpp"

// 0x8213ec _ZN28CMatchingGLLiveLobbyObserver13ClearRoomListEv
// Partial: the original destroys each tGLRoomInfo in the room list (+0x18..+0x20) and resets
// the end pointer. The room list is not modelled, so this is a no-op.
void CMatchingGLLiveLobbyObserver::ClearRoomList() {}

// 0x821474 _ZN28CMatchingGLLiveLobbyObserver18GetRoomIndexByNameEPKc
// Partial: the original compares each room's name (+0x04 of tGLRoomInfo) with LC_API_STRCMP_0 and
// returns the index, or -1. The room list is not modelled, so this always returns -1.
std::int32_t CMatchingGLLiveLobbyObserver::GetRoomIndexByName(const char* /*name*/) const {
    return -1;
}

// 0x8214cc _ZN28CMatchingGLLiveLobbyObserver20GetMemberIndexByNameEPKc
// Partial: the original compares the name at +0x28 of each 84-byte member entry and returns the
// index, or -1. The member list is not modelled, so this always returns -1.
std::int32_t CMatchingGLLiveLobbyObserver::GetMemberIndexByName(const char* /*name*/) const {
    return -1;
}

// 0x8216a8 _ZN28CMatchingGLLiveLobbyObserver29OnMPProcessPushGameserverAddrEP10DataPacket
// Partial. The original copies the dedicated-server info (GetDedicatedServer, 0x2c-byte record),
// calls ParseDedicatedServerInfo, resets a CNetworkId, sets CMatchingGLLive::s_IsGameStarting,
// calls CConnectionManager::Connect(1, id) and then CMatchingGLLive::SetGCState(8). Only the
// s_IsGameStarting store is reproduced: the dedicated-server accessors and the connect call are
// not declared in this category.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushGameserverAddr(DataPacket* /*packet*/) {
    CMatchingGLLive::s_IsGameStarting = true;
    return 0;
}

// 0x8217a8 _ZN28CMatchingGLLiveLobbyObserver22OnMPListSessionSuccessEP10DataPacket
// Partial. Reproduced: state (+0x14) = 7 at entry, lastResult (+0x48) = 0 and updateTime (+0x10)
// = time(nullptr) at exit. Not reproduced: the flag byte at +0x07 (not in the header yet); the
// PrintDatapacket trace; the packet reads of the session count and the room records; the
// room-list rebuild (ClearRoomList and the room insert or replace); the CRoomSearchFilter test
// against CMatching+26424; and the JoinRoom and SetGCState(5) branch for a logged-in host.
void CMatchingGLLiveLobbyObserver::OnMPListSessionSuccess(DataPacket* /*packet*/) {
    state_ = 7;
    lastResult_ = 0;
    updateTime_ = static_cast<std::int32_t>(std::time(nullptr));
}

// 0x822b78 _ZN28CMatchingGLLiveLobbyObserver23OnMPLeaveSessionSuccessEv
// Partial. Reproduced: the field08 and field0c stores (-1 each), gameStarted (+0x06) = false,
// lastResult (+0x48) = 0. Not reproduced: erasing every member from the member list (+0x24).
// The original returns the erase result, or 0 when the list is empty; this returns 0.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPLeaveSessionSuccess() {
    gameStarted_ = false;
    field08_ = -1;
    field0c_ = -1;
    lastResult_ = 0;
    return 0;
}

// 0x822c58 _ZN28CMatchingGLLiveLobbyObserver28OnMPProcessPushKickOutPlayerEP10DataPacket
// Partial. Not reproduced: the packet string reads (name of the kicked player); the comparison of
// that name with the local GLX login name (GLXPlayerLogin +0x74); the member erase and the
// re-index of the member list; the CMatching event (4194318 or 4194314 on the CMatching queue)
// and SyncMemberId; the GLXPlayerMPLobby::mpSendLeaveLobby branch for the local player.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushKickOutPlayer(DataPacket* /*packet*/) {
    return 0;
}

// 0x822db4 _ZN28CMatchingGLLiveLobbyObserver32OnMPProcessPushJoinLobbyLaunchedEP10DataPacket
// Partial. The original returns -1 when the member list already holds 16 or more entries; that
// count is not available here, so the check is omitted. Not reproduced: the packet reads that
// fill a tMemberInfo (id, name, flags), the push_back into the member list, the CMatching event
// 4194316 and the vector node clean-up.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushJoinLobbyLaunched(DataPacket* /*packet*/) {
    return 0;
}

// 0x822fa4 _ZN28CMatchingGLLiveLobbyObserver24OnMPCreateSessionSuccessEP10DataPacket
// Partial. Reproduced: state (+0x14) = 4, gameStarted (+0x06) = true at entry; lastResult (+0x48)
// = 0 at exit. Not reproduced: the packet reads of the host id (+0x08) and the room name, the
// member-list reset and the single host member push, the SetGCState(6) call, the CMatching event
// queued on CMatchingGLLive::s_eventQueueGLLiveInternal (event 4), and the login-name or
// room-name copy into the member entry.
void CMatchingGLLiveLobbyObserver::OnMPCreateSessionSuccess(DataPacket* /*packet*/) {
    state_ = 4;
    gameStarted_ = true;
    lastResult_ = 0;
}

// 0x8231e0 _ZN28CMatchingGLLiveLobbyObserver22OnMPJoinSessionSuccessEP10DataPacket
// Partial. Reproduced: gameStarted (+0x06) = true at entry; state (+0x14) = 5 and lastResult
// (+0x48) = 0 at exit. Not reproduced: the member-list reset, the per-member packet reads
// (id, name, flags, state), the local-member index store at +0x0c (when the name matches the
// login name or the anonymous name), the SyncMemberId call and the final SetGCState(8).
void CMatchingGLLiveLobbyObserver::OnMPJoinSessionSuccess(DataPacket* /*packet*/) {
    gameStarted_ = true;
    state_ = 5;
    lastResult_ = 0;
}

// 0x823510 _ZN28CMatchingGLLiveLobbyObserver31OnMPProcessPushSetUserParameterEP10DataPacket
// Partial. Not reproduced: the two packet string reads (member name and value), the search of the
// member list for the name, and the assignment of the value into the member's string at +0x0c.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushSetUserParameter(DataPacket* /*packet*/) {
    return 0;
}

// 0x8235f8 _ZN28CMatchingGLLiveLobbyObserver34OnMPProcessPushJoinLobbyUnlaunchedEP10DataPacket
// Partial. Same as OnMPProcessPushJoinLobbyLaunched except that it does not set the launched flag
// (+0x50 of the member entry) and does not reset the event. The member-count guard (16) and the
// packet reads are not reproduced.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushJoinLobbyUnlaunched(DataPacket* /*packet*/) {
    return 0;
}

// 0x823808 _ZN28CMatchingGLLiveLobbyObserver25OnMPProcessPushRejoinGameEP10DataPacket
// Partial. Not reproduced: the lookup of the rejoining member by name, the packet reads of the
// id, the name, the flags and the state, the update of an existing entry or the push of a new one,
// and the member-state byte at +0x50 of the entry.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushRejoinGame(DataPacket* /*packet*/) {
    return 0;
}

// 0x823ac4 _ZN28CMatchingGLLiveLobbyObserver25OnMPProcessPushLeaveLobbyEP10DataPacket
// Partial. The 9.4 KB body is not rebuilt in this batch. Not reproduced: the lower-case comparison
// of the lobby name and the sign-in name, the GetGLXPlayerMPLobby()->IsMaster check (vtable +0x3c),
// the event 13 on the GLLive queue, the member-entry copy and removal, and the CSignIn branch.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushLeaveLobby(DataPacket* /*packet*/) {
    return 0;
}

// 0x823fd8 _ZN28CMatchingGLLiveLobbyObserver23OnMPProcessPushMessagesEP10DataPacketi
// Dispatch on (type - 17). Cases 0..63 are routed as in the original. The PrintDatapacket trace
// is not called, and the result of that call (the value returned for an unmatched type) is not
// available, so unmatched types return 0.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushMessages(DataPacket* packet, std::int32_t type) {
    switch (type - 17) {
        case 0:
            return OnMPProcessPushJoinLobbyUnlaunched(packet);
        case 1:
            return OnMPProcessPushJoinLobbyLaunched(packet);
        case 4:
            return OnMPProcessPushLeaveLobby(packet);
        case 15:
            return OnMPProcessPushKickOutPlayer(packet);
        case 18:
            return OnMPProcessPushSetGameParameter(packet);
        case 21:
            return OnMPProcessPushSetUserParameter(packet);
        case 24:
            return OnMPProcessPushSetUserStatus(packet);
        case 27:
            return OnMPProcessPushStartGame(packet);
        case 30:
            return OnMPProcessPushRejoinGame(packet);
        case 33:
            return OnMPProcessPushGameserverAddr(packet);
        case 34:
            return OnMPProcessPushGameserverParams(packet);
        case 59:
            return OnMPProcessPushAutomatchUser(packet);
        case 63:
            return OnMPProcessPushSameAccountLogin(packet);
        default:
            return 0;
    }
}

// 0x821bac _ZN28CMatchingGLLiveLobbyObserver17GetMemberIdVectorEv
// Partial: the original copies each member id (+0x00 of the entry) into the result vector. The
// member list is not modelled, so the result is always empty.
std::vector<std::int32_t> CMatchingGLLiveLobbyObserver::GetMemberIdVector() const {
    return {};
}

// 0x822324 _ZN28CMatchingGLLiveLobbyObserver19GetMemberByMemberIdEi
// Partial: the original returns the entry whose id equals memberId, or a static invalid-member
// sentinel (id -1) when none matches. The member list is not modelled, so this returns nullptr.
tMemberInfo* CMatchingGLLiveLobbyObserver::GetMemberByMemberId(std::int32_t /*memberId*/) {
    return nullptr;
}

// 0x822638 _ZN28CMatchingGLLiveLobbyObserver14SetPlayerParamESs
// Partial: the original assigns the value to the string at +0x0c of the member entry chosen by
// the index field (+0x0c of the observer, not identified). The member list is not modelled.
void CMatchingGLLiveLobbyObserver::SetPlayerParam(std::string /*value*/) {}

// 0x822950 _ZN28CMatchingGLLiveLobbyObserver18CollapseMemberListEv
// Partial: the original keeps only the entries whose flag (+0x50) is set, and replaces the list.
// The member list is not modelled, so this is a no-op.
void CMatchingGLLiveLobbyObserver::CollapseMemberList() {}
