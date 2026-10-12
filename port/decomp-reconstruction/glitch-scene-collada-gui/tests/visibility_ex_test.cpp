#include "glitch-scene-collada-gui/visibility_ex.hpp"

#include <cstdio>

using namespace glitch::collada::animation_track;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CVisibilityEx v{};
    std::uint8_t out = 0xff;
    v.visible_flags = 0;
    visibility_ex::retrieveValue(&v, nullptr, &out);
    CHECK(out == 0);
    v.visible_flags = 3;
    visibility_ex::retrieveValue(&v, nullptr, &out);
    CHECK(out == 1);
    CHECK(visibility_ex::getValueSize() == 4);
    std::uint32_t id = 0;
    visibility_ex::getIdentityValue(&v, &id);
    CHECK(id == 1);
    std::printf("%s\n", failures ? "visibility_ex: FAILED" : "visibility_ex: ok");
    return failures != 0;
}
