#include "glitch-scene-collada-gui/recon/slice_12.hpp"

#include <cstddef>

namespace glitch::collada::recon_12 {
namespace {

float keyValue(const SAnimationAccessor& accessor, int index) {
    return accessorOutput(accessor, 0)->values[index];
}

// a + t * (b - a), the order of the original blend.
float blend(float a, float b, float t) {
    return a + (t * (b - a));
}

// Writes the keyed component K of an N-word value. With a default present the other words are
// copied from the default; without one only word 0 is written. Returns the default pointer, or null.
// RequireDefaultPointer mirrors the checks where the original tests the default pointer as well as the flag.
template <std::size_t N, std::size_t K, bool RequireDefaultPointer>
const float* storeKeyed(const SAnimationAccessor& accessor, float* out, float value) {
    const bool hasDefault = accessorHasDefaultValue(accessor);
    const float* defaults = hasDefault ? accessorDefaultValue(accessor) : nullptr;
    if (hasDefault && (!RequireDefaultPointer || defaults != nullptr)) {
        for (std::size_t i = 0; i < N; ++i) {
            if (i != K) {
                out[i] = defaults[i];
            }
        }
        out[K] = value;
        return defaults;
    }
    out[0] = value;
    return nullptr;
}

}  // namespace

namespace CInterpreter_f4_K2 {

// 0x61d528 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi2EfEEfLi4ENS1_17SUseDefaultValuesILi2EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, void* out) {
    return storeKeyed<4, 2, true>(accessor, static_cast<float*>(out), keyValue(accessor, key0));
}

// 0x61d5f0 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi2EfEEfLi4ENS1_17SUseDefaultValuesILi2EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
// partial: blend of two keys; return differs: original returns default pointer bits when a default exists; reconstructed returns blended value
float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    const float value = blend(keyValue(accessor, key0), keyValue(accessor, key1), t);
    storeKeyed<4, 2, false>(accessor, static_cast<float*>(out), value);
    return value;
}

// 0x61d728 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi2EfEEfLi4ENS1_17SUseDefaultValuesILi2EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return storeKeyed<4, 2, false>(accessor, static_cast<float*>(out), keyValue(accessor, key1) - keyValue(accessor, key0));
}

// 0x61d7b0 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi2EfEEfLi4ENS1_17SUseDefaultValuesILi2EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
// partial: relative blend of three keys; return differs: original returns default pointer bits when a default exists; reconstructed returns blended value
float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, int key2, float t, void* out) {
    const float base = keyValue(accessor, key0);
    const float value = blend(keyValue(accessor, key1) - base, keyValue(accessor, key2) - base, t);
    storeKeyed<4, 2, false>(accessor, static_cast<float*>(out), value);
    return value;
}

}  // namespace CInterpreter_f4_K2

namespace CVirtualEx_f4_K2 {

// 0x61d5a0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* out) {
    return CInterpreter_f4_K2::getKeyBasedValueEx(accessor, key0, out);
}

// 0x61d5b0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* target, animation_track::CApplicatorInfo* info) {
    float values[4];
    CInterpreter_f4_K2::getKeyBasedValueEx(accessor, key0, values);
    return setParameterCvt4(target, applicatorParameterIndex(info), 0, values);
}

// 0x61d6b4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
// partial: thunk forwards to keyed interpolator; target CInterpreter_f4_K2::getKeyBasedValueEx
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    return CInterpreter_f4_K2::getKeyBasedValueEx(accessor, key0, key1, t, out);
}

// 0x61d704 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: thunk forwards to applyKeyBasedValueEx; inherits apply deviation; target CApplyValueEx_f4_K2::applyKeyBasedValueEx
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    return CApplyValueEx_f4_K2::applyKeyBasedValueEx(accessor, key0, key1, t, target, info);
}

// 0x61d79c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return CInterpreter_f4_K2::getKeyBasedValueEx(accessor, key0, key1, out);
}

// 0x61d890 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
// partial: thunk forwards to keyed interpolator; target CInterpreter_f4_K2::getKeyBasedValueEx
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, int key2, float t, void* out) {
    return CInterpreter_f4_K2::getKeyBasedValueEx(accessor, key0, key1, key2, t, out);
}

}  // namespace CVirtualEx_f4_K2

