// Stand-in for Connection, the base class of ConnectionLobby. Owned by another slice; declared
// here so ConnectionLobby compiles on its own. Only the members and calls ConnectionLobby makes
// are declared. Virtual slots are numbered by their vtable offsets in the original.
#pragma once

#include <cstdint>

#include "net-transport/data_packet.hpp"

// The socket object that ConnectionLobby reads and writes. Slots +0x40, +0x44, +0x48, +0x4c.
class ConnectionSocket {
public:
    virtual ~ConnectionSocket() = default;
    virtual int Select(int mode) = 0;                    // +0x40: 0 = read ready, 1 = write ready; <0 on error
    virtual int Send(const void* data, int length) = 0;  // +0x44: bytes sent, <0 on error
    virtual int Recv(void* data, int length) = 0;        // +0x48: bytes received, 0 when closed
    virtual bool HasPendingData() const = 0;             // +0x4c: non-zero when data can be read
};

class Connection {
public:
    Connection(char* host, int port) {
        (void)host;
        (void)port;
    }
    virtual ~Connection() = default;

    // Virtual slots +0x08..+0x14 of the Connection vtable. ConnectionLobby overrides them.
    virtual void sendData() {}                               // +0x08
    virtual void receiveDataLen() {}                         // +0x0c
    virtual int sendEstablishConnectionPackage() { return 0; }  // +0x10
    virtual int sendFinishConnectionPackage() { return 0; }     // +0x14

    // Queue operations. Stand-ins: nothing is queued.
    int addOutgoingPacket(DataPacket* packet) {
        (void)packet;
        return 0;
    }
    int addIncomingPacket(DataPacket* packet) {
        (void)packet;
        return 0;
    }
    void cleanRetryData() {}
    bool isConnected() const { return true; }
    void disconnect() {}
};
