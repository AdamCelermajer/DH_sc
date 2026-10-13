// Batch e2-net-session-online-CMatchingGLLive-07f9f6-0 (CMatchingGLLive, 12 functions), revised for the
// CMatchingGLLive layout of c_matching_gl_live.hpp (CMatching base, members at their original
// offsets). Integrator decision 6 applied: JoinRoomInternal is reconstructed; Initialize,
// WritePacketData, AddServer, CreateJoinRoomInternal and the others keep a precise note of what is
// still missing.
//
// Not reconstructed: GetRoomList (0x82051c). It returns a std::vector<tRoomInfo>; the tRoomInfo
// layout is now in t_room_info.hpp, but the body also needs the lobby observer's tGLRoomInfo list
// (+0x18 of the observer), which the observer header does not declare.

#include "net-session-online/c_matching_gl_live.hpp"

#include <cstdint>
#include <string>

#include "net-session-online/c_lobby_parameter_and_query.hpp"
#include "net-session-online/c_room_attributes.hpp"
#include "net-session-online/c_sign_in_gl_live_observer.hpp"
#include "net-session-online/deps/platform_stand_ins.hpp"
#include "net-session-online/deps/stream_bytes.hpp"
#include "net-transport/connection_manager.hpp"  // CConnectionManager (real)
#include "net-session-online/deps/matching_queue.hpp"
#include "net-transport/net_bit_stream.hpp"
#include "net-transport/net_network_id.hpp"
#include "net-session-online/deps/xp_api.hpp"
#include "net-session-online/glx_player_mp_lobby.hpp"
#include "net-session-online/glx_player_serever_config.hpp"
#include "net-session-online/glx_player_user.hpp"

// dword_A33870: bss pointer, zero at load; the only recovered reference is Initialize's literal pool.
const char* CMatchingGLLive::s_serverConfigGameVersion = nullptr;

// 0x81f264 _ZN15CMatchingGLLive10InitializeEi
// Partial. Reproduced, in the original order: CMatching::Initialize (CMatching::Initialize, itself partial),
// the early return when the base flag +0x0c is already set, the GLXPlayerSereverConfig (0x44 bytes, stored at
// +0x6710) built from the game version pointer dword_A33870, the GLXPlayerUser (0x130 bytes, +0x6718), the
// CLobbyParameterAndQuery (0x40 bytes, +0x6808), GLXPlayerSereverConfig::SendGetServerConfig with its status
// (1 sets the login observer's configured flag and calls SetGCState(1); -1 prints "oconf.bar not found!"),
// the four packet registrations (types 2 and 3 and 9 to sBroadcastPacketReceiverCallback, type 4 to
// sPacketReceiverCallback; the second argument 2, 1, 0, 1 is the channel), the server member id +0x3644 = 1,
// and the return of the status.
// Not reproduced: GLXPlayerComponent::RegisterObserver(config, s_GLXPlayerLoginObserver). RegisterObserver
// takes a GLXPlayerObserver and CSignInGLLiveObserver is a standalone class in this port, so the config has
// no observer. The stack copy of the login observer that the original builds (fields +0x04, +0x05, 128 bytes
// from +0x06 and the word at +0x88) is never read and is destroyed at the end, so it is not built. The
// base CMatching::Initialize is partial (see its row), and dword_A33870 is a null pointer here because no
// writer is recovered (s_serverConfigGameVersion).
std::int32_t CMatchingGLLive::Initialize(std::int32_t memberCapacity) {
    CMatching::Initialize(memberCapacity);
    if (initialized_) {
        return 0;
    }
    std::int32_t status = initialized_ ? 1 : 0;  // v13 = the byte at +0x0c, which is clear here

    glxServerConfig_ = new GLXPlayerSereverConfig(s_serverConfigGameVersion);  // +0x6710
    GetGLXPlayerLoginObserver();  // creates the observer the original copies and registers
    glxUser_ = new GLXPlayerUser();                          // +0x6718
    lobbyQuery_ = new CLobbyParameterAndQuery();             // +0x6808

    glxServerConfig_->SendGetServerConfig(status);
    if (status == 1) {
        GetGLXPlayerLoginObserver()->SetConfigured(true);  // *(observer + 4) = 1
        SetGCState(1);
    } else if (status == -1) {
        XP_DEBUG_OUT("oconf.bar not found!\n");
    }

    CConnectionManager& manager = (*CConnectionManager::GetInstance());
    manager.RegisterPacketType(2, 2, &CMatchingGLLive::sBroadcastPacketReceiverCallback);
    manager.RegisterPacketType(3, 1, &CMatchingGLLive::sBroadcastPacketReceiverCallback);
    manager.RegisterPacketType(9, 0, &CMatchingGLLive::sBroadcastPacketReceiverCallback);
    manager.RegisterPacketType(4, 1, &CMatchingGLLive::sPacketReceiverCallback);

    serverMemberId_ = 1;  // +0x3644
    return status;
}

