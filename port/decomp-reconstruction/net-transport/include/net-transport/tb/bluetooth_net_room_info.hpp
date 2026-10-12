// CBluetoothNetRoomInfo: Bluetooth room-info NetStruct from libDungeonHunter2.so.
//
// Layout (32-bit offsets, from the constructor and destructor pseudocode and the vtable listing):
//   +0x000 vptr (CBluetoothNetRoomInfo vtable, 80 bytes; slots +0x00 and +0x04 are the destructors,
//          +0x08..+0x44 are the NetStruct virtuals)
//   +0x000..+0x12f  base NetStruct (vtable off_9924D8 while the base dtor runs). NetStruct is not
//          reconstructed in the port yet, so this range is not declared here.
//   +0x130  NetStructString<256u> member (the name string; built from an empty std::string)
//   +0x168  NetStructMember-derived member (vptr off_963618, then off_963648, then off_9635E8)
//   +0x16c  int, initial value 32
//   +0x170  u64, initial value 0
//   +0x178  int, initial value -1
//   +0x17c  int, initial value -1
//   +0x180  int, initial value 0
//   +0x184  byte, initial value 0
//   +0x188  int, reset to 0; when it was non-zero, the member at +0x168 gets SetChanged
//
// Layout request (for the integrator, owner header not yet in the port): the base NetStruct
// (0x130 bytes) and the types of the members at +0x130 and +0x168 must be declared before this
// class can be given its own fields. The class is therefore declared with its methods only.
#pragma once

class CBluetoothNetRoomInfo {
public:
    CBluetoothNetRoomInfo();          // 0x800d7c  partial
    ~CBluetoothNetRoomInfo();         // 0x801a0c  partial
};
