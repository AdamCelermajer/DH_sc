// Checks the reconstructed float CVirtualEx bodies: getValueSize and getBlendedValue.
// applyValue and applyBlendedValue are partial and are not tested here.
#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"

#include <cstdio>

static int failures = 0;

#define CHECK(c)                                                     \
    do {                                                             \
        if (!(c)) {                                                  \
            std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); \
            ++failures;                                              \
        }                                                            \
    } while (0)

int main() {
    using glitch::collada::animation_track::CVirtualEx;
    using glitch::collada::animation_track::FloatApplyValueEx;

    CVirtualEx<FloatApplyValueEx> track;
    CHECK(track.getValueSize() == 4);

    float values[3] = {1.0f, 2.0f, 3.0f};
    float weights[3] = {0.5f, 0.25f, 2.0f};
    float out = -1.0f;

    track.getBlendedValue(values, weights, 1, &out);
    CHECK(out == 1.0f);  // count 1 copies values[0]; weights not read

    track.getBlendedValue(values, weights, 0, &out);
    CHECK(out == 0.0f);  // count 0 gives 0.0

    track.getBlendedValue(values, weights, 3, &out);
    CHECK(out == 7.0f);  // 1*0.5 + 2*0.25 + 3*2

    track.getBlendedValue(values, weights, 2, &out);
    CHECK(out == 1.0f);  // 1*0.5 + 2*0.25

    if (failures == 0) {
        std::printf("animation_track_CVirtualEx_float_blend_test OK\n");
        return 0;
    }
    return 1;
}
