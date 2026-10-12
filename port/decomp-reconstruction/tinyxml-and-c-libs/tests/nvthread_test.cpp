#include "tinyxml-and-c-libs/nvthread.hpp"

#include <chrono>
#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CHECK(NVThreadInit(1234) == 1234);

    const auto start = std::chrono::steady_clock::now();
    CHECK(NVThreadSleep(20) == 0);
    const auto slept = std::chrono::steady_clock::now() - start;
    CHECK(slept >= std::chrono::milliseconds(20));
    CHECK(NVThreadSleep(0) == 0);

    std::printf("%s\n", failures ? "nvthread: FAILED" : "nvthread: ok");
    return failures != 0;
}