// 0x81f488 _ZN15CMatchingGLLive16JoinRoomInternalEy
// Returns -1 when the member count (virtual slot +0x60) is above the member capacity (base +0x08). Otherwise,
// under the mutex at +0x675c, it sends mpSendJoinLobby(room id, the byte at +0x0f, the player parameter
// string's data and length), sets the room operation +0x6834 to 9, stores the room id in the int at +0x680c,
// clears the pending flag at +0x3630, and returns 0. The room id argument is 64-bit; the lobby request and the
// stored key use its low 32 bits, as the original does. The player parameter string is the STLport string at
// +0x67f0: its start and finish pointers are the words the decompiled body reads at +0x6804 and +0x6800.
// Deviation: the lobby is the existing s_GLXPlayerMPLobby; the original creates it on first use.
std::int32_t CMatchingGLLive::JoinRoomInternal(std::uint64_t roomId) {
    if (GetMemberCount() > memberCapacity_) {
        return -1;
    }
    serversMutex_.Lock();
    const std::int32_t lobbyId = static_cast<std::int32_t>(roomId);
    GetGLXPlayerMPLobby()->mpSendJoinLobby(lobbyId, unk_0f, &playerParam_[0],
                                           static_cast<std::int32_t>(playerParam_.size()));
    roomOp_ = 9;                 // +0x6834
    roomKey_ = lobbyId;          // +0x680c
    roomOperationPending_ = false;  // +0x3630
    serversMutex_.Unlock();
    return 0;
}

// 0x81f53c _ZN15CMatchingGLLive15WritePacketDataEiiR12NetBitStream
// Partial. Reproduced: the room attributes are fetched through virtual slot +0xa4 and their NetStruct enabled
// flag is set to IsHost(); the 32 member info elements at +0x3a00 are each enabled when this object is the
// server or the element index equals the local member's index (CMatching::GetMemberIndex of +0x3640), then
// serialized, and any element that wrote makes the result 1.
// Not reproduced: CRoomAttributes::Serialize(room, a2, a3, stream) and
// NetStructArray<CMatching::MemberInfoNetStruct,32>::Serialize(this + 0x18, ...) (their result is OR-ed into the
// return value): the NetStruct wire format lives in net-transport, and the (int, int, NetBitStream&)
// overload of CRoomAttributes::Serialize is not declared. The element serializer is a stand-in that writes
// nothing and reports no data, so the function always returns 0 and the stream is not written.
std::int32_t CMatchingGLLive::WritePacketData(std::int32_t a2, std::int32_t a3, NetBitStream& stream) {
    CRoomAttributes* room = GetRoomAttributes();
    room->SetNetEnabled(IsHost());  // NetStruct::SetEnabled(room + 8, IsHost())
    // CRoomAttributes::Serialize and NetStructArray<...>::Serialize: see above.
    std::int32_t result = 0;
    for (int i = 0; i < kMemberInfoCount; ++i) {
        const bool enabled = IsServer() || GetMemberIndex(memberId_) == i;
        memberInfo_[i].SetEnabled(enabled);
        if (memberInfo_[i].Serialize(stream, a2, a3) != 0) {
            result = 1;
        }
    }
    return result;
}

