// CMatchingLocal::ServerBackupNetStruct: a replicated struct element of CMatchingLocal's server
// list. Bodies are in src/tb_session-online-CMatchingLocal_ServerBackupNetStruct-0.cpp.
//
// Layout (original 32-bit offsets, kept as comments; the object is 344 bytes = 0x158):
//   +0x00            vptr (not modelled; the port has no virtual dispatch here)
//   +0x04..+0x10b    NetStruct base and fields not recovered
//   +0x10c..+0x11f   packet history: std::map<int, std::map<int, NetStruct::tPacketHistory>>
//                    (the rb-tree header, root, leftmost, rightmost and node count)
//   +0x130..+0x157   NetStructInt<32> member (40 bytes). Value at +0x150.
//
// The constructor does not initialise the packet history header at +0x10c; the port
// default-constructs the map empty (see the ledger note).
#pragma once

#include <cstdint>
#include <map>

#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/deps/net_struct_stub.hpp"

class CMatchingLocal::ServerBackupNetStruct : public NetStructBaseStub {
public:
    // 0x806cac _ZN14CMatchingLocal21ServerBackupNetStructC1Ev (partial, see the .cpp)
    ServerBackupNetStruct();
    // 0x8062d0 _ZN14CMatchingLocal21ServerBackupNetStructD1Ev
    ~ServerBackupNetStruct();
    // 0x806140 _ZN14CMatchingLocal21ServerBackupNetStructD0Ev: the deleting destructor.
    void DeletingDestructor();

private:
    // +0x10c: packet history (std::map header in the original).
    std::map<std::int32_t, std::map<std::int32_t, NetStructBaseStub::tPacketHistory>> packetHistory_;
    // +0x130: NetStructInt<32> member (40 bytes). Its value is at +0x150.
    NetStructInt32Stub member_;
};
