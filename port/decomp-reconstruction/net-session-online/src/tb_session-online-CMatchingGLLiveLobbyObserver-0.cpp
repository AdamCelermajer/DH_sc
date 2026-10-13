// Lobby observer bodies (CMatchingGLLiveLobbyObserver). The room list (+0x18, tGLRoomInfo records)
// and the member list (+0x24, tMemberInfo entries) are modelled in the owner header and in
// tb/t_gl_room_info.hpp and t_member_info.hpp. The packet reads are not: DataPacket's vptr slots
// +0x0c, +0x14 and +0x40 are not declared in net-transport, so each body that reads a packet keeps
// only the state and list effects it can make, and is partial. Unmodelled parts are listed per
// function. Original field offsets are kept as comments; see c_matching_gl_live_observers.hpp.

#include <cstring>
#include <ctime>
#include <utility>
#include <vector>

#include "net-session-online/c_matching_gl_live.hpp"
#include "net-session-online/c_matching_gl_live_observers.hpp"
#include "net-session-online/tb/t_gl_room_info.hpp"
#include "net-transport/data_packet.hpp"
#include "net-transport/deps/byte_array.hpp"  // CustomFree (inline, the C allocator here)

// 0x8213ec _ZN28CMatchingGLLiveLobbyObserver13ClearRoomListEv
// Reconstructed: the original pops each room record from the back of the list, runs its destructor
// (tGLRoomInfo::~tGLRoomInfo, 0x82138c, reconstructed in the pack4 batch) and frees the record with
// CustomFree. A null entry is only popped. The list is empty afterwards, as in the original.
void CMatchingGLLiveLobbyObserver::ClearRoomList() {
    while (!roomList_.empty()) {
        tGLRoomInfo* room = roomList_.back();
        roomList_.pop_back();
        if (room != nullptr) {
            room->~tGLRoomInfo();
            CustomFree(room);
        }
    }
}

// 0x821474 _ZN28CMatchingGLLiveLobbyObserver18GetRoomIndexByNameEPKc
// Reconstructed: each room's name (+0x04 of tGLRoomInfo) is compared with the argument by strcmp
// (LC_API_STRCMP_0 is read as strcmp elsewhere in the port); the first match's index is returned,
// or -1. A null record or name is skipped here, where the original would fault.
std::int32_t CMatchingGLLiveLobbyObserver::GetRoomIndexByName(const char* name) const {
    for (std::size_t i = 0; i < roomList_.size(); ++i) {
        const tGLRoomInfo* room = roomList_[i];
        const char* roomName = room ? room->name() : nullptr;
        if (roomName && name && std::strcmp(roomName, name) == 0) {
            return static_cast<std::int32_t>(i);
        }
    }
    return -1;
}

// 0x8214cc _ZN28CMatchingGLLiveLobbyObserver20GetMemberIndexByNameEPKc
// Reconstructed: the name at +0x28 of each member entry is compared with the argument by strcmp; the
// first match's index is returned, or -1. A null argument matches nothing (the original would fault).
std::int32_t CMatchingGLLiveLobbyObserver::GetMemberIndexByName(const char* name) const {
    if (!name) {
        return -1;
    }
    for (std::size_t i = 0; i < memberList_.size(); ++i) {
        if (std::strcmp(memberList_[i].Name(), name) == 0) {
            return static_cast<std::int32_t>(i);
        }
    }
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
// Partial. Reproduced on every path: the flag byte (+0x07) = true and state (+0x14) = 7 at entry.
// The exit stores, lastResult (+0x48) = 0 and updateTime (+0x10) = time(nullptr), belong to the
// count > 0 branch only. For a count of 0 or less the original calls ClearRoomList and branches to the
// epilogue (assembly labels loc_821AA0 and loc_821A70), which skips both stores (loc_821A58). This
// body runs both stores on every path anyway: the session count (DataPacket vptr +0x14, not declared)
// is not read, so the branch cannot be chosen, and the count <= 0 path is not reproduced.
// Not reproduced: the PrintDatapacket trace; the count read that picks the branch; the ClearRoomList
// call for a count of 0 or less; for a positive count, the room record reads, the CRoomAttributes
// unserialize from the blob (the char* overload is not declared), the CRoomSearchFilter test against
// CMatching+26424, the record allocation and insert or replace by name; and the JoinRoom and
// SetGCState(5) branch for a logged-in host.
void CMatchingGLLiveLobbyObserver::OnMPListSessionSuccess(DataPacket* /*packet*/) {
    flag07_ = true;
    state_ = 7;
    lastResult_ = 0;
    updateTime_ = static_cast<std::int32_t>(std::time(nullptr));
}

// 0x822b78 _ZN28CMatchingGLLiveLobbyObserver23OnMPLeaveSessionSuccessEv
// Partial: the side effects are reproduced (gameStarted (+0x06) = false, field0c and field08 = -1,
// the member list erased, lastResult (+0x48) = 0). The original returns the erase result: the
// position of the erased range, which is an address and has no portable value. This returns 0.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPLeaveSessionSuccess() {
    gameStarted_ = false;
    field0c_ = -1;
    field08_ = -1;
    memberList_.clear();
    lastResult_ = 0;
    return 0;
}

