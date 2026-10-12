// Batch e2-net-transport-pack0-e3f33b-0: CNetPlayerInfo (constructor, Create, Delete) and the
// NetStructInt<6u> members (destructors, TestValue, Read, Write) from libDungeonHunter2.so.
//
// The NetStructInt<6u> members are explicit specializations: the generic bodies live in
// src/tb_transport-NetStructInt-0.cpp, which instantiates only 8, 16, 17 and 32. The <6> forms are
// declared in include/net-transport/tb/net_struct_int.hpp and defined here, with the same logic.

#include <cstdint>

#include "net-transport/tb/cnet_player_info.hpp"
#include "net-transport/tb/net_struct_int.hpp"

namespace {

// abs32 as the original computes it. INT32_MIN stays negative (the negation wraps).
std::int32_t Abs32(std::int32_t value) {
    return value < 0 ? static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(value)) : value;
}

}  // namespace

// ---------------------------------------------------------------------------------------------
// CNetPlayerInfo

// 0x80fc28 CNetPlayerInfo::CNetPlayerInfo()   [partial]
// The original builds the NetStruct base (NetStruct::NetStruct), seven NetStructMember blocks at
// +0x130, +0x158, +0x180, +0x1a8, +0x1d0, +0x1f8 and +0x220, a NetStructString<32> at +0x248,
// declares the eight members with NetStruct::DeclareMember, and ends with Reset(). None of that
// is in CNetPlayerInfo yet (see the layout request in cnet_player_info.hpp), so this body writes
// the fields the class declares, with the values the original leaves in them:
//   - the member id block (+0x158) has its value word zeroed, so member_id_ ends at 0;
//   - the connection, unk and state blocks are zeroed by their constructors, and Reset() then
//     writes -1, -1 and 0 (Reset's writes are repeated here in the same order);
//   - applied_state_ (+0x280) is set to 0.
// Not ported: the NetStruct base construction and vtable, the block vtables and sizes, the
// NetStructMember::SetChanged calls, the NetStructString<32> member, and the DeclareMember calls.
CNetPlayerInfo::CNetPlayerInfo() {
    member_id_ = 0;
    conn_index_ = -1;
    unk_1c8 = -1;
    state_ = 0;
    applied_state_ = 0;
}

// 0x8103d4 CNetPlayerInfo::Create()   [partial]
// The original calls operator new(0x288) and then the constructor, and returns the object. The
// port allocates sizeof(CNetPlayerInfo), which differs from 0x288 until the NetStruct base and the
// member blocks are declared. The Hex-Rays body takes a pointer argument it never uses, so the
// port declares Create as a static member.
CNetPlayerInfo* CNetPlayerInfo::Create() {
    return new CNetPlayerInfo();
}

// 0x81009c CNetPlayerInfo::Delete(CNetPlayerInfo*)   [partial]
// A null pointer is returned unchanged. Otherwise the original calls the object's virtual slot +4
// (the deleting destructor) with (this, a2). This port has no virtual destructor, so `delete`
// runs the same destructor. The second argument (a2) and the return value are not kept, because
// no caller in the tree uses them.
void CNetPlayerInfo::Delete(CNetPlayerInfo* p) {
    delete p;
}

// ---------------------------------------------------------------------------------------------
// NetStructInt<6u>

// 0x8100b8 NetStructInt<6u>::~NetStructInt() (D1)
// The complete destructor has an empty body. It is defaulted in net_struct_int.hpp for every Bits
// value, so no code is written here.

// 0x8108d8 NetStructInt<6u>::~NetStructInt() (D0, deleting destructor)
// Skipped: the original sets the vtable to off_963648, then frees the object with CustomFree. C++
// `delete` on the object covers this, as it does for the other NetStructInt forms.

// 0x8103b8 NetStructInt<6u>::TestValue(int const&)
// Accepts magnitudes up to 0x1f, which is the largest value 5 magnitude bits can hold.
template <>
bool NetStructInt<6u>::TestValue(const std::int32_t& value) const {
    return Abs32(value) <= 0x1F;
}

// 0x81090c NetStructInt<6u>::Read(NetBitStream&)
// Reads the 5-bit magnitude, then one sign bit (1 = negative). The signed value goes to the
// virtual SetValue (vtable +0x1c, the original's +28 call).
template <>
void NetStructInt<6u>::Read(NetBitStream& stream) {
    std::int32_t magnitude = static_cast<std::int32_t>(stream.ReadU32(5));
    std::int32_t sign = stream.ReadByte(1) ? -1 : 1;
    SetValue(magnitude * sign);
}

// 0x81096c NetStructInt<6u>::Write(NetBitStream&)
// Writes abs(value) in 5 bits, then the sign bit (1 when negative). The original's return value
// (the WriteByte result) is dropped: NetBitStream::WriteByte and NetStructMember::Write are void
// in this port.
template <>
void NetStructInt<6u>::Write(NetBitStream& stream) const {
    std::int32_t value = value_;
    stream.WriteU32(static_cast<std::uint32_t>(Abs32(value)), 5);
    stream.WriteByte(value < 0 ? 1 : 0, 1);
}
