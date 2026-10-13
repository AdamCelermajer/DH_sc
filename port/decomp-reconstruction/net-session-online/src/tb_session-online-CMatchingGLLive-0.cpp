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
#include "net-session-online/deps/connection_manager_stand_in.hpp"
#include "net-session-online/deps/matching_queue.hpp"
#include "net-session-online/deps/net_bit_stream.hpp"
#include "net-session-online/glx_player_login.hpp"
#include "net-session-online/t_friend_info.hpp"

// 0x81bfe4 _ZN15CMatchingGLLive17ProcessLostPacketEii
// Partial. Calls CMatching::ProcessLostPacket (CMatchingBase::ProcessLostPacket, itself partial), then NetStruct
// vtable slot +0x44 on each of the 32 member entries at +0x3a00 (stride 0x168). The entry's slot is a stand-in
// that does nothing (the packet history belongs to net-transport's NetStruct).
void CMatchingGLLive::ProcessLostPacket(std::int32_t a2, std::int32_t a3) {
    CMatchingBase::ProcessLostPacket(a2, a3);
    for (MemberInfoNetStruct& entry : memberInfo_) {
        entry.ProcessLostPacket(a2, a3);
    }
}

// 0x81c034 _ZN15CMatchingGLLive23ProcessAcknowledgedPacketEii
// Partial. Same shape as ProcessLostPacket: the original calls CMatching::ProcessAcknowledgedPacket (0x7fed08,
// not ported in CMatchingBase) and then NetStruct slot +0x40 on each member entry. Only the loop over the entries
// runs here, and the entry's slot is a stand-in that does nothing.
void CMatchingGLLive::ProcessAcknowledgedPacket(std::int32_t a2, std::int32_t a3) {
    for (MemberInfoNetStruct& entry : memberInfo_) {
        entry.ProcessAcknowledgedPacket(a2, a3);
    }
}

// 0x81c200 _ZN15CMatchingGLLive24ProcessMemberJoinMessageEiR12NetBitStream
// Reads a signed count byte, then for each entry 32 bytes: a 4-byte member id followed by a 28-byte CNetworkId (the
// original reads all 32 bytes into a stack object that holds the id and the address). An entry whose id is not
// the local member id (LocalMemberId(), vtable +0x6c) and is not connected posts event 8388614 on the matching
// queue with the id. The connection check uses the connection manager stand-in (deps/connection_manager_stand_in.hpp).
void CMatchingGLLive::ProcessMemberJoinMessage(std::int32_t /*a2*/, NetBitStream& stream) {
    const std::int8_t count = static_cast<std::int8_t>(stream.ReadByte(8));
    for (int i = 0; i < count; ++i) {
        std::uint8_t entry[32] = {};
        stream.ReadByteArray(entry, sizeof entry);
        std::int32_t memberId = 0;
        std::memcpy(&memberId, entry, sizeof memberId);
        if (memberId != LocalMemberId() && !GetConnectionMgr().IsConnected(memberId)) {
            MatchingInternalEventQueue().AddEvent(8388614, &memberId, sizeof memberId);
        }
    }
}

// 0x81c2c4 _ZN15CMatchingGLLive17SendClientRequestEv
// Writes one byte (2) to a 0x800-byte stream and broadcasts it as packet type 2 through the connection manager
// (CConnectionManager::SendBroadcast; the original passes the buffer and its length rounded up to whole bytes).
// Returns the send result. Partial only through the stand-in: the connection manager is the recording stand-in of
// deps/connection_manager_stand_in.hpp, whose send reports 0.
std::int32_t CMatchingGLLive::SendClientRequest() {
    NetBitStream stream(0x800);
    const std::uint8_t kind = 2;
    stream.WriteByteArray(&kind, 1);
    return GetConnectionMgr().SendBroadcast(2, stream);
}

// 0x81c460 _ZN15CMatchingGLLive9GetRoomIdEv
// Partial. The original returns -1 when the sign-in logged-in flag (CSignIn +0x11) is set, and
// otherwise the room id stored at +0x08 of the lobby observer. The flag has no accessor and the
// observer field at +0x08 is not identified, so this returns 0 for now.
std::int64_t CMatchingGLLive::GetRoomId() { return 0; }

// 0x81c494 _ZN15CMatchingGLLive20GetMemberByMemberIdEi
// Partial. The original forwards to CMatchingGLLiveLobbyObserver::GetMemberByMemberId on the lobby
// observer, which is not declared in its header. Returns 0 for now.
std::int32_t CMatchingGLLive::GetMemberByMemberId(std::int32_t /*memberId*/) { return 0; }

