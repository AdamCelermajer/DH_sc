// CBluetoothNetRoomInfo: constructor and destructor. Batch r1n-net-transport-pack0-34ed2e-0 (redo pass).
//
// The base NetStruct, the NetStructString<256u> member and the NetStructInt<32u> member are real now, so
// the members are declared in the class (tb/bluetooth_net_room_info.hpp) and the field stores of the
// original are the member initialisers. The layout comments are in that header.

#include "net-transport/tb/bluetooth_net_room_info.hpp"

// 0x800d7c CBluetoothNetRoomInfo::CBluetoothNetRoomInfo()
// Partial: verified in the listing - the constructor loads the value word at +0x188 (LDR R2,[R4,#0x188])
// before storing the member's zero there, and branches on it: when the loaded word was non-zero it
// stores 0 at +0x188 and calls NetStructMember::SetChanged on the member at +0x168. Nothing in the
// listing initialises +0x188 before that load, so in the original the branch (and therefore the
// SetChanged call) depends on whatever was in memory there. The port's value_ starts at 0
// (C++ value-initialisation), so the port always stores 0 without SetChanged.
// The base NetStruct construction, the non-virtual NetStructInt<32u> field stores (+0x16c 32, +0x170
// zero, +0x178/-0x17c -1, +0x180 0, +0x184 0), the vptr stores and the name_-then-value_ DeclareMember
// order all match the original.
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
