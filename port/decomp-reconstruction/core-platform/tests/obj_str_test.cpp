#include "core-platform/obj_str.hpp"

#include <cstdio>
#include <cstring>
#include <vector>

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
    Point2D<int> p2{3, -4};
    ObjToStr(buf, p2);
    CHECK(std::strcmp(buf, "3,-4") == 0);
    bool b = true;
    ObjToStr(buf, b);
    CHECK(std::strcmp(buf, "1") == 0);
    float f = 0.5f;
    ObjToStr(buf, f);
    CHECK(std::strcmp(buf, "0.500000") == 0);
    std::vector<Point3D<float>> pts;
    std::strcpy(buf, "keep");
    ObjToStr(buf, pts);
    CHECK(std::strcmp(buf, "keep") == 0);  // empty body: buffer untouched

    int parsed = 0;
    StrToObj("17", parsed);
    CHECK(parsed == 17);
    bool flag = false;
    StrToObj("0", flag);
    CHECK(!flag);
    StrToObj("5", flag);
    CHECK(flag);
    float fv = 0.f;
    StrToObj("2.25", fv);
    CHECK(fv == 2.25f);
    std::printf("%s\n", failures ? "obj_str: FAILED" : "obj_str: ok");
    return failures != 0;
}
