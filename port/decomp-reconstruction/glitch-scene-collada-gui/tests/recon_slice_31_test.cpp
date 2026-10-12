// Tests for slice 31 reconstructions (see src/recon/slice_31.cpp).
// The external calls the slice makes into other category code are stubbed
// here so the object links; the tests use recording fakes for the attribute
// and node interfaces.
#include <climits>
#include <cstdio>
#include <string>
#include <vector>

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
int CBatchMesh_sort(CBatchMesh*, glitch::video::IVideoDriver*) { return 0; }
void CBatchMesh_updateBoundingBox(CBatchMesh*) {}
void CBatchMesh_initStaticSegmentBoundingBoxes(CBatchMesh*) {}
void CBatchSceneNode_setVisibleIndexCache(CBatchSceneNode&, bool) {}
void CCameraSceneNode_recalculateViewArea(CCameraSceneNode&) {}
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

// Attribute fake: records the names of every add call and push, in order.
class RecordingAttributes : public IAttributes {
public:
    std::vector<std::string> names;

    void addInt(const char* n, int, bool) override { names.emplace_back(n); }
    void addFloat(const char* n, float, bool) override { names.emplace_back(n); }
    void addBool(const char* n, bool, bool) override { names.emplace_back(n); }
    void addString(const char* n, const char*, bool) override { names.emplace_back(n); }
    void addStringW(const char* n, const wchar_t*, bool) override { names.emplace_back(n); }
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

// Node fake with fixed answers for the virtuals the entries call.
class FakeNode : public ISceneNode {
public:
    glitch::core::vector3d<float> translation{3.0f, 4.0f, 0.0f};
    glitch::core::aabbox3d<float> box{};
    float bias = 0.0f;
    int sortOrder = 7;

    const glitch::core::vector3d<float>* getAbsoluteTranslation() const override { return &translation; }
    const glitch::core::aabbox3d<float>* getBoundingBox() const override { return &box; }
    float getDistanceBias() const override { return bias; }
    int getDefaultSortOrder() const override { return sortOrder; }
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

void testDistanceEntry() {
    FakeNode node;
    node.box.MinEdge = {-1.0f, -1.0f, -1.0f};
    node.box.MaxEdge = {1.0f, 1.0f, 1.0f};
    // Translation (3,4,0) against the origin gives d2 = 25; the box extent
    // squared is 12, so the distance is 25 - 6 = 19.
    CSceneManager::SDistanceNodeEntry entry(&node, {0.0f, 0.0f, 0.0f}, nullptr);
    CHECK(entry.node == &node);
    CHECK(entry.distance > 18.999f && entry.distance < 19.001f);
}

void testDefaultEntrySortOrder() {
    FakeNode node;
    CSceneManager::SDefaultNodeEntry fixed(&node, {}, nullptr, 5);
    CHECK(fixed.sortOrder == 5);
    CSceneManager::SDefaultNodeEntry deflt(&node, {}, nullptr, INT_MAX);
    CHECK(deflt.sortOrder == 7);
}

void testTransparentEntry() {
    FakeNode node;
    node.bias = 2.0f;
    // Without a transform the node translation is used: d2 = 25, plus bias 2.
    CSceneManager::STransparentNodeEntry entry(&node, {0.0f, 0.0f, 0.0f}, {}, nullptr, nullptr, 9);
    CHECK(entry.sortOrder == 9);
    CHECK(entry.distance > 26.999f && entry.distance < 27.001f);
}

}  // namespace

int main() {
    testFaderSetColor();
    testFaderSerializeNames();
    testSkinSetColor();
    testDistanceEntry();
    testDefaultEntrySortOrder();
    testTransparentEntry();
    if (failures != 0) {
        std::fprintf(stderr, "%d failure(s)\n", failures);
        return 1;
    }
    std::printf("slice 31 tests passed\n");
    return 0;
}