namespace CApplyValueEx_f4_K2 {

// 0x61d6d0 _ZN6glitch7collada15animation_track13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi2EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: blend through keyed interpolator then setParameterCvt; inherits keyed blend return deviation
int applyKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    float values[4];
    CInterpreter_f4_K2::getKeyBasedValueEx(accessor, key0, key1, t, values);
    return setParameterCvt4(target, applicatorParameterIndex(info), 0, values);
}

}  // namespace CApplyValueEx_f4_K2

namespace CInterpreter_f4_K3 {

// 0x61d8b4 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi3EfEEfLi4ENS1_17SUseDefaultValuesILi3EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, void* out) {
    return storeKeyed<4, 3, true>(accessor, static_cast<float*>(out), keyValue(accessor, key0));
}

// 0x61d97c _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi3EfEEfLi4ENS1_17SUseDefaultValuesILi3EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
// partial: blend of two keys; return differs: original returns default pointer bits when a default exists; reconstructed returns blended value
float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    const float value = blend(keyValue(accessor, key0), keyValue(accessor, key1), t);
    storeKeyed<4, 3, false>(accessor, static_cast<float*>(out), value);
    return value;
}

// 0x61daa4 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi3EfEEfLi4ENS1_17SUseDefaultValuesILi3EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return storeKeyed<4, 3, false>(accessor, static_cast<float*>(out), keyValue(accessor, key1) - keyValue(accessor, key0));
}

}  // namespace CInterpreter_f4_K3

namespace CVirtualEx_f4_K3 {

// 0x61d92c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* out) {
    return CInterpreter_f4_K3::getKeyBasedValueEx(accessor, key0, out);
}

// 0x61d93c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* target, animation_track::CApplicatorInfo* info) {
    float values[4];
    CInterpreter_f4_K3::getKeyBasedValueEx(accessor, key0, values);
    return setParameterCvt4(target, applicatorParameterIndex(info), 0, values);
}

// 0x61da30 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
// partial: thunk forwards to keyed interpolator; target CInterpreter_f4_K3::getKeyBasedValueEx
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    return CInterpreter_f4_K3::getKeyBasedValueEx(accessor, key0, key1, t, out);
}

// 0x61da80 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: thunk forwards to applyKeyBasedValueEx; inherits apply deviation; target CApplyValueEx_f4_K3::applyKeyBasedValueEx
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    return CApplyValueEx_f4_K3::applyKeyBasedValueEx(accessor, key0, key1, t, target, info);
}

// 0x61db18 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return CInterpreter_f4_K3::getKeyBasedValueEx(accessor, key0, key1, out);
}

}  // namespace CVirtualEx_f4_K3

namespace CApplyValueEx_f4_K3 {

// 0x61da4c _ZN6glitch7collada15animation_track13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi3EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: blend through keyed interpolator then setParameterCvt; inherits keyed blend return deviation
int applyKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    float values[4];
    CInterpreter_f4_K3::getKeyBasedValueEx(accessor, key0, key1, t, values);
    return setParameterCvt4(target, applicatorParameterIndex(info), 0, values);
}

}  // namespace CApplyValueEx_f4_K3

namespace CVirtualEx_f1_Scalar {

// 0x61db2c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
SKeyOutput* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* out) {
    SKeyOutput* output = accessorOutput(accessor, 0);
    *static_cast<float*>(out) = output->values[key0];
    return output;
}

// 0x61f46c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    SKeyOutput* output = accessorOutput(accessor, 0);
    const float value = output->values[key1] - output->values[key0];
    *static_cast<float*>(out) = value;
    return value;
}

// 0x61f4f8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, int key2, float t, void* out) {
    return CInterpreter_f1_Lerp::getKeyBasedValueEx(accessor, key0, key1, key2, t, out);
}

}  // namespace CVirtualEx_f1_Scalar

