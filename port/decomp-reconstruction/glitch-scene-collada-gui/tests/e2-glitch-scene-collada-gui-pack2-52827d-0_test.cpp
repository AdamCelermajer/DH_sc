// Test for batch e2-glitch-scene-collada-gui-pack2-52827d-0: the arithmetic and size members of
// CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodePositionMixin<float>>> and the quaternion size.
// Returns non-zero on the first failing check.
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CVirtualEx_PositionMixinVec3f.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CVirtualEx_QuatMixinFloat.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_PositionMixinVec3f.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_QuatMixinFloat.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CBlender_quaternion.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_CMixinPosFloat3.hpp"
#include "glitch-scene-collada-gui/layouts/animation_track.hpp"

// The batch source also calls out-of-line members of the accessor, the blender and the applicators. Their bodies are
// not reconstructed in this tree and none is reached by this test, so each is stubbed to abort if called.
namespace glitch::collada {
std::uint32_t SAnimationAccessor::getOutput(int) const {
    std::abort();
}
bool SAnimationAccessor::hasDefaultValue() const {
    std::abort();
}
}  // namespace glitch::collada

namespace glitch::collada::animation_track {
int CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodePositionMixin<float>>::applyBlendedValueEx(
    void*, float*, int, void*, CApplicatorInfo*) {
    std::abort();
}
int CApplyValueEx<glitch::core::quaternion, CSceneNodeQuaternionMixin<float>>::applyBlendedValueEx(
    void*, float*, int, void*, CApplicatorInfo*) {
    std::abort();
}
std::uint32_t CBlender<glitch::core::quaternion, 1, glitch::core::quaternion>::getBlendedValueEx(void*, float*, int,
                                                                                                 void*) {
    std::abort();
}
float CInterpreter<CMixin<float, 3, SSceneNodePosition, -1, float>, float, 3, SUseDefaultLerp<float>>::getKeyBasedValueEx(
    const glitch::collada::SAnimationAccessor&, int, int, float, float*) {
    std::abort();
}
}  // namespace glitch::collada::animation_track

// The batch source links CBlender<quaternion> and the key-reading interpreters, which call quaternion math and
// the accessor key table helper. None of these are reached by this test (it covers the vec3 blend and the sizes),
// so they are stubbed to abort if they are ever called.
namespace glitch::core {
quaternion quaternion::slerp(quaternion, quaternion, float) {
    std::abort();
}
quaternion quaternion::operator*(const quaternion&) const {
    std::abort();
}
}  // namespace glitch::core

namespace glitch::collada::tb_ext {
const float* accessorOutputKeys(const glitch::collada::SAnimationAccessor&) {
    std::abort();
}
}  // namespace glitch::collada::tb_ext

namespace {

using glitch::collada::animation_track::CVirtualEx;
using glitch::collada::animation_track::CApplyValueEx;
using PosVec3 = CApplyValueEx<glitch::core::vector3d<float>,
                              glitch::collada::animation_track::CSceneNodePositionMixin<float>>;
using QuatMixin = CApplyValueEx<glitch::core::quaternion,
                                glitch::collada::animation_track::CSceneNodeQuaternionMixin<float>>;

int failures = 0;

void check(bool condition, const char* what) {
    if (!condition) {
        std::printf("FAIL: %s\n", what);
        ++failures;
    }
}

bool near(float a, float b) {
    return std::fabs(a - b) < 1e-5f;
}

}  // namespace

int main() {
    const CVirtualEx<PosVec3> position{};
    const CVirtualEx<QuatMixin> quaternion{};

    // getValueSize: vec3 is 12 bytes, quaternion is 16 bytes.
    check(position.getValueSize() == 12, "position getValueSize is 12");
    check(quaternion.getValueSize() == 16, "quaternion getValueSize is 16");

    // getBlendedValue, count 1: copies the first vector without weighting it.
    {
        float values[3] = {1.0f, 2.0f, 3.0f};
        float weights[1] = {0.25f};
        float out[3] = {0.0f, 0.0f, 0.0f};
        position.getBlendedValue(values, weights, 1, out);
        check(near(out[0], 1.0f) && near(out[1], 2.0f) && near(out[2], 3.0f), "count 1 copies the first vector");
    }

    // getBlendedValue, count 2: weighted sum of the two vectors.
    {
        float values[6] = {1.0f, 0.0f, 0.0f, 0.0f, 2.0f, 4.0f};
        float weights[2] = {0.5f, 0.25f};
        float out[3] = {9.0f, 9.0f, 9.0f};
        position.getBlendedValue(values, weights, 2, out);
        // x = 0.5 * 1 + 0.25 * 0 = 0.5; y = 0.5 * 0 + 0.25 * 2 = 0.5; z = 0.5 * 0 + 0.25 * 4 = 1.0
        check(near(out[0], 0.5f) && near(out[1], 0.5f) && near(out[2], 1.0f), "count 2 weighted sum");
    }

    // getBlendedValue, count 0: writes zeros.
    {
        float values[3] = {5.0f, 5.0f, 5.0f};
        float weights[1] = {1.0f};
        float out[3] = {7.0f, 7.0f, 7.0f};
        position.getBlendedValue(values, weights, 0, out);
        check(near(out[0], 0.0f) && near(out[1], 0.0f) && near(out[2], 0.0f), "count 0 writes zeros");
    }

    if (failures != 0) {
        std::printf("%d check(s) failed\n", failures);
        return 1;
    }
    std::printf("e2-glitch-scene-collada-gui-pack2-52827d-0 test: all checks passed\n");
    return 0;
}
