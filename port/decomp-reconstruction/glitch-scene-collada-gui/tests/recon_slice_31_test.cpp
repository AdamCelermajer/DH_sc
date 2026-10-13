// Tests for slice 31 reconstructions (see src/recon/slice_31.cpp).
// The external calls the slice makes into other category code are stubbed
// here so the object links; the tests use recording fakes for the attribute
// and node interfaces.
#include <cstdio>
#include <string>
#include <vector>

#include "glitch-core-io/io/attributes.hpp"
#include "glitch-video/material.hpp"
#include "glitch-video/texture.hpp"
#include "glitch-scene-collada-gui/recon/slice_31.hpp"

using namespace glitch::collada::recon_31;

// ---------------------------------------------------------------------------
// Stubs for calls into other category code (defined elsewhere in the port).
// ---------------------------------------------------------------------------
const char* const glitch::collada::recon_31::GUIAlignmentNames[] = {"Upper", "Lower", "Scale", nullptr};
const char* const glitch::collada::recon_31::GUISkinTypeNames[] = {"Default", nullptr};
const char* const glitch::collada::recon_31::GUISkinColorNames[21] = {};
const char* const glitch::collada::recon_31::GUISkinSizeNames[10] = {};
const char* const glitch::collada::recon_31::GUISkinTextNames[8] = {};
const char* const glitch::collada::recon_31::GUISkinIconNames[23] = {};
const char* const glitch::collada::recon_31::AutomaticCullingNames[] = {"Off", nullptr};

namespace glitch::collada::recon_31 {
void ISceneNode_serializeAttributes(const ISceneNode*, IAttributes*, SAttributeReadWriteOptions*) {}
void ISceneNode_deserializeAttributes(ISceneNode*, IAttributes*, SAttributeReadWriteOptions*) {}
void CLight_deserializeAttributes(glitch::video::CLight*, IAttributes*, SAttributeReadWriteOptions*) {}
void CGUIMessageBox::refreshControls() {}
void CGUIScrollBar::refreshControls() {}
void IGUIElement::setAlignment(int, int, int, int) {}
}  // namespace glitch::collada::recon_31

namespace {

int failures = 0;

#define CHECK(cond)                                                                        \
    do {                                                                                   \
        if (!(cond)) {                                                                     \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);          \
            ++failures;                                                                    \
        }                                                                                  \
    } while (0)

// Attribute fake: records the names of every add call and push, in order. It derives from the canonical
// CAttributes so the slots it does not record (the other 160 or so) are the real implementation.
class RecordingAttributes : public glitch::io::CAttributes {
public:
    RecordingAttributes() : glitch::io::CAttributes(nullptr) {}
    std::vector<std::string> names;

    void addInt(const char* n, int, bool) override { names.emplace_back(n); }
    void addFloat(const char* n, float, bool) override { names.emplace_back(n); }
    void addBool(const char* n, bool, bool) override { names.emplace_back(n); }
    void addString(const char* n, const char*, bool) override { names.emplace_back(n); }
    void addString(const char* n, const wchar_t*, bool) override { names.emplace_back(n); }
    void addEnum(const char* n, int, const char* const*, bool) override { names.emplace_back(n); }
    void addColor(const char* n, glitch::video::SColor, bool) override { names.emplace_back(n); }
    void addPosition2d(const char* n, glitch::core::position2d<int>, bool) override { names.emplace_back(n); }
    void addRect(const char* n, glitch::core::rect<int>, bool) override { names.emplace_back(n); }
    void addVector3d(const char* n, const glitch::core::vector3d<float>&, bool) override { names.emplace_back(n); }
    void addTexture(const char* n, boost::intrusive_ptr<glitch::video::ITexture>, bool) override {
        names.emplace_back(n);
    }
    bool existsAttribute(const char*) const override { return false; }
    int getInt(const char*) const override { return 0; }
    float getFloat(const char*) const override { return 0.0f; }
    bool getBool(const char*) const override { return false; }
    std::string getString(const char*) const override { return {}; }
    std::wstring getStringW(const char*) const override { return {}; }
    int getEnum(const char*, const char* const*) const override { return 0; }
    glitch::video::SColor getColor(const char*) const override { return {}; }
    glitch::core::position2d<int> getPosition2d(const char*) const override { return {}; }
    glitch::core::rect<int> getRect(const char*) const override { return {}; }
    glitch::core::vector3d<float> getVector3d(const char*) const override { return {}; }
    boost::intrusive_ptr<glitch::video::ITexture> getTexture(const char*) const override { return {}; }
    void push(const char* n) override { names.emplace_back(n); }
    void pop() override {}
};

// Concrete fader for the serialise test; IGUIElement's pure virtuals are no-ops.
class TestFader : public CGUIInOutFader {
public:
    void setId(int) override {}
    void setName(const char*) override {}
    void setText(const wchar_t*) override {}
    void setVisible(bool) override {}
    void setEnabled(bool) override {}
    void updateAbsolutePosition() override {}
    void setRelativePosition(const glitch::core::rect<int>&) override {}
    const char* getName() const override { return "fader"; }
    const wchar_t* getCaption() const override { return L""; }
};

glitch::video::SColor color(std::uint32_t value) {
    glitch::video::SColor c;
    c.color = value;
    return c;
}

void testFaderSetColor() {
    TestFader f;
    f.fadeMode = 1;
    CGUIInOutFader_setColor(f, color(1), color(2));
    CHECK(f.fullColor.color == 1);
    CHECK(f.transColor.color == 2);

    f.fadeMode = 2;
    CGUIInOutFader_setColor(f, color(3), color(4));
    CHECK(f.fullColor.color == 4);
    CHECK(f.transColor.color == 3);

    f.fadeMode = 0;
    CGUIInOutFader_setColor(f, color(5), color(6));
    CHECK(f.fullColor.color == 4);  // unchanged for other modes
}

void testFaderSerializeNames() {
    TestFader f;
    RecordingAttributes attrs;
    IGUIElement_serializeAttributes(f, &attrs, nullptr);
    CGUIInOutFader_serializeAttributes(f, &attrs, nullptr);
    // Base names come first, then the two colours.
    CHECK(attrs.names.size() >= 2);
    CHECK(attrs.names[attrs.names.size() - 2] == "FullColor");
    CHECK(attrs.names[attrs.names.size() - 1] == "TransColor");
}

void testSkinSetColor() {
    CGUISkin skin{};
    CGUISkin_setColor(skin, 20, color(0xABCDEF));
    CHECK(skin.colors[20].color == 0xABCDEF);
    CGUISkin_setColor(skin, 21, color(0x123456));  // out of range, ignored
    CHECK(skin.colors[20].color == 0xABCDEF);
}

}  // namespace

int main() {
    testFaderSetColor();
    testFaderSerializeNames();
    testSkinSetColor();
    if (failures != 0) {
        std::fprintf(stderr, "%d failure(s)\n", failures);
        return 1;
    }
    std::printf("slice 31 tests passed\n");
    return 0;
}