namespace CInterpreter_f2_K0 {

// 0x61e4ac _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA2_fS6_EEEELi0EfEEfLi2ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, void* out) {
    return storeKeyed<2, 0, true>(accessor, static_cast<float*>(out), keyValue(accessor, key0));
}

// 0x61e560 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA2_fS6_EEEELi0EfEEfLi2ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
// partial: blend of two keys; return differs: original returns default pointer bits when a default exists; reconstructed returns blended value
float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    const float value = blend(keyValue(accessor, key0), keyValue(accessor, key1), t);
    storeKeyed<2, 0, false>(accessor, static_cast<float*>(out), value);
    return value;
}

// 0x61e678 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA2_fS6_EEEELi0EfEEfLi2ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return storeKeyed<2, 0, false>(accessor, static_cast<float*>(out), keyValue(accessor, key1) - keyValue(accessor, key0));
}

// 0x61e6ec _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA2_fS6_EEEELi0EfEEfLi2ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
// partial: relative blend of three keys; return differs: original returns default pointer bits when a default exists; reconstructed returns blended value
float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, int key2, float t, void* out) {
    const float base = keyValue(accessor, key0);
    const float value = blend(keyValue(accessor, key1) - base, keyValue(accessor, key2) - base, t);
    storeKeyed<2, 0, false>(accessor, static_cast<float*>(out), value);
    return value;
}

}  // namespace CInterpreter_f2_K0

namespace CVirtualEx_f2_K0 {

// 0x61e510 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* out) {
    return CInterpreter_f2_K0::getKeyBasedValueEx(accessor, key0, out);
}

// 0x61e520 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* target, animation_track::CApplicatorInfo* info) {
    float values[2];
    CInterpreter_f2_K0::getKeyBasedValueEx(accessor, key0, values);
    return setParameterCvt2(target, applicatorParameterIndex(info), 0, values);
}

// 0x61e604 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
// partial: thunk forwards to keyed interpolator; target CInterpreter_f2_K0::getKeyBasedValueEx
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    return CInterpreter_f2_K0::getKeyBasedValueEx(accessor, key0, key1, t, out);
}

// 0x61e654 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: thunk forwards to applyKeyBasedValueEx; inherits apply deviation; target CApplyValueEx_f2_K0::applyKeyBasedValueEx
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    return CApplyValueEx_f2_K0::applyKeyBasedValueEx(accessor, key0, key1, t, target, info);
}

// 0x61e6d8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return CInterpreter_f2_K0::getKeyBasedValueEx(accessor, key0, key1, out);
}

// 0x61e7b4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
// partial: thunk forwards to keyed interpolator; target CInterpreter_f2_K0::getKeyBasedValueEx
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, int key2, float t, void* out) {
    return CInterpreter_f2_K0::getKeyBasedValueEx(accessor, key0, key1, key2, t, out);
}

}  // namespace CVirtualEx_f2_K0

namespace CApplyValueEx_f2_K0 {

// 0x61e620 _ZN6glitch7collada15animation_track13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi0EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: blend through keyed interpolator then setParameterCvt; inherits keyed blend return deviation
int applyKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    float values[2];
    CInterpreter_f2_K0::getKeyBasedValueEx(accessor, key0, key1, t, values);
    return setParameterCvt2(target, applicatorParameterIndex(info), 0, values);
}

}  // namespace CApplyValueEx_f2_K0

namespace CInterpreter_f2_K1 {

// 0x61e7d8 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA2_fS6_EEEELi1EfEEfLi2ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, void* out) {
    return storeKeyed<2, 1, true>(accessor, static_cast<float*>(out), keyValue(accessor, key0));
}

// 0x61e88c _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA2_fS6_EEEELi1EfEEfLi2ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
// partial: blend of two keys; return differs: original returns default pointer bits when a default exists; reconstructed returns blended value
float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    const float value = blend(keyValue(accessor, key0), keyValue(accessor, key1), t);
    storeKeyed<2, 1, false>(accessor, static_cast<float*>(out), value);
    return value;
}

// 0x61e9a4 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA2_fS6_EEEELi1EfEEfLi2ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return storeKeyed<2, 1, false>(accessor, static_cast<float*>(out), keyValue(accessor, key1) - keyValue(accessor, key0));
}

