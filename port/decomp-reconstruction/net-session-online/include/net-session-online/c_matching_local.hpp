// CMatchingLocal: the matching layer for a local (LAN or Bluetooth-free) session. Only
// the small accessors are reconstructed here. The room list, the server list, the
// packet callbacks and the destructors are queued.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x3638 local member id (int)
//   +0x4b6d room closed flag (byte)
//   +0x4b8d room hidden flag (byte). When set, IsRoomHidden reads true.
#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "net-session-online/deps/matching_stand_ins.hpp"
#include "net-session-online/deps/net_mutex.hpp"
#include "net-session-online/t_matching_peer.hpp"

// The 40-byte record that SendServerResponse writes and ProcessServerMessage reads
// (0x28 bytes on the wire; the last three bytes are padding).
struct RoomServerHeader {
    CNetworkId listenId;       // +0x00 (28 bytes)
    std::int32_t memberId;     // +0x1c
    std::int32_t memberCount;  // +0x20
    std::uint8_t flag;         // +0x24 (true: remove the server)
    std::uint8_t reserved[3];  // +0x25 (padding, zeroed)
};
static_assert(sizeof(RoomServerHeader) == 40, "RoomServerHeader is the 40-byte wire record");

class CMatchingLocal {
public:
    // 0x807e44 _ZN14CMatchingLocalC2Ev: sets the counters and empty containers. The original
    // also runs Reset() and the NetStruct member constructors, which are not in this port yet.
    CMatchingLocal();
    // 0x807f80 _ZN14CMatchingLocalD1Ev: the members release themselves. The NetStruct member
    // teardown of the original is not in this port yet.
    ~CMatchingLocal();

    // 0x801b08 _ZN14CMatchingLocal12IsRoomClosedEv
    bool IsRoomClosed() const { return roomClosed_ != 0; }
    // 0x801b14 _ZN14CMatchingLocal12IsRoomHiddenEv: the hidden flag, else the closed flag.
    bool IsRoomHidden() const;
    // 0x801b3c _ZN14CMatchingLocal11SetMemberIdEi
    void SetMemberId(std::int32_t memberId) { memberId_ = memberId; }
    // 0x805664 _ZN14CMatchingLocal13GetMemberMaskEi: see member_mask.hpp.
    std::int32_t GetMemberMask(std::int32_t memberId) const;
    // 0x805694 _ZN14CMatchingLocal19GetInvitationRoomIdEv: returns 0 (64-bit).
    std::uint64_t GetInvitationRoomId() const { return 0; }
    // 0x8056a0 _ZN14CMatchingLocal16GetTransportTypeE14tPROTOCOL_TYPE: table {1, 2, 1}
    // indexed by the protocol type; any type above 2 gives 0. The object is not read.
    static std::int32_t GetTransportType(std::uint32_t protocolType);
    // 0x801fb0 _ZN14CMatchingLocal14SendInvitationEPKcS1_b: posts event 0x800015, returns 0.
    // The original's names are not used by the body.
    std::int32_t SendInvitation(const char* from, const char* to, bool flag);
    // 0x801fe8 _ZN14CMatchingLocal17ReceiveInvitationEv: posts event 0x800017, returns 0.
    std::int32_t ReceiveInvitation();
    // 0x805998 _ZN14CMatchingLocal26ProcessChangeServerMessageEiR12NetBitStream: reads the
    // 4-byte server id into +0x363c, then posts event 0x80000b carrying it.
    void ProcessChangeServerMessage(std::int32_t from, class NetBitStream& stream);

    // 0x8074a4 _ZN14CMatchingLocal5ResetEv: resets the room state. Not in any slice yet; declared
    // for the callers in this port. Tests supply a no-op.
    void Reset();