// 0x81c4ac _ZN15CMatchingGLLive6IsInRoomEv
// While the game is starting: true for the server (CMatching::IsServer), otherwise true when the connection
// manager has at least one connection (GetConnectionCount(false) > 0). Otherwise: false while signed out, and
// when signed in the lobby observer's byte at +0x06. The connection count comes from the connection manager
// stand-in; the byte is the observer's game-started flag (GameStartedForTest).
bool CMatchingGLLive::IsInRoom() const {
    if (s_IsGameStarting) {
        if (IsServer()) return true;
        return GetConnectionMgr().GetConnectionCount(false) > 0;
    }
    CSignIn* signIn = CSignIn::Get();
    if (signIn != nullptr && signIn->LoggedInForTest()) {
        return GetGLXPlayerMPLobbyObserver()->GameStartedForTest();
    }
    return false;
}

// 0x81c524 _ZNK15CMatchingGLLive14GetMemberCountEv
// Partial. While the game is starting the count is the connection count plus one. Otherwise the original returns
// 0 when the lobby observer's byte at +0x06 is clear, and the size of the observer's member vector (84-byte
// tMemberInfo entries at +0x24..+0x28) when it is set. That vector is not modelled on the observer, so the
// second case returns 0.
std::int32_t CMatchingGLLive::GetMemberCount() const {
    if (s_IsGameStarting) {
        return GetConnectionMgr().GetConnectionCount(false) + 1;
    }
    return 0;  // Partial: the observer's member vector size is not available.
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
// Partial. The original forwards the packet to CMatchingGLLive::PacketReceiverCallback on the
// CMatching singleton (CMatching::s_Matching), which is not declared in this port.
void CMatchingGLLive::sPacketReceiverCallback(std::int32_t /*from*/, char* /*data*/, int /*size*/) {}

// 0x81cec0 _ZN15CMatchingGLLive12IsFriendWithEPKcb
// Partial. The original copies the friend list (GetFriendsList with the flag, virtual slot +0x8c) and
// returns true when any entry's name string (tFriendInfo offset +0x18 in the original) equals the
// argument. tFriendInfo's name accessor and the offset mismatch with the placeholder layout in
// t_friend_info.hpp are open. Returns false for now.
bool CMatchingGLLive::IsFriendWith(const char* /*name*/, bool /*flag*/) { return false; }

// 0x81cf30 _ZN15CMatchingGLLive16SendInvitationEPKcS1_b
// Partial. The original returns 1 when the friend-observer flag at +0x04 of the object at this+0x66c4
// is clear. Otherwise it sends a message to each friend in the list (GLXPlayerMessage::SendOnlineMessage,
// kind 8) and posts event 8388628. The member object and the queue are not declared, so this returns 0
// without sending.
std::int32_t CMatchingGLLive::SendInvitation(const char* /*from*/, const char* /*to*/, bool /*flag*/) {
    return 0;
}

// 0x81dba0 _ZN15CMatchingGLLive19GetGLLiveMemberIdListEv
// Partial. The original copies the member id field of each entry of the lobby observer's member
// vector (84-byte entries, id at +0x00) into a new std::vector<int>. The member vector is not
// declared on the observer, so the result is empty.
std::vector<std::int32_t> CMatchingGLLive::GetGLLiveMemberIdList() { return {}; }

// 0x81ded4 _ZN15CMatchingGLLive6IsInRoomEi
// While the game is starting the local member id (+0x3640) is answered by IsInRoom() (vtable +0x64) and any other
// id by CConnectionManager::IsConnected. Otherwise the id must be in 0 .. GetMemberCount() - 1; the original
// copies the lobby observer's tMemberInfo vector first, which has no other effect. It is partial through
// GetMemberCount, whose lobby branch is partial.
bool CMatchingGLLive::IsInRoom(std::int32_t memberId) {
    if (s_IsGameStarting) {
        if (memberId_ == memberId) return IsInRoom();
        return GetConnectionMgr().IsConnected(memberId);
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
// virtual slot +0xa4, not reachable here), loads the 32-entry member info array
// (NetStructArray<CMatching::MemberInfoNetStruct,32>::Load), then calls each member's virtual
// slot +0x18 and syncs member ids when one returns non-zero. None of those layouts are declared,
// so the stream is not read.
void CMatchingGLLive::ReadPacketData(std::int32_t /*a2*/, std::int32_t /*a3*/, NetBitStream& /*stream*/) {}

// 0x81ec5c _ZN15CMatchingGLLive14GetFriendsListEb
// Partial. The original builds a list of tFriendInfo from the GLX user-friend object: for each
// friend it stores the user number (atoi), the name, and the state, and keeps the entry unless the
// flag is set and the state is 3. tFriendInfo has no field setters in t_friend_info.hpp, so the list
// cannot be filled here and is returned empty.
std::vector<tFriendInfo> CMatchingGLLive::GetFriendsList(bool /*notInState3*/) { return {}; }

// 0x81ee10 AddFriendInGameList and 0x81ee6c GetGLFriendsList are in src/c_matching_gl_live_member_calls.cpp.
