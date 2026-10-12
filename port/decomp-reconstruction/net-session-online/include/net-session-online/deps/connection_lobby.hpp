// Stand-in for ConnectionLobby, the lobby packet sender. The send* methods are
// called by GLXPlayerMPLobby. Their packet encoding belongs to net-transport,
// so they are inline no-ops here. Argument types follow the mangled names of the
// original methods.
#pragma once

#include <cstdint>

class CLobbyParameterAndQuery;

class ConnectionLobby {
public:
    std::uint32_t requestTimeMs = 0;  // +0x2050, written by GLXPlayerMPLobby after each request

    void sendGetPlayerCounter() {}
    void sendGetLobbyForFriend(std::int32_t, std::uint8_t, std::uint8_t, char*, std::int32_t) {}
    void sendGetLobbyForFriendWithGameParam(std::int32_t, std::uint8_t, std::uint8_t, char*,
                                            std::int32_t, CLobbyParameterAndQuery*) {}
    void sendGetLobbyForNameWithGameParam(char*) {}
    void sendKickOutPackage(char*) {}
    void sendCancelAutoMatchPackage() {}
    void sendAutoMatchPackage(char*, std::int32_t, char*, std::int32_t, std::int16_t, std::int16_t) {}
    void sendGetLobbyForName(char*) {}
    void sendGetLobbyInfo(std::int32_t) {}
    void sendSetUserParameter(char*, std::int16_t) {}
    void sendRejoinLobbyPackage(std::int32_t) {}
    void sendSetPlayerStatusPackage(std::uint8_t) {}
    void sendLunchLobbyGamePackage() {}
    void sendLeaveLobbyPackage() {}
    void sendGetLobbyListPackage(std::int32_t, std::int32_t, std::int32_t, std::uint8_t,
                                 CLobbyParameterAndQuery*, std::uint8_t) {}
    void sendGetLobbyListPackageWithGameParam(std::int32_t, std::int32_t, std::int32_t, std::uint8_t,
                                              CLobbyParameterAndQuery*, std::uint8_t) {}
    void sendSetGameParameter(char*, std::int16_t, CLobbyParameterAndQuery*) {}
    void sendJoinLobbyPackage(std::int32_t, std::uint8_t, char*, std::int32_t) {}
    void sendLobbyLoginPackage(long, char*, std::int16_t, std::uint8_t) {}
    void sendLobbyLoginPackageWithGameCenter(long, char*, std::int16_t, std::uint8_t, std::uint8_t,
                                             char*, char*) {}
};

// Connection::isConnected() and Connection::disconnect(), the transport
// connection that backs the lobby. Stand-ins; the real transport is net-transport.
struct Connection {
    static bool isConnected() { return false; }
    static void disconnect(ConnectionLobby*) {}
};