// 0x61ea18 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA2_fS6_EEEELi1EfEEfLi2ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
// partial: relative blend of three keys; return differs: original returns default pointer bits when a default exists; reconstructed returns blended value
float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, int key2, float t, void* out) {
    const float base = keyValue(accessor, key0);
    const float value = blend(keyValue(accessor, key1) - base, keyValue(accessor, key2) - base, t);
    storeKeyed<2, 1, false>(accessor, static_cast<float*>(out), value);
    return value;
}

}  // namespace CInterpreter_f2_K1

namespace CVirtualEx_f2_K1 {

// 0x61e83c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* out) {
    return CInterpreter_f2_K1::getKeyBasedValueEx(accessor, key0, out);
}

// 0x61e84c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* target, animation_track::CApplicatorInfo* info) {
    float values[2];
    CInterpreter_f2_K1::getKeyBasedValueEx(accessor, key0, values);
    return setParameterCvt2(target, applicatorParameterIndex(info), 0, values);
}

// 0x61e930 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
// partial: thunk forwards to keyed interpolator; target CInterpreter_f2_K1::getKeyBasedValueEx
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    return CInterpreter_f2_K1::getKeyBasedValueEx(accessor, key0, key1, t, out);
}

// 0x61e980 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: thunk forwards to applyKeyBasedValueEx; inherits apply deviation; target CApplyValueEx_f2_K1::applyKeyBasedValueEx
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    return CApplyValueEx_f2_K1::applyKeyBasedValueEx(accessor, key0, key1, t, target, info);
}

// 0x61ea04 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return CInterpreter_f2_K1::getKeyBasedValueEx(accessor, key0, key1, out);
}

// 0x61eadc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
// partial: thunk forwards to keyed interpolator; target CInterpreter_f2_K1::getKeyBasedValueEx
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, int key2, float t, void* out) {
    return CInterpreter_f2_K1::getKeyBasedValueEx(accessor, key0, key1, key2, t, out);
}

}  // namespace CVirtualEx_f2_K1

namespace CApplyValueEx_f2_K1 {

// 0x61e94c _ZN6glitch7collada15animation_track13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: blend through keyed interpolator then setParameterCvt; inherits keyed blend return deviation
int applyKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    float values[2];
    CInterpreter_f2_K1::getKeyBasedValueEx(accessor, key0, key1, t, values);
    return setParameterCvt2(target, applicatorParameterIndex(info), 0, values);
}

}  // namespace CApplyValueEx_f2_K1

namespace CInterpreter_f3_K0 {

// 0x61eb00 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA3_fS6_EEEELi0EfEEfLi3ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, void* out) {
    return storeKeyed<3, 0, true>(accessor, static_cast<float*>(out), keyValue(accessor, key0));
}

// 0x61ebc0 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA3_fS6_EEEELi0EfEEfLi3ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
// partial: blend of two keys; return differs: original returns default pointer bits when a default exists; reconstructed returns blended value
float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    const float value = blend(keyValue(accessor, key0), keyValue(accessor, key1), t);
    storeKeyed<3, 0, false>(accessor, static_cast<float*>(out), value);
    return value;
}

// 0x61ecec _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA3_fS6_EEEELi0EfEEfLi3ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return storeKeyed<3, 0, false>(accessor, static_cast<float*>(out), keyValue(accessor, key1) - keyValue(accessor, key0));
}

// 0x61ed6c _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA3_fS6_EEEELi0EfEEfLi3ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
// partial: relative blend of three keys; return differs: original returns default pointer bits when a default exists; reconstructed returns blended value
float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, int key2, float t, void* out) {
    const float base = keyValue(accessor, key0);
    const float value = blend(keyValue(accessor, key1) - base, keyValue(accessor, key2) - base, t);
    storeKeyed<3, 0, false>(accessor, static_cast<float*>(out), value);
    return value;
}

}  // namespace CInterpreter_f3_K0

namespace CVirtualEx_f3_K0 {

// 0x61eb70 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* out) {
    return CInterpreter_f3_K0::getKeyBasedValueEx(accessor, key0, out);
}

