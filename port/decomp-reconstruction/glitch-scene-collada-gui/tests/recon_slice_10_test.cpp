// Exercises the slice 10 key-based value paths: interpreter shapes, the CVirtualEx
// forwarders, the color delta, the identity fill, the quaternion-angle blends and the
// applicator. The quaternion helpers are test doubles; their real bodies are not in this slice.
#include "glitch-scene-collada-gui/recon/slice_10.hpp"

#include <cstdio>
#include <cstring>

using namespace glitch::collada::recon_10;

namespace glitch::collada::recon_10 {
float* quaternionFromAngleAxis(float* out, float angle, const float* axis) {
    out[0] = axis[0];
    out[1] = axis[1];
    out[2] = axis[2];
    out[3] = angle;
    return out;
}
void quaternionSlerp(float* out, const float* from, const float* to, float t) {
    for (int i = 0; i < 4; ++i) out[i] = from[i] + t * (to[i] - from[i]);
}
void quaternionMultiply(float* out, const float* lhs, const float* rhs) {
    for (int i = 0; i < 4; ++i) out[i] = lhs[i] + rhs[i];
}
}  // namespace glitch::collada::recon_10

namespace {

struct RecordingApplicator : CApplicatorInfo {
    float last[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    int calls = 0;
    void applyValue(const float* value) override {
        std::memcpy(last, value, sizeof last);
        ++calls;
    }
};

std::uint32_t wordOf(float value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof bits);
    return bits;
}

float asFloat(std::uint32_t bits) {
    float value = 0.0f;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

}  // namespace

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)
#define NEAR(a, b) (((a) - (b) < 1e-4f) && ((b) - (a) < 1e-4f))

