// CMatching base layout: the room and member layer that CMatchingLocal, CMatchingGLLive and
// CMatchingBluetooth derive from. This header does not replace deps/matching.hpp (the stand-in
// that the message layer uses). It records the layout recovered from the bodies below, so the
// blocked rows can be written against real offsets.
//
// Source addresses (32-bit ARM, Hex-Rays bodies):
//   ProcessLostPacket(int,int) 0x7fed84, SetRoomMemberAttibute 0x7ff008, GetRoomMemberAttibute 0x7ff054,
//   GetRoomMask 0x8000dc, TestRoomMask 0x8001d4, UpdateMemberIdList 0x8002bc, GetMemberIdList 0x800808,
//   SetTeamId 0x8008ac, WritePacketData 0x8010a4, CMatchingLocal::GetRoomAttributes 0x801b54.
//   Also referenced: GetMemberIndex 0x7fe59c, IsMemberServer 0x7fe544, IsServer (non-virtual), Initialize 0x7ff85c.
//   The constructor initialises data up to +0x3628 (three words at +0x3620..+0x3628 follow the element
//   array); the first member of every derived class (CMatchingLocal, CMatchingBluetooth,
//   CMatchingGLLive) sits at +0x3630.
//
// Layout (original 32-bit offsets, kept as comments; the member array is 32 entries of 432 bytes):
//   +0x00 vptr
//   +0x04 not touched by the constructor or any recovered body.
//   +0x08 member capacity (int). Initialize(int) stores its argument here; CMatchingGLLive::JoinRoomInternal
//         refuses when GetMemberCount() is above it, and CreateJoinRoomInternal sends it to the lobby.
//   +0x0c initialized flag (byte), +0x0d room hidden flag (byte), +0x0e room closed flag (byte; LockRoom
//         sets it), +0x0f byte that the constructor clears. CMatchingGLLive's constructor sets it to 3 and
//         passes it as the byte argument of GLXPlayerMPLobby::mpSendJoinLobby. The role is not identified.
//   +0x10 room mask dirty flag (byte). GetRoomMask recomputes when set, then clears it.
//   +0x18 NetStructArray<MemberInfoNetStruct,32> header. Its vptr is not identified.
//   +0x1c member count (int, 0..32). Loops compare against this.
//   +0x20 MemberInfo[32], stride 432 (0x1b0). Element i base = 0x20 + 432*i.
//         Element-relative offsets:
//         +0x000 NetStruct-derived object (vptr). Virtual slot +0x44 on it is ProcessLostPacket(int,int).
//         +0x130 member id object (NetStructMember, 40 bytes). Cached member id (int, -1 when empty) at +0x150.
//         +0x158 team id object (NetStructMember, 40 bytes). Cached team id (int) at +0x178.
//                SetTeamId stores the uint8 argument here as an int.
//         +0x180 room member attribute buffer (NetStructByteArray<64>, 40 bytes). Data pointer at +0x1a0,
//                size (int) at +0x1a4. GetRoomMemberAttibute reads them. Set copies into the buffer.
//         +0x1a8 previous member id (int). UpdateMemberIdList compares it with the current id and queues
//                an event on a change.
//         +0x1a8..+0x1b0 is the end of the element. Nothing else is identified there.
//   Index-based addressing in the bodies: CMatching+0x20+432*i+elem_off, written as this+432*i+X where
//   X = 0x20 + elem_off (member id = X 0x170, team = X 0x178 object / 0x198 value, buffer = X 0x1a0).
//
// Virtual slots used by these bodies (32-bit byte offsets in the vtable):
//   +0x64 (100)  gate bool on UpdateMemberIdList. Name not identified.
//   +0x6c (108)  LocalMemberId() -> int.
//   +0x74 (116)  ServerMemberId() -> int.
//   +0x84 (132)  MemberBit(int) -> uint32 (bit for a member id in a mask).
//   +0x88 (136)  connected member id list, returned by value through a hidden pointer. Name not identified.
//   +0xa4 (164)  GetRoomAttributes() -> CRoomAttributes*. CMatchingLocal returns this+0x4660 (18016).
// The vtable listing (tools/decomp_vtable.py CMatchingGLLive) names the slots: +0x64 IsInRoom(), +0x6c GetMemberId()
// (LocalMemberId), +0x74 GetServerMemberId() (ServerMemberId), +0x84 GetMemberMask(int) (MemberBit),
// +0x88 CMatching::GetMemberIdList() (ConnectedMemberIds), +0xa4 GetRoomAttributes(). CMatchingGLLive
// overrides them under the names below. Slot +0x28 is Initialize(int), +0x2c Terminate(), +0x30 Update().
//
// Not identified:
//   - Vptr of the NetStructArray header at +0x18.
//   - Layout of the NetStruct base inside each MemberInfo element (its size is not recovered).
//   - Meaning of the event codes 0x800007 and 0x800006 queued by UpdateMemberIdList on
//     CMatching::m_eventQueueInternal (join and leave events are likely; not confirmed).
//   - Static cache CMatching::GetRoomMask()::maskCache is a function-local static, not a member.
//
// Deviation: MemberInfo below is a named struct, not a 432-byte byte image. Use the element offsets
// above when mapping to the original memory. The NetStruct base of each element is not modelled.
#pragma once