// 0x61eb80 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* target, animation_track::CApplicatorInfo* info) {
    float values[3];
    CInterpreter_f3_K0::getKeyBasedValueEx(accessor, key0, values);
    return setParameterCvt3(target, applicatorParameterIndex(info), 0, values);
}

// 0x61ec78 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
// partial: thunk forwards to keyed interpolator; target CInterpreter_f3_K0::getKeyBasedValueEx
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    return CInterpreter_f3_K0::getKeyBasedValueEx(accessor, key0, key1, t, out);
}

// 0x61ecc8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: thunk forwards to applyKeyBasedValueEx; inherits apply deviation; target CApplyValueEx_f3_K0::applyKeyBasedValueEx
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    return CApplyValueEx_f3_K0::applyKeyBasedValueEx(accessor, key0, key1, t, target, info);
}

// 0x61ed58 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return CInterpreter_f3_K0::getKeyBasedValueEx(accessor, key0, key1, out);
}

// 0x61ee40 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
// partial: thunk forwards to keyed interpolator; target CInterpreter_f3_K0::getKeyBasedValueEx
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, int key2, float t, void* out) {
    return CInterpreter_f3_K0::getKeyBasedValueEx(accessor, key0, key1, key2, t, out);
}

}  // namespace CVirtualEx_f3_K0

namespace CApplyValueEx_f3_K0 {

// 0x61ec94 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi0EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: blend through keyed interpolator then setParameterCvt; inherits keyed blend return deviation
int applyKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    float values[3];
    CInterpreter_f3_K0::getKeyBasedValueEx(accessor, key0, key1, t, values);
    return setParameterCvt3(target, applicatorParameterIndex(info), 0, values);
}

}  // namespace CApplyValueEx_f3_K0

namespace CInterpreter_f4_K0 {

// 0x61ee64 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi0EfEEfLi4ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, void* out) {
    return storeKeyed<4, 0, true>(accessor, static_cast<float*>(out), keyValue(accessor, key0));
}

// 0x61ef2c _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi0EfEEfLi4ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
// partial: blend of two keys; return differs: original returns default pointer bits when a default exists; reconstructed returns blended value
float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    const float value = blend(keyValue(accessor, key0), keyValue(accessor, key1), t);
    storeKeyed<4, 0, false>(accessor, static_cast<float*>(out), value);
    return value;
}

// 0x61f054 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi0EfEEfLi4ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return storeKeyed<4, 0, false>(accessor, static_cast<float*>(out), keyValue(accessor, key1) - keyValue(accessor, key0));
}

// 0x61f0dc _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi0EfEEfLi4ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
// partial: relative blend of three keys; return differs: original returns default pointer bits when a default exists; reconstructed returns blended value
float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, int key2, float t, void* out) {
    const float base = keyValue(accessor, key0);
    const float value = blend(keyValue(accessor, key1) - base, keyValue(accessor, key2) - base, t);
    storeKeyed<4, 0, false>(accessor, static_cast<float*>(out), value);
    return value;
}

}  // namespace CInterpreter_f4_K0

namespace CVirtualEx_f4_K0 {

// 0x61eedc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* out) {
    return CInterpreter_f4_K0::getKeyBasedValueEx(accessor, key0, out);
}

// 0x61eeec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* target, animation_track::CApplicatorInfo* info) {
    float values[4];
    CInterpreter_f4_K0::getKeyBasedValueEx(accessor, key0, values);
    return setParameterCvt4(target, applicatorParameterIndex(info), 0, values);
}

// 0x61efe0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
// partial: thunk forwards to keyed interpolator; target CInterpreter_f4_K0::getKeyBasedValueEx
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    return CInterpreter_f4_K0::getKeyBasedValueEx(accessor, key0, key1, t, out);
}

// 0x61f030 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: thunk forwards to applyKeyBasedValueEx; inherits apply deviation; target CApplyValueEx_f4_K0::applyKeyBasedValueEx
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    return CApplyValueEx_f4_K0::applyKeyBasedValueEx(accessor, key0, key1, t, target, info);
}

