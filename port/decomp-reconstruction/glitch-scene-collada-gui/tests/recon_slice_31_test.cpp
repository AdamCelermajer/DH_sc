// Tests for slice 31 reconstructions (see src/recon/slice_31.cpp): the billboard colour accessor and the attribute names of the
// billboard node. The external calls the slice makes into other category code are stubbed here so the object links; the test uses a recording
// fake for the attribute interface.
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
const char* const glitch::collada::recon_31::AutomaticCullingNames[] = {"none", nullptr};

namespace glitch::collada::recon_31 {
void ISceneNode_serializeAttributes(const ISceneNode*, IAttributes*, SAttributeReadWriteOptions*) {}
void ISceneNode_deserializeAttributes(ISceneNode*, IAttributes*, SAttributeReadWriteOptions*) {}
void CLight_deserializeAttributes(glitch::video::CLight*, IAttributes*, SAttributeReadWriteOptions*) {}
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

// Attribute fake: records the names of every add call. It derives from the canonical CAttributes so the slots it does not record are the real
// implementation.
class RecordingAttributes : public glitch::io::CAttributes {
public:
    RecordingAttributes() : glitch::io::CAttributes(nullptr) {}
    std::vector<std::string> names;

    void addFloat(const char* n, float, bool) override { names.emplace_back(n); }
    void addColor(const char* n, glitch::video::SColor, bool) override { names.emplace_back(n); }
};

class TestBillboard : public CBillboardSceneNode {
public:
    glitch::core::dimension2d<float> lastSize{};
    const glitch::core::vector3d<float>* getAbsoluteTranslation() const override { return nullptr; }
    const glitch::core::aabbox3d<float>* getBoundingBox() const override { return nullptr; }
    float getDistanceBias() const override { return 0.0f; }
    int getDefaultSortOrder() const override { return 0; }
    void setSize(const glitch::core::dimension2d<float>& size) override { lastSize = size; }
};

glitch::video::SColor color(std::uint32_t value) {
    glitch::video::SColor c;
    c.color = value;
    return c;
}

void testBillboardGetColor() {
    TestBillboard node;
    node.colorTop = color(0x11);
    node.colorBottom = color(0x22);
    glitch::video::SColor top = color(0);
    glitch::video::SColor bottom = color(0);
    CBillboardSceneNode_getColor(node, top, bottom);
    CHECK(top.color == 0x11u);
    CHECK(bottom.color == 0x22u);
}

void testBillboardSerializeNames() {
    TestBillboard node;
    RecordingAttributes attrs;
    CBillboardSceneNode_serializeAttributes(node, &attrs, nullptr);
    CHECK((attrs.names == std::vector<std::string>{"Width", "Height", "Shade_Top", "Shade_Down"}));
}

}  // namespace

int main() {
    testBillboardGetColor();
    testBillboardSerializeNames();
    if (failures != 0) {
        std::fprintf(stderr, "%d failure(s)\n", failures);
        return 1;
    }
    std::printf("slice 31 tests passed\n");
    return 0;
}
