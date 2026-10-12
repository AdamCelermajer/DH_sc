// Batch e2-net-session-online-CMatchingGLLive-07f9f6-0 (CMatchingGLLive, 12 functions).
//
// Every body here is partial. The original code reads layout that the owner header does not
// declare yet (the CMatching base at +0x0c..+0x6000, the room attribute copy at +0x3650, the
// connected member table, the lobby buffer at +0x6800/+0x6804, the CLobbyParameterAndQuery
// pointer at +0x6808, the member and room lists of the lobby observer) and calls lobby
// methods whose argument lists are not declared in glx_player_mp_lobby.hpp. Per the Track B
// rules, the bodies do not invent those fields. Each one keeps the part of its control flow
// that the port can express, and the rest is named in its note. Layout requests for the
// integrator are listed in the batch report and the ledger notes.
//
// Not reconstructed: GetRoomList (0x82051c). It returns a std::vector<tRoomInfo>, and tRoomInfo is
// not declared in any header of this tree, so there is no definition and no declaration.

#include "net-session-online/c_matching_gl_live.hpp"

#include <cstdint>

#include "net-session-online/c_room_attributes.hpp"
#include "net-session-online/deps/net_bit_stream.hpp"
#include "net-session-online/deps/network_id.hpp"

// 0x81f264 _ZN15CMatchingGLLive10InitializeEi
// Partial. The original calls CMatching::Initialize (base, not in this port) and returns 0 when the
// base flag at +0x0c is already set. It then creates the GLXPlayerSereverConfig (0x44 bytes, stored
// at +0x6710) and copies the login observer's fields (+0x04, +0x05, +0x06 and 128 bytes from +0x06),
// creates GLXPlayerUser (0x130 bytes, stored at +0x6718) and CLobbyParameterAndQuery (0x40 bytes,
// stored at +0x6808), sends the server config request, and registers four packet types with
// CConnectionManager (sBroadcastPacketReceiverCallback for types 2, 3 and 9, sPacketReceiverCallback
// for type 4). Setting field +0x3644 to 1 is in the original as well. The config constructor
// argument (dword_A33870), the observer copy, the GLXPlayerUser and lobby-parameter allocations and
// the packet registration are not reproduced here. This returns 0 until those layouts exist.
std::int32_t CMatchingGLLive::Initialize(std::int32_t /*a2*/) {
    return 0;
}

// 0x81f488 _ZN15CMatchingGLLive16JoinRoomInternalEy
// Partial. The original returns -1 when the member count (virtual slot +0x60) is above the
// capacity at +0x08 of the base, then locks the mutex at +0x675c, calls GLXPlayerMPLobby::
// mpSendJoinLobby(room id, byte at +0x0f, buffer at +0x6804, length from +0x6800 - +0x6804), sets
// the room state at +0x6834 to 9, stores the room id at +0x680c, clears the byte at +0x3630 and
// unlocks. The capacity field (+0x08 of the base), the mutex at +0x675c, the join-buffer pointer
// and length, and the byte at +0x3630 are not declared, so no call is made here and the state is
// not changed. Returns 0 without side effects.
std::int32_t CMatchingGLLive::JoinRoomInternal(std::uint64_t /*roomId*/) {
    return 0;
}

