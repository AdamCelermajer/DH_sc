// Checks the arithmetic of the three vec3<float> CVirtualEx instantiations in batch
// e2-glitch-scene-collada-gui-pack1-686a65-0: getValueSize (12 bytes) and getBlendedValue
// (count 1 copies, count 0 writes zeros, otherwise the weighted sum) for the scale mixin and the
// X and Y position components.
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CVirtualEx_ScaleMixinPosXYVec3f.hpp"

#include <cmath>
#include <cstdio>

using glitch::collada::animation_track::CVirtualEx;
using glitch::collada::animation_track::CApplyValueEx;
using glitch::collada::animation_track::CSceneNodeScaleMixin;
using glitch::collada::animation_track::CSceneNodePositionComponentMixin;
using glitch::collada::animation_track::CSceneNodePositionXEx;
using glitch::collada::animation_track::CSceneNodePositionYEx;
using glitch::core::vector3d;

using ScaleVirtual = CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodeScaleMixin<float>>>;
using PosYVirtual =
    CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>>;
using PosXVirtual =
    CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>>>;

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

// Runs the same blend checks against any of the three instantiations.
template <typename Virt>
static void checkBlend(const Virt& virt, const char* name) {
    check(virt.getValueSize() == 12, name);

    const float one[3] = {1.0f, 2.0f, 3.0f};
    const float w1[1] = {0.5f};
    float out1[3] = {0.0f, 0.0f, 0.0f};
    virt.getBlendedValue(const_cast<float*>(one), const_cast<float*>(w1), 1, out1);
    check(near(out1[0], 1.0f) && near(out1[1], 2.0f) && near(out1[2], 3.0f), name);

    float out0[3] = {9.0f, 9.0f, 9.0f};
    virt.getBlendedValue(const_cast<float*>(one), const_cast<float*>(w1), 0, out0);
    check(near(out0[0], 0.0f) && near(out0[1], 0.0f) && near(out0[2], 0.0f), name);

    // 0.25 * (1,2,3) + 0.5 * (4,5,6) = (2.25, 3.0, 3.75).
    const float two[6] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    const float w2[2] = {0.25f, 0.5f};
    float out2[3] = {0.0f, 0.0f, 0.0f};
    virt.getBlendedValue(const_cast<float*>(two), const_cast<float*>(w2), 2, out2);
    check(near(out2[0], 2.25f) && near(out2[1], 3.0f) && near(out2[2], 3.75f), name);
}

int main() {
    checkBlend(ScaleVirtual{}, "scale mixin: value size, count 1/0/2 blend");
    checkBlend(PosYVirtual{}, "position Y: value size, count 1/0/2 blend");
    checkBlend(PosXVirtual{}, "position X: value size, count 1/0/2 blend");

    if (failures == 0) {
        std::printf("e2-glitch-scene-collada-gui-pack1-686a65-0_test: all checks passed\n");
    }
    return failures == 0 ? 0 : 1;
}
