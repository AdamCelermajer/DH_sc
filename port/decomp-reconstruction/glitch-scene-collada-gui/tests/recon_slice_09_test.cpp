// Tests for slice 09 (animation_track key-based getters). Externals are stubbed here.
#include "glitch-scene-collada-gui/recon/slice_09.hpp"

#include <cmath>
#include <cstdio>

using namespace glitch::collada::recon_09;

namespace glitch::collada::recon_09::ext {

// Hamilton product, components ordered x y z w.
void quaternion_multiply(float out[4], const float a[4], const float b[4]) {
    out[0] = a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1];
    out[1] = a[3] * b[1] - a[0] * b[2] + a[1] * b[3] + a[2] * b[0];
    out[2] = a[3] * b[2] + a[0] * b[1] - a[1] * b[0] + a[2] * b[3];
    out[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];
}

void quaternion_slerp(float out[4], const float from[4], const float to[4], float t) {
    for (int c = 0; c < 4; ++c) out[c] = from[c] * (1.0f - t) + to[c] * t;
}

float applicator_light_color_applyValueEx(CApplicatorInfo*, const std::uint8_t* value) {
    return static_cast<float>(value[0]);
}

bool interp_quat_float_getKeyBasedValueEx(SAnimationAccessor*, int, int, float, float*) { return false; }

bool blender_quat_getBlendedValueEx(const void* values, const float* weights, int count, void* out) {
    const float* v = static_cast<const float*>(values);
    float* o = static_cast<float*>(out);
    for (int c = 0; c < 4; ++c) {
        float sum = 0.0f;
        for (int k = 0; k < count; ++k) sum += weights[k] * v[4 * k + c];
        o[c] = sum;
    }
    return true;
}

void* blender_quat_getAddedValueEx(const void*, const float*, int, void* out) { return out; }

}  // namespace glitch::collada::recon_09::ext