// 0x81f53c _ZN15CMatchingGLLive15WritePacketDataEiiR12NetBitStream
// Partial. The original reads the room attributes through virtual slot +0xa4 (the copy at +0x3650),
// sets the enabled flag of the room attribute NetStruct to IsHost(), serializes it, then serializes
// the NetStructArray of 32 MemberInfoNetStruct entries at +0x18 and, for each entry, sets its
// enabled flag (server, or the entry whose member index equals the local member) and serializes it
// through its vtable. The returned value is 1 when any part wrote data. The NetStructArray at +0x18,
// the per-entry NetStruct serializers and the room attribute NetStruct are not ported, so no
// stream data is written and the function returns 0.
std::int32_t CMatchingGLLive::WritePacketData(std::int32_t /*a2*/, std::int32_t /*a3*/, NetBitStream& /*stream*/) {
    return 0;
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
// Partial. Returns -1 when the user is not logged in (IsLoggedIn). Otherwise the original stores
// !flag at +0x67ec, copies the room attributes into +0x3650, serializes them to a 128-byte buffer,
// then sends either mpSendCreateLobbyWithGameCenter (when the game-center flag at +0x67e8 is set) or
// mpSendCreateLobby with the login name (read from the login object at +0x74), sets the room state at
// +0x6834 to 3, and returns 0. The room attribute copy at +0x3650, the char* Serialize overload and
// the create-lobby argument lists are not declared, so only the logged-in guard is reproduced here.
std::int32_t CMatchingGLLive::CreateJoinRoomInternal(bool /*flag*/, CRoomAttributes& /*attributes*/) {
    if (!IsLoggedIn()) {
        return -1;
    }
    return 0;  // Partial: create-lobby send and room state update not reproduced.
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
// Partial. The original runs the base CMatching::Update first, and on the first call with the config
// loaded it creates the GLLive message, friend and leaderboard components. Then it reads the sign-in
// state, binds the player UID, and processes the GLLive connection events (event 6291457 on the
// connection queue, Game Center state 6 and 7, and the lobby lookup by name). Finally it runs
// UpdateRoom, ProcessGLLiveEvents and ProcessFriendEvents and returns their OR. The part this port
// has (UpdateRoom, ProcessGLLiveEvents, ProcessFriendEvents, combined in the original order) is
// reproduced. The bootstrap, the sign-in and connection-event branches are not reproduced, because
// the base update, the sign-in singleton and the connection-manager event queue are not in this port.
std::int32_t CMatchingGLLive::Update() {
    const std::int32_t updated = UpdateRoom();
    const std::int32_t gl = ProcessGLLiveEvents() | updated;
    return gl | ProcessFriendEvents();
}

// 0x820e7c _ZN15CMatchingGLLive9AddServerEiR10CNetworkIdiR15CRoomAttributes
// Partial. The original locks the mutex at +0x675c and walks the server map at +0x39e8 (keyed by a
// member counter at +0x363c, which it increments) looking for an entry whose network id equals the
// argument. A matching entry gets its server field and room attributes replaced. Otherwise a new
// tMatchingPeer is inserted, filled with the network id, the room attributes, the transport name
// (through GetTransportMgr and its vtable slot +0x1c) and the server id. It then queues event 8388622
// on CMatching's queue when the byte at +0x3630 is set, and unlocks. The server map type, the entry
// layout and the transport manager are not declared in this port, so AddServer does nothing.
void CMatchingGLLive::AddServer(std::int32_t /*a2*/, CNetworkId& /*serverId*/, std::int32_t /*a4*/,
                                CRoomAttributes& /*attributes*/) {
    // Partial: server map insert and event queueing not reproduced.
}

// 0x821110 _ZN15CMatchingGLLive20ProcessServerMessageER10CNetworkIdR12NetBitStream
// The original runs only when the matching object is not the server (CMatching::IsServer, not
// reachable here). Then it reads a 0x28-byte network id from the stream, unserializes the room
// attributes, and calls AddServer with the network id's field 7 (member id), field 8 and the room
// attributes, then destroys the room attributes. The server check is not declared on this class,
// so no message is read and nothing is added.
void CMatchingGLLive::ProcessServerMessage(CNetworkId& /*from*/, NetBitStream& /*stream*/) {
    // Partial: IsServer() check, stream read and AddServer call not reproduced.
}

// 0x821218 _ZN15CMatchingGLLive30sBroadcastPacketReceiverCallbackER10CNetworkIdPci
// The original forwards the packet to CMatchingGLLive::BroadcastPacketReceiverCallback on the
// matching object (the static CMatching::s_Matching). This port has no static instance of
// CMatchingGLLive to forward to, so the forward is not made and the packet is dropped.
void CMatchingGLLive::sBroadcastPacketReceiverCallback(CNetworkId& /*from*/, char* /*data*/, int /*size*/) {
    // Partial: forward to the matching instance (CMatching::s_Matching) not reproduced.
}
