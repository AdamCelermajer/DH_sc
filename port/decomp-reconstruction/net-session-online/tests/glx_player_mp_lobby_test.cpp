#include "net-session-online/glx_player_mp_lobby.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

namespace {
struct CountingObserver : GLXPlayerMPBaseObserver {
    int invalid = 0;
    void OnInvalidState() override { ++invalid; }
};
}  // namespace

int main() {
    CountingObserver obs;

    // Init: state 0, result 0, pending kind 0x15 (21), not pending.
    GLXPlayerMPLobby lobby(&obs, nullptr);
    CHECK(!lobby.mpIsInSession());
    CHECK(!lobby.mpIsInGame());
    CHECK(!lobby.mpIsMaster());

    // Refused while state <= 1: result 0x32, observer notified.
    lobby.mpSendLeaveLobby();
    CHECK(lobby.LastResultForTest() == 0x32);
    CHECK(obs.invalid == 1);
    CHECK(!lobby.PendingForTest());

    // Allowed in state 2: result -1, kind 3 (leave), pending set.
    lobby.SetStateForTest(2);
    CHECK(lobby.mpIsInSession());
    CHECK(!lobby.mpIsInGame());
    lobby.mpSendLeaveLobby();
    CHECK(lobby.LastResultForTest() == -1);
    CHECK(lobby.PendingKindForTest() == 3);
    CHECK(lobby.PendingForTest());
    CHECK(obs.invalid == 1);

    // In game: state above 4.
    lobby.SetStateForTest(5);
    CHECK(lobby.mpIsInGame());

    // Cancel auto-match sets state to 2 and kind 0x11 (17).
    lobby.mpSendCancelAutoMatchRequest();
    CHECK(lobby.PendingKindForTest() == 17);
    CHECK(lobby.LastResultForTest() == -1);

    // Disconnect resets state to 0 and clears the pending id to -1 in the original.
    lobby.mpDisconnect();
    CHECK(!lobby.mpIsInSession());

    // Login has no state check: it always sends and sets kind 1.
    lobby.mpSendLobbyLogin(1L, nullptr, 0, 0);
    CHECK(lobby.PendingKindForTest() == 1);

    if (failures == 0) std::printf("all checks passed\n");
    return failures == 0 ? 0 : 1;
}
