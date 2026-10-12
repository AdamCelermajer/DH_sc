#include "net-session-online/c_matching_bluetooth.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CHECK(CMatchingBluetooth::GetTransportType(0) == 3);
    CHECK(CMatchingBluetooth::GetTransportType(1) == 4);
    CHECK(CMatchingBluetooth::GetTransportType(2) == 4);
    CHECK(CMatchingBluetooth::GetTransportType(3) == 0);
    CHECK(CMatchingBluetooth::GetTransportType(0xffffffffu) == 0);
    return failures == 0 ? 0 : 1;
}
