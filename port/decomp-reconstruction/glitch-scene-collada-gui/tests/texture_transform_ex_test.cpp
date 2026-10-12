#include "glitch-scene-collada-gui/texture_transform_ex.hpp"

#include <cstdio>
#include <cstring>

using namespace glitch::collada::animation_track;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CHECK(CTextureTransformEx_getValueSize() == 20);
    std::uint32_t id[5] = {9, 9, 9, 9, 9};
    CTextureTransformEx_getIdentityValue(id);
    float s0, s1;
    std::memcpy(&s0, &id[3], sizeof s0);
    std::memcpy(&s1, &id[4], sizeof s1);
    CHECK(id[0] == 0 && id[1] == 0 && id[2] == 0);
    CHECK(s0 == 1.0f && s1 == 1.0f);
    std::printf("%s\n", failures ? "texture_transform_ex: FAILED" : "texture_transform_ex: ok");
    return failures != 0;
}
