#include "glitch-core-io/io/attributes.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

using namespace glitch;

int main() {
    io::IAttribute base;
    // Base getters return the zero / default values from the original bodies.
    CHECK(base.getInt() == 0);
    CHECK(base.getFloat() == 0.f);
    CHECK(base.getBool() == false);
    CHECK(base.getEnum() == 0);
    CHECK(base.getColor().color == 0xffffffffu);
    CHECK(base.getColorf().r == 1.f && base.getColorf().a == 1.f);
    CHECK(base.getVector2d().X == 0.f && base.getVector2d().Y == 0.f);
    CHECK(base.getVector3d().Z == 0.f);
    CHECK(base.getVector4d().W == 0.f);
    CHECK(base.getVector2di().X == 0 && base.getVector3di().Z == 0 && base.getVector4di().W == 0);
    CHECK(base.getPosition().X == 0 && base.getPosition().Y == 0);
    CHECK(base.getRect().X2 == 0 && base.getRect().Y2 == 0);
    CHECK(base.getArray().empty());

    // Base setters are no-ops: the value is not stored.
    base.setInt(7);
    base.setFloat(2.f);
    base.setBool(true);
    CHECK(base.getInt() == 0 && base.getFloat() == 0.f && base.getBool() == false);

    if (failures) return 1;
    std::printf("iattribute_defaults_test ok\n");
    return 0;
}
