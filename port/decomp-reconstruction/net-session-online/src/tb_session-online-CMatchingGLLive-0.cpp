// Batch e1-net-session-online-CMatchingGLLive-0 (CMatchingGLLive, part 1 of 2).
//
// Most bodies in this batch read layouts that the lobby observer does not declare yet: its
// member vector (+0x24, +0x28) and the tFriendInfo accessors. Those bodies are partial: the part that
// the port can express is written, the rest is named in the note and returns the default value. The
// CMatching base, the member table at +0x3a00 and the friend list at +0x6bd8 are declared in
// c_matching_gl_live.hpp now.

#include "net-session-online/c_matching_gl_live.hpp"

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "net-session-online/c_matching_gl_live_observers.hpp"
#include "net-session-online/c_sign_in.hpp"
#include "net-session-online/deps/platform_stand_ins.hpp"
#include "net-session-online/deps/stream_bytes.hpp"
#include "net-transport/connection_manager.hpp"  // CConnectionManager (real)
#include "net-session-online/deps/matching_queue.hpp"
#include "net-transport/net_bit_stream.hpp"
#include "net-session-online/glx_player_login.hpp"
#include "net-session-online/glx_player_user_friend.hpp"
#include "net-session-online/t_friend_info.hpp"

// 0x81bfe4 _ZN15CMatchingGLLive17ProcessLostPacketEii
// Partial. Calls the base CMatching::ProcessLostPacket (partial in its own row), then NetStruct vtable slot +0x44
// on each of the 32 member entries at +0x3a00 (stride 0x168). The entry's slot is a stand-in that does nothing
// (the packet history belongs to net-transport's NetStruct). The original returns the result of the last slot
// call; the base declares this function void here, so that result is not produced.
void CMatchingGLLive::ProcessLostPacket(std::int32_t a2, std::int32_t a3) {
    CMatching::ProcessLostPacket(a2, a3);
    for (MemberInfoNetStruct& entry : memberInfo_) {
        entry.ProcessLostPacket(a2, a3);
    }
}

// 0x81c034 _ZN15CMatchingGLLive23ProcessAcknowledgedPacketEii
// Partial. Same shape as ProcessLostPacket: the original first calls the base CMatching::ProcessAcknowledgedPacket
// (0x7fed08, partial in its own row: the room attributes receive the call), then NetStruct slot +0x40 on each
// member entry. The entry's slot is a stand-in that does nothing. The original's return value (the last slot
// result) is not produced, as for ProcessLostPacket.
void CMatchingGLLive::ProcessAcknowledgedPacket(std::int32_t a2, std::int32_t a3) {
    CMatching::ProcessAcknowledgedPacket(a2, a3);
    for (MemberInfoNetStruct& entry : memberInfo_) {
        entry.ProcessAcknowledgedPacket(a2, a3);
    }
}

// 0x81c200 _ZN15CMatchingGLLive24ProcessMemberJoinMessageEiR12NetBitStream
// Reads a signed count byte, then for each entry 32 bytes: a 4-byte member id followed by a 28-byte CNetworkId (the
// original reads all 32 bytes into a stack object that holds the id and the address). An entry whose id is not
// the local member id (LocalMemberId(), vtable +0x6c) and is not connected posts event 8388614 on the matching
// queue with the id. The connection check uses the connection manager stand-in (deps/net-transport/connection_manager.hpp).
void CMatchingGLLive::ProcessMemberJoinMessage(std::int32_t /*a2*/, NetBitStream& stream) {
    const std::int8_t count = static_cast<std::int8_t>(stream.ReadByte(8));
    for (int i = 0; i < count; ++i) {
        std::uint8_t entry[32] = {};
        stream.ReadByteArray(entry, sizeof entry);
        std::int32_t memberId = 0;
        std::memcpy(&memberId, entry, sizeof memberId);
        if (memberId != LocalMemberId() && !CConnectionManager::GetInstance()->IsConnected(memberId)) {
            MatchingInternalEventQueue().AddEvent(8388614, &memberId, sizeof memberId);
        }
    }
}

