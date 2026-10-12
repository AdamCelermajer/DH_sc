#include "glitch-core-io/io/scalar_attributes.hpp"

#include <cstdio>
#include <cstring>
#include <cwchar>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

using namespace glitch::io;

int main() {
    CBoolAttribute b;
    CHECK(!b.getBool() && b.getInt() == 0 && b.getFloat() == 0.f);
    b.setInt(5);
    CHECK(b.getBool() && b.getInt() == 1 && b.getFloat() == 1.f);
    b.setFloat(0.f);
    CHECK(!b.getBool());
    b.setFloat(-0.25f);
    CHECK(b.getBool());
    CHECK(b.getType() == 3);

    CIntAttribute i;
    i.setFloat(7.9f);
    CHECK(i.getInt() == 7 && i.getFloat() == 7.f && i.getBool());
    i.setInt(0);
    CHECK(!i.getBool() && i.getType() == 0);

    CFloatAttribute f;
    f.setInt(3);
    CHECK(f.getFloat() == 3.f);
    CHECK(f.getInt() == 3);
    f.setFloat(-2.9f);
    CHECK(f.getInt() == -2 && f.getBool());
    CHECK(f.getType() == 1);
    CHECK(std::wcscmp(f.getTypeString(), L"float") == 0);

    CEnumAttribute e;
    e.setInt(2);
    e.enumValue = 2;
    CHECK(e.getEnum() == 2 && e.getFloat() == 2.f && e.getBool());
    e.setFloat(1.5f);
    CHECK(e.getInt() == 1 && e.getType() == 4);

    int marker = 0;
    CUserPointerAttribute p;
    CHECK(!p.getBool() && p.getType() == 0x1c);
    p.setUserPointer(&marker);
    CHECK(p.getBool() && p.getUserPointer() == &marker);
    CHECK(std::wcscmp(p.getTypeString(), L"userPointer") == 0);
    char text[64];
    p.getString(text);
    CUserPointerAttribute q;
    q.setString(text);
    CHECK(q.getUserPointer() == &marker);

    std::printf("%s\n", failures ? "scalar_attributes: FAILED" : "scalar_attributes: ok");
    return failures != 0;
}
