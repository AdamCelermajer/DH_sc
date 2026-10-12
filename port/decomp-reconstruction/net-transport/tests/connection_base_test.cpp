#include "net-transport/connection_base.hpp"

#include <cstdio>

#include "net-transport/default_data_packet.hpp"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // The stand-in socket never connects, so connect() moves to the failed state and returns 0.
    char host[] = "lobby.example";
    Connection conn(host, 9000);
    CHECK(!conn.isConnected());
    CHECK(conn.connect() == 0);
    CHECK(conn.connect() == 0);

    // With no host the connect fails at once.
    Connection no_host(nullptr, 0);
    CHECK(no_host.connect() == 0);

    // A packet queued while the link is down is dropped.
    CHECK(conn.addOutgoingPacket(new DefaultDataPacket()) == 0);

    // Incoming packets are queued in order and handed out once, each to its caller.
    CHECK(conn.getNextIncomingPacket() == nullptr);
    auto* first = new DefaultDataPacket();
    auto* second = new DefaultDataPacket();
    CHECK(conn.addIncomingPacket(first) == 0);
    CHECK(conn.addIncomingPacket(second) == 0);
    DataPacket* out = conn.getNextIncomingPacket();
    CHECK(out == first);
    delete out;
    out = conn.getNextIncomingPacket();
    CHECK(out == second);
    delete out;
    CHECK(conn.getNextIncomingPacket() == nullptr);

    // Once connected, a queued packet is sent and removed from the queue (the stand-in send
    // accepts it), and the request sends keep a retry copy.
    conn.set_connected(true);
    CHECK(conn.isConnected());
    CHECK(conn.sendLoginPackage(host) == 0);
    CHECK(conn.sendFinishGamePackage() == 0);
    CHECK(conn.getTimeOfLastReqest() >= 0);
    conn.sendRetryData();
    conn.sendData();

    // Packets without a payload build and queue as the original does.
    CHECK(conn.sendCreateSessionPackage(host, nullptr, 3) == 0);
    CHECK(conn.sendListSessionsAllPackage(1, 2) == 0);
    CHECK(conn.sendFinishConnectionPackage() >= 0);
    CHECK(conn.sendKickOutPlayerPackage(nullptr) == 0);

    // Only a non-null name is stored.
    conn.setName(host);
    conn.setName(nullptr);

    // The keep-alive check is quiet while the last send is recent.
    CHECK(!conn.keepConnectionAlive());

    // Disconnect clears the link and the retry copy, and is safe to repeat.
    conn.disconnect();
    CHECK(!conn.isConnected());
    CHECK(conn.getTimeOfLastReqest() == 0);
    conn.disconnect();

    // The base class is usable through the DataPacket interface (virtual destruction).
    std::printf("%s\n", failures ? "connection_base: FAILED" : "connection_base: ok");
    return failures ? 1 : 0;
}