// 0x81c2c4 _ZN15CMatchingGLLive17SendClientRequestEv
// Writes one byte (2) to a 0x800-byte stream and broadcasts it as packet type 2 through the connection manager
// (CConnectionManager::SendBroadcast; the original passes the buffer and its length rounded up to whole bytes).
// Returns the send result. Partial only through the stand-in: the connection manager is the recording stand-in of
// deps/net-transport/connection_manager.hpp, whose send reports 0.
std::int32_t CMatchingGLLive::SendClientRequest() {
    NetBitStream stream(0x800);
    const std::uint8_t kind = 2;
    stream.WriteByteArray(&kind, 1);
    auto bytes = StreamBytes(stream);
    return CConnectionManager::GetInstance()->SendBroadcast(2, bytes.data(), static_cast<int>(bytes.size()));
}

// 0x81c460 _ZN15CMatchingGLLive9GetRoomIdEv
// Partial. Returns -1 when the sign-in logged-in flag (CSignIn +0x11) is set; the flag is read through
// CSignIn::LoggedInForTest(), the only accessor. Otherwise the original returns the room id at +0x08 of the
// lobby observer. That field is not identified (the observer's field08_ is not modelled), so 0 is returned.
// The lobby observer is created on first use, as in the original.
std::int64_t CMatchingGLLive::GetRoomId() {
    CSignIn* signIn = CSignIn::Get();
    if (signIn != nullptr && signIn->LoggedIn()) {
        return -1;
    }
    GetGLXPlayerMPLobbyObserver();
    return 0;
}

// 0x81c494 _ZN15CMatchingGLLive20GetMemberByMemberIdEi
// Partial. The original forwards to CMatchingGLLiveLobbyObserver::GetMemberByMemberId on the lobby observer
// (created on first use). That lookup returns a tMemberInfo* and is partial in its own row (always null here).
// The original returns the pointer as its int result; a host pointer cannot be returned as int32 here, so the
// null result 0 is returned. A non-null member would be lost by this port (none can be produced yet).
std::int32_t CMatchingGLLive::GetMemberByMemberId(std::int32_t memberId) {
    (void)GetGLXPlayerMPLobbyObserver()->GetMemberByMemberId(memberId);
    return 0;
}

// 0x81c4ac _ZN15CMatchingGLLive6IsInRoomEv
// While the game is starting: true for the server (CMatching::IsServer), otherwise true when the connection
// manager has at least one connection (GetConnectionCount(false) > 0). Otherwise: false while signed out, and
// when signed in the lobby observer's byte at +0x06. The connection count comes from the connection manager
// stand-in; the byte is the observer's game-started flag (GameStartedForTest).
bool CMatchingGLLive::IsInRoom() const {
    if (s_IsGameStarting) {
        if (IsServer()) return true;
        // Deviation: the original always has the connection manager; a missing instance (before
        // CConnectionManager::Initialize, as in the unit tests) counts as no connections.
        CConnectionManager* manager = CConnectionManager::GetInstance();
        return manager != nullptr && manager->GetConnectionCount(false) > 0;
    }
    CSignIn* signIn = CSignIn::Get();
    if (signIn != nullptr && signIn->LoggedIn()) {
        return GetGLXPlayerMPLobbyObserver()->GameStarted();
    }
    return false;
}

// 0x81c524 _ZNK15CMatchingGLLive14GetMemberCountEv
// Partial. While the game is starting the count is the connection count plus one. Otherwise the original reads the
// lobby observer's game-started byte (+0x06, GameStartedForTest) and returns 0 when it is clear, and the size of the
// observer's member vector (84-byte tMemberInfo entries at +0x24..+0x28) when it is set. That vector is not modelled
// on the observer, so the second case returns 0 as well.
std::int32_t CMatchingGLLive::GetMemberCount() const {
    if (s_IsGameStarting) {
        return CConnectionManager::GetInstance()->GetConnectionCount(false) + 1;
    }
    const bool gameStarted = GetGLXPlayerMPLobbyObserver()->GameStarted();
    // Partial: when gameStarted is set, the original returns the observer's member vector size (not modelled).
    (void)gameStarted;
    return 0;
}

