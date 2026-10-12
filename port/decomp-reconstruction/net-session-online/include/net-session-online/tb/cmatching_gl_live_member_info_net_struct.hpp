// CMatchingGLLive::MemberInfoNetStruct: a replicated struct element of CMatchingGLLive's member
// table. Bodies are in src/tb_session-online-CMatchingGLLive_MemberInfoNetStruct-0.cpp.
//
// Layout (original 32-bit offsets, kept as comments; the total size is not recovered):
//   +0x00            vptr (not modelled; the port has no virtual dispatch here)
//   +0x04..+0x10b    NetStruct base fields not recovered
//   +0x10c..+0x11f   packet history: std::map<int, std::map<int, NetStruct::tPacketHistory>>
//                    (the rb-tree header, root, leftmost, rightmost and node count)
//   +0x130..         NetStructString<16> member (value std::string at +0x150)
//
// Not modelled: the destructor also destroys a std::string at +0x38. Its owner and its
// constructor are not identified, and the NetStruct base occupies that range, so the port
// declares no field there (see the ledger note on the destructor).
#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "net-session-online/c_matching_gl_live.hpp"
#include "net-session-online/deps/net_struct_stub.hpp"

// Stand-in for NetStructString<16> (net-transport, tb/net_struct_string.hpp). Only the value
// is modelled, at +0x20 of the member, as in NetStructMemberStub. Replace with the real
// NetStructString<16> when net-transport is linked into this translation unit.
class NetStructStringStub16 : public NetStructMemberStub {
public:
    NetStructStringStub16() = default;
    explicit NetStructStringStub16(const std::string& initial) : value(initial) {}

    std::string value;  // +0x20 (absolute +0x150 in MemberInfoNetStruct)
};

class CMatchingGLLive::MemberInfoNetStruct : public NetStructBaseStub {
public:
    // 0x81e680 _ZN15CMatchingGLLive19MemberInfoNetStructC2Ev (partial, see the .cpp)
    MemberInfoNetStruct();
    // 0x81d1f4 _ZN15CMatchingGLLive19MemberInfoNetStructD1Ev (partial, see the .cpp)
    ~MemberInfoNetStruct();

private:
    // +0x10c: packet history (std::map header in the original).
    std::map<std::int32_t, std::map<std::int32_t, NetStructBaseStub::tPacketHistory>> packetHistory_;
    // +0x130: NetStructString<16> member. Its value is at +0x150.
    NetStructStringStub16 member_;
};