// 0x822c58 _ZN28CMatchingGLLiveLobbyObserver28OnMPProcessPushKickOutPlayerEP10DataPacket
// Partial. Not reproduced: the packet string read of the kicked player's name (DataPacket vptr +0x40,
// not declared); the comparison with the local login nickname (GLXPlayerLogin +0x74, nickName()); on a
// mismatch, the member erase at the index from GetMemberIndexByName (now available) and the CMatching
// event 4194318; on a match, the GLXPlayerMPLobby::mpSendLeaveLobby branch, the erase and the event
// 4194314. SyncMemberId is not called. Returns 0.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushKickOutPlayer(DataPacket* /*packet*/) {
    return 0;
}

// 0x822db4 _ZN28CMatchingGLLiveLobbyObserver32OnMPProcessPushJoinLobbyLaunchedEP10DataPacket
// Partial. The guard is reproduced: -1 when the member list already holds 16 or more entries, with no
// side effects. Not reproduced: the packet reads that fill the new entry, in this order: the name
// (DataPacket vptr +0x40, not declared; copied to +0x28, at most 31 characters); a second string
// (vptr +0x40) into +0x08; a byte (vptr +0x0c) into +0x4c; and the value string (vptr +0x40) into the
// value at +0x0c, with its length at +0x24. The id (+0x00) is never read: the entry keeps -1, which
// the original sets before the reads. The launched flag (+0x50) is then set to 1, the entry is pushed
// into the member list and the CMatching event 4194316 is queued; none of that is reproduced. Returns
// 0 after the guard, so no entry is added.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushJoinLobbyLaunched(DataPacket* /*packet*/) {
    if (memberList_.size() > 15) {
        return -1;
    }
    return 0;
}

// 0x822fa4 _ZN28CMatchingGLLiveLobbyObserver24OnMPCreateSessionSuccessEP10DataPacket
// Partial. Reproduced: state (+0x14) = 4 and gameStarted (+0x06) = true at entry, the member list
// erased (when not empty), lastResult (+0x48) = 0 at exit. Not reproduced: the packet read of the host
// id into +0x08 (DataPacket vptr +0x14, not declared) and the room-name read; the single host member
// push, whose name is the login nickname or the anonymous name, and whose value comes from a CMatching
// string (CMatching+26600..+26628, not recovered); SetGCState(6); and the event 4 queued on
// CMatchingGLLive::s_eventQueueGLLiveInternal.
void CMatchingGLLiveLobbyObserver::OnMPCreateSessionSuccess(DataPacket* /*packet*/) {
    state_ = 4;
    gameStarted_ = true;
    memberList_.clear();
    lastResult_ = 0;
}

// 0x8231e0 _ZN28CMatchingGLLiveLobbyObserver22OnMPJoinSessionSuccessEP10DataPacket
// Partial. Reproduced: gameStarted (+0x06) = true at entry, the member list erased (when not empty),
// state (+0x14) = 5 and lastResult (+0x48) = 0 at exit. Not reproduced: the member-count read and the
// per-member packet reads (id, name, value, flags; DataPacket vptr +0x0c, +0x14 and +0x40 not declared),
// the local-member index store at +0x0c (when the name matches the login nickname or the anonymous
// name), the SyncMemberId call and the final SetGCState(8).
void CMatchingGLLiveLobbyObserver::OnMPJoinSessionSuccess(DataPacket* /*packet*/) {
    gameStarted_ = true;
    memberList_.clear();
    state_ = 5;
    lastResult_ = 0;
}

