// Test for the batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreter_glit-0.
// Stubs the two external callees so the forwarding logic runs without the engine.
// Build: bash tools/trackb/check_tu.sh --test <this file> <batch .cpp>

#include <cmath>
#include <cstdio>
#include <cstdint>

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_QuatFloatLerp.hpp"

namespace {

float g_keys[8] = {0, 1, 2, 3, 4, 5, 6, 7};  // two quaternion keys
glitch::collada::SAnimationOutput g_output{0, g_keys};

const void* g_seenValues = nullptr;
float g_seenWeights[2] = {0, 0};
int g_seenCount = 0;
void* g_seenOut = nullptr;
bool g_blenderResult = true;
int g_getOutputIndex = -1;

}  // namespace

namespace glitch::collada {
struct SAnimationAccessor {};  // local stub; the test never reads its members

namespace tb_ext {
const SAnimationOutput* accessorGetOutput(const SAnimationAccessor&, int index) {
    g_getOutputIndex = index;
    return &g_output;
}

bool quatBlenderGetBlendedValueEx(const void* values, const float* weights, int count, void* out) {
    g_seenValues = values;
    g_seenWeights[0] = weights[0];
    g_seenWeights[1] = weights[1];
    g_seenCount = count;
    g_seenOut = out;
    return g_blenderResult;
}
}  // namespace tb_ext
}  // namespace glitch::collada

int main() {
    using glitch::collada::animation_track::CInterpreter;
    using glitch::collada::animation_track::CSceneNodeQuaternionMixin;
    using glitch::collada::animation_track::SUseDefaultLerp;
    using Interp = CInterpreter<CSceneNodeQuaternionMixin<float>, float, 4, SUseDefaultLerp<float>>;

    int failures = 0;
    glitch::collada::SAnimationAccessor accessor;
    float outBuf[4] = {0, 0, 0, 0};
    Interp interp;

    // key 1, t = 0.25: values point at the second quaternion (16 bytes in), weights {0.75, 0.25}.
    bool r = interp.getKeyBasedValueEx(accessor, 1, 99, 0.25f, outBuf);
    if (!r || g_getOutputIndex != 0 || g_seenCount != 2 || g_seenOut != outBuf ||
        g_seenValues != static_cast<const void*>(&g_keys[4]) ||
        std::fabs(g_seenWeights[0] - 0.75f) > 1e-6f || std::fabs(g_seenWeights[1] - 0.25f) > 1e-6f) {
        std::printf("FAIL: key 1, t 0.25\n");
        ++failures;
    }

    // key 0, t = 0: all weight on the first key; blender return value passes through.
    g_blenderResult = false;
    r = interp.getKeyBasedValueEx(accessor, 0, 0, 0.0f, outBuf);
    if (r != false || g_seenValues != static_cast<const void*>(&g_keys[0]) ||
        g_seenWeights[0] != 1.0f || g_seenWeights[1] != 0.0f) {
        std::printf("FAIL: key 0, t 0\n");
        ++failures;
    }

    if (failures == 0) {
        std::printf("tb_CInterpreter_QuatFloatLerp_test: OK\n");
    }
    return failures == 0 ? 0 : 1;
}
