#include "net-transport/data_packet.hpp"
#include "net-transport/socket_abstract.hpp"
#include "net-transport/transport.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

namespace {
class FakeSocket : public CSocketAbstract {
public:
    unsigned last_options = 0;
    bool connected = true;

protected:
    int SetSocketOptions(unsigned int options) override {
        last_options = options;
        return 0;
    }
    bool IsConnected() const override { return connected; }
};
}  // namespace

int main() {
    CTransport t;
    CNetworkId peer;
    CHECK(t.Connect(peer) == 0);
    CHECK(t.Disconnect() == 0);
    t.StartBroadcast();
    t.StopBroadcast();

    DataPacket p;
    CHECK(p.GetMessageType() == -1);
    CHECK(p.GetMMOMessageType() == -1);
    CHECK(p.addBlob(nullptr, 0) == 0);

    FakeSocket s;
    s.Assign(42, 7);
    CHECK(s.IsConnectedFlag() && s.IsValid() && s.Handle() == 42);
    CHECK(s.last_options == 7);
    s.connected = false;
    s.Assign(-1, 0);
    CHECK(!s.IsConnectedFlag());

    std::printf("%s\n", failures ? "transport: FAILED" : "transport: ok");
    return failures != 0;
}
