// Tests for reconstruction slice 07 (recon/slice_07.cpp). Returns non-zero on failure.
#include <cmath>
#include <cstdint>
#include <cstdio>

#include "glitch-scene-collada-gui/recon/slice_07.hpp"

using namespace glitch::collada::recon_07;
namespace core = glitch::core;

namespace {

int failures = 0;
const float kPi = 3.14159265358979f;

void check(bool condition, const char* what) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", what);
        ++failures;
    }
}

bool near(float a, float b, float eps = 1e-4f) {
    return std::fabs(a - b) <= eps;
}

void testTriangleIndices() {
    const std::uint16_t idx16[] = {10, 11, 12, 13};
    std::uint32_t a = 0, b = 0, c = 0;
    getTriangleIndices(kIndex16, idx16, 1, a, b, c);
    check(a == 11 && b == 12 && c == 13, "16-bit triangle indices");

    const std::uint32_t idx32[] = {7, 8, 9, 10};
    getTriangleIndices(kIndex32, idx32, 0, a, b, c);
    check(a == 7 && b == 8 && c == 9, "32-bit triangle indices");

    const std::uint8_t idx8[] = {1, 2, 3, 4};
    getTriangleIndices(kIndex8, idx8, 1, a, b, c);
    check(a == 2 && b == 3 && c == 4, "8-bit triangle indices");
}

void testSwapIndices() {
    std::uint16_t idx16[] = {1, 2, 3};
    const std::uint32_t old = swapIndices(kIndex16, idx16, 0, 2);
    check(old == 1 && idx16[0] == 3 && idx16[2] == 1, "16-bit swap");

    std::uint8_t idx8[] = {5, 6};
    swapIndices(kIndex8, idx8, 0, 1);
    check(idx8[0] == 6 && idx8[1] == 5, "8-bit swap");
}

void testAngleWeight() {
    // Equilateral triangle: every angle is pi/3.
    core::vector3d<float> out{};
    getAngleWeight(out, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.5f, std::sqrt(3.0f) / 2.0f, 0.0f});
    const float third = kPi / 3.0f;
    check(near(out.X, third) && near(out.Y, third) && near(out.Z, third), "equilateral angles");

    // Right isoceles triangle at the origin: right angle at p0.
    getAngleWeight(out, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    check(near(out.X, kPi / 2.0f), "right angle at p0");
    check(near(out.Y, kPi / 4.0f), "45 degrees at p1");
}

void testKdTree() {
    KdEntry entry{7u, core::aabbox3d<float>{{0.0f, 0.0f, 0.0f}, {2.0f, 4.0f, 6.0f}}};
    // centre is (1, 2, 3)
    check(lessThanPlane(entry, 1.5f, 0), "x centre below plane");
    check(!lessThanPlane(entry, 0.5f, 0), "x centre above plane");
    check(lessThanPlane(entry, 3.5f, 2), "z centre below plane");

    check(near(distanceKdTree(entry, 0.0f, 0), 1.0f), "plane distance x");
    check(near(distanceKdTree(entry, 3.0f, 1), 1.0f), "plane distance y");

    KdEntry other{8u, core::aabbox3d<float>{{2.0f, 2.0f, 2.0f}, {4.0f, 4.0f, 4.0f}}};  // centre (3,3,3)
    check(near(distanceKdTree(entry, other), 4.0f + 1.0f + 0.0f), "centre distance squared");
    check(near(distanceKdTree(entry, other.second), 4.0f + 1.0f + 0.0f), "distance to box");
}

void testHandles() {
    CAnimationBlock block{};
    block.refCount = 1;
    CAnimationBlock_grab(block);
    check(block.refCount == 2, "grab increments count");
}

void testColours() {
    SLightColorApplicator light{};
    const std::uint8_t bytes[3] = {255, 0, 51};
    const float blue = CApplicator_SLightColor_applyValueEx(light, bytes);
    check(near(light.r, 1.0f) && near(light.g, 0.0f) && near(light.b, 0.2f) && near(light.a, 1.0f),
          "light colour applicator");
    check(near(blue, 0.2f), "applicator returns blue channel");

    const float source[8] = {0, 0, 0, 0, 0, 1.0f, 0.0f, 0.5f};
    std::uint8_t out[3] = {};
    const std::uint32_t last = CVirtualEx_LightColor_retrieveValue(source, out);
    check(out[0] == 255 && out[1] == 0 && out[2] == 127 && last == 127, "light colour retrieve");
}

}  // namespace

int main() {
    testTriangleIndices();
    testSwapIndices();
    testAngleWeight();
    testKdTree();
    testHandles();
    testColours();
    if (failures != 0) {
        std::fprintf(stderr, "%d failure(s)\n", failures);
        return 1;
    }
    std::puts("recon_slice_07 tests passed");
    return 0;
}
