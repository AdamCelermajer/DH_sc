// Tests for slice 00 reconstructions (see src/recon/slice_00.cpp).
#include <cstdio>

#include "glitch-scene-collada-gui/recon/slice_00.hpp"

using namespace glitch::collada::recon_00;

namespace {

int failures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                \
        }                                                              \
    } while (0)

glitch::core::aabbox3d<float> box(float x0, float y0, float z0, float x1, float y1, float z1) {
    glitch::core::aabbox3d<float> b{};
    b.MinEdge = {x0, y0, z0};
    b.MaxEdge = {x1, y1, z1};
    return b;
}

void testTweakable() {
    Tweakable t(nullptr);
    CHECK(t.getInstanceName() == 0);
    t.instanceName = 26;
    CHECK(t.getInstanceName() == 26);
    CHECK(t.getXML().empty());

    float f = 1.5f;
    t.registerVariable("speed", f);
    CHECK(t.names.size() == 1);
    CHECK(t.mappings.count("speed") == 1);
    CHECK(t.mappings["speed"].type == 1);
    CHECK(t.mappings["speed"].value == &f);

    // Registering the same name again must not add a second name.
    t.registerVariable("speed", f);
    CHECK(t.names.size() == 1);

    int i = 3;
    t.registerVariable("count", i);
    CHECK(t.mappings["count"].type == 0);
    CHECK(t.names.size() == 2);
}

void testMappingCopy() {
    TweakableMapping a(5, reinterpret_cast<void*>(0x10));
    a.name = "alpha";
    TweakableMapping b;
    b = a;
    CHECK(b.type == 5);
    CHECK(b.value == reinterpret_cast<void*>(0x10));
    CHECK(b.name == "alpha");
}

void testFrustum() {
    glitch::scene::SViewFrustum fr{};
    fr.box = box(-100, -100, -100, 100, 100, 100);
    for (auto& p : fr.planes) {
        p.Normal.X = 0.0f;
        p.Normal.Y = 0.0f;
        p.Normal.Z = 0.0f;
        p.D = -1000.0f;  // never separates anything
    }
    // Plane 0 keeps x <= 10 (the test vertex is x = min when nx >= 0).
    fr.planes[0].Normal.X = 1.0f;
    fr.planes[0].D = -10.0f;
    fr.planes[2].D = -1000.0f;
    fr.planes[3].D = -1000.0f;

    CHECK(fr.intersects(box(0, 0, 0, 1, 1, 1)));
    CHECK(!fr.intersects(box(20, 0, 0, 21, 1, 1)));
    CHECK(fr.intersectsWithoutBoxTest3(box(0, 0, 0, 1, 1, 1)));
    CHECK(!fr.intersectsWithoutBoxTest3(box(20, 0, 0, 21, 1, 1)));

    // Outside the frustum bounding box is rejected before the planes.
    CHECK(!fr.intersects(box(200, 0, 0, 201, 1, 1)));
}

void testGui() {
    GuiEnvironment env;
    GuiElement* root = env.getRootGUIElement();
    CHECK(root == &env.root);

    GuiElement child;
    child.relative = glitch::core::rect<int>{{0, 0}, {10, 10}};
    root->addChild(&child);
    CHECK(child.parent == root);
    CHECK(root->children.size() == 1);

    child.move(glitch::core::position2d<int>{5, 3});
    CHECK(child.relative.UpperLeftCorner.X == 5);
    CHECK(child.relative.UpperLeftCorner.Y == 3);
    CHECK(child.relative.LowerRightCorner.X == 15);
    CHECK(child.relative.LowerRightCorner.Y == 13);

    CHECK(child.remove() == 1);
    CHECK(root->children.empty());
    CHECK(child.parent == nullptr);

    child.setText(L"hello");
    CHECK(child.text == L"hello");

    CHECK(env.getElementById(0, nullptr) == root);
}

}  // namespace

int main() {
    testTweakable();
    testMappingCopy();
    testFrustum();
    testGui();
    if (failures != 0) {
        std::fprintf(stderr, "%d failure(s)\n", failures);
        return 1;
    }
    std::printf("recon_slice_00 tests passed\n");
    return 0;
}
