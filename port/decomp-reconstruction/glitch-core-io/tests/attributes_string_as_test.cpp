// addStringAs* and addString of glitch::io::CAttributes: the attribute is appended to the
// current group with its default value, then the value is set from the wide string.
#include <cstdio>

#include "glitch-core-io/io/attributes.hpp"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

using namespace glitch;

int main() {
    io::CAttributes attrs(nullptr);

    attrs.addStringAsInt("count", L"42", true);
    CHECK(attrs.getAttributeCount() == 1);
    CHECK(attrs.getInt("count") == 42);
    CHECK(attrs.getAttributeP("count")->flag);

    attrs.addStringAsBool("on", L"true", false);
    CHECK(attrs.getBool("on"));
    CHECK(!attrs.getAttributeP("on")->flag);

    // A string attribute keeps the text it was created with.
    attrs.addString("text", L"hello", false);
    CHECK(attrs.getString("text") == "hello");

    // Default values before the string is applied: the rectangle starts at zero, the
    // matrix at identity (its extra flag byte is not stored by the attribute).
    attrs.addStringAsRect("rect", L"", false);
    CHECK(attrs.getRect("rect").LowerRightCorner.X == 0 && attrs.getRect("rect").LowerRightCorner.Y == 0);
    attrs.addStringAsMatrix("m", L"", false);
    CHECK(attrs.getMatrix("m").M[0] == 1.f && attrs.getMatrix("m").M[1] == 0.f);

    // Default line: (0,0)-(1,1) for line2d, and the box covers (-1,-1,-1)..(1,1,1).
    attrs.addStringAsLine2d("seg", L"", false);
    CHECK(attrs.getLine2d("seg").end.X == 1.f && attrs.getLine2d("seg").start.X == 0.f);
    attrs.addStringAsBox3d("box", L"", false);
    CHECK(attrs.getBox3d("box").MinEdge.X == -1.f && attrs.getBox3d("box").MaxEdge.Z == 1.f);

    // Each add appends one attribute to the current group, in call order.
    CHECK(attrs.getAttributeCount() == 7);
    CHECK(attrs.getAttributeName(0) == std::string("count"));
    CHECK(attrs.getAttributeName(6) == std::string("box"));

    std::printf("%s\n", failures ? "attributes_string_as: FAILED" : "attributes_string_as: ok");
    return failures != 0;
}
