#include "glitch-scene-collada-gui/vector3d_ex.hpp"

#include <cstdio>

using namespace glitch::collada::animation_track;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CVector3dEx v;
    CHECK(CVector3dEx_getValueSize() == 12);
    CHECK(v.getValueSize() == 12);
    float id[3] = {9.f, 9.f, 9.f};
    CVector3dEx_getIdentityValue(id);
    CHECK(id[0] == 0.f && id[1] == 0.f && id[2] == 0.f);
    float src[3] = {1.f, 2.f, 3.f}, dst[3] = {0.f, 0.f, 0.f};
    CVector3dEx_applyValue(&v, src, dst);
    CHECK(dst[0] == 1.f && dst[1] == 2.f && dst[2] == 3.f);
    std::printf("%s\n", failures ? "vector3d_ex: FAILED" : "vector3d_ex: ok");
    return failures != 0;
}
