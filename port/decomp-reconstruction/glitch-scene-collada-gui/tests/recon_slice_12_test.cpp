// Tests for slice 12 (animation_track key-based interpolation). The accessor and the
// material-parameter externals are stubbed here; the real ones live in other modules.
#include "glitch-scene-collada-gui/recon/slice_12.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>

namespace glitch::collada {
struct SAnimationAccessor {
    recon_12::SKeyOutput output;
    const float* defaults;
    bool hasDefault;
};
namespace animation_track {
class CApplicatorInfo {
public:
    std::uint16_t parameterIndex;
};
}  // namespace animation_track

namespace recon_12 {
SKeyOutput* accessorOutput(const SAnimationAccessor& accessor, int) {
    return const_cast<SKeyOutput*>(&accessor.output);
}
bool accessorHasDefaultValue(const SAnimationAccessor& accessor) { return accessor.hasDefault; }
const float* accessorDefaultValue(const SAnimationAccessor& accessor) { return accessor.defaults; }
std::uint16_t applicatorParameterIndex(const animation_track::CApplicatorInfo* info) {
    return info->parameterIndex;
}

static int g_index = -1;
static float g_value[4] = {0.f, 0.f, 0.f, 0.f};
static int record(std::uint16_t index, int n, const float* v) {
    g_index = index;
    for (int i = 0; i < n; ++i) g_value[i] = v[i];
    return 1;
}
int setParameterCvt2(void*, std::uint16_t index, int, const float* v) { return record(index, 2, v); }
int setParameterCvt3(void*, std::uint16_t index, int, const float* v) { return record(index, 3, v); }
int setParameterCvt4(void*, std::uint16_t index, int, const float* v) { return record(index, 4, v); }
}  // namespace recon_12
}  // namespace glitch::collada

using namespace glitch::collada;
using namespace glitch::collada::recon_12;

static int failures = 0;
#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);     \
            ++failures;                                                    \
        }                                                                  \
    } while (0)

static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main() {
    const float keys[4] = {1.f, 2.f, 3.f, 4.f};
    const float defs[4] = {10.f, 20.f, 30.f, 40.f};
    SAnimationAccessor withDef{SKeyOutput{0, keys}, defs, true};
    SAnimationAccessor noDef{SKeyOutput{0, keys}, nullptr, false};
    float out[4] = {-1.f, -1.f, -1.f, -1.f};

    // (a) single key, default present: word K from the key, others from the default.
    const float* ret = CInterpreter_f4_K2::getKeyBasedValueEx(withDef, 1, out);
    CHECK(ret == defs);
    CHECK(near(out[0], 10.f) && near(out[1], 20.f) && near(out[2], 2.f) && near(out[3], 40.f));

    // (a) single key, no default: only word 0 is written.
    out[0] = out[1] = out[2] = out[3] = -1.f;
    ret = CInterpreter_f4_K2::getKeyBasedValueEx(noDef, 1, out);
    CHECK(ret == nullptr);
    CHECK(near(out[0], 2.f) && near(out[1], -1.f));

    // (b) key difference, default present.
    ret = CInterpreter_f4_K2::getKeyBasedValueEx(withDef, 0, 3, out);
    CHECK(ret == defs);
    CHECK(near(out[2], 3.f) && near(out[0], 10.f));

    // (c) blend of two keys at t = 0.5 with no default.
    float value = CInterpreter_f4_K2::getKeyBasedValueEx(noDef, 0, 2, 0.5f, out);
    CHECK(near(value, 2.f) && near(out[0], 2.f));

    // (d) relative blend of three keys at t = 0.5 with no default.
    value = CInterpreter_f4_K2::getKeyBasedValueEx(noDef, 0, 1, 3, 0.5f, out);
    CHECK(near(value, 2.f) && near(out[0], 2.f));

    // Scalar lerp: keys[1] + t * (keys[3] - keys[1]) - keys[0] = 3 - 1 = 2.
    float lerpOut = -1.f;
    float scalar = CInterpreter_f1_Lerp::getKeyBasedValueEx(noDef, 0, 1, 3, 0.5f, &lerpOut);
    CHECK(near(scalar, 2.f) && near(lerpOut, 2.f));

    // Apply through an applicator with index 7: keyed word 2 at t = 0.5 with default.
    glitch::collada::animation_track::CApplicatorInfo info{7};
    float target = 0.f;
    int rc = CApplyValueEx_f4_K2::applyKeyBasedValueEx(withDef, 0, 2, 0.5f, &target, &info);
    CHECK(rc == 1 && g_index == 7);
    CHECK(near(g_value[0], 10.f) && near(g_value[1], 20.f) && near(g_value[2], 2.f) && near(g_value[3], 40.f));

    // Virtual apply (single key) with applicator index 5 and no default.
    glitch::collada::animation_track::CApplicatorInfo info5{5};
    rc = CVirtualEx_f4_K2::applyKeyBasedValue(noDef, 2, &target, &info5);
    CHECK(rc == 1 && g_index == 5 && near(g_value[0], 3.f));

    // Virtual forwarder reaches the same interpolator.
    ret = CVirtualEx_f4_K3::getKeyBasedValue(withDef, 1, out);
    CHECK(ret == defs);
    CHECK(near(out[0], 10.f) && near(out[2], 30.f) && near(out[3], 2.f));

    // Scalar virtual single key reads the key directly.
    float s = -1.f;
    const SKeyOutput* o = CVirtualEx_f1_Scalar::getKeyBasedValue(noDef, 2, &s);
    CHECK(o == &noDef.output && near(s, 3.f));

    if (failures == 0) {
        std::printf("recon_slice_12_test: all checks passed\n");
        return 0;
    }
    std::printf("recon_slice_12_test: %d failure(s)\n", failures);
    return 1;
}