// 0x823510 _ZN28CMatchingGLLiveLobbyObserver31OnMPProcessPushSetUserParameterEP10DataPacket
// Partial. Not reproduced: the two string reads through DataPacket vptr +0x40 (not declared on the
// base). The first is the member name; the second is freed at once and discarded. With the name, the
// original scans the members in order for the first entry whose name (+0x28) is equal and, if one is
// found, reads the blob C-string through getBlob (vptr +0x50, declared in net-transport, not yet wired
// here) and assigns it to that member's value string (+0x0c), with strlen as the length. It does not
// go through SetPlayerParam, which assigns to the member at the observer's local index (+0x0c), and
// GetMemberIndexByName is not called here either, because the name is not read. Returns 0.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushSetUserParameter(DataPacket* /*packet*/) {
    return 0;
}

// 0x8235f8 _ZN28CMatchingGLLiveLobbyObserver34OnMPProcessPushJoinLobbyUnlaunchedEP10DataPacket
// Partial. The member-count guard is reproduced: -1 when the member list already holds 16 or more
// entries, before any packet read. Not reproduced: the four packet reads that fill the new entry, in
// this order: the name (copied to +0x28, at most 31 characters), a second string (+0x08), a flag byte
// (+0x4c) and the value (+0x0c, its length at +0x24). They go through DataPacket vptr +0x40
// (getShortLenString) and +0x0c (getByte), which the DataPacket base does not declare. Also not
// reproduced: the push_back of the entry, the launched flag (+0x50 = 1; the original sets it in this
// body, which the note on the Launched sibling does not show), and the CMatching event 4194316 on the
// outgoing queue. Returns 0 after the guard, so no entry is added.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushJoinLobbyUnlaunched(DataPacket* /*packet*/) {
    if (memberList_.size() > 15) {
        return -1;
    }
    return 0;
}

// 0x823808 _ZN28CMatchingGLLiveLobbyObserver25OnMPProcessPushRejoinGameEP10DataPacket
// Partial. The lookup by name is reproduced in GetMemberIndexByName, but the packet is not read, so the
// body returns 0 without touching the list. Not reproduced: the first string read (the lookup name,
// through DataPacket vptr +0x40, not declared on the base). When the name is found, the original reads a
// second string into +0x08, a flag byte into +0x4c (vptr +0x0c) and a third string into the value at
// +0x0c with its length at +0x24, then sets the launched flag (+0x50) to 1. When it is not found, the
// original pushes a new entry built the same way and calls CMatchingGLLive::SyncMemberId.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushRejoinGame(DataPacket* /*packet*/) {
    return 0;
}

// 0x823ac4 _ZN28CMatchingGLLiveLobbyObserver25OnMPProcessPushLeaveLobbyEP10DataPacket
// Partial. The 9.4 KB body is not rebuilt. Not reproduced: the two packet string reads (the lobby name,
// then the member name; DataPacket vptr +0x40, not declared on the base), so every branch is unreached.
// The original branches on the Game Center flag (CMatchingGLLive +0x67e8, private, no accessor). With
// the flag set, the lower-cased lobby name is compared with dword_A3387C (not identified); otherwise
// with the sign-in name (CSignIn credentials, protected, no accessor). On a match, when the GLX lobby
// is not master (mpIsMaster), the original sets the lobby's master byte (+0x81, no setter) and queues
// event 13 on CMatchingGLLive::s_eventQueueGLLiveInternal. The member lookup then erases the entry and
// renumbers the list (the copies it makes are not observable). Returns 0.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushLeaveLobby(DataPacket* /*packet*/) {
    return 0;
}

// 0x8212d0 _ZL15PrintDatapacketP10DataPacket. File-local in the original, so each user has its own copy.
// It calls the packet's getMessageBody (vptr +0x64) and returns getMessageLen (vptr +0x60). Both are
// declared on the DataPacket base. core-platform ledgers the original as skipped; this is the local copy
// the dispatcher needs.
static std::int32_t PrintDatapacket(DataPacket* packet) {
    (void)packet->getMessageBody();
    return static_cast<std::int32_t>(packet->getMessageLen());
}