#include <cstdint>
#include <vector>

#include "net-session-online/c_room_attributes.hpp"
#include "net-session-online/deps/net_bit_stream.hpp"

class CMatchingBase {
public:
    static constexpr int kMaxMembers = 32;           // NetStructArray<MemberInfoNetStruct,32>
    static constexpr int kMemberInfoStride = 432;    // 0x1b0 bytes per element
    static constexpr int kElementsBase = 0x20;       // first element, CMatching-relative

    // Element-relative offsets of MemberInfoNetStruct (see the layout above).
    static constexpr int kElemMemberIdObject = 0x130;
    static constexpr int kElemMemberIdValue = 0x150;
    static constexpr int kElemTeamObject = 0x158;
    static constexpr int kElemTeamValue = 0x178;
    static constexpr int kElemRoomAttrBuffer = 0x180;
    static constexpr int kElemRoomAttrData = 0x1a0;
    static constexpr int kElemRoomAttrSize = 0x1a4;
    static constexpr int kElemPreviousMemberId = 0x1a8;

    // One member record. Fields are named after their use; the NetStruct base is not modelled.
    struct MemberInfo {
        std::int32_t memberId = -1;                  // element +0x150
        std::int32_t teamId = 0;                     // element +0x178
        std::vector<std::uint8_t> roomAttribute;     // element +0x180 (NetStructByteArray<64>)
        std::int32_t previousMemberId = -1;          // element +0x1a8
    };

    // 0x7fff9c / 0x80003c CMatching::CMatching(): partial, see src/tb_session-online-CMatching-0.cpp.
    CMatchingBase();
    virtual ~CMatchingBase() = default;

    // Virtual slots (32-bit byte offsets in the comment). Names are the ones the bodies suggest.
    virtual int LocalMemberId() const = 0;                   // +0x6c
    virtual int ServerMemberId() const = 0;                  // +0x74
    virtual std::uint32_t MemberBit(int memberId) const = 0; // +0x84
    virtual bool UnknownGate() const = 0;                    // +0x64 (IsInRoom() in the derived classes)
    virtual std::vector<int> ConnectedMemberIds() const = 0; // +0x88 (CMatching::GetMemberIdList() in the vtables)
    virtual CRoomAttributes* GetRoomAttributes() = 0;        // +0xa4

    // Non-virtual in the original. The port keeps it virtual so tests can override.
    virtual bool IsServer() const = 0;

    // Seams for CConnectionManager (net-transport, GetConnectionMgr() in the original).
    // Not virtual slots of CMatching: the derived class forwards them to the manager.
    virtual std::vector<int> ConnectionMemberIdList() const = 0;  // GetConnMemberIdList(false)
    virtual bool IsConnectedMember(int memberId) const = 0;       // CConnectionManager::IsConnected
    // CConnectionManager::Disconnect(id, false). Not pure: a derived class that does not forward it keeps
    // the no-op. Added for CMatching::ProcessEvents (e2 batch); CMatchingLocal and the others do not forward it yet.
    virtual void DisconnectMember(int /*memberId*/) {}

