#include "net-session-online/glx_player_leaderboard.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    GLXPlayerLeaderboard lb;
    CHECK(lb.getLeaderboardSize() == -1);          // no names held yet
    CHECK(lb.getLeaderboardEntryPlayerName(0) == nullptr);
    CHECK(lb.getLeaderboardEntryPlayerPosition(0) == -1);
    CHECK(lb.getLeaderboardEntryPlayerScore(0) == static_cast<std::int32_t>(0xfff5d3d6u));
    CHECK(lb.getLeaderboardEntryPlayerCountry(0) == 0);
    CHECK(lb.getLeaderboardEntryPlayerScoreData(0) == 0);

    lb.SetCountForTest(2);
    lb.SetEntryForTest(0, "alice", 44, 1, 900, 7);
    lb.SetEntryForTest(1, "bob", 33, 2, 500, 3);
    CHECK(lb.getLeaderboardSize() == 2);
    CHECK(std::string(lb.getLeaderboardEntryPlayerName(1)) == "bob");
    CHECK(lb.getLeaderboardEntryPlayerCountry(0) == 44);
    CHECK(lb.getLeaderboardEntryPlayerPosition(1) == 2);
    CHECK(lb.getLeaderboardEntryPlayerScore(0) == 900);
    CHECK(lb.getLeaderboardEntryPlayerScoreData(1) == 3);
    CHECK(lb.getLeaderboardEntryPlayerName(2) == nullptr);   // index at the count is out of range
    CHECK(lb.getLeaderboardEntryPlayerName(-1) == nullptr);
    CHECK(lb.getLeaderboardEntryPlayerScore(2) == static_cast<std::int32_t>(0xfff5d3d6u));

    lb.SetCurrentForTest(5, 61, 1234, 9);
    CHECK(lb.getCurrentPlayerLeaderboardPosition() == 5);
    CHECK(lb.getCurrentPlayerLeaderboardCountry() == 61);
    CHECK(lb.getCurrentPlayerLeaderboardScore() == 1234);
    CHECK(lb.getCurrentPlayerLeaderboardScoreData() == 9);

    std::printf("%s\n", failures ? "glx_player_leaderboard: FAILED" : "glx_player_leaderboard: ok");
    return failures != 0;
}
