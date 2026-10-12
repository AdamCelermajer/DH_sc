// Stand-in for CMatching, the matching service that owns the local member ids.
// Owned by another category; declared here so the connection code compiles on its own.
// Virtual slots are named by their vtable offset because the names are not recovered
// for the derived class the connection code calls through.
#pragma once

#include <cstdint>

class CMatching {
public:
    virtual ~CMatching() = default;

    // Returns the matching object (CMatching::Get, _ZN9CMatching3GetEv). Stand-in: a static default.
    static CMatching* Get() {
        static CMatching instance;
        return &instance;
    }

    // _ZN9CMatching8IsServerEv. Stand-in: this device is never the server.
    bool IsServer() const { return false; }

    // Vtable +0x6c: the local id written into the connect handshake.
    virtual std::int32_t VirtualId6c() const { return 0; }
    // Vtable +0x74: compared with CConnection::matching_key_ by IsToServer.
    virtual std::int32_t VirtualId74() const { return 0; }
};
