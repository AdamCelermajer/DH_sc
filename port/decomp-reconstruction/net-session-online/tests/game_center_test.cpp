#include "net-session-online/game_center.hpp"
#include "net-session-online/game_center_info.hpp"
#include "net-session-online/multiplayer_callback.hpp"

#include <cstdio>
#include <cstring>
#include <string>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    GameCenter gc;
    CHECK(!gc.FeaturesEnabledForTest());
    gc.EnableGCFeatures();
    CHECK(gc.FeaturesEnabledForTest());
    gc.DisableGCFeatures();
    CHECK(!gc.FeaturesEnabledForTest());
    gc.ConnectToServer(1);
    gc.GCFriends(2);
    gc.ProcessGCAPIError("a", "b");
    CHECK(gc.GetGCPlayerAttributes() == static_cast<std::int32_t>(0xffff0000u));
    CHECK(gc.GetHostUid() == nullptr);
    CHECK(!gc.IsHost());
    gc.SetUidsForTest("host-1", "host-1");
    CHECK(gc.IsHost());
    gc.SetUidsForTest("host-1", "guest-2");
    CHECK(!gc.IsHost());
    CHECK(std::strcmp(gc.GetUserUid(), "guest-2") == 0);
    CHECK(std::strcmp(gc.GetHostUid(), "host-1") == 0);

    // The player list holds the local user after a reset, and GetPlayerCount reports it.
    gc.ResetPlayerList();
    CHECK(gc.PlayerListForTest().size() == 1);
    CHECK(gc.PlayerListForTest().front() == "guest-2");
    CHECK(gc.GetPlayerCount() == 1);
    gc.SetUidsForTest("host-1", nullptr);
    gc.ResetPlayerList();
    CHECK(gc.PlayerListForTest().size() == 1);
    CHECK(gc.PlayerListForTest().front().empty());
    CHECK(gc.GetPlayerCount() == 1);

    // The alias is stored as a string, and a copy comes back from the getter.
    CHECK(gc.GetGCPlayerAlias().empty());
    gc.SetGCPlayerAlias("Ranger");
    CHECK(gc.GetGCPlayerAlias() == "Ranger");
    gc.SetGCPlayerAlias(nullptr);
    CHECK(gc.GetGCPlayerAlias().empty());

    {
        GameCenterInfo info;
        info.IdsForTest().push_back("a");
        info.IdsForTest().push_back("b");
        CHECK(info.IdsForTest().size() == 2);
    }

    CHECK(MultiplayerCallback::idleState(nullptr) == 100);
    CHECK(MultiplayerCallback::ingameState(nullptr) == 300);

    std::printf("%s\n", failures ? "game_center: FAILED" : "game_center: ok");
    return failures != 0;
}
