#include "net-session-online/c_matching_local.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    GetConnectionMgr().ResetForTest();
    CMatchingLocal local;

    // GetMemberCount is the connected peers plus the local member.
    CHECK(local.GetMemberCount() == 1);
    GetConnectionMgr().Connect(3, CNetworkId{});
    GetConnectionMgr().Connect(4, CNetworkId{});
    CHECK(local.GetMemberCount() == 3);

    // KickMember disconnects that member and returns 0.
    CHECK(local.KickMember(3) == 0);
    CHECK(!GetConnectionMgr().IsConnected(3));
    CHECK(local.GetMemberCount() == 2);

    // IsInRoom: a connected member is in the room; the local member is not (stand-in);
    // an unknown member is not.
    CHECK(local.IsInRoom(4));
    CHECK(!local.IsInRoom(99));
    local.SetMemberId(4);
    CHECK(!local.IsInRoom(4));

    // GenerateNewMemberId with no room mask takes the counter through the bit-spreading map:
    // counter 0 -> 1, counter 1 -> 17, counter 2 -> 33.
    CMatchingLocal fresh;
    CHECK(fresh.GenerateNewMemberId() == 1);
    CHECK(fresh.GenerateNewMemberId() == 17);
    CHECK(fresh.GenerateNewMemberId() == 33);

    std::printf("%s\n", failures ? "c_matching_local_members: FAILED" : "c_matching_local_members: ok");
    return failures != 0;
}
