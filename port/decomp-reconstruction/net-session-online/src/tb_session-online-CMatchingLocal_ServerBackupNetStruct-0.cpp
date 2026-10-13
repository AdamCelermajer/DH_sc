#include "net-session-online/tb/cmatching_local_server_backup_net_struct.hpp"

#include "net-session-online/deps/platform_stand_ins.hpp"

// 0x806cac _ZN14CMatchingLocal21ServerBackupNetStructC1Ev
// Partial. The original reads the member value at +0x150 before any of its stores. If that
// value is nonzero, it sets the value to 0 and calls NetStructMember::SetChanged. The port
// value-initialises the member, so that read is always 0 and the SetChanged branch cannot run.
// The original's leftover contents at +0x150 are not known. The packet history at +0x10c is
// not initialised here in the original; the port default-constructs it empty.
// NetStructMember::SetChanged and DeclareMember are stand-ins (no-ops), see deps/net_struct_stub.hpp.
CMatchingLocal::ServerBackupNetStruct::ServerBackupNetStruct()
    : NetStructBaseStub() {
    const std::int32_t priorValue = member_.value;  // +0x150 read before the stores

    member_.changeStamp = 0;                 // +0x138 (qword)
    member_.sizeBits = 32;                   // +0x134
    member_.ackMask = 0xffffffffu;           // +0x140
    member_.sentMask = 0xffffffffu;          // +0x144
    member_.changeMask = 0;                  // +0x148
    member_.changed = false;                 // +0x14c

    if (priorValue != 0) {
        member_.value = 0;                   // +0x150
        member_.SetChanged();
    }

    DeclareMember(&member_);
}

// 0x8062d0 _ZN14CMatchingLocal21ServerBackupNetStructD1Ev
// Reconstructed. The original erases the packet history tree when it is not empty and resets the
// header, which is std::map::clear. The element type here is the stand-in NetStructBaseStub::tPacketHistory
// (empty). The real NetStruct::tPacketHistory (net-transport net_struct.hpp) is two 64-bit words with
// no destructor, so clear() does the same work. The vptr stores are the compiler's job here. The
// NetStruct base has no destructor body in the original.
CMatchingLocal::ServerBackupNetStruct::~ServerBackupNetStruct() {
    if (!packetHistory_.empty()) {
        packetHistory_.clear();
    }
}

// 0x806140 _ZN14CMatchingLocal21ServerBackupNetStructD0Ev
// Reconstructed. Deleting destructor: the D1 body, then CustomFree(this).
void CMatchingLocal::ServerBackupNetStruct::DeletingDestructor() {
    this->~ServerBackupNetStruct();
    CustomFree(this);
}
