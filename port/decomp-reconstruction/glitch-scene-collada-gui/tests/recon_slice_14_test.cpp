// Slice 14 test: the value-combining helpers used by the applicator wrappers.
// The wrappers themselves call externals (accessor output, parameter setter,
// vtable dispatch) that are not linked here, so only the header helpers are run.
#include "glitch-scene-collada-gui/recon/slice_14.hpp"

#include <cstdio>

using namespace glitch::collada::recon_14;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

static bool near(float a, float b) {
    float d = a - b;
    return d < 0.0001f && d > -0.0001f;
}

int main() {
    // float[2] single key is copied, weight ignored.
    {
        const float values[2] = {1.5f, -2.0f};
        const float weights[1] = {9.0f};
        float out[2] = {0.0f, 0.0f};
        weightedSumFloat2(values, weights, 1, out);
        CHECK(near(out[0], 1.5f));
        CHECK(near(out[1], -2.0f));
    }
    // float[2] two keys: weighted sums per component.
    {
        const float values[4] = {1.0f, 2.0f, 3.0f, 4.0f};
        const float weights[2] = {0.5f, 0.25f};
        float out[2] = {0.0f, 0.0f};
        weightedSumFloat2(values, weights, 2, out);
        CHECK(near(out[0], 1.25f));
        CHECK(near(out[1], 2.0f));
    }
    // float[2] zero keys: zeros.
    {
        float out[2] = {7.0f, 7.0f};
        weightedSumFloat2(nullptr, nullptr, 0, out);
        CHECK(near(out[0], 0.0f));
        CHECK(near(out[1], 0.0f));
    }
    // unsigned char[4] single key is copied.
    {
        const std::uint8_t values[4] = {1, 2, 3, 4};
        const float weights[1] = {0.3f};
        std::uint8_t out[4] = {0, 0, 0, 0};
        addWeightedUChar4(values, weights, 1, out);
        CHECK(out[0] == 1 && out[1] == 2 && out[2] == 3 && out[3] == 4);
    }
    // unsigned char[4] two keys: (10,20,30,40)*0.5 + (30,40,50,60)*0.5.
    {
        const std::uint8_t values[8] = {10, 20, 30, 40, 30, 40, 50, 60};
        const float weights[2] = {0.5f, 0.5f};
        std::uint8_t out[4] = {0, 0, 0, 0};
        addWeightedUChar4(values, weights, 2, out);
        CHECK(out[0] == 20 && out[1] == 30 && out[2] == 40 && out[3] == 50);
    }
    // Vector3 scratch arithmetic: a 12-byte key view is three floats.
    CHECK(sizeof(float[3]) == 12);

    if (failures) {
        std::printf("%d failure(s)\n", failures);
        return 1;
    }
    std::printf("slice_14 helpers: ok\n");
    return 0;
}