    // Lifecycle (c_matching_local_lifecycle.cpp). Base CMatching calls are not in this port.
    // 0x807c70 _ZN14CMatchingLocal10InitializeEi
    std::int32_t Initialize(std::int32_t arg);
    // 0x80605c _ZN14CMatchingLocal9TerminateEv
    std::int32_t Terminate();
    // 0x806098 _ZN14CMatchingLocal9PingRoomsEv: posts event 0x800010.
    std::int32_t PingRooms();

    // Rooms (c_matching_local_rooms.cpp). Room flags are set through the NetStruct members in
    // the original; here they are the plain fields below.
    // 0x806d64 _ZN14CMatchingLocal8HideRoomEv
    std::int32_t HideRoom();
    // 0x806e4c _ZN14CMatchingLocal9CloseRoomEv
    std::int32_t CloseRoom();
    // 0x806fc8 _ZN14CMatchingLocal8ShowRoomEv
    std::int32_t ShowRoom();
    // 0x8070bc _ZN14CMatchingLocal8OpenRoomEb
    std::int32_t OpenRoom(bool open);
    // 0x80795c _ZN14CMatchingLocal9LeaveRoomEv
    std::int32_t LeaveRoom();
    // 0x8079d4 _ZN14CMatchingLocal22CreateJoinRoomInternalEbR15CRoomAttributes
    std::int32_t CreateJoinRoomInternal(bool open, const CRoomAttributes& attributes);
    // 0x807bec _ZN14CMatchingLocal18SearchRoomInternalER17CRoomSearchFilterbh
    std::int32_t SearchRoomInternal(const CRoomSearchFilter& filter, bool flag, std::uint8_t mode);
    // 0x808774 _ZN14CMatchingLocal16JoinRoomInternalEy
    std::int32_t JoinRoomInternal(std::uint64_t key);
    // 0x808694 _ZN14CMatchingLocal23GetSearchRoomAttributesEy
    CRoomAttributes GetSearchRoomAttributes(std::uint64_t key) const;

    // Members (c_matching_local_members.cpp)
    // 0x805c04 _ZNK14CMatchingLocal14GetMemberCountEv
    std::int32_t GetMemberCount() const;
    // 0x805c1c _ZN14CMatchingLocal8IsInRoomEi (partial: see the .cpp)
    bool IsInRoom(std::int32_t memberId) const;
    // 0x805d38 _ZN14CMatchingLocal10KickMemberEi
    std::int32_t KickMember(std::int32_t memberId);
    // 0x80675c _ZN14CMatchingLocal19GenerateNewMemberIdEv (partial: see the .cpp)
    std::int32_t GenerateNewMemberId();

    // Servers (c_matching_local_servers.cpp)
    // 0x8094fc _ZN14CMatchingLocal9AddServerEiR10CNetworkIdiR15CRoomAttributes (partial)
    std::int32_t AddServer(std::int32_t memberId, const CNetworkId& address, std::int32_t rank,
                           const CRoomAttributes& attributes);
    // 0x8072d8 _ZN14CMatchingLocal12RemoveServerER10CNetworkId
    void RemoveServer(const CNetworkId& address);
    // 0x8073c4 _ZN14CMatchingLocal12PurgeServersEv
    std::int32_t PurgeServers();
    // 0x8097c4 _ZN14CMatchingLocal20ProcessServerMessageER10CNetworkIdR12NetBitStream
    void ProcessServerMessage(const CNetworkId& from, class NetBitStream& stream);
    // 0x808538 _ZN14CMatchingLocal13GetMemberNameEi (partial: transport names are stand-ins)
    std::string GetMemberName(std::int32_t memberId) const;

