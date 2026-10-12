#include "net-session-online/t_matching_peer.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // Default construction: zero fields, empty name, reset address.
    tMatchingPeer empty;
    CHECK(empty.memberId == 0);
    CHECK(empty.lastSeenMs == 0);
    CHECK(empty.name.empty());
    CHECK(empty.address.IsEqual(CNetworkId{}));

    // Copy keeps every field, including the address bytes and the name.
    tMatchingPeer source;
    source.memberId = 42;
    source.lastSeenMs = 1000;
    source.rank = 7;
    source.timeOffset = -5;
    source.name = "room.server";
    source.address.Bytes()[0] = 0xAB;
    tMatchingPeer copy(source);
    CHECK(copy.memberId == 42);
    CHECK(copy.lastSeenMs == 1000);
    CHECK(copy.rank == 7);
    CHECK(copy.timeOffset == -5);
    CHECK(copy.name == "room.server");
    CHECK(copy.address.IsEqual(source.address));
    CHECK(copy.address.Bytes()[0] == 0xAB);

    // The copy is independent of the source.
    source.name = "changed";
    CHECK(copy.name == "room.server");

    std::printf("%s\n", failures ? "t_matching_peer: FAILED" : "t_matching_peer: ok");
    return failures != 0;
}
