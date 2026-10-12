// Stand-in for the transport Connection that GLXPlayerMPBase holds at +0x54
// (net-transport owns the real class). The original calls
// Connection::isConnected(connection), then virtual slot 5 on it, and writes a
// time stamp at +0x08. The lobby's ConnectionLobby (connection_lobby.hpp) is a
// different object and keeps its own stand-in.
#pragma once

#include <cstdint>

class ConnectionTransport {
public:
    virtual ~ConnectionTransport() = default;
    virtual void VirtualSlot5() {}   // slot 5 (+0x14 of the vtable), name not recovered
    std::uint32_t timeMs = 0;        // +0x08, set from XP_API_GET_TIME by mpSendDisconnect
};

// Connection::isConnected(connection). The transport is not available here.
inline bool ConnectionIsConnected(const ConnectionTransport* /*connection*/) { return false; }