int main() {
    // Short keys 10, 20, 30 with scale 2 and offset 1 read as 21, 41, 61.
    const short shortKeys[3] = {10, 20, 30};
    const float scale = 2.0f;
    const float offset = 1.0f;
    SKeyTable shortTable;
    shortTable.keys = shortKeys;
    SAnimationAccessor acc;
    acc.reader.table = &shortTable;
    acc.reader.scale = &scale;
    acc.reader.offset = &offset;

    {
        // Two-key delta (CInterpreter, B shape): 61 - 21 = 40 in word 0 without a default.
        std::uint32_t out[3] = {0, 0, 0};
        const std::uint32_t* def = CInterpreter_PosY_short_getKeyBasedValueEx_615808(acc, 0, 2, out);
        CHECK(def == nullptr);
        CHECK(NEAR(asFloat(out[0]), 40.0f));
        // The CVirtualEx forwarder for the same instance returns the same value.
        std::uint32_t wrapped[3] = {0, 0, 0};
        CVirtualEx_PosY_short_getKeyBasedValue_6158d8(acc, 0, 2, wrapped);
        CHECK(wrapped[0] == out[0]);
    }
    {
        // With a default (7, 8, 9): Y component (word 1) takes the value, the rest copy the default.
        std::uint32_t defaults[3] = {wordOf(7.0f), wordOf(8.0f), wordOf(9.0f)};
        acc.defaultValue = defaults;
        std::uint32_t out[3] = {0, 0, 0};
        const std::uint32_t* def = CInterpreter_PosY_short_getKeyBasedValueEx_615808(acc, 0, 2, out);
        CHECK(def == defaults);
        CHECK(NEAR(asFloat(out[0]), 7.0f));
        CHECK(NEAR(asFloat(out[1]), 40.0f));
        CHECK(NEAR(asFloat(out[2]), 9.0f));
        acc.defaultValue = nullptr;
    }
    {
        // Three-key blend (CInterpreter, D shape): origin 21, begin 41 - 21 = 20, end 61 - 21 = 40,
        // t = 0.5 gives 30.
        std::uint32_t out[3] = {0, 0, 0};
        const float value = CInterpreter_PosY_short_getKeyBasedValueEx_6158ec(acc, 0, 1, 2, 0.5f, out);
        CHECK(NEAR(value, 30.0f));
        CHECK(NEAR(asFloat(out[0]), 30.0f));
    }

    {
        // Char keys 10 and -5 (CInterpreter, A shape): -5 * 2 + 1 = -9.
        const char charKeys[2] = {10, -5};
        SKeyTable charTable;
        charTable.keys = charKeys;
        SAnimationAccessor charAcc;
        charAcc.reader.table = &charTable;
        charAcc.reader.scale = &scale;
        charAcc.reader.offset = &offset;
        std::uint32_t out[3] = {0, 0, 0};
        CInterpreter_PosY_char_getKeyBasedValueEx_615a5c(charAcc, 1, out);
        CHECK(NEAR(asFloat(out[0]), -9.0f));
    }

    {
        // Byte colour delta: 250 - 10 = 240, truncated to char gives -16 (0xF0) in byte 3.
        const std::uint8_t colourKeys[2] = {10, 250};
        SKeyTable colourTable;
        colourTable.keys = colourKeys;
        SAnimationAccessor colourAcc;
        colourAcc.output = &colourTable;
        std::uint32_t defaults[1] = {0x04030201u};
        colourAcc.defaultValue = defaults;
        std::uint8_t out[4] = {0, 0, 0, 0};
        const std::uint32_t* def = CInterpreter_Color_uchar_getKeyBasedValueEx_617fdc(colourAcc, 0, 1, out);
        CHECK(def == defaults);
        CHECK(out[0] == 0x01 && out[1] == 0x02 && out[2] == 0x03);
        CHECK(out[3] == 0xF0);
    }

    {
        std::uint32_t identity[4] = {9u, 9u, 9u, 9u};
        CVirtualEx_Float4_c0_getIdentityValue_618e68(identity);
        CHECK(identity[0] == 0u && identity[1] == 0u && identity[2] == 0u && identity[3] == 0u);
    }

    {
        // Quaternion-angle, single key with a default: the angle word (3) takes the key value.
        std::uint32_t defaults[4] = {wordOf(1.5f), wordOf(2.5f), wordOf(3.5f), wordOf(4.5f)};
        acc.defaultValue = defaults;
        float quaternion[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        float* result = CInterpreterQuaternionAngle_QAngle_short_getKeyBasedValueEx_618064(acc, 1, quaternion);
        CHECK(result == quaternion);
        CHECK(NEAR(quaternion[0], 1.5f));
        CHECK(NEAR(quaternion[3], 41.0f));
        acc.defaultValue = nullptr;
    }
    {
        // Quaternion-angle, conjugate(from) * to with the test doubles: (-21 + 41) = 20.
        float quaternion[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        const float value = CInterpreterQuaternionAngle_QAngle_short_getKeyBasedValueEx_61810c(acc, 0, 1, quaternion);
        CHECK(NEAR(value, 20.0f));
        CHECK(NEAR(quaternion[0], 20.0f));
    }
    {
        // Quaternion-angle slerp of 41 and 61 at t = 0.5 is 51; conjugate(21) * 51 = 30.
        float quaternion[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        const float value = CInterpreterQuaternionAngle_QAngle_short_getKeyBasedValueEx_61821c(acc, 0, 1, 2, 0.5f, quaternion);
        CHECK(NEAR(value, 30.0f));
        CHECK(NEAR(quaternion[0], 30.0f));
    }

    {
        // Applicator path (char keys 10 and -5, t = 0.5): lerp(21, -9) = 6 reaches the applicator.
        const char charKeys[2] = {10, -5};
        SKeyTable charTable;
        charTable.keys = charKeys;
        SAnimationAccessor charAcc;
        charAcc.reader.table = &charTable;
        charAcc.reader.scale = &scale;
        charAcc.reader.offset = &offset;
        RecordingApplicator applicator;
        CVirtualEx_QAngle_char_applyKeyBasedValue_618498(charAcc, 0, 1, 0.5f, nullptr, &applicator);
        CHECK(applicator.calls == 1);
        CHECK(NEAR(applicator.last[0], 6.0f));
    }

    std::printf("%s\n", failures ? "recon_slice_10: FAILED" : "recon_slice_10: ok");
    return failures != 0;
}
