#include "net-session-online/tb/cmatching_local_room_info_net_struct.hpp"

#include "net-session-online/deps/platform_stand_ins.hpp"

// 0x806964 _ZN14CMatchingLocal17RoomInfoNetStructC1Ev
// Partial. The original reads each member's value before its stores. A nonzero value is set to 0
// and NetStructMember::SetChanged is called. The port value-initialises every member, so the reads
// are always 0 and the SetChanged branches cannot run. The leftover values in the original are not
// known. The packet history at +0x10c is default-constructed empty here. NetStruct::NetStruct,
// NetStructMember::SetChanged and DeclareMember are stand-ins (no-ops), see deps/net_struct_stub.hpp.
CMatchingLocal::RoomInfoNetStruct::RoomInfoNetStruct()
    : NetStructBaseStub() {
    // Member 1: NetStructBool at +0x130.
    const bool prior130 = unk130_.value;     // +0x14d, read before the stores
    unk130_.changeStamp = 0;                 // +0x138
    unk130_.sizeBits = 1;                    // +0x134
    unk130_.ackMask = 0xffffffffu;           // +0x140
    unk130_.sentMask = 0xffffffffu;          // +0x144
    unk130_.changeMask = 0;                  // +0x148
    unk130_.changed = false;                 // +0x14c
    if (prior130) {
        unk130_.value = false;               // +0x14d
        unk130_.SetChanged();
    }

    // Member 2: NetStructBool at +0x150.
    const bool prior150 = unk150_.value;     // +0x16d, read before the stores
    unk150_.changeStamp = 0;                 // +0x158
    unk150_.sizeBits = 1;                    // +0x154
    unk150_.ackMask = 0xffffffffu;           // +0x160
    unk150_.sentMask = 0xffffffffu;          // +0x164
    unk150_.changeMask = 0;                  // +0x168
    unk150_.changed = false;                 // +0x16c
    if (prior150) {
        unk150_.value = false;               // +0x16d
        unk150_.SetChanged();
    }

    // Member 3: NetStructInt<32> at +0x170.
    const std::int32_t prior170 = unk170_.value;  // +0x190, read before the stores
    unk170_.changeStamp = 0;                 // +0x178
    unk170_.sizeBits = 32;                   // +0x174
    unk170_.ackMask = 0xffffffffu;           // +0x180
    unk170_.sentMask = 0xffffffffu;          // +0x184
    unk170_.changeMask = 0;                  // +0x188
    unk170_.changed = false;                 // +0x18c
    if (prior170 != 0) {
        unk170_.value = 0;                   // +0x190
        unk170_.SetChanged();
    }

    // Member 4: NetStructInt<32> at +0x198.
    const std::int32_t prior198 = unk198_.value;  // +0x1b8, read before the stores
    unk198_.changeStamp = 0;                 // +0x1a0
    unk198_.sizeBits = 32;                   // +0x19c
    unk198_.ackMask = 0xffffffffu;           // +0x1a8
    unk198_.sentMask = 0xffffffffu;          // +0x1ac
    unk198_.changeMask = 0;                  // +0x1b0
    unk198_.changed = false;                 // +0x1b4
    if (prior198 != 0) {
        unk198_.value = 0;                   // +0x1b8
        unk198_.SetChanged();
    }

    DeclareMember(&unk130_);
    DeclareMember(&unk150_);
    DeclareMember(&unk170_);
    DeclareMember(&unk198_);
}

// 0x8061bc _ZN14CMatchingLocal17RoomInfoNetStructD1Ev
// Reconstructed. The original erases the packet history tree when it is not empty, then resets the
// header, which is std::map::clear. The element type is the stand-in NetStructBaseStub::tPacketHistory
// (empty). The real NetStruct::tPacketHistory is two 64-bit words with no destructor, so clear() does
// the same work. The NetStructMember vptr stores and the NetStruct vptr store of the original are the
// compiler's job. The NetStruct base layout (+0x04..+0x10b) is not modelled, and the destructor does not touch it.
CMatchingLocal::RoomInfoNetStruct::~RoomInfoNetStruct() {
    if (!packetHistory_.empty()) {
        packetHistory_.clear();
    }
}

// 0x80623c _ZN14CMatchingLocal17RoomInfoNetStructD0Ev
// Reconstructed, as D1 above: the deleting destructor is the D1 body, then CustomFree(this).
void CMatchingLocal::RoomInfoNetStruct::DeletingDestructor() {
    this->~RoomInfoNetStruct();
    CustomFree(this);
}
