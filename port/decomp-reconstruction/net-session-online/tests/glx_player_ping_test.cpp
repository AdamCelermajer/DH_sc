#include "net-session-online/glx_player_ping.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    GLXPlayerPing ping;
    CHECK(ping.Finish() == 0);
    CHECK(ping.StateForTest() == 1);

    std::printf("%s\n", failures ? "glx_player_ping: FAILED" : "glx_player_ping: ok");
    return failures != 0;
}
