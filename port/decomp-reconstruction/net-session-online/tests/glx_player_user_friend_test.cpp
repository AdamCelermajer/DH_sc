#include "net-session-online/glx_player_user_friend.hpp"

#include <cstdio>
#include <string>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // Empty list: every getter returns its default.
    {
        GLXPlayerUserFriend f;
        CHECK(f.GetFriendsCount() == 0);
        CHECK(f.GetUserNumber(0) == 0);
        CHECK(f.GetReputation(0) == -1.0);
        CHECK(f.GetBadReputationCount(0) == -1);
        CHECK(f.GetName(0) == nullptr);
    }

    GLXPlayerUserFriend f;
    f.ReserveForTest(3);
    f.SetNameForTest(0, "alice");
    f.SetNameForTest(2, "carol");
    CHECK(f.GetFriendsCount() == 3);
    CHECK(f.GetName(0) != nullptr);
    CHECK(f.GetName(0) != nullptr && std::string(f.GetName(0)) == "alice");
    // GetName is strictly less than the count: index 3 is out of range.
    CHECK(f.GetName(3) == nullptr);
    // Negative index returns the default.
    CHECK(f.GetState(-1) == -1);
    CHECK(f.GetLanguage(-1) == 0);
    // Inclusive getters accept index == count (original bound) but the stored
    // vector has no entry there, so the default is returned.
    CHECK(f.GetCountry(3) == 0);
    CHECK(f.GetCountry(1) == 0);

    if (failures == 0) std::printf("all checks passed\n");
    return failures == 0 ? 0 : 1;
}
