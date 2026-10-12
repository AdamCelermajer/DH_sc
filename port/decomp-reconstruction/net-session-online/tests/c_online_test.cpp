#include "net-session-online/c_online.hpp"

#include <cstdio>

#include "net-session-online/deps/matching.hpp"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

namespace {
// A matching object with fixed answers, for the online branch.
class FakeMatching : public CMatching {
public:
    std::uint32_t MemberBit(int) const override { return 0; }
    int LocalMemberId() const override { return localId; }
    int ServerMemberId() const override { return 0; }
    bool IsServer() const override { return server; }
    std::uint32_t GetRoomMask() const override { return 0; }
    int localId = 6;
    bool server = false;
};
}  // namespace

int main() {
    COnline online;
    online.DetectDisconnect();
    CHECK(online.IsAgeRestricted() == 0);
    CHECK(online.GetLocalMemberId() == 1);   // offline
    CHECK(online.IsServer());                // offline: server
    CHECK(online.IsTimeSynced());            // server: synced

    FakeMatching fake;
    CMatching::Set(&fake);
    online.SetIsOnlineGame(true);
    CHECK(online.GetLocalMemberId() == 6);
    CHECK(!online.IsServer());
    CHECK(!online.IsTimeSynced());           // client, not synced yet
    online.SetTimeSyncedForTest(true);
    CHECK(online.IsTimeSynced());
    fake.server = true;
    CHECK(online.IsServer());
    CMatching::Set(nullptr);

    std::printf("%s\n", failures ? "c_online: FAILED" : "c_online: ok");
    return failures != 0;
}
