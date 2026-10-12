// Host test for CInterpreter<CSceneNodeScaleXEx<float>, float, 3, SUseDefaultValues<0, float>>.
// The accessor is a stub here: key data and an optional default-value object. The engine layout is not used.
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_ScaleXFloat.hpp"

namespace glitch::collada {

struct SAnimationAccessor {
    const void* keyData;
    const void* defaultValue;
};

namespace tb_ext {
const void* accessorKeyData(const SAnimationAccessor& a) { return a.keyData; }
bool accessorHasDefaultValue(const SAnimationAccessor& a) { return a.defaultValue != nullptr; }
const void* accessorDefaultValue(const SAnimationAccessor& a) { return a.defaultValue; }
}  // namespace tb_ext

}  // namespace glitch::collada

namespace {

using glitch::collada::SAnimationAccessor;
using Interp = glitch::collada::animation_track::CInterpreter<
    glitch::collada::animation_track::CSceneNodeScaleXEx<float>, float, 3,
    glitch::collada::animation_track::SUseDefaultValues<0, float>>;

int failures = 0;

void check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL %s\n", what);
        ++failures;
    }
}

std::uint32_t wordAt(const void* p, int index) {
    std::uint32_t v;
    std::memcpy(&v, static_cast<const std::uint8_t*>(p) + 4 * index, sizeof v);
    return v;
}

}  // namespace

int main() {
    const float keys[3] = {1.0f, 3.0f, 7.0f};
    const std::uint32_t defaults[3] = {0u, 0x11111111u, 0x22222222u};

    SAnimationAccessor plain{keys, nullptr};
    SAnimationAccessor withDefault{keys, defaults};

    // 0x61dc4c: a single key, no default.
    {
        float out[3] = {0.0f, 0.0f, 0.0f};
        const int r = Interp::getKeyBasedValueEx(plain, 2, out);
        check(out[0] == 7.0f, "single key value");
        check(r == 0, "single key without default returns 0");
    }

    // 0x61dda0: the key difference, no default.
    {
        float out[3] = {0.0f, 0.0f, 0.0f};
        const int r = Interp::getKeyBasedValueEx(plain, 0, 2, out);
        check(out[0] == 6.0f, "difference value");
        check(r == 0, "difference without default returns 0");
    }

    // 0x61dccc: a linear blend, no default.
    {
        float out[3] = {0.0f, 0.0f, 0.0f};
        const float value = Interp::getKeyBasedValueEx(plain, 0, 1, 0.5f, out);
        check(value == 2.0f, "blend return value");
        check(out[0] == 2.0f, "blend stored value");
    }

    // 0x61de20: a blend of two deltas measured from the base key, no default.
    {
        // base 0, from 1, to 2: origin 1, begin 2, end 6, value 2 + 0.5 * 4 = 4.
        float out[3] = {0.0f, 0.0f, 0.0f};
        const float value = Interp::getKeyBasedValueEx(plain, 0, 1, 2, 0.5f, out);
        check(value == 4.0f, "base blend value");
        check(out[0] == 4.0f, "base blend stored value");
    }

    // Default-value object present: the default words at +0x04 and +0x08 are copied.
    {
        float out[3] = {0.0f, 0.0f, 0.0f};
        Interp::getKeyBasedValueEx(withDefault, 1, out);
        check(out[0] == 3.0f, "default present: key value");
        check(wordAt(out, 1) == 0x11111111u, "default present: word +4");
        check(wordAt(out, 2) == 0x22222222u, "default present: word +8");
    }
    {
        float out[3] = {0.0f, 0.0f, 0.0f};
        Interp::getKeyBasedValueEx(withDefault, 0, 2, out);
        check(out[0] == 6.0f, "default present: difference value");
        check(wordAt(out, 2) == 0x22222222u, "default present: difference word +8");
    }
    {
        float out[3] = {0.0f, 0.0f, 0.0f};
        Interp::getKeyBasedValueEx(withDefault, 0, 1, 0.5f, out);
        check(out[0] == 2.0f, "default present: blend value");
        check(wordAt(out, 1) == 0x11111111u, "default present: blend word +4");
    }
    {
        float out[3] = {0.0f, 0.0f, 0.0f};
        Interp::getKeyBasedValueEx(withDefault, 0, 1, 2, 0.5f, out);
        check(out[0] == 4.0f, "default present: base blend value");
        check(wordAt(out, 2) == 0x22222222u, "default present: base blend word +8");
    }

    std::printf(failures == 0 ? "all checks passed\n" : "%d check(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
