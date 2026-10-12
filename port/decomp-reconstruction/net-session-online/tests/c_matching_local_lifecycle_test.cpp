#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/deps/matching_queue.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// Reset() is 0x8074a4 and belongs to no slice yet. The lifecycle bodies only call it.
void CMatchingLocal::Reset() {}

int main() {
    CMatchingLocal local;
    GetConnectionMgr().ResetForTest();
    MatchingInternalEventQueue().Clear();

    // Initialize registers the three packet types and posts 0x800001 when not initialized.
    CHECK(local.Initialize(0) == 0);
    CHECK(GetConnectionMgr().HasCallback(2));
    CHECK(GetConnectionMgr().HasCallback(3));
    CHECK(GetConnectionMgr().HasCallback(4));
    CHECK(MatchingInternalEventQueue().HasEventOccured(0x800001, false));

    // Initialize does nothing when already initialized.
    GetConnectionMgr().ResetForTest();
    MatchingInternalEventQueue().Clear();
    local.SetInitializedForTest(true);
    CHECK(local.Initialize(0) == 0);
    CHECK(!GetConnectionMgr().HasCallback(2));
    CHECK(!MatchingInternalEventQueue().HasEventOccured(0x800001, false));

    // Terminate unregisters only when initialized, and clears the flag.
    CHECK(local.Terminate() == 0);
    CHECK(!local.IsInitializedForTest());
    GetConnectionMgr().ResetForTest();
    local.SetInitializedForTest(true);
    GetConnectionMgr().RegisterPacketType(2, 2, &CMatchingLocal::sBroadcastPacketReceiverCallback);
    CHECK(local.Terminate() == 0);
    CHECK(!GetConnectionMgr().HasCallback(2));

    // PingRooms posts 0x800010.
    MatchingInternalEventQueue().Clear();
    CHECK(local.PingRooms() == 0);
    CHECK(MatchingInternalEventQueue().HasEventOccured(0x800010, false));

    std::printf("%s\n", failures ? "c_matching_local_lifecycle: FAILED" : "c_matching_local_lifecycle: ok");
    return failures != 0;
}
