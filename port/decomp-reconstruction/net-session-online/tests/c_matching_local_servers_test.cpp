#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/deps/matching_queue.hpp"
#include "net-session-online/deps/platform_stand_ins.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// Reset() is 0x8074a4 and belongs to no slice yet. The room bodies, which these tests pull in, call it.
void CMatchingLocal::Reset() {}

static CNetworkId Address(std::uint8_t tag) {
    CNetworkId id;
    id.Bytes()[0] = tag;
    return id;
}

// Joins key `key` with a threshold of 10, returning true when it connected to `member`.
static bool JoinConnects(CMatchingLocal& local, std::uint64_t key, std::int32_t member) {
    GetConnectionMgr().ResetForTest();
    local.SetJoinThresholdForTest(10);
    local.JoinRoomInternal(key);
    return GetConnectionMgr().IsConnected(member);
}

int main() {
    GetConnectionMgr().ResetForTest();
    MatchingInternalEventQueue().Clear();
    OnlineClockStorage() = 100000;

    CMatchingLocal local;
    const CRoomAttributes attrs;

    // AddServer adds a new entry under key 1 (the first key), and a repeated address only refreshes it.
    CHECK(local.AddServer(11, Address(1), 3, attrs) == 0);  // key 1
    CHECK(JoinConnects(local, 1, 11));
    CHECK(local.AddServer(11, Address(1), 9, attrs) == 0);  // refresh: rank 9 now
    local.SetJoinThresholdForTest(5);
    GetConnectionMgr().ResetForTest();
    local.JoinRoomInternal(1);
    CHECK(!GetConnectionMgr().IsConnected(11));  // rank 9 is not under 5
    CHECK(local.AddServer(12, Address(2), 1, attrs) == 0);  // key 2
    local.SetJoinThresholdForTest(10);
    CHECK(JoinConnects(local, 2, 12));

    // RemoveServer removes the entry with that address; the join then finds nothing.
    local.RemoveServer(Address(2));
    MatchingInternalEventQueue().Clear();
    GetConnectionMgr().ResetForTest();
    local.JoinRoomInternal(2);
    CHECK(MatchingInternalEventQueue().HasEventOccured(0x80000c, false));

    // PurgeServers drops entries older than 20000 ms and keeps the others.
    CMatchingLocal purge;
    OnlineClockStorage() = 100000;
    CHECK(purge.AddServer(21, Address(3), 1, attrs) == 0);  // key 1, seen at 100000
    OnlineClockStorage() = 120000;                          // age 20000: kept
    CHECK(purge.PurgeServers() == 0);
    purge.SetJoinThresholdForTest(10);
    MatchingInternalEventQueue().Clear();
    GetConnectionMgr().ResetForTest();
    purge.JoinRoomInternal(1);
    CHECK(GetConnectionMgr().IsConnected(21));
    OnlineClockStorage() = 120001;                          // age 20001: purged
    CHECK(purge.PurgeServers() == 0);
    MatchingInternalEventQueue().Clear();
    GetConnectionMgr().ResetForTest();
    purge.JoinRoomInternal(1);
    CHECK(MatchingInternalEventQueue().HasEventOccured(0x80000c, false));

    // ProcessServerMessage on a record from another member adds it (flag 0), and removes it (flag 1).
    CMatchingLocal from;
    from.SetJoinThresholdForTest(10);
    RoomServerHeader header{};
    header.listenId = Address(9);
    header.memberId = 9;
    header.memberCount = 4;
    header.flag = 0;
    {
        NetBitStream stream(64);
        stream.WriteByteArray(&header, sizeof header);
        from.ProcessServerMessage(CNetworkId{}, stream);
    }
    GetConnectionMgr().ResetForTest();
    from.JoinRoomInternal(1);
    CHECK(GetConnectionMgr().IsConnected(9));

    header.flag = 1;
    {
        NetBitStream stream(64);
        stream.WriteByteArray(&header, sizeof header);
        from.ProcessServerMessage(CNetworkId{}, stream);
    }
    MatchingInternalEventQueue().Clear();
    GetConnectionMgr().ResetForTest();
    from.JoinRoomInternal(1);
    CHECK(MatchingInternalEventQueue().HasEventOccured(0x80000c, false));

    // GetMemberName: the local member gives the local transport name (empty here); a remote
    // member gives the part of its transport name before the first '.'.
    TransportNamesForTest().clear();
    TransportNamesForTest()[0] = "alpha.beta";
    CHECK(local.GetMemberName(5) == "alpha");
    local.SetMemberId(7);
    CHECK(local.GetMemberName(7).empty());
    TransportNamesForTest().clear();

    std::printf("%s\n", failures ? "c_matching_local_servers: FAILED" : "c_matching_local_servers: ok");
    return failures != 0;
}
