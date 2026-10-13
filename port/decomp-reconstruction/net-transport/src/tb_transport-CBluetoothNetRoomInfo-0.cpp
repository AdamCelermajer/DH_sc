// CBluetoothNetRoomInfo: constructor and destructor. Batch r1n-net-transport-pack0-34ed2e-0 (redo pass).
//
// The base NetStruct, the NetStructString<256u> member and the NetStructInt<32u> member are real now, so
// the members are declared in the class (tb/bluetooth_net_room_info.hpp) and the field stores of the
// original are the member initialisers. The layout comments are in that header.

#include "net-transport/tb/bluetooth_net_room_info.hpp"

// 0x800d7c CBluetoothNetRoomInfo::CBluetoothNetRoomInfo()
// Partial: the original reads the value word at +0x188 before it stores 0 there, and calls
// NetStructMember::SetChanged on the member when that word was non-zero. Nothing in the original
// initialises +0x188 before that read, so the branch depends on memory the constructor does not set.
// The port's member starts at 0 (C++ value-initialisation), so the SetChanged branch is never taken.
// The member declarations (name_, then value_) and DeclareMember calls follow the original order.
CBluetoothNetRoomInfo::CBluetoothNetRoomInfo()
    : NetStruct(), name_(std::string()) {
    if (value_.GetValue() != 0) {
        value_.SetValue(0);
        value_.SetChanged();
    }
    DeclareMember(&name_);
    DeclareMember(&value_);
}

// 0x801a0c CBluetoothNetRoomInfo::~CBluetoothNetRoomInfo()
// Reconstructed. The original sets the vptrs of the members and the base (so the inlined destructors
// run in the right order) and then frees the name string and the NetStruct packet-history map. Those
// are the member and base destructors here, so the body is defaulted.
CBluetoothNetRoomInfo::~CBluetoothNetRoomInfo() = default;
