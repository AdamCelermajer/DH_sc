// CMatching base layout: the room and member layer that CMatchingLocal, CMatchingGLLive and
// CMatchingBluetooth derive from. This header does not replace deps/matching.hpp (the stand-in
// that the message layer uses). It records the layout recovered from the bodies below, so the
// blocked rows can be written against real offsets.
//
// Source addresses (32-bit ARM, Hex-Rays bodies):
//   ProcessLostPacket(int,int) 0x7fed84, SetRoomMemberAttibute 0x7ff008, GetRoomMemberAttibute 0x7ff054,
//   GetRoomMask 0x8000dc, TestRoomMask 0x8001d4, UpdateMemberIdList 0x8002bc, GetMemberIdList 0x800808,
//   SetTeamId 0x8008ac, WritePacketData 0x8010a4, CMatchingLocal::GetRoomAttributes 0x801b54.
//   Also referenced: GetMemberIndex 0x7fe59c, IsMemberServer 0x7fe544, IsServer (non-virtual).
//
// Layout (original 32-bit offsets, kept as comments; the member array is 32 entries of 432 bytes):
//   +0x00 vptr
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
// NOTE: the +0x34, +0x48 and +0xac slots named in the brief do not occur in these bodies. Only +0x84,
// +0x6c and +0x74 appear in NOTES.md. Those three are not confirmed here.
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

    virtual ~CMatchingBase() = default;

    // Virtual slots (32-bit byte offsets in the comment). Names are the ones the bodies suggest.
    virtual int LocalMemberId() const = 0;                   // +0x6c
    virtual int ServerMemberId() const = 0;                  // +0x74
    virtual std::uint32_t MemberBit(int memberId) const = 0; // +0x84
    virtual bool UnknownGate() const = 0;                    // +0x64 (name not identified)
    virtual std::vector<int> ConnectedMemberIds() const = 0; // +0x88 (name not identified)
    virtual CRoomAttributes* GetRoomAttributes() = 0;        // +0xa4

    // Non-virtual in the original. The port keeps it virtual so tests can override.
    virtual bool IsServer() const = 0;

    // CMatching::ProcessLostPacket(int,int) 0x7fed84 is declared here; the body is not ported.
    // The room attributes receive the call through the NetStruct base (stand-in: no-op).
    void ProcessLostPacketRoom(int a1, int a2) {
        if (CRoomAttributes* room = GetRoomAttributes()) room->ProcessLostPacket(a1, a2);
        // Each element's NetStruct slot +0x44 is then called for members 0..memberCount-1. The
        // element type is not ported yet, so this step is not reproduced here.
    }

protected:
    bool roomMaskDirty_ = false;                 // +0x10
    int memberCount_ = 0;                        // +0x1c
    MemberInfo members_[kMaxMembers];            // +0x20, stride 432
};
