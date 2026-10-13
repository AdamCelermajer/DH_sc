// Test for the wave 08 animation_track helpers. The rows are the data of output record 0 of the canonical SAnimationAccessor.

#include <cstdio>

#include "glitch-scene-collada-gui/recon/v2/wave_08.hpp"

using namespace glitch::collada;
using namespace glitch::collada::recon_v2_08;

static int failures = 0;
#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                \
        }                                                              \
    } while (0)

int main() {
    // Rows: row0 = (10,20,30), row1 = (50,60,70), row2 = (100,100,100).
    SLightColorRow rows[3] = {{{10, 20, 30}}, {{50, 60, 70}}, {{100, 100, 100}}};

    // Both keyframes use output record 0.
    SAnimationKeyframe keyframes[2] = {};
    SAnimationOutput outputs[1] = {};
    outputs[0].count = 3;
    outputs[0].data = rows;
    SAnimation animation;
    animation.keyframes = keyframes;

    SAnimationAccessor acc(&animation, outputs, nullptr);

    // getOutput returns the output record of keyframe 0.
    CHECK(acc.getOutput(0) == &outputs[0]);

    // 0x6126c8: blend rows 0 and 1 at t = 0.5 -> (30, 40, 50); returns component 2.
    std::uint8_t out1[3] = {0, 0, 0};
    std::uint32_t r1 = CInterpreter_SLightColor_getKeyBasedValueEx(acc, 0, 0, 0.5f, out1);
    CHECK(out1[0] == 30 && out1[1] == 40 && out1[2] == 50);
    CHECK(r1 == 50);

    // 0x6126c8 at t = 0 returns row 0 exactly.
    std::uint8_t out1b[3] = {0, 0, 0};
    CInterpreter_SLightColor_getKeyBasedValueEx(acc, 0, 0, 0.0f, out1b);
    CHECK(out1b[0] == 10 && out1b[1] == 20 && out1b[2] == 30);

    // 0x612820: row a4 minus row a3 with a3 = 0, a4 = 1 -> (40, 40, 40); returns the output record.
    std::uint8_t out2[3] = {0, 0, 0};
    const SAnimationOutput* r2 = CVirtualEx_CLightColor_getKeyBasedValue_delta(acc, 0, 1, out2);
    CHECK(out2[0] == 40 && out2[1] == 40 && out2[2] == 40);
    CHECK(r2 == &outputs[0]);

    // 0x612874: lerp row a3 (0) toward row a4 (1) at t = 0.5 -> (30, 40, 50), minus row a2 (0) -> (20, 20, 20).
    std::uint8_t out3[3] = {0, 0, 0};
    std::uint32_t r3 = CInterpreter_SLightColor_getKeyBasedValueEx_offset(acc, 0, 0, 1, 0.5f, out3);
    CHECK(out3[0] == 20 && out3[1] == 20 && out3[2] == 20);
    CHECK(r3 == 50);

    // 0x612874 with a2 = 1 (row 1 = (50,60,70)) subtracted from lerp (row 0 -> row 2 at t = 0.5 -> (55,60,65)).
    std::uint8_t out4[3] = {0, 0, 0};
    CInterpreter_SLightColor_getKeyBasedValueEx_offset(acc, 1, 0, 2, 0.5f, out4);
    CHECK(out4[0] == 5 && out4[1] == 0 && out4[2] == 0xFB);  // 65 - 70 wraps to 0xFB

    if (failures == 0) {
        std::printf("wave_08 test: all checks passed\n");
        return 0;
    }
    std::printf("wave_08 test: %d failure(s)\n", failures);
    return 1;
}
