// CMatchingBluetooth: the Bluetooth room layer. Only the static transport-type lookup is
// reconstructed so far. The object layout (the room info, the peer map and the device
// name string at the end of the 43 KB object), the constructors, Initialize, Terminate,
// ProcessEvents and Update are not recovered yet; see NOTES.md.
#pragma once

#include <cstdint>
#include <string>

#include "net-session-online/c_room_attributes.hpp"

class CMatchingBluetooth {
public:
    // 0x801b60 _ZN18CMatchingBluetooth16GetTransportTypeE14tPROTOCOL_TYPE: table {3, 4, 4}
    // indexed by the protocol type; any type above 2 gives 0. The object is not read.
    static std::int32_t GetTransportType(std::uint32_t protocolType);

    // 0x803cbc CMatchingBluetooth::GenerateBluetoothDeviceName(tMatchingPeer) (partial, see the cpp).
    // Deviation: the original takes the peer by value and its `this` is unused. The port takes the
    // two peer fields the body reads (member id and room attributes), because the tMatchingPeer
    // stand-in declares a second CRoomAttributes that clashes with c_room_attributes.hpp.
    static std::string GenerateBluetoothDeviceName(std::int32_t memberId, const CRoomAttributes& attributes);
};
