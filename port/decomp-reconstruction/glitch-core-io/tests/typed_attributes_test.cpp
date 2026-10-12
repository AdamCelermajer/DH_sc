#include "glitch-core-io/io/typed_attributes.hpp"

#include <cstdio>
#include <cwchar>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

using namespace glitch::io;

int main() {
    CColorAttribute color;
    CColorfAttribute colorf;
    CVector2DAttribute v2;
    CVector3DIAttribute v3i;
    CQuaternionAttribute quat;
    CTriangleAttribute tri;
    CRectAttribute rect;
    CLightAttribute light;

    CHECK(color.getType() == 5);
    CHECK(colorf.getType() == 6 && std::wcscmp(colorf.getTypeString(), L"colorf") == 0);
    CHECK(v2.getType() == 7 && std::wcscmp(v2.getTypeString(), L"vector2d") == 0);
    CHECK(v3i.getType() == 0xb && std::wcscmp(v3i.getTypeString(), L"vector3di") == 0);
    CHECK(quat.getType() == 0x10 && std::wcscmp(quat.getTypeString(), L"quaternion") == 0);
    CHECK(tri.getType() == 0x13 && std::wcscmp(tri.getTypeString(), L"triangle") == 0);
    CHECK(rect.getType() == 0xe);
    CHECK(light.getType() == 0x1b);

    std::printf("%s\n", failures ? "typed_attributes: FAILED" : "typed_attributes: ok");
    return failures != 0;
}
