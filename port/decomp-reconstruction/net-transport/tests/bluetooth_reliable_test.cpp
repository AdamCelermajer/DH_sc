#include "net-transport/bluetooth_transport.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // A new reliable transport is not initialized and is not listening.
    CBluetoothReliable transport;
    CHECK(transport.GetType() == 4);
    CHECK(!transport.IsListening());

    // Connect fails until the transport is initialized.
    CNetworkId peer;
    peer.Reset();
    CHECK(transport.Connect(peer) == -1);
    CHECK(transport.Initialize() == 0);
    CHECK(transport.Initialize() == -1);
    CHECK(transport.Connect(peer) == 0);
    CHECK(transport.Connect(peer) == 0);

    // Open and Listen return 0 (the pseudocode does not reach the socket).
    CHECK(transport.Open("addr", "name", 1) == 0);
    CHECK(transport.Listen("addr", "name") == 0);

    // The id getters return a reset id.
    CNetworkId local = transport.GetLocalNetworkId();
    CNetworkId remote = transport.GetPeerNetworkId();
    CHECK(local.Flags() == 0);
    CHECK(remote.Flags() == 0);

    CHECK(transport.Disconnect() == 0);
    CHECK(transport.Disconnect() == -1);

    // Create builds an initialized transport through the same steps as the original.
    CBluetoothReliable* created = CBluetoothReliable::Create("first", "second", 3);
    CHECK(created != nullptr);
    if (created) {
        CHECK(created->GetType() == 4);
        delete created;
    }

    // The constructor with arguments behaves like the default one.
    CBluetoothReliable with_args(7, true);
    CHECK(with_args.Initialize() == 0);

    // InitializeTransport is a stub in this port: it reports success and does not set the flag.
    CHECK(CBluetoothReliable::InitializeTransport() == 0);

    std::printf("%s\n", failures ? "bluetooth_reliable: FAILED" : "bluetooth_reliable: ok");
    return failures ? 1 : 0;
}
