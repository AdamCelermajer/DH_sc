// Runs the float CVirtualEx bodies (getValueSize, getBlendedValue, and the forwarders). The two
// external stand-ins are defined here so the test links without the engine.
#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"

#include <cstdint>
#include <cstdio>

namespace glitch::collada::animation_track {

namespace tb_standin {
namespace {
int g_set_calls = 0;
void* g_set_target = nullptr;
std::uint16_t g_set_index = 99;
unsigned g_set_flags = 99;
const float* g_set_value = nullptr;

int g_blend_calls = 0;
void* g_blend_values = nullptr;
float* g_blend_weights = nullptr;
int g_blend_count = -1;
void* g_blend_target = nullptr;
CApplicatorInfo* g_blend_info = nullptr;
}  // namespace

int setParameterCvtFloat(void* target, std::uint16_t index, unsigned flags, const float* value) {
    ++g_set_calls;
    g_set_target = target;
    g_set_index = index;
    g_set_flags = flags;
    g_set_value = value;
    return 7;
}

int applyBlendedValueExFloat(void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    ++g_blend_calls;
    g_blend_values = values;
    g_blend_weights = weights;
    g_blend_count = count;
    g_blend_target = target;
    g_blend_info = info;
    return 42;
}
}  // namespace tb_standin

}  // namespace glitch::collada::animation_track

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    using glitch::collada::animation_track::CVirtualEx;
    using glitch::collada::animation_track::FloatApplyValueEx;
    namespace standin = glitch::collada::animation_track::tb_standin;

    CVirtualEx<FloatApplyValueEx> track;
    CHECK(track.getValueSize() == 4);

    float values[3] = {1.0f, 2.0f, 3.0f};
    float weights[3] = {0.5f, 0.25f, 2.0f};
    float out = -1.0f;

    track.getBlendedValue(values, weights, 1, &out);
    CHECK(out == 1.0f);  // count 1 copies values[0], weights ignored

    track.getBlendedValue(values, weights, 0, &out);
    CHECK(out == 0.0f);  // count 0 gives zero

    track.getBlendedValue(values, weights, 3, &out);
    CHECK(out == 7.0f);  // 1*0.5 + 2*0.25 + 3*2

    int blended = track.applyBlendedValue(values, weights, 3, &out, nullptr);
    CHECK(blended == 42);
    CHECK(standin::g_blend_calls == 1);
    CHECK(standin::g_blend_values == values);
    CHECK(standin::g_blend_weights == weights);
    CHECK(standin::g_blend_count == 3);
    CHECK(standin::g_blend_target == &out);
    CHECK(standin::g_blend_info == nullptr);

    int set = track.applyValue(values, &out, nullptr);
    CHECK(set == 7);
    CHECK(standin::g_set_calls == 1);
    CHECK(standin::g_set_target == &out);
    CHECK(standin::g_set_index == 0);
    CHECK(standin::g_set_flags == 0u);
    CHECK(standin::g_set_value == values);

    std::printf("%s\n", failures ? "animation_track_CVirtualEx_float: FAILED" : "animation_track_CVirtualEx_float: ok");
    return failures != 0;
}
