// Checks the arithmetic of CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<
// CSceneNodeScaleXEx<float>, 0, float>>>: value size and getBlendedValue (0x6278a8).
#include "glitch-scene-collada-gui/tb/animation_track_cvirtual_scale_x.hpp"

#include <cmath>
#include <cstdio>

using glitch::collada::animation_track::CVirtualEx;
using glitch::collada::animation_track::CApplyValueEx;
using glitch::collada::animation_track::CSceneNodeScaleComponentMixin;
using glitch::collada::animation_track::CSceneNodeScaleXEx;
using glitch::core::vector3d;

using ScaleX = CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleXEx<float>, 0, float>>>;

static int failures = 0;

static void check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++failures;
    }
}

static bool near(float a, float b) {
    return std::fabs(a - b) < 1e-6f;
}

int main() {
    const ScaleX virt{};
    check(virt.getValueSize() == 12, "getValueSize is 12 bytes");

    // count == 1 copies the single vector.
    const float one[3] = {1.0f, 2.0f, 3.0f};
    const float w1[1] = {0.5f};
    float out1[3] = {0.0f, 0.0f, 0.0f};
    virt.getBlendedValue(const_cast<float*>(one), const_cast<float*>(w1), 1, out1);
    check(near(out1[0], 1.0f) && near(out1[1], 2.0f) && near(out1[2], 3.0f), "count 1 copies the vector");

    // count == 0 writes zeros.
    float out0[3] = {9.0f, 9.0f, 9.0f};
    virt.getBlendedValue(const_cast<float*>(one), const_cast<float*>(w1), 0, out0);
    check(near(out0[0], 0.0f) && near(out0[1], 0.0f) && near(out0[2], 0.0f), "count 0 writes zeros");

    // count == 2 is the weighted sum: 0.25 * (1,2,3) + 0.5 * (4,5,6) = (2.25, 3.0, 3.75).
    const float two[6] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    const float w2[2] = {0.25f, 0.5f};
    float out2[3] = {0.0f, 0.0f, 0.0f};
    virt.getBlendedValue(const_cast<float*>(two), const_cast<float*>(w2), 2, out2);
    check(near(out2[0], 2.25f) && near(out2[1], 3.0f) && near(out2[2], 3.75f), "count 2 weighted sum");

    if (failures == 0) {
        std::printf("animation_track_cvirtual_scale_x_test: all checks passed\n");
    }
    return failures == 0 ? 0 : 1;
}