    // Messages (c_matching_local_messages.cpp)
    // 0x805d58 _ZN14CMatchingLocal23SendChangeServerMessageEv
    std::int32_t SendChangeServerMessage();
    // 0x805de0 _ZN14CMatchingLocal18SendServerResponseEb (partial: room attributes not written)
    std::int32_t SendServerResponse(bool flag);
    // 0x805f2c _ZN14CMatchingLocal17SendClientRequestEv
    std::int32_t SendClientRequest();
    // 0x805ee0 _ZN14CMatchingLocal20ProcessClientMessageER10CNetworkIdR12NetBitStream (partial)
    void ProcessClientMessage(const CNetworkId& from, class NetBitStream& stream);
    // 0x805fa8 _ZN14CMatchingLocal22PacketReceiverCallbackEiPci
    void PacketReceiverCallback(std::int32_t from, char* data, std::uint32_t size);
    // 0x80601c _ZN14CMatchingLocal23sPacketReceiverCallbackEiPci
    static void sPacketReceiverCallback(std::int32_t from, char* data, int size);
    // 0x8098cc _ZN14CMatchingLocal31BroadcastPacketReceiverCallbackER10CNetworkIdPci
    void BroadcastPacketReceiverCallback(CNetworkId& from, char* data, std::uint32_t size);
    // 0x80995c _ZN14CMatchingLocal32sBroadcastPacketReceiverCallbackER10CNetworkIdPci
    static void sBroadcastPacketReceiverCallback(CNetworkId& from, char* data, int size);
    // 0x80890c _ZN14CMatchingLocal6UpdateEv (partial: see update.cpp)
    std::int32_t Update();
    // 0x801b54 _ZN16CMatchingLocal17GetRoomAttributesEv: the room attributes at +0x4660.
    CRoomAttributes* GetRoomAttributes() { return &roomAttributes_; }

    // The matching object the static callbacks route to (CMatching::s_Matching in the original).
    static CMatchingLocal* s_Matching;

    // Test seams. Not in the original.
    void SetRoomFlagsForTest(bool closed, bool hiddenFlag) {
        roomClosed_ = closed ? 1 : 0;
        roomHiddenFlag_ = hiddenFlag ? 1 : 0;
    }
    std::int32_t MemberIdForTest() const { return memberId_; }
    std::int32_t ServerMemberIdForTest() const { return serverMemberId_; }
    void SetInitializedForTest(bool value) { initialized_ = value; }
    void SetBaseRoomClosedForTest(bool value) { baseRoomClosed_ = value; }
    void SetJoinThresholdForTest(std::int32_t value) { joinThreshold_ = value; }
    bool IsInitializedForTest() const { return initialized_; }

private:
    std::int32_t memberId_ = 0;     // +0x3638
    std::int32_t serverMemberId_ = 0;  // +0x363c (name inferred, as in CMatchingGLLive)
    std::uint8_t roomClosed_ = 0;   // +0x4b6d
    std::uint8_t roomHiddenFlag_ = 0;  // +0x4b8d

    // Base-class fields of the original (CMatching). Their writers are not in this port yet.
    bool initialized_ = false;             // +0x0c (set by the base Initialize)
    bool hiddenBeforeClose_ = false;       // +0x0d (CloseRoom saves IsRoomHidden() here)
    bool baseRoomClosed_ = false;          // +0x0e (read by ShowRoom and OpenRoom)
    std::int32_t joinThreshold_ = 0;       // +0x08 (rank limit used by JoinRoomInternal and ProcessServerMessage)

    bool roomOperationPending_ = false;    // +0x3630
    std::int32_t nextPeerKey_ = 1;         // +0x3634 (key of the next server entry)
    std::int32_t memberIdCounter_ = 0;     // +0x4bd8 (value of the member-id counter, see GenerateNewMemberId)
    std::int32_t serverPollTime_ = 0;      // +0x465c (online clock at the last server poll)
    CNetMutex serversMutex_;               // +0x463c
    std::map<std::uint64_t, tMatchingPeer> servers_;  // +0x4644 (the map; its count is at +0x4654)
    CRoomAttributes roomAttributes_;       // +0x4660
    CRoomSearchFilter roomSearchFilter_;   // +0x49f8

    // Vtable slot +0x58 of ProcessClientMessage. Not named in the vtable dump; the port
    // treats the request as not accepted (see c_matching_local_messages.cpp).
    bool ClientRequestAccepted(const CNetworkId& from) const;
};
