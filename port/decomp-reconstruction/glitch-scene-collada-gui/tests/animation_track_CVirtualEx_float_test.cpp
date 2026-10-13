// Runs the float CVirtualEx bodies: getValueSize, getBlendedValue, and the applyBlendedValue forwarder.
//
// An earlier version of this test defined two external stand-ins (setParameterCvtFloat and
// applyBlendedValueExFloat) and expected the library to call them. The library never referenced those
// names: the callees they stood for are declared and defined elsewhere (CApplyValueEx<float>::applyBlendedValueEx,
// 0x62008c, in the library) or not mapped yet (the material setter setParameterCvt<float>, 0x5c6b8c in
// glitch-video), so the checks on them could not pass. Against the binary:
//   - 0x620118 applyBlendedValue is one tail call to the static CApplyValueEx<float>::applyBlendedValueEx with the
//     same five arguments. This test checks that forward against the callee itself.
//   - 0x618fe4 applyValue is a tail call to the material setter. The setter is not mapped, the body is partial,
//     and its result is not asserted here.
#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_float.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    using glitch::collada::animation_track::CVirtualEx;
    using glitch::collada::animation_track::FloatApplyValueEx;

    CVirtualEx<FloatApplyValueEx> track;
    CHECK(track.getValueSize() == 4);

    float values[3] = {1.0f, 2.0f, 3.0f};
    float weights[3] = {0.5f, 0.25f, 2.0f};
    float out = -1.0f;

    track.getBlendedValue(values, weights, 1, &out);
    CHECK(out == 1.0f);  // count 1 copies values[0], weights ignored

    track.getBlendedValue(values, weights, 0, &out);
    CHECK(out == 0.0f);  // count 0 gives zero

    track.getBlendedValue(values, weights, 2, &out);
    CHECK(out == 1.0f);  // 1*0.5 + 2*0.25

    track.getBlendedValue(values, weights, 3, &out);
    CHECK(out == 7.0f);  // 1*0.5 + 2*0.25 + 3*2

    // The forwarder returns what the static callee returns for the same arguments, and it does not write the
    // output buffer (the callee blends into a local and hands it to the material setter).
    out = -1.0f;
    const int forwarded = track.applyBlendedValue(values, weights, 3, &out, nullptr);
    const int direct = FloatApplyValueEx::applyBlendedValueEx(values, weights, 3, &out, nullptr);
    CHECK(forwarded == direct);
    CHECK(out == -1.0f);

    std::printf("%s\n", failures ? "animation_track_CVirtualEx_float: FAILED" : "animation_track_CVirtualEx_float: ok");
    return failures != 0;
}