// 0x81f648 _ZN15CMatchingGLLive18SendGameParametersEv
// Partial. Only the host sends game parameters. For the host the original serializes the room
// attributes (virtual slot +0xa4) to a 128-byte buffer with Serialize(char*, 0x80, 15), calls
// virtual slot +0x08 on CLobbyParameterAndQuery at +0x6808 with that buffer, then sends
// GLXPlayerMPLobby::mpSendSetGameParameter(buffer, 128, query). Both the char* Serialize overload and
// the mpSendSetGameParameter overload with a buffer argument are not declared in this port, so the
// host branch does nothing. Non-host callers return at once, as in the original.
void CMatchingGLLive::SendGameParameters() {
    if (!IsHost()) {
        return;
    }
    // Partial: host branch (serialize room attributes, send game parameters) not reproduced.
}

// 0x81f6f8 _ZN15CMatchingGLLive10KickMemberEi
// Partial. The original returns 1 when this is not the host, or when the lobby's virtual slot +0x34
// returns null. It then looks up the member by id on the lobby observer (CMatchingGLLiveLobbyObserver::
// GetMemberByMemberId, declared but returns nullptr in this port) and copies the member's fields into
// a kick record (name, ids and flags). A member with id -1 returns 1. Otherwise it calls
// mpSendKickOut(record) and queues event 4194318 on CMatching's queue. The lobby slot, the member
// record layout (tMemberInfo) and the CMatching queue are not declared, so this returns 1 after the
// host check only (no kick is sent).
std::int32_t CMatchingGLLive::KickMember(std::int32_t /*memberId*/) {
    if (!IsHost()) {
        return 1;
    }
    return 1;  // Partial: kick record copy and mpSendKickOut not reproduced.
}

// 0x81f948 _ZN15CMatchingGLLive20CreateJoinRoomInternalEbR15CRoomAttributes
// Partial. Returns -1 when the user is not logged in (IsLoggedIn). Reproduced after that: the byte at +0x67ec
// takes the negated flag, and the attributes are copied into the create-room attributes at +0x3650.
// Not reproduced: the original then serializes the attributes into a 128-byte buffer
// (CRoomAttributes::Serialize(char*, 0x80, 15), not declared), stores that buffer as parameter 0 of the
// CLobbyParameterAndQuery at +0x6808 (virtual slot +0x08), sends mpSendCreateLobbyWithGameCenter (when the Game
// Center flag +0x67e8 is set; its arguments are the globals dword_A33880, dword_A33884 and the name list
// dword_A33888, none recovered) or mpSendCreateLobby (login name from GLXPlayerLogin +0x74, the flag, the base
// byte +0x0e, the member capacity, the buffer, the player parameter string's data and length and the query),
// and sets the room operation +0x6834 to 3. Because no request is sent, the room operation is left unchanged.
std::int32_t CMatchingGLLive::CreateJoinRoomInternal(bool flag, CRoomAttributes& attributes) {
    if (!IsLoggedIn()) {
        return -1;
    }
    unk_67ec = !flag;                 // +0x67ec
    createRoomAttributes_ = attributes;  // +0x3650, CRoomAttributes::operator=
    return 0;
}

// 0x82078c _ZN15CMatchingGLLive10UpdateRoomEv
// Partial. Returns -1 when the user is not logged in. The rest of the original is an event-driven
// state machine: it consumes GLLive events 4, 5, 6, 7 and 8 from s_eventQueueGLLiveInternal and
// reads the lobby observer's state (+0x14), last result (+0x48) and room id (+0x08), then depending
// on the room state at +0x6834 (3, 7 or 9) it sends player status 3, updates the room map at
// +0x6720, calls the virtual slot +0x90 (room reset) and queues events 4194312 or 8388622 on the
// CMatching queues. The observer fields are not reachable through the observer header, the room
// map entry type is not declared for the room list, and the CMatching queues are not in this port.
// Returns 0 for a logged-in user until those are available.
std::int32_t CMatchingGLLive::UpdateRoom() {
    if (!IsLoggedIn()) {
        return -1;
    }
    return 0;  // Partial: event-driven state machine not reproduced.
}

