// Stand-ins for the DataPacket virtuals (getDataType, getMessageLen, getMessageBody). DataPacket
// now declares those slots (data_packet.hpp), and DataPacketLobby, DefaultDataPacket and their
// tests call them directly. connection_base.cpp still includes these stand-ins, which return
// empty values; it should call the virtuals too, and then this header can be removed.
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
