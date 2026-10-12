// CMatching::MemberInfoNetStruct: one element of CMatching's replicated member array
// (NetStructArray<MemberInfoNetStruct,32>, 432 bytes per element). Bodies are in
// src/tb_session-online-CMatching_MemberInfoNetStruct-0.cpp.
//
// Naming: the original class is nested in CMatching (symbols _ZN9CMatching19MemberInfoNetStruct...).
// The CMatching stand-in in deps/matching.hpp has no nested declarations and this batch does not
// edit it, so the class is declared at file scope as CMatchingMemberInfoNetStruct. The integrator
// moves it into CMatching when that stand-in is replaced.
//
// Layout (original 32-bit offsets, kept as comments; element base = 0 here):
//   +0x000..+0x10b  NetStruct base (stub, not modelled)
//   +0x10c..+0x11f  packet history: std::map header (rb-tree header, root, leftmost, rightmost, count)
//   +0x130..+0x157  member id NetStructInt<32> (40 bytes). Value at +0x150.
//   +0x158..+0x17f  team id NetStructInt<8> (40 bytes). Value at +0x178. The port uses the 32-bit
//                   stub with sizeBits set to 8 by the constructor.
//   +0x180..+0x1a7  room member attribute NetStructByteArray<64> (40 bytes). Data pointer at +0x1a0,
//                   size (int) at +0x1a4.
//   +0x1a8          previous member id (int). -1 when constructed.
//   +0x1a8..+0x1b0  end of the element. Nothing else is identified there.
#pragma once

#include <cstdint>
#include <map>

#include "net-session-online/deps/net_struct_stub.hpp"

// NetStructByteArray<64> stand-in. Layout relative to the member object: data pointer at +0x20,
// size at +0x24 (so +0x1a0 and +0x1a4 in the element). The net-transport template is not used
// here because it would redefine CustomFree, which platform_stand_ins.hpp already declares.
class NetStructByteArray64Stub : public NetStructMemberStub {
public:
    void* data = nullptr;          // +0x20 (element +0x1a0)
    std::int32_t size = 0;         // +0x24 (element +0x1a4)
};

class CMatchingMemberInfoNetStruct : public NetStructBaseStub {
public:
    // 0x7ffd04 _ZN9CMatching19MemberInfoNetStructC1Ev. Partial, see the .cpp.
    CMatchingMemberInfoNetStruct();
    // 0x7ffc28 _ZN9CMatching19MemberInfoNetStructD1Ev.
    ~CMatchingMemberInfoNetStruct();
    // 0x7ffce8 _ZN9CMatching19MemberInfoNetStructD0Ev: the deleting destructor (D1, then CustomFree).
    void DeletingDestructor();

    // Element fields. Public so CMatching's member code can read the cached ids.
    NetStructInt32Stub memberId_;               // +0x130, value at +0x150
    NetStructInt32Stub teamId_;                 // +0x158, value at +0x178
    NetStructByteArray64Stub roomAttribute_;    // +0x180, data at +0x1a0, size at +0x1a4
    std::int32_t previousMemberId_ = -1;        // +0x1a8

private:
    // +0x10c: packet history (std::map header in the original).
    std::map<std::int32_t, std::map<std::int32_t, NetStructBaseStub::tPacketHistory>> packetHistory_;
};
