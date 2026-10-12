#include "net-session-online/glx_player_user.hpp"

#include <cstdio>
#include <string>

// Test literals are passed to the char* parameters of the original signatures.
#pragma GCC diagnostic ignored "-Wwrite-strings"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

static bool Is(const char* p, const char* expected) { return p != nullptr && std::string(p) == expected; }

int main() {
    GLXPlayerUser u;

    // setPassword keeps a copy; null clears, and an empty string is kept as "".
    u.setPassword("pw");
    CHECK(Is(u.getPassword(), "pw"));
    u.setPassword("");
    CHECK(Is(u.getPassword(), ""));
    u.setPassword(nullptr);
    CHECK(u.getPassword() == nullptr);

    // The text handlers store non-empty text and leave empty text as null.
    u.processUserChatWarning("warn");
    CHECK(Is(u.ChatWarningForTest(), "warn"));
    u.processUserChatWarning("");
    CHECK(u.ChatWarningForTest() == nullptr);

    u.processPromoAttachment("promo");
    CHECK(Is(u.PromoAttachmentForTest(), "promo"));
    u.processRssAttachment("rss");
    CHECK(Is(u.RssAttachmentForTest(), "rss"));

    // Trophy icon: the mode picks the field. The default mode (-1) stores nothing.
    u.processUserTrophyIcon("x");
    CHECK(u.IconText0ForTest() == nullptr && u.IconText1ForTest() == nullptr);
    u.sendGetTrophyIcon(5, 0);  // sets mode 0
    u.processUserTrophyIcon("zero");
    CHECK(Is(u.IconText0ForTest(), "zero"));
    u.sendGetTrophyIcon(5, 1);  // sets mode 1
    u.processUserTrophyIcon("one");
    CHECK(Is(u.IconText1ForTest(), "one"));
    u.sendGetTrophyIcon(5, 3);  // mode 3 is rejected by sendGetTrophyIcon, so mode stays 1
    u.processUserTrophyIcon("again");
    CHECK(Is(u.IconText1ForTest(), "again"));

    // processUserInformation clears the user information first.
    u.SetUserNameForTest("bob");
    // "ignored" has no fields, so the name is the empty field, not the old "bob".
    u.processUserInformation("ignored");
    CHECK(u.getUserName() == nullptr || u.getUserName()[0] == '\0');

    // processUserGameList clears the game list first.
    u.SetGameForTest(0, "p", "g", 1);
    u.SetGameCountForTest(1);
    // "ignored" has no separators, so the count is 0 + 2 - 1 = 1 record, as the ledger says.
    u.processUserGameList("ignored");
    CHECK(u.getNumberUserGameList() == 1);

    if (failures == 0) std::printf("all checks passed\n");
    return failures == 0 ? 0 : 1;
}
