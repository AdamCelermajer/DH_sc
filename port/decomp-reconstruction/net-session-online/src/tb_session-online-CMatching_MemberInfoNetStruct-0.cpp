// Batch e1-net-session-online-CMatching_MemberInfoNetStruct-0 (CMatching::MemberInfoNetStruct).
// Declarations: include/net-session-online/tb/cmatching_member_info_net_struct.hpp.
// Offsets in comments are the original 32-bit element offsets (see the header).

#include "net-session-online/tb/cmatching_member_info_net_struct.hpp"

#include "net-session-online/deps/platform_stand_ins.hpp"

// 0x7ffd04 _ZN9CMatching19MemberInfoNetStructC1Ev
// PARTIAL. Two gaps, both from the original reading memory that is not yet initialised:
//   1. The member id value (+0x150) and team id value (+0x178) are read before any store. If either
//      is nonzero, the original sets it to 0 and calls NetStructMember::SetChanged. The port
//      value-initialises both members, so those reads are always 0 and the SetChanged branches
//      cannot run.
//   2. The NetStructByteArray<64> constructor body (not in this batch) is called with an empty
//      ByteArray {null, 0}. The port default-constructs the byte array to that same empty value,
//      and the original's p cleanup has nothing to free.
// The DeclareMember calls and NetStructMember::SetChanged are stand-ins (see deps/net_struct_stub.hpp).
CMatchingMemberInfoNetStruct::CMatchingMemberInfoNetStruct()
    : NetStructBaseStub() {
    // Member id object (+0x130).
    const std::int32_t priorMemberId = memberId_.value;  // +0x150, read before any store
    memberId_.changeStamp = 0;                           // +0x138 (qword)
    memberId_.sizeBits = 32;                             // +0x134
    memberId_.sentMask = 0xffffffffu;                    // +0x144
    memberId_.ackMask = 0xffffffffu;                     // +0x140
    memberId_.changeMask = 0;                            // +0x148
    memberId_.changed = false;                           // +0x14c
    if (priorMemberId != 0) {
        memberId_.value = 0;                             // +0x150
        memberId_.SetChanged();
    }

    // Team id object (+0x158).
    const std::int32_t priorTeamId = teamId_.value;      // +0x178, read before any store
    teamId_.changeStamp = 0;                             // +0x160 (qword)
    teamId_.sizeBits = 8;                                // +0x15c
    teamId_.sentMask = 0xffffffffu;                      // +0x16c
    teamId_.ackMask = 0xffffffffu;                       // +0x168
    teamId_.changeMask = 0;                              // +0x170
    teamId_.changed = false;                             // +0x174
    if (priorTeamId != 0) {
        teamId_.value = 0;                               // +0x178
        teamId_.SetChanged();
    }

    // Room member attribute buffer (+0x180). The original passes the empty ByteArray p = {null, 0}
    // to NetStructByteArray<64>::NetStructByteArray; see the PARTIAL note above.
    roomAttribute_.data = nullptr;                       // +0x1a0
    roomAttribute_.size = 0;                             // +0x1a4

    previousMemberId_ = -1;                              // +0x1a8

    DeclareMember(&memberId_);
    DeclareMember(&teamId_);
    DeclareMember(&roomAttribute_);

    // Final values. The original stores -1 into the member id and 0 into the team id through the
    // virtual SetValue (vtable +0x1c) of each member, from a local whose value was set to -1 (member id)
    // or 0 (team id) before the call. The port assigns the value directly, as NetStructMemberType<T>::
    // SetValue does (value_ = value). The vtable name of this slot is not in the dump, so this
    // equivalence is assumed, not checked against the binary.
    memberId_.value = -1;                                // SetValue(-1), +0x150
    teamId_.value = 0;                                   // SetValue(0), +0x178
}

// 0x7ffc28 _ZN9CMatching19MemberInfoNetStructD1Ev
// The original frees the room member attribute data block when it is set (CustomFree, then the
// pointer is cleared; the size is left as it is), then erases the packet history tree when it is
// not empty and resets its header. std::map::clear does the same. The NetStruct base destructor has
// no separate call in the original; its body is inlined here, and it is the map step above.
// Vptr stores are the compiler's job here.
CMatchingMemberInfoNetStruct::~CMatchingMemberInfoNetStruct() {
    void* data = roomAttribute_.data;                    // +0x1a0
    if (data) {
        CustomFree(data);
        roomAttribute_.data = nullptr;                   // +0x1a0
    }
    if (!packetHistory_.empty()) {
        packetHistory_.clear();                          // +0x10c..+0x11f
    }
}

// 0x7ffce8 _ZN9CMatching19MemberInfoNetStructD0Ev
// Deleting destructor: the D1 body, then CustomFree(this).
void CMatchingMemberInfoNetStruct::DeletingDestructor() {
    this->~CMatchingMemberInfoNetStruct();
    CustomFree(this);
}
