#include "net-session-online/c_online_impl.hpp"

#include <cstdio>
#include <string>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    COnlineImpl online;
    CHECK(online.GetPlayerName() == std::string("PLAYER"));
    return failures == 0 ? 0 : 1;
}
