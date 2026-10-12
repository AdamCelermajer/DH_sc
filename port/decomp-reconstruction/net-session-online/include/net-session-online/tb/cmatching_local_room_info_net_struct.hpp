// CMatchingLocal::RoomInfoNetStruct: the replicated room record of CMatchingLocal. It declares four
// members: two NetStructBool and two NetStructInt<32>. Bodies are in
// src/tb_session-online-CMatchingLocal_RoomInfoNetStruct-0.cpp.
//
// Layout (original 32-bit offsets, kept as comments; the port's NetStruct base is a field-less stand-in,
// so the members do not sit at these offsets in memory):
//   +0x00            vptr (not modelled; the port has no virtual dispatch here)
//   +0x04..+0x10b    NetStruct base fields not recovered
//   +0x10c..+0x11f   packet history: std::map<int, std::map<int, NetStruct::tPacketHistory>>
//   +0x130           NetStructBool member (32 bytes). Value at +0x14d.
//   +0x150           NetStructBool member (32 bytes). Value at +0x16d.
//   +0x170           NetStructInt<32> member (40 bytes). Value at +0x190.
//   +0x198           NetStructInt<32> member (40 bytes). Value at +0x1b8.
//   The members end at +0x1c0. The object size is not recovered.
//
// Member names are not identified: the constructor is the only evidence for them, so they are
// named by offset (unk130_ and so on).
#pragma once

#include <cstdint>
#include <map>

#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/deps/net_struct_stub.hpp"

class CMatchingLocal::RoomInfoNetStruct : public NetStructBaseStub {
public:
    // 0x806964 _ZN14CMatchingLocal17RoomInfoNetStructC1Ev (partial, see the .cpp)
    RoomInfoNetStruct();
    // 0x8061bc _ZN14CMatchingLocal17RoomInfoNetStructD1Ev (partial, see the .cpp)
    ~RoomInfoNetStruct();
    // 0x80623c _ZN14CMatchingLocal17RoomInfoNetStructD0Ev: the deleting destructor.
    void DeletingDestructor();

private:
    // +0x10c: packet history (std::map header in the original).
    std::map<std::int32_t, std::map<std::int32_t, NetStructBaseStub::tPacketHistory>> packetHistory_;
    NetStructBoolStub unk130_;     // +0x130 NetStructBool, value at +0x14d
    NetStructBoolStub unk150_;     // +0x150 NetStructBool, value at +0x16d
    NetStructInt32Stub unk170_;    // +0x170 NetStructInt<32>, value at +0x190
    NetStructInt32Stub unk198_;    // +0x198 NetStructInt<32>, value at +0x1b8
};
