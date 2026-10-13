// Tests for the CBluetoothReliable functions of batch r1n-net-transport-CBluetoothReliable-350a35-0.
// Covers the state changes that run without the engine: Connect's initialized check, Listen's
// forward to Open, Create's initialize-then-open sequence, and InitializeTransport's partial stub.
#include "net-transport/bluetooth_transport.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // Connect needs an initialized transport (-1 otherwise), then returns 0 for a new peer and
    // for a peer that is already stored.
    CBluetoothReliable transport;
    CNetworkId peer;
    peer.Reset();
    CHECK(transport.Connect(peer) == -1);
    CHECK(transport.Initialize() == 0);
    CHECK(transport.Connect(peer) == 0);
    CHECK(transport.Connect(peer) == 0);
    CHECK(transport.Disconnect() == 0);
    CHECK(transport.Connect(peer) == -1);

    // Listen forwards to Open with channel 1 and returns Open's result (0).
    CBluetoothReliable listener;
    CHECK(listener.Listen("address", "name") == 0);
    CHECK(listener.Open("address", "name", 1) == 0);

    // Create builds a transport that is already initialized (its Initialize call ran first).
    CBluetoothReliable* created = CBluetoothReliable::Create("Default Bluetooth Session", "host", 1);
    CHECK(created != nullptr);
    if (created) {
        CHECK(created->GetType() == 4);
        CHECK(created->Initialize() == -1);
        CHECK(created->Disconnect() == 0);
        CHECK(created->Disconnect() == -1);
        delete created;
    }

    // The id getters do not set presence bit 3 in this port (partial: see bluetooth_transport.cpp).
    CNetworkId local = transport.GetLocalNetworkId();
    CNetworkId remote = transport.GetPeerNetworkId();
    CHECK(local.Flags() == 0);
    CHECK(remote.Flags() == 0);

    // InitializeTransport is partial: it returns 0 and does not create the Bluetooth session.
    CHECK(CBluetoothReliable::InitializeTransport() == 0);

    std::printf("%s\n", failures ? "r1n-net-transport-CBluetoothReliable-350a35-0: FAILED"
                                 : "r1n-net-transport-CBluetoothReliable-350a35-0: ok");
    return failures ? 1 : 0;
}
