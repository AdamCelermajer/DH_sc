// Checks for batch e2-glitch-scene-collada-gui-pack0-397dcd-0: the ScaleY CVirtualEx arithmetic and getInstance,
// and the CCameraSceneNode constructor member writes. Returns non-zero on failure.
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "glitch-scene-collada-gui/scene_nodes.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CVirtualEx_ScaleYVec3f.hpp"

using glitch::collada::animation_track::CApplyValueEx;
using glitch::collada::animation_track::CSceneNodeScaleComponentMixin;
using glitch::collada::animation_track::CSceneNodeScaleYEx;
using glitch::collada::animation_track::CVirtualEx;
using glitch::core::vector3d;

namespace {

using ScaleYTrack = CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<float>, 1, float>>>;

int failures = 0;

void check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++failures;
    }
}

bool nearly(float a, float b) {
    return std::fabs(a - b) < 1e-5f;
}

std::uint32_t bitsOf(float f) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &f, sizeof bits);
    return bits;
}

}  // namespace

int main() {
    // getValueSize is three floats.
    check(ScaleYTrack{}.getValueSize() == 12, "ScaleY getValueSize is 12");

    // getInstance returns the same function-local instance on every call.
    check(ScaleYTrack::getInstance() != nullptr, "ScaleY getInstance is not null");
    check(ScaleYTrack::getInstance() == ScaleYTrack::getInstance(), "ScaleY getInstance is a singleton");

    // getBlendedValue: count 1 copies the first vector, weights unused.
    const float values[6] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    const float weights[2] = {0.5f, 0.25f};
    float out[3] = {9.0f, 9.0f, 9.0f};
    ScaleYTrack{}.getBlendedValue(values, weights, 1, out);
    check(nearly(out[0], 1.0f) && nearly(out[1], 2.0f) && nearly(out[2], 3.0f), "ScaleY getBlendedValue count 1 copies");

    // count 2: weighted sum, x = 0.5*1 + 0.25*4 etc.
    ScaleYTrack{}.getBlendedValue(values, weights, 2, out);
    check(nearly(out[0], 0.5f * 1.0f + 0.25f * 4.0f), "ScaleY getBlendedValue x sum");
    check(nearly(out[1], 0.5f * 2.0f + 0.25f * 5.0f), "ScaleY getBlendedValue y sum");
    check(nearly(out[2], 0.5f * 3.0f + 0.25f * 6.0f), "ScaleY getBlendedValue z sum");

    // count 0 writes zero.
    ScaleYTrack{}.getBlendedValue(values, weights, 0, out);
    check(nearly(out[0], 0.0f) && nearly(out[1], 0.0f) && nearly(out[2], 0.0f), "ScaleY getBlendedValue count 0 is zero");

    // CCameraSceneNode constructor writes its own members.
    const vector3d<float> position{10.0f, 20.0f, 30.0f};
    const vector3d<float> lookAt{1.0f, 2.0f, 3.0f};
    glitch::scene::CCameraSceneNode camera(7, position, lookAt, true);
    // The id and the position now reach the ISceneNode base constructor.
    check(camera.getID() == 7, "camera id forwarded to the ISceneNode base");
    check(camera.getPosition().X == 10.0f && camera.getPosition().Y == 20.0f && camera.getPosition().Z == 30.0f,
          "camera position forwarded to the ISceneNode base");
    check(camera.target.X == 1.0f && camera.target.Y == 2.0f && camera.target.Z == 3.0f, "camera target copied");
    check(camera.upVector.X == 0.0f && camera.upVector.Y == 1.0f && camera.upVector.Z == 0.0f, "camera up vector is (0,1,0)");
    check(bitsOf(camera.fov) == 0x3FA0D97Cu, "camera fov bits 0x3FA0D97C");
    check(bitsOf(camera.aspectRatio) == 0x3FAAAAABu, "camera aspect bits 0x3FAAAAAB");
    check(camera.nearValue == 1.0f, "camera near value 1.0");
    check(camera.farValue == 2048.0f, "camera far value 2048.0");
    check(camera.farToInfinity == true, "camera farToInfinity from argument");
    check(camera.inputReceiverEnabled == true, "camera input receiver enabled");
    check(camera.mag == 0.0f, "camera mag is not written by the constructor");

    if (failures == 0) {
        std::printf("e2 test passed\n");
    }
    return failures == 0 ? 0 : 1;
}
