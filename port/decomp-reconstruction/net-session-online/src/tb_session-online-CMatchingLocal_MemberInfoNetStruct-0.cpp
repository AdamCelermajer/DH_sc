#include "net-session-online/tb/CMatchingLocal_MemberInfoNetStruct.hpp"

#include <cstdint>

#include "net-session-online/deps/platform_stand_ins.hpp"

namespace {

// Base fields of intMember_ that the original constructor writes inline (no constructor call):
// size 32 bits, change stamp 0, ackMask -1, sentMask -1, changeMask 0, changed false.
constexpr stand_in::NetStructMember::Init kIntMemberInit{32, 0, 0xffffffffu, 0xffffffffu, 0, false};

// CNetworkId::Reset on a local, as the original does before copying it into the member.
CNetworkId ZeroNetworkId() {
    CNetworkId id;
    id.Reset();
    return id;
}

}  // namespace

// 0x806b98 CMatchingLocal::MemberInfoNetStruct::MemberInfoNetStruct(void)
//
// Partial. The original's control flow, in order (Hex-Rays):
//   1. NetStruct::NetStruct(this).
//   2. The int member at +0x130 is set up inline: sizeBits 32, change stamp 0, ackMask -1,
//      sentMask -1, changeMask 0, changed false.
//   3. The value slot of the int member (+0x150) is read. When it is non-zero, it is stored as 0
//      and the member's SetChanged() runs.
//   4. The CNetworkId member at +0x158 is built with a zeroed CNetworkId
//      (CNetworkId::Reset on a local, then NetStructMemberType<CNetworkId>::NetStructMemberType).
//   5. The vptr at +0x158 is then overwritten with an unidentified vtable (off_992008).
//   6. DeclareMember is called for the int member, then for the CNetworkId member.
//
// Deviations and missing pieces:
//   - Step 5 is not expressible in C++: the member keeps the dynamic type NetStructMemberType<CNetworkId>.
//     The vtable behind off_992008 is not identified, so the member's behaviour may differ.
//   - C++ constructs both members before the body runs, so step 4 happens before step 3. The two
//     do not touch each other, so the order is not observable here.
//   - Step 3 reads a value slot the original constructor never clears. In C++ the member starts
//     at 0, so the reset branch cannot run. It is kept so the control flow matches the decompile.
//   - The base is the stand-in in deps/net_struct_stand_in.hpp. The real NetStruct also owns the
//     std::map packet history (+0x10c), which is not modelled.
//   - SetChanged() comes from the stand-in, not the real body (0x814f84). See that header.
CMatchingLocal_MemberInfoNetStruct::CMatchingLocal_MemberInfoNetStruct()
    : stand_in::NetStruct(),
      intMember_(kIntMemberInit, 0),
      networkIdMember_(ZeroNetworkId()) {
    if (intMember_.RawValue() != 0) {
        intMember_.RawValue() = 0;
        intMember_.SetChanged();
    }
    DeclareMember(&intMember_);
    DeclareMember(&networkIdMember_);
}

// 0x806258 CMatchingLocal::MemberInfoNetStruct::~MemberInfoNetStruct()
//
// Partial. The original (complete-object destructor) does:
//   1. Store the destructor-phase vptrs: +0x130 and +0x158 get off_963648, and the own vptr
//      gets off_9924D8.
//   2. When the NetStruct packet history map (+0x10c, count at +0x11c) is non-empty, erase its
//      tree (_M_erase on the root at +0x110) and reset the header: root null, leftmost and
//      rightmost point to the header, count 0.
//
// C++ runs step 1 implicitly. Step 2 needs the packet history map, which the stand-in NetStruct
// does not hold, so this body is empty. The members are destroyed by the C++ rules. Partial
// until the packet history map is modelled in the NetStruct base.
CMatchingLocal_MemberInfoNetStruct::~CMatchingLocal_MemberInfoNetStruct() {}

// 0x806344 CMatchingLocal::MemberInfoNetStruct::~MemberInfoNetStruct()  (D0, deleting variant)
//
// Partial. Same body as 0x806258, then the block is released with CustomFree(this).
// C++ has no deleting destructor, so the body is a static member that the caller uses in place
// of a delete expression. The deviation is the packet history erase, shared with 0x806258.
void CMatchingLocal_MemberInfoNetStruct::DeletingDestructor(CMatchingLocal_MemberInfoNetStruct* self) {
    self->~CMatchingLocal_MemberInfoNetStruct();
    CustomFree(self);
}
