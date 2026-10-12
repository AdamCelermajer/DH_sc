#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/deps/matching_queue.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// Reset() is 0x8074a4 and belongs to no slice yet. The room bodies only call it.
void CMatchingLocal::Reset() {}

static void Clean() {
    GetConnectionMgr().ResetForTest();
    MatchingInternalEventQueue().Clear();
}

// Feeds a 0x800 record to ProcessChangeServerMessage so the server id becomes `id`.
static void SetServerId(CMatchingLocal& local, std::int32_t id) {
    NetBitStream stream(16);
    stream.WriteByteArray(&id, sizeof id);
    local.ProcessChangeServerMessage(0, stream);
}

int main() {
    // Hide, close, show and open.
    {
        CMatchingLocal local;
        Clean();
        CHECK(local.HideRoom() == 0);
        CHECK(local.IsRoomHidden());
        CHECK(MatchingInternalEventQueue().HasEventOccured(0x80000a, false));

        Clean();
        local.SetRoomFlagsForTest(false, false);
        CHECK(local.CloseRoom() == 0);
        CHECK(local.IsRoomClosed());
        CHECK(local.IsRoomHidden());

        Clean();
        local.SetRoomFlagsForTest(false, true);
        local.SetBaseRoomClosedForTest(false);
        CHECK(local.ShowRoom() == 0);
        CHECK(!local.IsRoomHidden());
        CHECK(MatchingInternalEventQueue().HasEventOccured(0x80000a, false));

        Clean();
        CHECK(local.OpenRoom(false) == 0);
        CHECK(!local.IsRoomClosed());
        CHECK(local.IsRoomHidden());

        // ShowRoom and OpenRoom do nothing while the base room-closed flag is set.
        Clean();
        local.SetBaseRoomClosedForTest(true);
        local.SetRoomFlagsForTest(true, true);
        CHECK(local.ShowRoom() == 0);
        CHECK(local.IsRoomHidden());
        CHECK(!MatchingInternalEventQueue().HasEventOccured(0x80000a, false));
        CHECK(local.OpenRoom(true) == 0);
        CHECK(local.IsRoomClosed());
    }

    // CreateJoinRoomInternal refuses when not initialized; otherwise it takes the first id and,
    // with the room open and not hidden, sends a server response (broadcast type 2).
    {
        CMatchingLocal local;
        Clean();
        CHECK(local.CreateJoinRoomInternal(true, CRoomAttributes()) == -1);

        local.SetInitializedForTest(true);
        CHECK(local.CreateJoinRoomInternal(true, CRoomAttributes()) == 0);
        CHECK(local.MemberIdForTest() == 1);
        CHECK(local.ServerMemberIdForTest() == 1);
        CHECK(MatchingInternalEventQueue().HasEventOccured(0x800003, false));
        CHECK(!GetConnectionMgr().Sent().empty());
        CHECK(GetConnectionMgr().Sent().back().packetType == 2);
        CHECK(GetConnectionMgr().Sent().back().broadcast);
    }

    // SearchRoomInternal posts 0x80000e only when the server list is not empty.
    {
        CMatchingLocal local;
        Clean();
        CHECK(local.SearchRoomInternal(CRoomSearchFilter(), false, 0) == -1);
        local.SetInitializedForTest(true);
        CHECK(local.SearchRoomInternal(CRoomSearchFilter(), false, 0) == 0);
        CHECK(!MatchingInternalEventQueue().HasEventOccured(0x80000e, false));
        CHECK(local.AddServer(4, CNetworkId{}, 1, CRoomAttributes()) == 0);
        CHECK(local.SearchRoomInternal(CRoomSearchFilter(), false, 0) == 0);
        CHECK(MatchingInternalEventQueue().HasEventOccured(0x80000e, false));
    }

    // JoinRoomInternal: an unknown key posts 0x80000c; a rank under the threshold connects
    // and sets the server id; a rank at or above it posts 0x80000d.
    {
        CMatchingLocal local;
        Clean();
        CHECK(local.JoinRoomInternal(99) == 0);
        CHECK(MatchingInternalEventQueue().HasEventOccured(0x80000c, false));

        Clean();
        local.SetJoinThresholdForTest(5);
        CHECK(local.AddServer(11, CNetworkId{}, 3, CRoomAttributes()) == 0);  // key 1, rank 3
        CHECK(local.JoinRoomInternal(1) == 0);
        CHECK(GetConnectionMgr().IsConnected(11));
        CHECK(local.ServerMemberIdForTest() == 11);

        Clean();
        local.SetJoinThresholdForTest(5);
        CNetworkId other;
        other.Bytes()[0] = 2;
        CHECK(local.AddServer(12, other, 9, CRoomAttributes()) == 0);  // key 2, rank 9
        CHECK(local.JoinRoomInternal(2) == 0);
        CHECK(MatchingInternalEventQueue().HasEventOccured(0x80000d, false));
        CHECK(!GetConnectionMgr().IsConnected(12));
    }

    // LeaveRoom: when this member is the server it answers first; then it disconnects everything
    // and posts 0x800004.
    {
        CMatchingLocal local;
        Clean();
        local.SetMemberId(5);
        SetServerId(local, 5);
        GetConnectionMgr().Connect(8, CNetworkId{});
        Clean();
        GetConnectionMgr().Connect(8, CNetworkId{});
        CHECK(local.LeaveRoom() == 0);
        CHECK(!GetConnectionMgr().IsConnected(8));
        CHECK(MatchingInternalEventQueue().HasEventOccured(0x800004, false));
        // The server response goes out before the disconnect.
        CHECK(!GetConnectionMgr().Sent().empty());
        CHECK(GetConnectionMgr().Sent().front().packetType == 2);
    }

    // GetSearchRoomAttributes is safe for an unknown key.
    {
        CMatchingLocal local;
        (void)local.GetSearchRoomAttributes(7);
    }

    std::printf("%s\n", failures ? "c_matching_local_rooms: FAILED" : "c_matching_local_rooms: ok");
    return failures != 0;
}
