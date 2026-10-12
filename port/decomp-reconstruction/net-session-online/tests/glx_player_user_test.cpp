#include "net-session-online/glx_player_user.hpp"

#include <cstdio>
#include <string>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

static bool Is(const char* p, const char* expected) { return p != nullptr && std::string(p) == expected; }

int main() {
    // Defaults: strings are null, ints are 0, and the idle state and best score are -1.
    {
        GLXPlayerUser u;
        CHECK(u.getUserName() == nullptr);
        CHECK(u.getEmail() == nullptr);
        CHECK(u.getEmailFlag() == 0);
        CHECK(u.getSex() == 0);
        CHECK(u.getOnlineTime() == 0);
        CHECK(u.getNumberUserGameList() == 0);
        CHECK(u.getUserGamePID(0) == nullptr);
        CHECK(u.getUserGameTrophy(0) == -1);
        CHECK(u.getCurrentUserState() == -1);
        CHECK(u.getUserBestScore() == -1);
        CHECK(u.getUserReputation() == 0.0);
        CHECK(u.getAvatarStatus(0) == 0);
        CHECK(u.getAvatarKey(0) == nullptr);
    }

    GLXPlayerUser u;
    u.SetUserNameForTest("bob");
    CHECK(Is(u.getUserName(), "bob"));
    u.SetUserNameForTest("");  // an empty name is not stored
    CHECK(u.getUserName() == nullptr);

    // Game lists: index 0 is unset, index 1 holds the values.
    u.SetGameForTest(1, "pid1", "game1", 77);
    u.SetGameCountForTest(2);
    CHECK(u.getNumberUserGameList() == 2);
    CHECK(u.getUserGamePID(0) == nullptr);
    CHECK(Is(u.getUserGamePID(1), "pid1"));
    CHECK(Is(u.getUserGameName(1), "game1"));
    CHECK(u.getUserGameTrophy(0) == -1);
    CHECK(u.getUserGameTrophy(1) == 77);
    CHECK(u.getUserGameTrophy(2) == -1);   // past the end
    CHECK(u.getUserGameTrophy(-1) == -1);  // negative index
    CHECK(u.getUserGamePID(2) == nullptr);

    // Avatar list: entries below the count are visible.
    u.SetAvatarForTest(0, "k0", "u0", 3);
    CHECK(Is(u.getAvatarKey(0), "k0"));
    CHECK(Is(u.getAvatarLastUpdate(0), "u0"));
    CHECK(u.getAvatarStatus(0) == 3);
    CHECK(u.getAvatarKey(1) == nullptr);   // not set
    CHECK(u.getAvatarStatus(-1) == 0);     // negative index

    // Clearing the avatar list drops the entries and the count.
    u.clearAvatarList();
    CHECK(u.getAvatarKey(0) == nullptr);
    CHECK(u.getAvatarStatus(0) == 0);

    // Clearing the game list drops the game entries and the count.
    u.clearUserGameList();
    CHECK(u.getNumberUserGameList() == 0);
    CHECK(u.getUserGamePID(1) == nullptr);
    CHECK(u.getUserGameTrophy(1) == -1);

    // Clearing the user information drops the name and resets the flags.
    u.clearUserInformation();
    CHECK(u.getUserName() == nullptr);

    // clearUserState resets the state to idle.
    u.clearUserState();
    CHECK(u.getCurrentUserState() == -1);

    u.processUploadAvatar(nullptr);  // empty in the original

    if (failures == 0) std::printf("all checks passed\n");
    return failures == 0 ? 0 : 1;
}
