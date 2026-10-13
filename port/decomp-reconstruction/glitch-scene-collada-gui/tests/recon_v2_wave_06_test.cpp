// Test for recon v2 wave 06. The keys and the default value come from the canonical SAnimationAccessor; setParameterCvt is a
// test double that records its calls.
#include <cstdio>
#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/recon/v2/wave_06.hpp"

namespace rc = glitch::collada::recon_v2_06;
using glitch::collada::SAnimation;
using glitch::collada::SAnimationAccessor;
using glitch::collada::SAnimationDefaultValue;
using glitch::collada::SAnimationKeyframe;
using glitch::collada::SAnimationOutput;
using glitch::collada::animation_track::CApplicatorInfo;

namespace {

struct Call {
    void* target;
    std::uint16_t id;
    int zero;
    std::uint32_t value[4];
    int words;
};
std::vector<Call> g_calls;

int g_failures = 0;

void check(bool ok, const char* what) {
    if (!ok) {
        ++g_failures;
        std::printf("FAIL: %s\n", what);
    } else {
        std::printf("ok:   %s\n", what);
    }
}

int record(void* target, std::uint16_t id, int zero, const std::uint32_t* value, int words) {
    Call c{target, id, zero, {0, 0, 0, 0}, words};
    for (int i = 0; i < words; ++i) c.value[i] = value[i];
    g_calls.push_back(c);
    return 1;
}

}  // namespace

// ---- externs ----
namespace glitch::collada::recon_v2_06 {

int set_parameter_cvt_vec2(void* t, std::uint16_t id, int z, const std::uint32_t* v) { return record(t, id, z, v, 2); }
int set_parameter_cvt_vec3(void* t, std::uint16_t id, int z, const std::uint32_t* v) { return record(t, id, z, v, 3); }
int set_parameter_cvt_vec4(void* t, std::uint16_t id, int z, const std::uint32_t* v) { return record(t, id, z, v, 4); }

}  // namespace glitch::collada::recon_v2_06

namespace {

// One animation: three keyframes that all use output 0, whose data is the float key array; an optional default value.
struct Fixture {
    SAnimationKeyframe keyframes[3] = {};
    float keys[3] = {1.0f, 3.0f, 5.0f};
    SAnimationOutput outputs[1] = {};
    std::uint32_t defaultWords[4] = {0, 0, 0, 0};
    SAnimationDefaultValue defaultValue;
    SAnimation animation;

    explicit Fixture(bool hasDefault) {
        g_calls.clear();
        outputs[0].count = 3;
        outputs[0].data = keys;
        defaultValue.value = defaultWords;
        animation.keyframes = keyframes;
        animation.defaultValue = hasDefault ? &defaultValue : nullptr;
    }

    SAnimationAccessor accessor() { return SAnimationAccessor(&animation, outputs, nullptr); }
};

std::uint32_t pointerBits(const void* p) {
    return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
}

}  // namespace

int main() {
    // Blend of two keys without a default: 1 + 0.5 * (3 - 1) = 2.0.
    {
        Fixture f(false);
        SAnimationAccessor a = f.accessor();
        std::uint32_t out[2] = {0, 0};
        float r = rc::interp_f2_k0_blend(&a, 0, 1, 0.5f, out);
        check(r == 2.0f && out[0] == rc::word_of(2.0f), "f2 K0 blend without default");
    }
    // Same blend with a default: out[1] takes default word +4, return carries the pointer bits.
    {
        Fixture f(true);
        f.defaultWords[1] = 0xABu;
        SAnimationAccessor a = f.accessor();
        std::uint32_t out[2] = {0, 0};
        float r = rc::interp_f2_k0_blend(&a, 0, 1, 0.5f, out);
        check(out[0] == rc::word_of(2.0f) && out[1] == 0xABu && rc::word_of(r) == pointerBits(f.defaultWords),
              "f2 K0 blend with default (pointer-bit return)");
    }
    // Relative three-key blend, no default: v12 = 2, v13 = 4, 2 + 0.5 * (4 - 2) = 3.0.
    {
        Fixture f(false);
        SAnimationAccessor a = f.accessor();
        std::uint32_t out[3] = {0, 0, 0};
        float r = rc::interp_f3_k0_relative3(&a, 0, 1, 2, 0.5f, out);
        check(r == 3.0f && out[0] == rc::word_of(3.0f), "f3 K0 relative three-key without default");
    }
    // f4 K1 blend with default: words 0, blend, default +8, default +12; return pointer bits.
    {
        Fixture f(true);
        f.defaultWords[0] = 0x11u;
        f.defaultWords[2] = 0x22u;
        f.defaultWords[3] = 0x33u;
        SAnimationAccessor a = f.accessor();
        std::uint32_t out[4] = {0, 0, 0, 0};
        float r = rc::interp_f4_k1_blend(&a, 0, 1, 0.5f, out);
        check(out[0] == 0x11u && out[1] == rc::word_of(2.0f) && out[2] == 0x22u && out[3] == 0x33u &&
                  rc::word_of(r) == pointerBits(f.defaultWords),
              "f4 K1 blend with default");
    }
    // f2 K0 apply forwards the interpolated pair with the applicator's parameter id.
    {
        Fixture f(false);
        SAnimationAccessor a = f.accessor();
        glitch::collada::animation_track::CMaterialApplicatorInfo info;
        info.parameterId = 7;
        int target = 0;
        int ret = rc::apply_f2_k0(&a, 0, 1, 0.5f, &target, &info);
        check(ret == 1 && g_calls.size() == 1 && g_calls[0].target == &target && g_calls[0].id == 7 &&
                  g_calls[0].words == 2 && g_calls[0].value[0] == rc::word_of(2.0f),
              "f2 K0 apply -> setParameterCvt vec2");
    }
    // f4 K1 apply with default forwards the four words.
    {
        Fixture f(true);
        f.defaultWords[0] = 0x11u;
        f.defaultWords[2] = 0x22u;
        f.defaultWords[3] = 0x33u;
        SAnimationAccessor a = f.accessor();
        glitch::collada::animation_track::CMaterialApplicatorInfo info;
        info.parameterId = 3;
        int target = 0;
        rc::apply_f4_k1(&a, 0, 1, 0.5f, &target, &info);
        check(g_calls.size() == 1 && g_calls[0].id == 3 && g_calls[0].words == 4 &&
                  g_calls[0].value[0] == 0x11u && g_calls[0].value[1] == rc::word_of(2.0f) &&
                  g_calls[0].value[2] == 0x22u && g_calls[0].value[3] == 0x33u,
              "f4 K1 apply with default -> setParameterCvt vec4");
    }

    std::printf("%s (%d failure%s)\n", g_failures ? "FAILED" : "PASSED", g_failures, g_failures == 1 ? "" : "s");
    return g_failures ? 1 : 0;
}