namespace {

int g_failures = 0;

#define CHECK(cond)                                                       \
    do {                                                                  \
        if (!(cond)) {                                                    \
            std::printf("FAIL line %d: %s\n", __LINE__, #cond);           \
            ++g_failures;                                                 \
        }                                                                 \
    } while (0)

bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

}  // namespace

int main() {
    using namespace glitch::collada::recon_09;

    // R1 (0x6126c8): blend key 0 and key 1 at t = 0.5.
    {
        const std::uint8_t data[6] = {0, 100, 200, 100, 200, 50};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        std::uint8_t dst[3] = {};
        const unsigned r = interp_light_u8x3::getKeyBasedValueEx(&acc, 0, 0, 0.5f, dst);
        CHECK(dst[0] == 50 && dst[1] == 150 && dst[2] == 125);
        CHECK(r == 125);
    }

    // R5 (0x612820): out = key a3 - key a2 with byte wrap.
    {
        const std::uint8_t data[6] = {10, 20, 30, 15, 25, 10};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        std::uint8_t dst[3] = {};
        SAnimationOutput* ret = virtual_light_u8x3::getKeyBasedValue(&acc, 0, 1, dst);
        CHECK(ret == &out);
        CHECK(dst[0] == 5 && dst[1] == 5 && dst[2] == 236);
    }

    // R6 (0x612874): blend(key a3, key a4, t) - key a2.
    {
        const std::uint8_t data[9] = {10, 10, 10, 0, 0, 0, 100, 100, 100};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        std::uint8_t dst[3] = {};
        const unsigned r = interp_light_u8x3::getKeyBasedValueEx(&acc, 0, 1, 2, 0.5f, dst);
        CHECK(dst[0] == 40 && dst[1] == 40 && dst[2] == 40);
        CHECK(r == 50);  // returns the last blended component, not the stored delta
    }

    // R2 (0x6127a4): applicator receives the blended value (stub returns value[0]).
    {
        const std::uint8_t data[6] = {10, 0, 0, 0, 0, 0};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        CHECK(near(apply_light_u8x3::applyKeyBasedValueEx(&acc, 0, 1, 0.0f, nullptr), 10.0f));
    }

    // R13 (0x612ab0): out = key a3 - key a2 for two floats; returns y difference.
    {
        const float data[4] = {1.0f, 2.0f, 4.0f, 8.0f};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        float dst[2] = {};
        const float r = virtual_float2::getKeyBasedValue(&acc, 0, 1, dst);
        CHECK(near(dst[0], 3.0f) && near(dst[1], 6.0f));
        CHECK(near(r, 6.0f));
    }

    // R14 (0x612afc): (k3 + t*(k4 - k3)) - k2 per component.
    {
        const float data[6] = {1.0f, 1.0f, 2.0f, 4.0f, 4.0f, 8.0f};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        float dst[2] = {};
        const float r = interp_float2::getKeyBasedValueEx(&acc, 0, 1, 2, 0.5f, dst);
        CHECK(near(dst[0], 2.0f) && near(dst[1], 5.0f));
        CHECK(near(r, 5.0f));
    }

    // Reader-based position/scale tests use scales = 1 and offsets = 0.
    const float unit3[3] = {1.0f, 1.0f, 1.0f};
    const float zero3[3] = {0.0f, 0.0f, 0.0f};

    // R55 (0x613efc): out = key a3 - key a2 for 3-component shorts.
    {
        const std::int16_t data[6] = {1, 2, 3, 4, 6, 8};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        acc.scales = unit3;
        acc.offsets = zero3;
        float dst[3] = {};
        const float r = interp_pos_f3_short::getKeyBasedValueEx(&acc, 0, 1, dst);
        CHECK(near(dst[0], 3.0f) && near(dst[1], 4.0f) && near(dst[2], 5.0f));
        CHECK(near(r, 5.0f));
    }

    // R57 (0x613fec): (k3 + t*(k4 - k3)) - k2 with k2 = 0, k3 = 10, k4 = 20, t = 0.5.
    {
        const std::int16_t data[9] = {0, 0, 0, 10, 10, 10, 20, 20, 20};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        acc.scales = unit3;
        acc.offsets = zero3;
        float dst[3] = {};
        interp_pos_f3_short::getKeyBasedValueEx(&acc, 0, 1, 2, 0.5f, dst);
        CHECK(near(dst[0], 15.0f) && near(dst[1], 15.0f) && near(dst[2], 15.0f));
    }

    // R39 (0x6138f4): blend of two quaternion keys through the blender stub.
    {
        const std::int16_t data[8] = {1, 0, 0, 0, 0, 0, 0, 0};
        const float unit4[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        const float zero4[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        acc.scales = unit4;
        acc.offsets = zero4;
        float dst[4] = {};
        CHECK(interp_quat_short::getKeyBasedValueEx(&acc, 0, 1, 0.25f, dst));
        CHECK(near(dst[0], 0.75f));
    }

    // R35 (0x6136e0): conj(identity) * q = q.
    {
        const std::int16_t data[8] = {0, 0, 0, 1, 1, 2, 3, 4};
        const float unit4[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        const float zero4[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        acc.scales = unit4;
        acc.offsets = zero4;
        float dst[4] = {};
        const float r = interpq_quat_short::getKeyBasedValueEx(&acc, 0, 1, dst);
        CHECK(near(dst[0], 1.0f) && near(dst[1], 2.0f) && near(dst[2], 3.0f) && near(dst[3], 4.0f));
        CHECK(near(r, 1.0f));
    }

    // R78 (0x614a90): default present, components 0..2 from default, component 3 animated.
    {
        const std::int16_t data[1] = {5};
        const float def[3] = {7.0f, 8.0f, 9.0f};
        const float scale1[1] = {1.0f};
        const float offset0[1] = {0.0f};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        acc.scales = scale1;
        acc.offsets = offset0;
        acc.defaultValue = def;
        float dst[4] = {};
        const float* ret = interp_quat_angle_short::getKeyBasedValueEx(&acc, 0, dst);
        CHECK(ret == def);
        CHECK(near(dst[0], 7.0f) && near(dst[1], 8.0f) && near(dst[2], 9.0f) && near(dst[3], 5.0f));
    }

    // R82 (0x614dec) without default: only component 0 written, null returned.
    {
        const std::int16_t data[1] = {4};
        const float scale1[1] = {2.0f};
        const float offset0[1] = {1.0f};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        acc.scales = scale1;
        acc.offsets = offset0;
        float dst[3] = {-1.0f, -1.0f, -1.0f};
        const float* ret = interp_pos_x_short::getKeyBasedValueEx(&acc, 0, dst);
        CHECK(ret == nullptr);
        CHECK(near(dst[0], 9.0f) && near(dst[1], -1.0f) && near(dst[2], -1.0f));
    }

    // R88 (0x6150ac) without default: v3 = v(a3) - v(a2), v4 = v(a4) - v(a2), result = v3 + t*(v4 - v3).
    {
        const std::int16_t data[3] = {0, 2, 6};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        acc.scales = unit3;
        acc.offsets = zero3;
        float dst[3] = {};
        const float r = interp_pos_x_short::getKeyBasedValueEx(&acc, 0, 1, 2, 0.5f, dst);
        CHECK(near(dst[0], 4.0f));
        CHECK(near(r, 4.0f));
    }

    // R98 (0x61562c) with default: out = {def[0], value, def[2]}.
    {
        const std::int16_t data[1] = {4};
        const float def[3] = {1.0f, 2.0f, 3.0f};
        const float scale1[1] = {1.0f};
        const float offset0[1] = {0.0f};
        SAnimationOutput out{0, data};
        SAnimationAccessor acc;
        acc.output = &out;
        acc.scales = scale1;
        acc.offsets = offset0;
        acc.defaultValue = def;
        float dst[3] = {};
        const float* ret = interp_pos_y_short::getKeyBasedValueEx(&acc, 0, dst);
        CHECK(ret == def);
        CHECK(near(dst[0], 1.0f) && near(dst[1], 4.0f) && near(dst[2], 3.0f));
    }

    if (g_failures == 0) {
        std::printf("recon_slice_09_test: all checks passed\n");
        return 0;
    }
    std::printf("recon_slice_09_test: %d failure(s)\n", g_failures);
    return 1;
}