// 0x61f0c8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return CInterpreter_f4_K0::getKeyBasedValueEx(accessor, key0, key1, out);
}

// 0x61f1b8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
// partial: thunk forwards to keyed interpolator; target CInterpreter_f4_K0::getKeyBasedValueEx
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, int key2, float t, void* out) {
    return CInterpreter_f4_K0::getKeyBasedValueEx(accessor, key0, key1, key2, t, out);
}

}  // namespace CVirtualEx_f4_K0

namespace CApplyValueEx_f4_K0 {

// 0x61effc _ZN6glitch7collada15animation_track13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi0EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: blend through keyed interpolator then setParameterCvt; inherits keyed blend return deviation
int applyKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    float values[4];
    CInterpreter_f4_K0::getKeyBasedValueEx(accessor, key0, key1, t, values);
    return setParameterCvt4(target, applicatorParameterIndex(info), 0, values);
}

}  // namespace CApplyValueEx_f4_K0

namespace CInterpreter_f4_K1 {

// 0x61f1dc _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi1EfEEfLi4ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, void* out) {
    return storeKeyed<4, 1, true>(accessor, static_cast<float*>(out), keyValue(accessor, key0));
}

// 0x61f2a4 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi1EfEEfLi4ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
// partial: blend of two keys; return differs: original returns default pointer bits when a default exists; reconstructed returns blended value
float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    const float value = blend(keyValue(accessor, key0), keyValue(accessor, key1), t);
    storeKeyed<4, 1, false>(accessor, static_cast<float*>(out), value);
    return value;
}

// 0x61f3e4 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi1EfEEfLi4ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return storeKeyed<4, 1, false>(accessor, static_cast<float*>(out), keyValue(accessor, key1) - keyValue(accessor, key0));
}

}  // namespace CInterpreter_f4_K1

namespace CVirtualEx_f4_K1 {

// 0x61f254 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* out) {
    return CInterpreter_f4_K1::getKeyBasedValueEx(accessor, key0, out);
}

// 0x61f264 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, void* target, animation_track::CApplicatorInfo* info) {
    float values[4];
    CInterpreter_f4_K1::getKeyBasedValueEx(accessor, key0, values);
    return setParameterCvt4(target, applicatorParameterIndex(info), 0, values);
}

// 0x61f370 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
// partial: thunk forwards to keyed interpolator; target CInterpreter_f4_K1::getKeyBasedValueEx
float getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* out) {
    return CInterpreter_f4_K1::getKeyBasedValueEx(accessor, key0, key1, t, out);
}

// 0x61f3c0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: thunk forwards to applyKeyBasedValueEx; inherits apply deviation; target CApplyValueEx_f4_K1::applyKeyBasedValueEx
int applyKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    return CApplyValueEx_f4_K1::applyKeyBasedValueEx(accessor, key0, key1, t, target, info);
}

// 0x61f458 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const float* getKeyBasedValue(const SAnimationAccessor& accessor, int key0, int key1, void* out) {
    return CInterpreter_f4_K1::getKeyBasedValueEx(accessor, key0, key1, out);
}

}  // namespace CVirtualEx_f4_K1

namespace CApplyValueEx_f4_K1 {

// 0x61f38c _ZN6glitch7collada15animation_track13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
// partial: blend through keyed interpolator then setParameterCvt; inherits keyed blend return deviation
int applyKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, float t, void* target, animation_track::CApplicatorInfo* info) {
    float values[4];
    CInterpreter_f4_K1::getKeyBasedValueEx(accessor, key0, key1, t, values);
    return setParameterCvt4(target, applicatorParameterIndex(info), 0, values);
}

}  // namespace CApplyValueEx_f4_K1

namespace CInterpreter_f1_Lerp {

// 0x61f4a0 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEfLi1ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key0, int key1, int key2, float t, void* out) {
    const float value = blend(keyValue(accessor, key1), keyValue(accessor, key2), t) - keyValue(accessor, key0);
    *static_cast<float*>(out) = value;
    return value;
}

}  // namespace CInterpreter_f1_Lerp

}  // namespace glitch::collada::recon_12
