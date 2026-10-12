// Enumeration, light and box attribute adders and by-name setters of CAttributes.
#include <cstdio>

#include "glitch-core-io/io/attributes.hpp"
#include "glitch-core-io/io/scalar_attributes.hpp"
#include "glitch-core-io/io/typed_attributes.hpp"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

using namespace glitch;

int main() {
    static const char* const choices[] = {"low", "high", nullptr};

    io::CAttributes attrs(nullptr);
    // addEnum(int): the index is stored on the enumeration just added.
    attrs.addEnum("level", 1, choices, true);
    CHECK(attrs.getAttributeCount() == 1);
    CHECK(attrs.getAttributeP("level")->flag);
    CHECK(attrs.getInt("level") == 1);

    // The box setter updates an existing box and adds a missing one with the flag clear.
    attrs.addBool("unused", false, false);
    glitch::core::aabbox3d<float> box{{0.f, 0.f, 0.f}, {1.f, 2.f, 3.f}};
    attrs.setAttribute("box", box);
    CHECK(attrs.getAttributeCount() == 3);
    CHECK(!attrs.getAttributeP("box")->flag);
    box.MaxEdge.X = 5.f;
    attrs.setAttribute("box", box);
    CHECK(attrs.getAttributeCount() == 3);

    // The string enumeration setter appends a new enumeration when the name is missing.
    attrs.setAttribute("mode", "high", choices);
    CHECK(attrs.getAttributeCount() == 4);
    CHECK(!attrs.getAttributeP("mode")->flag);

    // A light is added with the given flag, and a missing light by name is added with the flag clear.
    attrs.addLight("lamp", boost::intrusive_ptr<video::CLight>(), true);
    CHECK(attrs.getAttributeCount() == 5);
    CHECK(attrs.getAttributeP("lamp")->flag);
    attrs.setAttribute("spot", boost::intrusive_ptr<video::CLight>());
    CHECK(attrs.getAttributeCount() == 6);

    // The narrow string add appends a string attribute with the flag given.
    attrs.addString("text", "abc", true);
    CHECK(attrs.getAttributeCount() == 7);
    CHECK(attrs.getAttributeP("text")->flag);
    CHECK(attrs.getAttributeP("text")->getType() == 2);

    // A texture is added by addTexture, and a missing one by setAttribute gets flag clear.
    attrs.addTexture("tex", boost::intrusive_ptr<video::ITexture>(), false);
    CHECK(attrs.getAttributeCount() == 8);
    attrs.setAttribute("tex", boost::intrusive_ptr<video::ITexture>());
    CHECK(attrs.getAttributeCount() == 8);
    attrs.setAttribute("tex2", boost::intrusive_ptr<video::ITexture>());
    CHECK(attrs.getAttributeCount() == 9);
    CHECK(!attrs.getAttributeP("tex2")->flag);

    // A missing array is added with the array not kept (partial); a second call sets it.
    attrs.setAttribute("arr", std::vector<std::wstring>{L"a"});
    CHECK(attrs.getAttributeCount() == 10);
    CHECK(!attrs.getAttributeP("arr")->flag);
    CHECK(attrs.getArray("arr").empty());
    attrs.setAttribute("arr", std::vector<std::wstring>{L"a", L"b"});
    CHECK(attrs.getAttributeCount() == 10);
    CHECK(attrs.getArray("arr").size() == 2);

    // The narrow string constructor clears isWide; the value is not kept (partial).
    io::CStringAttribute narrow("narrow", "text", true);
    CHECK(narrow.flag);
    CHECK(!narrow.isWide);

    if (failures == 0) std::printf("attributes_enum_light_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
