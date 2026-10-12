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

    // The integer view round-trips: setInt stores the bytes as unit floats and
    // getInt packs them back (rotated by one byte, as the original does).
    CColorfAttribute colorInt("ci", glitch::video::SColorf{0.f, 0.f, 0.f, 0.f}, false);
    colorInt.setInt(0x11223344);
    CHECK(colorInt.floatValues.size() == 4);
    CHECK(colorInt.getInt() == 0x11223344);
    CHECK(colorInt.floatValues[0] > 0.2f && colorInt.floatValues[0] < 0.21f);  // 0x33 / 255
    CColorAttribute colorPacked("cp", glitch::video::SColorf{0.f, 0.f, 0.f, 0.f}, false);
    colorPacked.setFloat(static_cast<float>(0x00112233));  // exact in float; truncates to int
    CHECK(colorPacked.getInt() == 0x00112233);

    std::printf("%s\n", failures ? "typed_attributes: FAILED" : "typed_attributes: ok");
    return failures != 0;
}
