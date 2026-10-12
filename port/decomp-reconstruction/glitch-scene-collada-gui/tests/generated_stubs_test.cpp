#include "glitch-scene-collada-gui/generated_stubs.hpp"

#include <cstdio>

using namespace glitch::collada::stubs;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CHECK(stub_60e62c(1, 2) == 0u);
    CHECK(stub_60ee1c() == 16u);
    stub_60ee24(3);
    std::printf("%s\n", failures ? "generated_stubs: FAILED" : "generated_stubs: ok");
    return failures != 0;
}
