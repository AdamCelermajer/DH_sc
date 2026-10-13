// Stand-ins for the classes CConnectionManager calls into. CMatching, CConnection,
// CEventQueueBase and CTransportManager are owned by other slices; only the members the
// connection manager uses are declared here. Slot numbers are the byte offsets of the
// virtual calls in the original vtables. Bodies are no-op or simple test doubles and must
// be replaced by the real classes when the categories are merged.
#pragma once

#include <cstdint>

#include "net-transport/connection.hpp"
#include "net-transport/net_bit_stream.hpp"
#include "net-transport/net_network_id.hpp"

class CTransportManager;  // full class: net-transport/deps/transport_manager.hpp

// CMatching: the matching/session state. Only the calls below are used.
class CMatching {
public:
    // 0x... _ZN9CMatching3GetEv (the Hex-Rays call sites pass a dropped argument)
    static CMatching* Get();
    // _ZN9CMatching8IsServerEv
    bool IsServer() const { return server_; }
    // Maps a network member id to its connection slot index (0..31), -1 when there is none.
    int GetMemberIndex(int member_id) const;

    // Virtual slots. The names are provisional; the byte offsets are authoritative.
    int LocalMemberId() const { return 0; }                 // vtable +0x6c
    void SetLocalMemberId(int value) { (void)value; }       // vtable +0x70 (provisional)
    int ServerMemberId() const { return 0; }                // vtable +0x74
    // vtable +0x68, CMatchingLocal::IsInRoom(int): whether the member index is in the room. Stand-in: the
    // value the test set (false by default).
    bool IsInRoom(int member_index) const { (void)member_index; return in_room_; }
    int NewMemberId() const { return 0; }                   // vtable +0xac
    int TransportHandle(int protocol) const { (void)protocol; return 0; }  // vtable +0x34

    // The member bit used by the replicated-struct masks (NetStruct::SetEnabled, Resend, HasDataToSend,
    // ProcessLostPacket, ...). Stand-in for CMatching vtable +0x84: the bit of the member's slot
    // (GetMemberIndex), 0 when the member has none. The real class is CMatchingBase::MemberBit in
    // net-session-online.
    std::uint32_t MemberBit(int member_id) const {  // vtable +0x84
        const int index = GetMemberIndex(member_id);
        return index >= 0 ? (1u << index) : 0u;
    }
    // 0x8000dc CMatching::GetRoomMask(), non-virtual. Stand-in: the mask the test set (0 by default).
    std::uint32_t GetRoomMask() const { return room_mask_; }
    // 0x8001d4 CMatching::TestRoomMask(bits): true when every bit of the room mask is in bits.
    bool TestRoomMask(std::uint32_t bits) const { return (room_mask_ & bits) == room_mask_; }

    // Test hook for the stand-in: the server flag returned by IsServer().
    static void SetServerForTest(bool server);
    // Test hook for the stand-in: the room mask returned by GetRoomMask().
    static void SetRoomMaskForTest(std::uint32_t mask);
    // Test hook for the stand-in: the value returned by IsInRoom(int).
    static void SetInRoomForTest(bool in_room);

private:
    bool server_ = false;
    std::uint32_t room_mask_ = 0;
    bool in_room_ = false;
};

// CConnection is the real class in net-transport/connection.hpp.

// CEventQueueBase: the event queue at +0x08 of the connection manager.
// Vtable: +0x00 AddEvent, +0x0c Clear.
class CEventQueueBase {
public:
    virtual ~CEventQueueBase() = default;
    virtual int AddEvent(int event, void* data, int size);  // +0x00
    virtual int Clear();                                    // +0x0c
};

// Calls CConnectionManager makes on CTransportManager. The class itself is in the
// transport-manager slice; until it is merged these are no-op stand-ins that return 0.
namespace transport_manager_calls {
void Update(CTransportManager& tm);
void Initialize(CTransportManager& tm);
void Terminate(CTransportManager& tm);
void InitializeTransport(CTransportManager& tm, std::intptr_t transport);
void Disconnect(CTransportManager& tm, CNetworkId& peer);
// CTransportManager::Send with a network id: (transport, to, packet type, data, size).
int Send(CTransportManager& tm, std::intptr_t transport, const CNetworkId& to,
         int packet_type, const void* data, int size);
// CTransportManager::Send without a network id: (transport, packet type, data, size).
int Send(CTransportManager& tm, std::intptr_t transport, int packet_type,
         const void* data, int size);
}  // namespace transport_manager_calls
