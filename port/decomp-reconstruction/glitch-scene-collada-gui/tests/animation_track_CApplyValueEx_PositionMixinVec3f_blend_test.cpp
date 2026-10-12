// Blend-step test for CApplyValueEx<vector3d<float>, CSceneNodePositionMixin<float>>::applyBlendedValueEx.
// Covers the three count paths of the assembly at 0x6284a4. Returns non-zero on failure.
#include <cmath>
#include <cstdio>

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_PositionMixinVec3f.hpp"

namespace {

int failures = 0;

void expectNear(float got, float want, const char* what) {
    if (std::fabs(got - want) > 1e-6f) {
        std::printf("FAIL %s: got %f want %f\n", what, static_cast<double>(got), static_cast<double>(want));
        ++failures;
    }
}

}  // namespace

int main() {
    using glitch::collada::animation_track::blendPositionVec3f;

    const float values[6] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    const float weights[2] = {0.5f, 0.25f};
    float out[3] = {-1.0f, -1.0f, -1.0f};

    // count 1 copies the first vector and ignores the weight.
    blendPositionVec3f(values, weights, 1, out);
    expectNear(out[0], 1.0f, "count1 x");
    expectNear(out[1], 2.0f, "count1 y");
    expectNear(out[2], 3.0f, "count1 z");

    // count 0 gives zero.
    blendPositionVec3f(values, weights, 0, out);
    expectNear(out[0], 0.0f, "count0 x");
    expectNear(out[1], 0.0f, "count0 y");
    expectNear(out[2], 0.0f, "count0 z");

    // count 2: x = 0.5*1 + 0.25*4 = 1.5, y = 0.5*2 + 0.25*5 = 2.25, z = 0.5*3 + 0.25*6 = 3.0.
    blendPositionVec3f(values, weights, 2, out);
    expectNear(out[0], 1.5f, "count2 x");
    expectNear(out[1], 2.25f, "count2 y");
    expectNear(out[2], 3.0f, "count2 z");

    if (failures == 0) {
        std::printf("animation_track_CApplyValueEx_PositionMixinVec3f_blend_test: all checks passed\n");
    }
    return failures == 0 ? 0 : 1;
}
