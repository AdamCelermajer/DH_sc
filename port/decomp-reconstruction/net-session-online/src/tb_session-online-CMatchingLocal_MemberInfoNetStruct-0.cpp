// CMatchingLocal::MemberInfoNetStruct: the per-member record of CMatchingLocal's OWN member table
// (32 elements at +0x4BE0, stride 408), the table CMatchingLocal::ReadPacketData / WritePacketData /
// ProcessLostPacket / ProcessAcknowledgedPacket walk and that CMatchingLocal::IsInRoom(int) reads.
//
// WHAT THE OLD HEADER GOT WRONG: c_matching_local.hpp declared this name only as an empty forward
// declaration, so nothing defined it and the four 408-stride bodies above were left partial for want of
// it. The name is NOT a re-declaration of CMatching::MemberInfoNetStruct: the symbol
// _ZN14CMatchingLocal19MemberInfoNetStructC1Ev (0x806b98) and its own vtable
// _ZTVN14CMatchingLocal19MemberInfoNetStructE prove a separate class, and the only NetStructArray
// instantiation in the binary names the base one
// (NetStructArray<N9CMatching19MemberInfoNetStructELj32EE). This file defines the constructor; the
// destructor (0x806258 D1, 0x806344 D0) and the record layout are declared in the owner header.
//
// Layout evidence, from 0x806b98 and the 408-stride call sites:
//   NetStruct::NetStruct(this) first, then the derived vptr, then one int member built inline with
//   sizeBits 32 at member+0x04, stamp 0, ackMask/sentMask -1, changeMask 0, changed false and its value
//   word, then a NetStructMemberType<CNetworkId> built from a CNetworkId::Reset() local at member +0x28
//   (its vptr overwritten with off_992008 afterwards), then DeclareMember for the int member and for the
//   CNetworkId member, in that order. The port's MemberInfoNetStruct holds the int as a real
//   NetStructInt<32> member (the vtable the call sites use for SetValue is the NetStructInt<32> one) and
//   the address as a plain CNetworkId, because the CNetworkId member's vtable is one of the two
//   unidentified tables this category has not dumped (off_992008 for the value, off_963648 for the
//   destroyed state).
#include "net-session-online/c_matching_local.hpp"

// 0x806b98 _ZN14CMatchingLocal19MemberInfoNetStructC1Ev
// Reconstructed. The two "read the value before any store, clear it and SetChanged when nonzero" branches
// the pseudocode shows cannot run in C++ (the int member is value-initialised) and are not a behavioural
// difference: a fresh member's change stamp is 0, so SetChanged there would only advance
// NetStruct::s_changeCounter -- the case this category has already settled (NOTES.md).
CMatchingLocal::MemberInfoNetStruct::MemberInfoNetStruct() : NetStruct() {
    // The binary builds the int member inline and then writes it through the SetValue slot from a stack
    // temporary; NetStructInt<32>'s constructor plus SetValue is that sequence.
    idMember_.SetValue(0);
    DeclareMember(&idMember_);
}
