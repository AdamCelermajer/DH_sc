#include "net-session-online/game_center.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // With the gate open, a start sets the start flag and records the start time.
    GameCenter open;
    open.SetStartGateForTest(false);
    open.GCStartMultiplayerGame();
    CHECK(open.StartFlagForTest());
    // Just started, so the 150 s timeout has not expired.
    CHECK(!open.GCStartGameTimeoutExpired());

    // With the gate closed, nothing happens.
    GameCenter closed;
    closed.SetStartGateForTest(true);
    closed.GCStartMultiplayerGame();
    CHECK(!closed.StartFlagForTest());

    // The bool overload clears the gate and then starts.
    GameCenter flagged;
    flagged.SetStartGateForTest(true);
    flagged.GCStartMultiplayerGame(true);
    CHECK(!flagged.StartGateForTest());
    CHECK(flagged.StartFlagForTest());

    if (failures == 0) std::printf("game_center_start_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
