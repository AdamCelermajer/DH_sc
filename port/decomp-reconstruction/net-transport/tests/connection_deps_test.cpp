#include "net-transport/deps/connection_deps.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // CMatching stand-in: singleton, server flag and identity slot mapping.
    CHECK(CMatching::Get() == CMatching::Get());
    CMatching::SetServerForTest(true);
    CHECK(CMatching::Get()->IsServer());
    CMatching::SetServerForTest(false);
    CHECK(!CMatching::Get()->IsServer());
    CHECK(CMatching::Get()->GetMemberIndex(31) == 31);
    CHECK(CMatching::Get()->GetMemberIndex(32) == -1);
    CHECK(CMatching::Get()->GetMemberIndex(-1) == -1);

    // CConnectionStandIn stand-in: state and member id accessors.
    CConnectionStandIn conn;
    CHECK(conn.State() == 0);
    CHECK(conn.MemberId() == -1);
    conn.SetState(4);
    conn.SetMemberId(7);
    CHECK(conn.State() == 4);
    CHECK(conn.MemberId() == 7);
    CHECK(conn.Send(5, nullptr, 0) == 0);

    // CEventQueueBase stand-in returns 0.
    CEventQueueBase queue;
    CHECK(queue.AddEvent(1, nullptr, 0) == 0);
    CHECK(queue.Clear() == 0);

    std::printf("%s\n", failures ? "connection_deps: FAILED" : "connection_deps: ok");
    return failures != 0;
}
