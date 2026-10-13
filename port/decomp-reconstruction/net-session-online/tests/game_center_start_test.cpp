#include "net-session-online/game_center.hpp"

#include <cstdio>

#include "core-platform/gc_singleton.hpp"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // The start gate, the start flag and the start time are on the GameCenter singleton, as in the original.
    GameCenter* gc = GCSingleton<GameCenter>::GetInstance();

    // With the gate closed, nothing happens.
    gc->SetStartGateForTest(true);
    gc->GCStartMultiplayerGame();
    CHECK(!gc->StartFlagForTest());

    // With the gate open, a start sets the start flag and records the start time.
    gc->SetStartGateForTest(false);
    gc->GCStartMultiplayerGame();
    CHECK(gc->StartFlagForTest());
    // Just started, so the 150 s timeout has not expired.
    CHECK(!gc->GCStartGameTimeoutExpired());

    // A local object is not the singleton, so a start on it does not set the singleton's state.
    GameCenter local;
    local.SetStartGateForTest(false);
    local.GCStartMultiplayerGame();
    CHECK(!local.StartFlagForTest());

    // The bool overload clears the singleton's gate and then starts (the flag is already set).
    gc->SetStartGateForTest(true);
    gc->GCStartMultiplayerGame(true);
    CHECK(!gc->StartGateForTest());
    CHECK(gc->StartFlagForTest());

    if (failures == 0) std::printf("game_center_start_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
