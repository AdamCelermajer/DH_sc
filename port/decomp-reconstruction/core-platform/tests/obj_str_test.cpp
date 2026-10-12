#include "core-platform/obj_str.hpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    char buf[64];
    int i = -42;
    ObjToStr(buf, i);
    CHECK(std::strcmp(buf, "-42") == 0);
    glitch::core::vector3d<float> v{1.f, 2.5f, -3.f};
    ObjToStr(buf, v);
    CHECK(std::strcmp(buf, "1.000000,2.500000,-3.000000") == 0);
    int parsed = 0;
    StrToObj("17", parsed);
    CHECK(parsed == 17);
    std::printf("%s\n", failures ? "obj_str: FAILED" : "obj_str: ok");
    return failures != 0;
}
