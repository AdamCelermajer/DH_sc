// Checks for the slice 16 animation_track applicator helpers.
#include "glitch-scene-collada-gui/recon/slice_16.hpp"

#include <cmath>
#include <cstdio>

using namespace glitch::collada::recon_16;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main() {
    // lerpKeyPair3: t = 0 gives k0, t = 1 gives k1, t = 0.5 gives the midpoint.
    const float k0[3] = {1.0f, 2.0f, 3.0f};
    const float k1[3] = {5.0f, 6.0f, 7.0f};
    float out[3];
    CHECK(near(lerpKeyPair3(k0, k1, 0.0f, out), 1.0f) && near(out[1], 2.0f) && near(out[2], 3.0f));
    lerpKeyPair3(k0, k1, 1.0f, out);
    CHECK(near(out[0], 5.0f) && near(out[1], 6.0f) && near(out[2], 7.0f));
    CHECK(near(lerpKeyPair3(k0, k1, 0.5f, out), 3.0f) && near(out[1], 4.0f) && near(out[2], 5.0f));

    // Quantized char keys: key0 = {2,4,6}, key1 = {10,0,-2}; scale 0.5, bias 1.
    const signed char keys[6] = {2, 4, 6, 10, 0, -2};
    const float scale[3] = {0.5f, 0.5f, 0.5f};
    const float bias[3] = {1.0f, 1.0f, 1.0f};
    SKeyInputQuantized<signed char> in{keys, scale, bias};
    // key0 -> {2,3,4}; key1 -> {6,1,0}; t = 0.5 -> {4,2,2}.
    interpolateQuantized3(in, 0, 1, 0.5f, out);
    CHECK(near(out[0], 4.0f) && near(out[1], 2.0f) && near(out[2], 2.0f));

    // Raw float keys: key0 = k0, key1 = k1 packed as 3-element keys.
    const float raw[6] = {1.0f, 2.0f, 3.0f, 5.0f, 6.0f, 7.0f};
    SKeyInputFloat fin{raw};
    interpolateFloat3(fin, 0, 1, 1.0f, out);
    CHECK(near(out[0], 5.0f) && near(out[2], 7.0f));

    // blendVec3: count 1 copies the key ignoring the weight; count 2 is a weighted sum.
    const float weights[2] = {0.25f, 0.5f};
    const float two[6] = {4.0f, 8.0f, 12.0f, 2.0f, 2.0f, 2.0f};
    blendVec3(two, weights, 1, out);
    CHECK(near(out[0], 4.0f) && near(out[1], 8.0f) && near(out[2], 12.0f));
    blendVec3(two, weights, 2, out);
    CHECK(near(out[0], 2.0f) && near(out[1], 3.0f) && near(out[2], 4.0f));
    blendVec3(two, weights, 0, out);
    CHECK(near(out[0], 0.0f) && near(out[1], 0.0f) && near(out[2], 0.0f));

    // A getBlendedValue instantiation forwards to blendVec3.
    getBlendedValue_ScaleX_char(two, weights, 2, out);
    CHECK(near(out[0], 2.0f) && near(out[1], 3.0f));

    if (failures == 0) std::printf("recon_slice_16_test: OK\n");
    return failures == 0 ? 0 : 1;
}
