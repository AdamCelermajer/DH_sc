// Batch r1n-net-transport-CBluetoothUnreliable-2d7a15-0: Open, Listen, Create, the two-argument
// constructor and AcceptConnections' neighbours. Only observable behaviour is checked here.
#include "net-transport/bluetooth_transport.hpp"

#include <cstdio>
#include <string>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // Create builds a transport that has already been initialised: a second Initialize fails.
    CBluetoothUnreliable* made = CBluetoothUnreliable::Create(std::string("first"), std::string("second"), 1);
    CHECK(made != nullptr);
    if (made != nullptr) {
        CHECK(made->GetType() == 3);
        CHECK(made->Initialize() == -1);
        CHECK(made->Disconnect() == 0);
        CHECK(made->Disconnect() == -1);
        delete made;
    }

    // Open and Listen return 0 and do not initialise the transport (the original only stores the channel).
    CBluetoothUnreliable link;
    CHECK(link.Open("addr", "name", 1) == 0);
    CHECK(link.Listen("addr", "name") == 0);
    CHECK(!link.IsListening());
    CHECK(link.Initialize() == 0);
    CHECK(link.Disconnect() == 0);

    // The two-argument constructor takes the same defaults as the default constructor.
    CBluetoothUnreliable with_args(5, true);
    CHECK(with_args.GetType() == 3);
    CHECK(!with_args.IsListening());
    CHECK(with_args.Initialize() == 0);
    CHECK(with_args.Disconnect() == 0);

    // InitializeTransport is partial in this port: it reports 0 and does not create the default transport.
    CHECK(CBluetoothUnreliable::InitializeTransport() == 0);

    std::printf("%s\n", failures ? "bluetooth_unreliable_batch: FAILED" : "bluetooth_unreliable_batch: ok");
    return failures != 0;
}
