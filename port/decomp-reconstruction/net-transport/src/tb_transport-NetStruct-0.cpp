// NetStruct functions of batch e1-net-transport-NetStruct-0 (part 1 of 2).
// The class is declared in include/net-transport/net_struct.hpp. Functions without a body here
// are partial: the comment above each one names what is missing.
#include "net-transport/net_struct.hpp"

#include <cassert>

// 0x81324c _ZN9NetStruct13DeclareMemberEP15NetStructMember
// Original: count at +0x104, member stored at +0x04 + 4*count, count incremented. The original has
// no bounds check; the port asserts instead of writing past the array. The original also returns
// this in r0; the C++ method is void.
void NetStruct::DeclareMember(NetStructMember* member) {
    assert(member_count_ < kMaxMembers);
    members_[member_count_] = member;
    ++member_count_;
}

// 0x813264 _ZN9NetStruct15ConditionalLoadEbR12NetBitStreamij
// Virtual Load (vt +0x14) when the flag is set, otherwise virtual Erase (vt +0x1c).
int NetStruct::ConditionalLoad(bool conditional, NetBitStream& stream, int a4, std::uint32_t a5) {
    if (conditional) {
        return Load(stream, a4, a5);
    }
    return Erase(stream);
}

// 0x81329c _ZN9NetStruct11GetSizeBitsEv
// Returns 1 for an empty struct. Otherwise 1 plus the sum of each member's GetSizeBits (vt +0x10).
int NetStruct::GetSizeBits() {
    if (member_count_ <= 0) {
        return 1;
    }
    int bits = 1;
    for (int i = 0; i < member_count_; ++i) {
        bits += members_[i]->GetSizeBits();
    }
    return bits;
}

// 0x8132f0 _ZN9NetStruct12GetSizeBytesEv
// Bits / 8, plus one when there is a remainder. Calls the virtual GetSizeBits (vt +0x38).
std::uint32_t NetStruct::GetSizeBytes() {
    const std::uint32_t bits = static_cast<std::uint32_t>(GetSizeBits());
    return (bits >> 3) + ((bits & 7u) != 0 ? 1u : 0u);
}

// 0x81334c _ZN9NetStruct24AreChangesAcknowledgedByEi
// partial: needs CMatching::MemberBit (vt +0x84, on CMatching::Get()). The net-transport CMatching
// stand-in (deps/connection_deps.hpp) does not declare it. Body not written.

// 0x8133c0 _ZN9NetStruct14ResendInternalEj
// partial: the counter increment and the per-member SetSentFlag(mask, false, stamp) loop are known,
// and the stamp is the same value for every member. The return value is the last member's
// SetSentFlag result, but NetStructMember::SetSentFlag is declared void in net_struct.hpp, so the
// return value cannot be reproduced. Body not written.

// 0x81344c _ZN9NetStruct6ResendEi
// partial: needs CMatching::MemberBit. Body not written.

// 0x81347c _ZN9NetStruct6ResendEv
// partial: needs CMatching::GetRoomMask, which the net-transport CMatching stand-in does not declare.
// Body not written.

// 0x81349c _ZN9NetStruct10SetAckedByEi
// partial: needs CMatching::MemberBit. NetStructMember::SetAcked(bits) is declared void, while the
// original returns the last call's result. Body not written.

// 0x8134f4 _ZN9NetStruct12LoadInternalEbR12NetBitStreami
// partial: the presence bit (NetBitStream::ReadBit) and the per-member loop are clear, but
// (1) the mask needs CMatching::MemberBit; (2) on the non-acknowledged path the original calls the
// member's virtual slot +0x00 with (member, stream), and that slot is not identified in
// NetStructMember (the declared slots are Write +0x04, Read +0x08, PostLoad +0x0c, GetSizeBits +0x10);
// (3) the return value 0 / 1 / 2 is not reproduced. Body not written.

// 0x8135e4 _ZN9NetStruct5EraseER12NetBitStream
int NetStruct::Erase(NetBitStream& stream) {
    // Forwards to LoadInternal(false, stream, -1). The forwarding is complete; LoadInternal is partial.
    return LoadInternal(false, stream, -1);
}

// 0x8135f4 _ZN9NetStruct4LoadER12NetBitStreamij
// partial: stores a4 at +0x128, runs LoadInternal(true, stream, memberId), then if the post-load flag
// is set calls PostLoad(a4) (vt +0x0c) on each member. NetStructMember::PostLoad is not declared in
// net_struct.hpp at the moment (it was there in an earlier revision), and LoadInternal is partial too.
// Body not written.

// 0x81366c _ZN9NetStruct10SetEnabledEb
// partial: enabling needs CMatching::MemberBit(CMatching::Get()->LocalMemberId()) for the mask
// (vt +0x84 and vt +0x6c), and stores that mask in each member's change-mask field, which is protected
// in NetStructMember and not reachable from here. Body not written.

// 0x813704 _ZN9NetStruct22AreChangesAcknowledgedEv
// Returns true when the struct is disabled or empty. Otherwise true only when every member reports
// AreChangesAcknowledged, stopping at the first that does not.
bool NetStruct::AreChangesAcknowledged() {
    if (!enabled_ || member_count_ <= 0) {
        return true;
    }
    for (int i = 0; i < member_count_; ++i) {
        if (!members_[i]->AreChangesAcknowledged()) {
            return false;
        }
    }
    return true;
}

// 0x81375c _ZN9NetStruct13HasDataToSendEi
// partial: needs CMatching::MemberBit (vt +0x84) and CMatching::GetRoomMask. The disabled path
// returns the sign bit of memberId (a2 >> 31); that part is known. Body not written.

// 0x8137ec _ZN9NetStruct16GetChangedBitMapEv
// Bit i is set when member i reports IsChanged. Every member is queried (IsChanged clears the flag).
// The port guards the shift for i >= 32, since ARM LSL by a register is 0 there.
std::uint32_t NetStruct::GetChangedBitMap() {
    if (member_count_ <= 0) {
        return 0;
    }
    std::uint32_t bits = 0;
    for (int i = 0; i < member_count_; ++i) {
        if (members_[i]->IsChanged() && i < 32) {
            bits |= 1u << i;
        }
    }
    return bits;
}

// 0x81383c _ZN9NetStruct9IsChangedEv
// True when any member reports IsChanged. The scan stops at the first changed member, as in the
// original, so the later members' IsChanged flags are left as they are.
bool NetStruct::IsChanged() {
    for (int i = 0; i < member_count_; ++i) {
        if (members_[i]->IsChanged()) {
            return true;
        }
    }
    return false;
}

// 0x81388c _ZN9NetStructC1Ev
// 0x8138f4 _ZN9NetStructC2Ev
// partial: the original sets the vptr, zeroes the member count and flags and the members, and
// initialises the std::map header at +0x10c (the header points at itself). The map type is not
// declared in the port. The fields that exist are initialised by the default member initialisers
// in net_struct.hpp, so the body is empty here.
NetStruct::NetStruct() {}

// 0x81395c _ZN9NetStructD1Ev
// partial: the original erases the packet history map when its node count is non-zero, then resets
// the map header. The map is not declared in the port. Body not written.
