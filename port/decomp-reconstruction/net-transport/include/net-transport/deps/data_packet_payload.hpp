// Stand-ins for the DataPacket virtuals that DataPacketLobby calls on another packet
// (its source constructor and addPacket). The DataPacket header does not declare them
// yet, and that class is owned by another slice, so they return empty values here.
// Replace them with the virtual calls once DataPacket declares the slots.
#pragma once

#include <cstddef>
#include <cstdint>

#include "net-transport/data_packet.hpp"

// vtable +0x74 (116): data type of the source packet
inline int PayloadDataType(DataPacket& packet) {
    (void)packet;
    return 0;
}

// vtable +0x60 (96): body length in bytes
inline std::size_t PayloadLength(DataPacket& packet) {
    (void)packet;
    return 0;
}

// vtable +0x64 (100): body bytes
inline const std::uint8_t* PayloadBody(DataPacket& packet) {
    (void)packet;
    return nullptr;
}
