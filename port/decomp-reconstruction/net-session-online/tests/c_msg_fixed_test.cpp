#include "net-session-online/c_msg_fixed.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CMsgIsHost host;
    CHECK(host.GetDataSize() == 4);
    CHECK(host.GetDataPtr() != nullptr);
    host.SetProperties();

    CMsgRaisedEvent raised;
    CHECK(raised.GetDataSize() == 0x14);
    raised.SetProperties();
    CHECK(raised.IsReliable());

    CMsgAttackResult attack;
    CHECK(attack.GetDataSize() == 0x34);

    CMsgGlobalDeath death;
    CHECK(death.GetDataSize() == 0);

    std::printf("%s\n", failures ? "c_msg_fixed: FAILED" : "c_msg_fixed: ok");
    return failures != 0;
}