// 0x823fd8 _ZN28CMatchingGLLiveLobbyObserver23OnMPProcessPushMessagesEP10DataPacketi
// Reconstructed. The packet is dumped first (PrintDatapacket), and that value is the result for a type
// with no case. The switch is on (type - 17) in unsigned arithmetic, as the original compares. The
// handlers are called qualified, as in the original, so they are not dispatched through the vtable.
// Cases 18, 24, 34 and 59 are passed the packet; the original calls them without it, and those bodies
// do not read it.
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushMessages(DataPacket* packet, std::int32_t type) {
    const std::int32_t printed = PrintDatapacket(packet);
    switch (static_cast<std::uint32_t>(type) - 17u) {
        case 0u:
            return CMatchingGLLiveLobbyObserver::OnMPProcessPushJoinLobbyUnlaunched(packet);
        case 1u:
            return CMatchingGLLiveLobbyObserver::OnMPProcessPushJoinLobbyLaunched(packet);
        case 4u:
            return CMatchingGLLiveLobbyObserver::OnMPProcessPushLeaveLobby(packet);
        case 15u:
            return CMatchingGLLiveLobbyObserver::OnMPProcessPushKickOutPlayer(packet);
        case 18u:
            return CMatchingGLLiveLobbyObserver::OnMPProcessPushSetGameParameter(packet);
        case 21u:
            return CMatchingGLLiveLobbyObserver::OnMPProcessPushSetUserParameter(packet);
        case 24u:
            return CMatchingGLLiveLobbyObserver::OnMPProcessPushSetUserStatus(packet);
        case 27u:
            return CMatchingGLLiveLobbyObserver::OnMPProcessPushStartGame(packet);
        case 30u:
            return CMatchingGLLiveLobbyObserver::OnMPProcessPushRejoinGame(packet);
        case 33u:
            return CMatchingGLLiveLobbyObserver::OnMPProcessPushGameserverAddr(packet);
        case 34u:
            return CMatchingGLLiveLobbyObserver::OnMPProcessPushGameserverParams(packet);
        case 59u:
            return CMatchingGLLiveLobbyObserver::OnMPProcessPushAutomatchUser(packet);
        case 63u:
            return CMatchingGLLiveLobbyObserver::OnMPProcessPushSameAccountLogin(packet);
        default:
            return printed;
    }
}

// 0x821bac _ZN28CMatchingGLLiveLobbyObserver17GetMemberIdVectorEv
// Reconstructed: each member's id (+0x00 of the entry) is appended in list order.
std::vector<std::int32_t> CMatchingGLLiveLobbyObserver::GetMemberIdVector() const {
    std::vector<std::int32_t> ids;
    ids.reserve(memberList_.size());
    for (const tMemberInfo& member : memberList_) {
        ids.push_back(member.MemberId());
    }
    return ids;
}

// 0x822324 _ZN28CMatchingGLLiveLobbyObserver19GetMemberByMemberIdEi
// Reconstructed: the first entry whose id equals memberId. When none matches, the original returns
// its static invalid-member sentinel (id -1, initialised on first use), which is a function-local
// static here.
tMemberInfo* CMatchingGLLiveLobbyObserver::GetMemberByMemberId(std::int32_t memberId) {
    for (tMemberInfo& member : memberList_) {
        if (member.MemberId() == memberId) {
            return &member;
        }
    }
    static tMemberInfo invalidMember(-1);
    return &invalidMember;
}

// 0x822638 _ZN28CMatchingGLLiveLobbyObserver14SetPlayerParamESs
// Reconstructed: the value is assigned to the string (+0x0c) of the member at the local member index
// (+0x0c of the observer). Nothing is done when the index is negative or past the end of the list.
// The mangled name has no return type, so the function is void.
void CMatchingGLLiveLobbyObserver::SetPlayerParam(std::string value) {
    if (field0c_ < 0 || static_cast<std::size_t>(field0c_) >= memberList_.size()) {
        return;
    }
    memberList_[static_cast<std::size_t>(field0c_)].Value() = std::move(value);
}

// 0x822950 _ZN28CMatchingGLLiveLobbyObserver18CollapseMemberListEv
// Reconstructed: keeps the entries whose launched flag (+0x50) is set, in their order, and replaces
// the list with them.
void CMatchingGLLiveLobbyObserver::CollapseMemberList() {
    std::vector<tMemberInfo> kept;
    for (const tMemberInfo& member : memberList_) {
        if (member.Launched()) {
            kept.push_back(member);
        }
    }
    memberList_ = std::move(kept);
}