// 0x81c6cc _ZN15CMatchingGLLive10IsMyFriendEPc
// A null name returns -1. A name equal to the logged-in nickname (GLXPlayerLogin +0x74) returns 0. Otherwise
// the game friend list (+0x6bd8) is searched by name: 1 when an entry matches, 2 when none does.
std::int32_t CMatchingGLLive::IsMyFriend(char* name) {
    if (name == nullptr) return -1;
    const char* nick = GetGLXPlayerLogin()->nickName();
    if (nick != nullptr && std::strcmp(nick, name) == 0) {  // LC_API_STRCMP_0
        return 0;
    }
    for (const GLFriendInfo& entry : glFriends_) {
        if (std::strcmp(entry.name.c_str(), name) == 0) {
            return 1;
        }
    }
    return 2;
}

// 0x81c9e8 _ZN15CMatchingGLLive23sPacketReceiverCallbackEiPci
// Reconstructed: the original forward is a plain call to CMatchingGLLive::PacketReceiverCallback on the
// CMatching singleton (CMatching::s_Matching), with the first argument as the sender, and the port's call
// is that call. Deviation (host-necessitated): the binary does not check s_Matching for null; this port
// returns when it is null, as sBroadcastPacketReceiverCallback does (e2 batch, same callee class).
void CMatchingGLLive::sPacketReceiverCallback(std::int32_t from, char* data, int size) {
    auto* matching = static_cast<CMatchingGLLive*>(CMatching::s_Matching);
    if (matching == nullptr) {
        return;
    }
    matching->PacketReceiverCallback(from, data, size);
}

// 0x81cec0 _ZN15CMatchingGLLive12IsFriendWithEPKcb
// Partial. The original copies the friend list (GetFriendsList with the flag, virtual slot +0x8c) and
// returns true when any entry's name string (tFriendInfo offset +0x18 in the original) equals the argument.
// Two gaps: t_friend_info.hpp has no member at +0x18 (its placeholder fields are at +0x00, +0x04, +0x1c, +0x20,
// +0x38 and +0x50), so the name cannot be read (layout request); and GetFriendsList is partial and returns an
// empty list. Returns false for now.
bool CMatchingGLLive::IsFriendWith(const char* /*name*/, bool /*flag*/) { return false; }

// 0x81cf30 _ZN15CMatchingGLLive16SendInvitationEPKcS1_b
// Partial. The original returns 1 when the friend observer's flag (+0x04 of the object at +0x6704) is clear.
// Otherwise it sends a message to each friend in the list (GLXPlayerMessage::SendOnlineMessage at +0x6708, kind 8)
// and posts event 8388628. Not reproduced: the friend observer is null in this port (the original would fault
// reading its flag, so the flag check cannot be made here); the friend names are needed for the send and
// tFriendInfo has no name at +0x18 (see IsFriendWith); and the friend list is partial. Returns 0 without sending.
std::int32_t CMatchingGLLive::SendInvitation(const char* /*from*/, const char* /*to*/, bool /*flag*/) {
    return 0;
}

// 0x81dba0 _ZN15CMatchingGLLive19GetGLLiveMemberIdListEv
// Partial. The original first copies the lobby observer's member vector (84-byte entries, id at +0x00) and then
// builds the std::vector<int> of the ids. The member vector is not declared on the observer, so the id list is
// empty. The lobby observer is created on first use, as in the original.
std::vector<std::int32_t> CMatchingGLLive::GetGLLiveMemberIdList() {
    (void)GetGLXPlayerMPLobbyObserver();
    return {};
}

