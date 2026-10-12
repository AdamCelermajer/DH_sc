#include "net-session-online/glx_player_user.hpp"

#include <cstdio>
#include <string>

#include "net-session-online/glx_player_observer.hpp"

// Test literals are passed to the char* parameters of the original signatures.
#pragma GCC diagnostic ignored "-Wwrite-strings"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

namespace {
// Captures the request text instead of sending it.
class CapturingUser : public GLXPlayerUser {
public:
    void SendByGet(char* request) override {
        ++getCount;
        get = request;
    }
    void SendByPost(char* request) override {
        ++postCount;
        post = request;
    }
    int getCount = 0;
    int postCount = 0;
    std::string get;
    std::string post;
};

// Records the rejected-argument reports (slot 3, OnResponseError).
class RecordingObserver : public GLXPlayerObserver {
public:
    void OnResponseError(std::int32_t code, std::int32_t detail) override {
        ++errors;
        lastCode = code;
        lastDetail = detail;
    }
    int errors = 0;
    std::int32_t lastCode = 0;
    std::int32_t lastDetail = 0;
};
}  // namespace

int main() {
    CapturingUser u;
    RecordingObserver obs;
    u.RegisterObserver(&obs);
    u.SetGGI(12);
    u.SetUID("alice");

    u.sendGetChatWarning();
    CHECK(u.get == "f|99|i|12|u|alice|");

    u.sendGetAdvRSS(-1);  // POST; -1 leaves out the "p" key
    CHECK(u.post == "f|88|i|12|u|alice|");
    CHECK(u.postCount == 1);
    u.sendGetAdvRSS(4);
    CHECK(u.post == "f|88|i|12|u|alice|p|4|");

    u.sendGetAdvAttachment(3);
    CHECK(u.get == "f|89|i|12|u|alice|a|3|");

    u.sendSetUserState(2);
    CHECK(u.get == "f|97|i|12|u|alice|a|2|");
    u.sendSetUserState(5);  // not 2 or 3: observer slot 3 with (0x61, -100), nothing sent
    CHECK(obs.lastCode == 0x61 && obs.lastDetail == -100);
    CHECK(u.getCount == 3);  // chat warning, attachment and state so far

    u.sendGetLiveFeeds(0);  // kind 1..20 only
    CHECK(u.getCount == 3);
    u.sendGetLiveFeeds(1);
    CHECK(u.get == "f|111|i|12|u|alice|c|1|");
    CHECK(u.getCount == 4);

    u.sendGetTrophyIcon(7, 3);  // mode must be below 3: nothing sent
    CHECK(u.getCount == 4);
    u.sendGetTrophyIcon(7, 2);
    CHECK(u.get == "f|84|i|12|u|alice|t|7|rt|2|");

    u.sendGetGameTrophyList(-1);
    CHECK(u.get == "f|86|i|12|u|alice|");
    u.sendGetGameTrophyList(2);
    CHECK(u.get == "f|86|i|12|u|alice|p|2|");

    u.sendDelUserStoreData(nullptr);
    CHECK(obs.lastCode == 0x48 && obs.lastDetail == -100);
    u.sendDelUserStoreData("k1");
    CHECK(u.get == "f|72|i|12|u|alice|k|k1|");

    u.sendGetUserAvatarList();
    CHECK(u.get == "f|103|i|12|u|alice|");
    u.sendDelUserAvatar("x");
    CHECK(u.get == "f|102|i|12|u|alice|d|x|");
    u.sendDownloadUserAvatar("a");
    CHECK(u.get == "f|101|i|12|u|alice|d|a|");

    u.sendGetUserUID("carol", true);
    CHECK(u.get == "f|58|i|12|u|alice|un|carol|");
    u.sendGetUserUID("carol", false);
    CHECK(u.get == "f|58|i|12|u|alice|n|carol|");
    u.sendGetUserUID(nullptr, false);
    CHECK(obs.lastCode == 0x3a && obs.lastDetail == -100);

    u.sendPointUserReputation("dan", 2, false);  // points must be 1 or 5
    CHECK(obs.lastCode == 0x44 && obs.lastDetail == -100);
    u.sendPointUserReputation("dan", 5, false);
    CHECK(u.get == "f|68|i|12|u|alice|n|dan|r|5|");

    u.sendSetUserStoreData("key", "val");
    CHECK(u.post == "f|66|i|12|u|alice|k|key|d|val|");

    u.sendRetrievalPassword("eve");
    CHECK(u.get == "f|52|i|12|u|eve|");

    u.sendChangeEmail("u", "p", "e");
    CHECK(u.get == "f|51|i|12|u|u|p|p|ne|e|");

    u.sendChangePassword("bob", "old", "pw", "pw2");  // new password and repeat differ
    CHECK(obs.lastCode == 0x32 && obs.lastDetail == -100);
    u.sendChangePassword("bob", "old", "pw", "pw");
    CHECK(u.get == "f|50|i|12|u|bob|p|old|np|pw|vp|pw|");

    u.sendChangeUserName("u", "p", "n", true);
    CHECK(u.get == "f|14|i|12|u|u|p|p|nu|n|o|1|");

    u.sendGetUsedApp(nullptr);  // without a name the user id is used
    CHECK(u.get == "f|57|i|12|u|alice|");

    // The upload is partial: only the null case is reproduced.
    u.sendUploadUserAvatar(nullptr);
    CHECK(obs.lastCode == 0x47 && obs.lastDetail == -100);

    if (failures == 0) std::printf("all checks passed\n");
    return failures == 0 ? 0 : 1;
}