    // 0x7fe59c CMatching::GetMemberIndex (glx_320e98.cpp has the ledgered copy on CMatching).
    // Lowest set bit of MemberBit(memberId); -1 when the mask is empty.
    std::int32_t GetMemberIndex(int memberId) const {
        std::uint32_t mask = MemberBit(memberId);
        if (mask == 0) return -1;
        std::int32_t index = 0;
        while ((mask & 1u) == 0) {
            mask >>= 1;
            ++index;
        }
        return index;
    }
    // 0x7fe544 CMatching::IsMemberServer: memberId >= 0 and equal to ServerMemberId().
    bool IsMemberServer(int memberId) const { return memberId >= 0 && memberId == ServerMemberId(); }

    // 0x7fe5e0 CMatching::GetTeamId(int): team value of the member's slot, 0 when the member is not in the table.
    int GetTeamId(int memberId) const;
    // 0x7fe618 CMatching::GetMemberIdByIndex(unsigned int). Partial: index past the 32-slot table returns -1
    // (the original reads past the array).
    std::int32_t GetMemberIdByIndex(std::uint32_t index) const;
    // 0x7fe574 CMatching::LockRoom(bool). Partial: the virtual slot +0x48 call is not ported.
    int LockRoom(bool callSlot);
    // 0x7fed84 CMatching::ProcessLostPacket(int,int). Partial: the room attributes receive the call;
    // the per-member NetStruct slot +0x44 is not ported.
    void ProcessLostPacket(int a2, int a3);
    // 0x7ff008 CMatching::SetRoomMemberAttibute(int,void const*,int). Partial: the value is copied;
    // the NetStruct change notification (buffer slot +0x1c) is not ported.
    void SetRoomMemberAttibute(int memberId, const void* data, int size);
    // 0x7ff054 CMatching::GetRoomMemberAttibute(int,void*,int). The fourth argument is unused by
    // the body: the stored size is copied and returned.
    int GetRoomMemberAttibute(int memberId, void* out, int outSize);
    // 0x8000dc CMatching::GetRoomMask(). Non-virtual. Cache is a function-local static, as decompiled.
    std::uint32_t GetRoomMask();
    // 0x8001d4 CMatching::TestRoomMask(unsigned int): true when every bit of the room mask is in bits.
    bool TestRoomMask(std::uint32_t bits);
    // 0x8002bc CMatching::UpdateMemberIdList(). Partial: the NetStruct change notifications
    // (NetStructMember::SetChanged and the member Set slots) are not ported; the stored values and
    // the queued events are.
    void UpdateMemberIdList();
    // 0x800808 CMatching::GetMemberIdList(): ids of the members whose id is >= 0, in slot order.
    std::vector<int> GetMemberIdList() const;
    // 0x8008ac CMatching::SetTeamId(int,unsigned char). Partial: the stored value is set; the NetStruct
    // change notification is not ported.
    void SetTeamId(int memberId, std::uint8_t teamId);

    // 0x801120 CMatching::ProcessEvents(). Partial, see src/e2-net-session-online-pack0-9acceb-0.cpp.
    // The original returns the last callee's pointer; the port returns void.
    void ProcessEvents();
    // 0x8018fc CMatching::Update() (vtable +0x30, the vtables of CMatchingLocal and CMatchingGLLive replace
    // it). Partial: the initialized branch is not ported.
    virtual int Update();
    // 0x7ff85c CMatching::Initialize(int) (vtable +0x28). Partial, see src/tb_session-online-CMatching-0.cpp.
    virtual int Initialize(int memberCapacity);

protected:
    std::int32_t memberCapacity_ = 0;            // +0x08 (Initialize(int) argument)
    bool initialized_ = false;                   // +0x0c (IsServer requires it; same field as CMatchingLocal::initialized_)
    bool roomHidden_ = false;                    // +0x0d (IsRoomHidden of CMatchingGLLive; CMatchingLocal saves the hidden state here)
    bool baseRoomClosed_ = false;                // +0x0e (set by LockRoom)
    std::uint8_t unk_0f = 0;                     // +0x0f (cleared by the constructor; CMatchingGLLive sets 3)
    bool roomMaskDirty_ = false;                 // +0x10
    int memberCount_ = 0;                        // +0x1c
    MemberInfo members_[kMaxMembers];            // +0x20, stride 432
};
