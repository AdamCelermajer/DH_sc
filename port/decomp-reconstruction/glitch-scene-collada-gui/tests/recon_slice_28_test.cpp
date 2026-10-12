// Tests for recon slice 28: the blend and visibility loops that have no virtual or
// accessor call. Everything else in the slice is a stub and is not exercised here.
#include <cstdint>
#include <cstdio>

#include "glitch-scene-collada-gui/recon/slice_28.hpp"

namespace anim = glitch::collada::recon_28::animation_track;

static int failures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                \
        }                                                              \
    } while (0)

static bool near(float a, float b) {
    const float d = a - b;
    return d < 1e-5f && d > -1e-5f;
}

int main() {
    // CFloatEx::getBlendedValue: 1*0.5 + 2*1 + 3*2 = 8.5
    {
        float values[3] = {1.0f, 2.0f, 3.0f};
        float weights[3] = {0.5f, 1.0f, 2.0f};
        float out = -1.0f;
        float r = anim::CFloatEx::getBlendedValue(nullptr, values, weights, 3, &out);
        CHECK(near(r, 8.5f));
        CHECK(near(out, 8.5f));
        out = -1.0f;
        r = anim::CFloatEx::getBlendedValue(nullptr, values, weights, 0, &out);
        CHECK(near(r, 0.0f));
        CHECK(near(out, 0.0f));
    }

    // CFloatEx::getAddedValue and CWeightEx::getAddedValue share the same loop.
    {
        float values[2] = {4.0f, -1.0f};
        float weights[2] = {0.25f, 2.0f};
        float out = 0.0f;
        float r = anim::CFloatEx::getAddedValue(nullptr, values, weights, 2, &out);
        CHECK(near(r, -1.0f));
        CHECK(near(out, -1.0f));
    }

    // CWeightEx::applyBlendedValue ignores the applicator and returns the same sum.
    {
        float values[2] = {2.0f, 3.0f};
        float weights[2] = {1.0f, 1.0f};
        float out = 0.0f;
        float r = anim::CWeightEx::applyBlendedValue(nullptr, values, weights, 2, &out, nullptr);
        CHECK(near(r, 5.0f));
        CHECK(near(out, 5.0f));
    }

    // CVector3dEx::getAddedValue: weighted sum of xyz triples.
    {
        float vectors[6] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
        float weights[2] = {1.0f, 0.5f};
        float out[3] = {9.0f, 9.0f, 9.0f};
        anim::CVector3dEx::getAddedValue(nullptr, vectors, weights, 2, out);
        CHECK(near(out[0], 3.0f));
        CHECK(near(out[1], 4.5f));
        CHECK(near(out[2], 6.0f));
    }

    // CVisibilityEx::getBlendedValue: visible only if every weighted flag is set.
    {
        std::uint32_t flags_a[3] = {1, 1, 0};
        float w_a[3] = {1.0f, 0.0f, 1.0f};
        std::uint32_t out = 9;
        anim::CVisibilityEx::getBlendedValue(nullptr, flags_a, w_a, 3, &out);
        CHECK(out == 0);

        std::uint32_t flags_b[3] = {1, 1, 0};
        float w_b[3] = {1.0f, 0.0f, 0.0f};  // the unset flag has zero weight
        out = 9;
        anim::CVisibilityEx::getBlendedValue(nullptr, flags_b, w_b, 3, &out);
        CHECK(out == 1);
    }

    // CVisibilityEx::getAddedValue: visible if any weighted flag is set.
    {
        std::uint32_t flags_a[3] = {0, 1, 0};
        float w_a[3] = {0.0f, 1.0f, 1.0f};
        std::uint32_t out = 9;
        anim::CVisibilityEx::getAddedValue(nullptr, flags_a, w_a, 3, &out);
        CHECK(out == 1);

        std::uint32_t flags_b[3] = {0, 0, 0};
        float w_b[3] = {1.0f, 1.0f, 1.0f};
        out = 9;
        anim::CVisibilityEx::getAddedValue(nullptr, flags_b, w_b, 3, &out);
        CHECK(out == 0);
    }

    if (failures != 0) {
        std::printf("%d failure(s)\n", failures);
        return 1;
    }
    std::printf("recon_slice_28 tests passed\n");
    return 0;
}
