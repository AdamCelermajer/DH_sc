#include "net-session-online/game_center.hpp"

#include <cstdint>
#include <cstdio>

#include "core-platform/gc_singleton.hpp"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // The start gate, the start flag and the start time are on the GameCenter singleton, as in the original.
    GameCenter* gc = GCSingleton<GameCenter>::GetInstance();

    // The entry guard is on the gate: the binary returns at once when the gate byte (+0x15) is ZERO
    // (asm 0x499f34: LDRB R4,[R0,#0x15]; CMP R4,#0; BEQ loc_499F48 skips the tail).
    gc->SetStartGateForTest(false);
    gc->GCStartMultiplayerGame();
    CHECK(!gc->StartFlagForTest());

    // With the gate set, a start sets the start flag and records the start time.
    gc->SetStartGateForTest(true);
    gc->GCStartMultiplayerGame();
    CHECK(gc->StartFlagForTest());
    // Just started, so the 150 s timeout has not expired.
    CHECK(!gc->GCStartGameTimeoutExpired());

    // A local object is not the singleton, so a start on it does not set the singleton's state.
    GameCenter local;
    local.SetStartGateForTest(false);
    local.GCStartMultiplayerGame();
    CHECK(!local.StartFlagForTest());

    // The bool overload clears the singleton's gate and then starts. The flag is already set, but the
    // cleared gate means the no-argument overload returns at once.
    gc->SetStartGateForTest(true);
    gc->GCStartMultiplayerGame(true);
    CHECK(!gc->StartGateForTest());
    CHECK(gc->StartFlagForTest());

    // Both overloads return the register value the binary's tail leaves: the no-argument one returns the
    // GS singleton pointer when the gate is clear, and the bool one passes that same value through.
    gc->SetStartGateForTest(false);
    const int cleared = gc->GCStartMultiplayerGame();
    CHECK(cleared == static_cast<int>(reinterpret_cast<std::uintptr_t>(gc)));
    const int clearedThroughBool = gc->GCStartMultiplayerGame(true);
    CHECK(clearedThroughBool == static_cast<int>(reinterpret_cast<std::uintptr_t>(gc)));

    if (failures == 0) std::printf("game_center_start_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
