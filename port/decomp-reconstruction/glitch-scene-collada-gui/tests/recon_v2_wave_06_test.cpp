// Test for recon v2 wave 06. Externs are test doubles: a word-addressed guest memory,
// a default-value flag, and a recorder for setParameterCvt calls.
#include <cstdio>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "glitch-scene-collada-gui/recon/v2/wave_06.hpp"

namespace rc = glitch::collada::recon_v2_06;
using glitch::collada::SAnimationAccessor;
using glitch::collada::animation_track::CApplicatorInfo;

namespace {

std::unordered_map<std::uint32_t, std::uint32_t> g_mem;
bool g_has_default = false;
std::uint32_t g_default_ptr = 0;

struct Call {
    std::uint32_t target;
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

int record(std::uint32_t target, std::uint16_t id, int zero, const std::uint32_t* value, int words) {
    Call c{target, id, zero, {0, 0, 0, 0}, words};
    for (int i = 0; i < words; ++i) c.value[i] = value[i];
    g_calls.push_back(c);
    return 1;
}

}  // namespace

// ---- externs ----
namespace glitch::collada::recon_v2_06 {

std::uint32_t guest_load_u32(std::uint32_t guest_addr) {
    auto it = g_mem.find(guest_addr);
    return it == g_mem.end() ? 0u : it->second;
}

bool accessor_has_default_value(const SAnimationAccessor*) { return g_has_default; }

std::uint32_t accessor_get_default_value(const SAnimationAccessor*) { return g_default_ptr; }

int set_parameter_cvt_vec2(std::uint32_t t, std::uint16_t id, int z, const std::uint32_t* v) { return record(t, id, z, v, 2); }
int set_parameter_cvt_vec3(std::uint32_t t, std::uint16_t id, int z, const std::uint32_t* v) { return record(t, id, z, v, 3); }
int set_parameter_cvt_vec4(std::uint32_t t, std::uint16_t id, int z, const std::uint32_t* v) { return record(t, id, z, v, 4); }

}  // namespace glitch::collada::recon_v2_06

namespace {

// Guest layout: track T (keyframes at T+8), keyframe table K (28-byte entries, index at +0x18),
// outputs O (entry j base word at O+8j+8 = float array), float array F of scalar keys.
constexpr std::uint32_t kTrack = 0x1000;
constexpr std::uint32_t kKeys = 0x2000;
constexpr std::uint32_t kOutputs = 0x3000;
constexpr std::uint32_t kFloats = 0x4000;
constexpr std::uint32_t kDefault = 0x5000;

void setup(bool has_default) {
    g_mem.clear();
    g_calls.clear();
    g_has_default = has_default;
    g_default_ptr = kDefault;
    g_mem[kTrack + 8] = kKeys;
    for (std::uint32_t k = 0; k < 3; ++k) g_mem[kKeys + 28 * k + 0x18] = 0;  // all keys -> output 0
    g_mem[kOutputs + 8] = kFloats;                                          // getOutput(a1,0)+4 base
    const float keys[3] = {1.0f, 3.0f, 5.0f};
    for (std::uint32_t k = 0; k < 3; ++k) g_mem[kFloats + 4 * k] = rc::word_of(keys[k]);
}

SAnimationAccessor make_accessor() {
    SAnimationAccessor a{};
    a.track = kTrack;
    a.outputs = kOutputs;
    return a;
}

}  // namespace

int main() {
    // Blend of two keys without a default: 1 + 0.5 * (3 - 1) = 2.0.
    {
        setup(false);
        SAnimationAccessor a = make_accessor();
        std::uint32_t out[2] = {0, 0};
        float r = rc::interp_f2_k0_blend(&a, 0, 1, 0.5f, out);
        check(r == 2.0f && out[0] == rc::word_of(2.0f), "f2 K0 blend without default");
    }
    // Same blend with a default: out[1] takes default word +4, return carries the pointer bits.
    {
        setup(true);
        g_mem[kDefault + 4] = 0xABu;
        SAnimationAccessor a = make_accessor();
        std::uint32_t out[2] = {0, 0};
        float r = rc::interp_f2_k0_blend(&a, 0, 1, 0.5f, out);
        check(out[0] == rc::word_of(2.0f) && out[1] == 0xABu && rc::word_of(r) == kDefault,
              "f2 K0 blend with default (pointer-bit return)");
    }
    // Relative three-key blend, no default: v12 = 2, v13 = 4, 2 + 0.5 * (4 - 2) = 3.0.
    {
        setup(false);
        SAnimationAccessor a = make_accessor();
        std::uint32_t out[3] = {0, 0, 0};
        float r = rc::interp_f3_k0_relative3(&a, 0, 1, 2, 0.5f, out);
        check(r == 3.0f && out[0] == rc::word_of(3.0f), "f3 K0 relative three-key without default");
    }
    // f4 K1 blend with default: words 0, blend, default +8, default +12; return pointer bits.
    {
        setup(true);
        g_mem[kDefault] = 0x11u;
        g_mem[kDefault + 8] = 0x22u;
        g_mem[kDefault + 12] = 0x33u;
        SAnimationAccessor a = make_accessor();
        std::uint32_t out[4] = {0, 0, 0, 0};
        float r = rc::interp_f4_k1_blend(&a, 0, 1, 0.5f, out);
        check(out[0] == 0x11u && out[1] == rc::word_of(2.0f) && out[2] == 0x22u && out[3] == 0x33u &&
                  rc::word_of(r) == kDefault,
              "f4 K1 blend with default");
    }
    // f2 K0 apply forwards the interpolated pair with the applicator's parameter id.
    {
        setup(false);
        SAnimationAccessor a = make_accessor();
        CApplicatorInfo info{};
        info.parameterId = 7;
        int ret = rc::apply_f2_k0(&a, 0, 1, 0.5f, 0x9000u, &info);
        check(ret == 1 && g_calls.size() == 1 && g_calls[0].target == 0x9000u && g_calls[0].id == 7 &&
                  g_calls[0].words == 2 && g_calls[0].value[0] == rc::word_of(2.0f),
              "f2 K0 apply -> setParameterCvt vec2");
    }
    // f4 K1 apply with default forwards the four words.
    {
        setup(true);
        g_mem[kDefault] = 0x11u;
        g_mem[kDefault + 8] = 0x22u;
        g_mem[kDefault + 12] = 0x33u;
        SAnimationAccessor a = make_accessor();
        CApplicatorInfo info{};
        info.parameterId = 3;
        rc::apply_f4_k1(&a, 0, 1, 0.5f, 0x9100u, &info);
        check(g_calls.size() == 1 && g_calls[0].id == 3 && g_calls[0].words == 4 &&
                  g_calls[0].value[0] == 0x11u && g_calls[0].value[1] == rc::word_of(2.0f) &&
                  g_calls[0].value[2] == 0x22u && g_calls[0].value[3] == 0x33u,
              "f4 K1 apply with default -> setParameterCvt vec4");
    }

    std::printf("%s (%d failure%s)\n", g_failures ? "FAILED" : "PASSED", g_failures, g_failures == 1 ? "" : "s");
    return g_failures ? 1 : 0;
}