// 0x820a6c _ZN15CMatchingGLLive6UpdateEv
// Partial. The base CMatching::Update (CMatching::Update, partial in its own row) runs first, as in the
// original. The original then, on the first call with the config loaded, creates the GLLive message,
// friend and leaderboard components (+0x6708..+0x671c, +0x67eb). Then it reads the sign-in state, binds the
// player UID, and processes the GLLive connection events (event 6291457 on the connection queue, Game Center
// state 6 and 7, and the lobby lookup by name). Finally it runs UpdateRoom, ProcessGLLiveEvents and
// ProcessFriendEvents and returns their OR. The part this port has (the base update, UpdateRoom,
// ProcessGLLiveEvents, ProcessFriendEvents, combined in the original order) is reproduced. The component
// bootstrap, the sign-in branch and the connection-event branches are not reproduced, because the
// components' observers, the sign-in update and the connection-manager event queue are not in this port.
std::int32_t CMatchingGLLive::Update() {
    CMatching::Update();  // CMatching::Update (partial in its own row)
    const std::int32_t updated = UpdateRoom();
    const std::int32_t gl = ProcessGLLiveEvents() | updated;
    return gl | ProcessFriendEvents();
}

// 0x820e7c _ZN15CMatchingGLLive9AddServerEiR10CNetworkIdiR15CRoomAttributes
// Under the mutex at +0x675c, walks the server map at +0x39e8 in key order for an entry whose address equals
// the argument. A match gets its rank (+0x3c) replaced and its last-seen time (+0x20) set to the online clock,
// and the function returns. Otherwise a tMatchingPeer is built with the member id, the address, the online
// clock, the transport name of the address, the rank and a copy of the room attributes, and is inserted under
// the next key (the counter at +0x363c, which is incremented). When the pending flag at +0x3630 is set it posts
// event 8388622 on the matching queue.
// Partial: the transport name comes from the transport stand-in (GetTransportNameOf, the original asks the
// transport manager's vtable slot +0x1c) and the clock from the online clock stand-in. The peer's time offset
// (+0x40) is copied from an uninitialised temporary in the original; the port keeps 0.
void CMatchingGLLive::AddServer(std::int32_t memberId, CNetworkId& serverId, std::int32_t rank,
                                CRoomAttributes& attributes) {
    serversMutex_.Lock();
    for (auto& entry : servers_) {
        if (entry.second.address.IsEqual(serverId)) {
            entry.second.lastSeenMs = OnlineNowMs();  // +0x20
            entry.second.rank = rank;                 // +0x3c
            serversMutex_.Unlock();
            return;
        }
    }
    tMatchingPeer peer;
    peer.memberId = memberId;
    peer.address = serverId;
    peer.lastSeenMs = OnlineNowMs();
    peer.name = GetTransportNameOf(serverId);
    peer.rank = rank;
    peer.roomAttributes = attributes;
    const std::uint64_t key = static_cast<std::uint64_t>(static_cast<std::int64_t>(nextPeerKey_));
    ++nextPeerKey_;  // +0x363c
    servers_[key] = peer;
    if (roomOperationPending_) {
        MatchingInternalEventQueue().AddEvent(8388622, nullptr, 0);
    }
    serversMutex_.Unlock();
}

// 0x821110 _ZN15CMatchingGLLive20ProcessServerMessageER10CNetworkIdR12NetBitStream
// Partial, no body. The original runs only when IsServer() is false (CMatching::IsServer is reachable now).
// It then reads a 0x28-byte record (a 28-byte CNetworkId, the member id at +0x1c and the member count at +0x20),
// unserializes the room attributes from the stream, calls AddServer(member id, address, member count,
// attributes) and destroys the attributes. CRoomAttributes::Unserialize(NetBitStream&) is declared in
// c_room_attributes.hpp but not defined (the NetStruct wire format is net-transport's), so the attributes of
// a server message cannot be read and nothing is added.
void CMatchingGLLive::ProcessServerMessage(CNetworkId& /*from*/, NetBitStream& /*stream*/) {
    // Partial: IsServer() check, stream read, Unserialize and the AddServer call not reproduced.
}

// 0x821218 _ZN15CMatchingGLLive30sBroadcastPacketReceiverCallbackER10CNetworkIdPci
// The original forwards the packet to CMatchingGLLive::BroadcastPacketReceiverCallback on the
// matching object (the static CMatching::s_Matching). This port has no static instance of
// CMatchingGLLive to forward to, so the forward is not made and the packet is dropped.
void CMatchingGLLive::sBroadcastPacketReceiverCallback(CNetworkId& /*from*/, char* /*data*/, int /*size*/) {
    // Partial: forward to the matching instance (CMatching::s_Matching) not reproduced.
}
