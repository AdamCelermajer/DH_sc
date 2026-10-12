// Batch e1-net-transport-CBluetoothNetRoomInfo-0: constructor and destructor of CBluetoothNetRoomInfo.
//
// Both functions are partial. The base NetStruct, the NetStructString<256u> member and the
// NetStructMember base-class notifications (SetChanged, DeclareMember) are not reconstructed in the
// port, so the statements that call them are documented here and not executed. The layout comments
// are in tb/bluetooth_net_room_info.hpp.

#include "net-transport/tb/bluetooth_net_room_info.hpp"

// 0x800d7c CBluetoothNetRoomInfo::CBluetoothNetRoomInfo()
// partial: the pseudocode, in order, is:
//   1. NetStruct::NetStruct(this)                          (base ctor, not in the port)
//   2. vptr = CBluetoothNetRoomInfo vtable (off_991E10)
//   3. NetStructString<256u>(this+0x130, std::string(""))  (not in the port)
//   4. destroy the temporary std::string (SSO buffer at +0x150 or CustomFree of heap data)
//   5. +0x170 = 0 (u64); +0x16c = 32; +0x178 = -1; +0x17c = -1; +0x180 = 0; +0x184 = 0
//   6. if the old value of +0x188 was non-zero: +0x188 = 0 and NetStructMember::SetChanged(this+0x168)
//   7. vptr of member +0x168 = off_9635E8
//   8. NetStruct::DeclareMember(this, this+0x130) and NetStruct::DeclareMember(this, this+0x168)
// Steps 1, 3, 6 and 8 are missing, so no body is written. The integrator needs the NetStruct layout.
CBluetoothNetRoomInfo::CBluetoothNetRoomInfo() {
}

// 0x801a0c CBluetoothNetRoomInfo::~CBluetoothNetRoomInfo()
// partial: the pseudocode, in order, is:
//   1. vptr of +0x130 member = off_963790 (NetStructString<256u> vptr)
//   2. vptr = off_991E10 (vtable of CBluetoothNetRoomInfo) then vptr = off_9924D8 (NetStruct vtable)
//   3. vptr of +0x168 member = off_963648
//   4. std::string at +0x164 is freed: if its buffer is not the inline SSO buffer at +0x150 and the
//      pointer is non-null, the capacity (end-start) > 0x80 selects CustomFree, otherwise the node
//      allocator deallocates it. Missing: NetStructString<256u> layout, so this is not written.
//   5. if the NetStruct member map at +0x10c (vector-of-maps slot) is non-empty: erase its rb-tree
//      (NetStruct::tPacketHistory map) and reset the header pointers. This is NetStruct's own member,
//      so it is not written.
// Steps 1, 3, 4 and 5 need the base and member types; the destructor is left empty.
CBluetoothNetRoomInfo::~CBluetoothNetRoomInfo() {
}
