#include "net-session-online/c_matching_bluetooth.hpp"

#include <cstdio>

#include "net-transport/deps/transport_test_support.hpp"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

namespace {

// A listening transport of type 1 (UDP) with a local address, which counts the broadcasts it is asked to start.
class BroadcastCounter : public transport_test_support::RecordingTransport {
public:
    BroadcastCounter() : RecordingTransport(1) { create_flags_ = 1; }
    CNetworkId GetLocalNetworkId() override {
        CNetworkId id;
        id.Reset();
        id.SetAddress1(5, 7);
        return id;
    }
    void StartBroadcast() override { ++started; }
    int started = 0;
};

}  // namespace

int main() {
    CHECK(CMatchingBluetooth::GetTransportType(0) == 3);
    CHECK(CMatchingBluetooth::GetTransportType(1) == 4);
    CHECK(CMatchingBluetooth::GetTransportType(2) == 4);
    CHECK(CMatchingBluetooth::GetTransportType(3) == 0);
    CHECK(CMatchingBluetooth::GetTransportType(0xffffffffu) == 0);

    // SetServerBroadcast: EnableBroadcast with the listen id (the address of the one listening transport), which
    // that transport's local id equals, starts its broadcast.
    {
        transport_test_support::Install();
        CTransportManager::Initialize();
        transport_test_support::TestTransportManager* manager = transport_test_support::Install();
        auto* counter = new BroadcastCounter();
        manager->UseOnly(counter);
        CMatchingBluetooth bluetooth;
        bluetooth.SetServerBroadcast();
        CHECK(counter->started == 1);
        CTransportManager::Terminate();
    }
    return failures == 0 ? 0 : 1;
}
