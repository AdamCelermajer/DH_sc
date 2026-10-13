// CMatchingBluetooth: the Bluetooth room layer. Only the static transport-type lookup is
// reconstructed so far. The object layout (the room info, the peer map and the device
// name string at the end of the 43 KB object), the constructors, Initialize, Terminate,
// ProcessEvents and Update are not recovered yet; see NOTES.md.
#pragma once

#include <cstdint>
#include <string>

#include "net-session-online/c_room_attributes.hpp"
#include "net-session-online/t_matching_peer.hpp"  // tMatchingPeer (value type of the original's parameter)

class CMatchingBluetooth {
public:
    // 0x801b60 _ZN18CMatchingBluetooth16GetTransportTypeE14tPROTOCOL_TYPE: table {3, 4, 4}
    // indexed by the protocol type; any type above 2 gives 0. The object is not read.
    static std::int32_t GetTransportType(std::uint32_t protocolType);

    // 0x803cbc CMatchingBluetooth::GenerateBluetoothDeviceName(tMatchingPeer) (partial, see the cpp).
    // The original takes the peer by value (passed by reference in the ABI) and its `this` is unused,
    // so this is static and takes the peer by const reference.
    static std::string GenerateBluetoothDeviceName(const tMatchingPeer& peer);

    // 0x801c58 _ZN18CMatchingBluetooth18SetServerBroadcastEv: the transport manager starts the broadcast of the
    // transport that listens on the manager's own listen id (EnableBroadcast(GetListenNetworkId(0))). The object
    // is not read.
    void SetServerBroadcast();
};