// 0x81ded4 _ZN15CMatchingGLLive6IsInRoomEi
// While the game is starting the local member id (+0x3640) is answered by IsInRoom() (vtable +0x64) and any other
// id by CConnectionManager::IsConnected. Otherwise the id must be in 0 .. GetMemberCount() - 1; the original
// copies the lobby observer's tMemberInfo vector first, which has no other effect. It is partial through
// GetMemberCount, whose lobby branch is partial.
bool CMatchingGLLive::IsInRoom(std::int32_t memberId) {
    if (s_IsGameStarting) {
        if (memberId_ == memberId) return IsInRoom();
        return CConnectionManager::GetInstance()->IsConnected(memberId);
    }
    return memberId >= 0 && memberId < GetMemberCount();
}

// 0x81df90 _ZN15CMatchingGLLive13GetMemberNameEi
// Partial. The original finds the member whose id matches in the lobby observer's member vector
// (84-byte entries, name string at +0x28) and returns a copy of its name, or an empty string. The
// member vector is not declared, so the result is always the empty string.
std::string CMatchingGLLive::GetMemberName(std::int32_t /*memberId*/) { return std::string(); }

// 0x81e25c _ZN15CMatchingGLLive12SyncMemberIdEv
// Partial. The original, while the logged-in user is in the starting game, walks the member id list
// (virtual slot +0x88), and for each id copies the member name (lower-cased) and matches it against
// the lobby member names, writing the id into the member vector. The member vector and the name
// accessors are not declared, so this does nothing.
void CMatchingGLLive::SyncMemberId() {}

// 0x81e4d0 _ZN15CMatchingGLLive14ReadPacketDataEiiR12NetBitStream
// Partial. The original unserializes the room attributes (CRoomAttributes::Unserialize through
// virtual slot +0xa4), loads the 32-entry member info array
// (NetStructArray<CMatching::MemberInfoNetStruct,32>::Load), then calls each member's virtual
// slot +0x18 and syncs member ids when one returns non-zero.
// CORRECTION: CRoomAttributes::Unserialize(int, NetBitStream&) IS declared (dh2/CRoomAttributes.hpp:87,
// 0x817fec) and has a body, so the room half can be written. What is missing is the element half: the port
// inlines NetStructArray<...>::Load in its callers instead of using the template, and the per-element
// slot +0x18 (ConditionalLoad) needs the real CMatchingGLLive::MemberInfoNetStruct (the 360-byte record at
// +0x3a00), which is still a stand-in. So the stream is not read.
void CMatchingGLLive::ReadPacketData(std::int32_t /*a2*/, std::int32_t /*a3*/, NetBitStream& /*stream*/) {}

// 0x81ec5c _ZN15CMatchingGLLive14GetFriendsListEb
// Partial. The original returns an empty list unless the friend observer's flag (byte +0x04 of the object at
// +0x6704) is set. Then, for each friend of the GLX user-friend object (+0x6700, count from GetFriendsCount), it
// builds a tFriendInfo with the user number (+0x00), the name (+0x04) and the state (+0x1c), and keeps the entry
// unless the argument is set and the state is 3. Deviation: the binary's GetUserNumber returns a string pointer
// that the original passes to atoi; the port's GetUserNumber already returns the number, so it is used as it is.
// A null friend observer or friend list gives an empty list here (the original reads through them).
std::vector<tFriendInfo> CMatchingGLLive::GetFriendsList(bool notInState3) {
    std::vector<tFriendInfo> list;
    if (friendObserver_ == nullptr || !friendObserver_->Flag() || glxUserFriend_ == nullptr) {
        return list;
    }
    for (std::int32_t index = 0; index < glxUserFriend_->GetFriendsCount(); ++index) {
        tFriendInfo info;
        info.SetValue00(static_cast<std::int32_t>(glxUserFriend_->GetUserNumber(index)));
        const char* name = glxUserFriend_->GetName(index);
        info.SetText04(name != nullptr ? std::string(name) : std::string());
        const std::int32_t state = glxUserFriend_->GetState(index);
        info.SetValue1c(state);
        if (!notInState3 || state != 3) {
            list.push_back(info);
        }
    }
    return list;
}

// 0x81ee10 AddFriendInGameList and 0x81ee6c GetGLFriendsList are in src/c_matching_gl_live_member_calls.cpp.
