// Reconstruction slice 16: glitch::collada::animation_track value applicators.
//
// The applicator wrappers (CApplyValueEx, CInterpreter, CVirtualEx) are not declared
// in any existing header, so every body below is a free function here and its ledger
// row is partial with note "unattached". Thunks that only forward to these bodies are
// not reimplemented. Original member offsets are kept as comments (+0x..).
#pragma once

#include <cstddef>
#include <cstdint>

namespace glitch::collada::recon_16 {

// CInputReader<T, float, 3> fields behind a quantized track: keys is the key array of
// 3-component T values, scale and bias hold one float per component. The reader is
// built from SAnimationAccessor in the original; that fetch is not mapped here.
template <typename T>
struct SKeyInputQuantized {
    const T* keys;
    const float* scale;
    const float* bias;
};

// Raw float keys, read from SAnimationAccessor::getOutput(0) + 4 in the original.
struct SKeyInputFloat {
    const float* keys;
};

// CApplicatorInfo as used by the material parameter applicators: the parameter id is the
// 16-bit word at +0x08. Only that field is mapped.
struct SApplicatorInfoParam {
    std::uint8_t reserved_[8];  // +0x00..+0x07 not mapped
    std::uint16_t parameterId;  // +0x08
};

// Key pair lerp: out = (1 - t) * k0 + t * k1 per component. Returns out[0], which is the
// pseudocode result of the getKeyBasedValueEx family.
inline float lerpKeyPair3(const float* k0, const float* k1, float t, float* out) {
    const float s = 1.0f - t;
    for (std::size_t c = 0; c < 3; ++c) {
        out[c] = t * k1[c] + s * k0[c];
    }
    return out[0];
}

// Dequantize one key: value = key * scale + bias per component.
template <typename T>
inline void dequantizeKey3(const T* key, const float* scale, const float* bias, float* value) {
    for (std::size_t c = 0; c < 3; ++c) {
        value[c] = static_cast<float>(key[c]) * scale[c] + bias[c];
    }
}

// Quantized key interpolation. Keys are addressed by element index (3 per key).
template <typename T>
inline float interpolateQuantized3(const SKeyInputQuantized<T>& in, int key0, int key1, float t, float* out) {
    float v0[3];
    float v1[3];
    dequantizeKey3(in.keys + 3 * key0, in.scale, in.bias, v0);
    dequantizeKey3(in.keys + 3 * key1, in.scale, in.bias, v1);
    return lerpKeyPair3(v0, v1, t, out);
}

// Raw float key interpolation (3 floats per key).
inline float interpolateFloat3(const SKeyInputFloat& in, int key0, int key1, float t, float* out) {
    return lerpKeyPair3(in.keys + 3 * key0, in.keys + 3 * key1, t, out);
}

// Weighted sum of count 3-component keys: sum = sum_i weights[i] * keys[i]. Zero for count 0.
inline void sumVec3(const float* keys, const float* weights, int count, float* sum) {
    sum[0] = 0.0f;
    sum[1] = 0.0f;
    sum[2] = 0.0f;
    for (int i = 0; i < count; ++i) {
        for (std::size_t c = 0; c < 3; ++c) {
            sum[c] = sum[c] + weights[i] * keys[3 * i + c];
        }
    }
}

// Blend used by getBlendedValue, getAddedValue and the float[3] applicators: a single key is
// copied without its weight; otherwise the weighted sum is written (zero for count 0).
inline void blendVec3(const float* keys, const float* weights, int count, float* out) {
    if (count == 1) {
        out[0] = keys[0];
        out[1] = keys[1];
        out[2] = keys[2];
        return;
    }
    sumVec3(keys, weights, count, out);
}

// 0x627f80 _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIcEEfLi3ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
inline float getKeyBasedValueEx_Scale_char(const SKeyInputQuantized<char>& input, int key0, int key1, float t, float* out) {
    return interpolateQuantized3(input, key0, key1, t, out);
}

// 0x6280e4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
inline void applyKeyBasedValueEx_Scale_char(const SKeyInputQuantized<char>& input, int key0, int key1, float t, void* target) {
    float value[3] = {0.0f, 0.0f, 0.0f};
    interpolateQuantized3(input, key0, key1, t, value);
    // unresolved: virtual call on target at vtable +0x94 omitted
    static_cast<void>(target);
}

// 0x628168 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi0EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_Material_float3_0(const float* keys, const float* weights, int count, void* target, const SApplicatorInfoParam* info) {
    float value[3];
    blendVec3(keys, weights, count, value);
    // unattached: IMaterialParameters::setParameterCvt(target, info->parameterId, 0, value) not mapped
    static_cast<void>(target);
    static_cast<void>(info);
}

// 0x62827c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEE19applyBlendedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
inline void applyBlendedValueEx_Position_short(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x628390 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEE17applyAddedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_Position_short(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x6285b8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEE17applyAddedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_Position_float(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x6287cc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
inline void applyKeyBasedValueEx_Position_float(const SKeyInputFloat& input, int key0, int key1, float t, void* target) {
    float value[3] = {0.0f, 0.0f, 0.0f};
    interpolateFloat3(input, key0, key1, t, value);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x628950 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
inline void applyKeyBasedValueEx_Scale_float(const SKeyInputFloat& input, int key0, int key1, float t, void* target) {
    float value[3] = {0.0f, 0.0f, 0.0f};
    interpolateFloat3(input, key0, key1, t, value);
    // unresolved: virtual call on target at vtable +0x94 omitted
    static_cast<void>(target);
}

// 0x6289d4 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA3_fS6_EEEELin1EfEEfLi3ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
inline float getKeyBasedValueEx_Material_float3_m1(const SKeyInputFloat& input, int key0, int key1, float t, float* out) {
    return interpolateFloat3(input, key0, key1, t, out);
}

// 0x628af0 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
inline void applyKeyBasedValueEx_Material_float3_m1(const SKeyInputFloat& input, int key0, int key1, float t, void* target, const SApplicatorInfoParam* info) {
    float value[3];
    interpolateFloat3(input, key0, key1, t, value);
    // unattached: IMaterialParameters::setParameterCvt(target, info->parameterId, 0, value) not mapped
    static_cast<void>(target);
    static_cast<void>(info);
}

// 0x628b48 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_18SSceneNodePositionELin1EfEEfLi3ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
inline float getKeyBasedValueEx_Position_short(const SKeyInputQuantized<short>& input, int key0, int key1, float t, float* out) {
    return interpolateQuantized3(input, key0, key1, t, out);
}

// 0x628ca8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
inline void applyKeyBasedValueEx_Position_short(const SKeyInputQuantized<short>& input, int key0, int key1, float t, void* target) {
    float value[3] = {0.0f, 0.0f, 0.0f};
    interpolateQuantized3(input, key0, key1, t, value);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x628e40 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_PositionZ_float(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x628f54 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyBlendedValueEx_PositionY_char(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x629068 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_PositionY_char(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x62917c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyBlendedValueEx_PositionY_short(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x629290 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_PositionY_short(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x6294b8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_PositionY_float(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x6295cc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyBlendedValueEx_PositionX_char(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x6296e0 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_PositionX_char(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x6297f4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyBlendedValueEx_PositionX_short(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x629908 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_PositionX_short(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x629b30 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_PositionX_float(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x629c44 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEE19applyBlendedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
inline void applyBlendedValueEx_Position_char(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x629d58 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEE17applyAddedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_Position_char(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0xA4 omitted
    static_cast<void>(target);
}

// 0x629e6c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE15getBlendedValueEPvPfiSD_
inline void getBlendedValue_ScaleX_short(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x629f50 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE15getBlendedValueEPvPfiSD_
inline void getBlendedValue_ScaleX_char(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62a118 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE15getBlendedValueEPvPfiSD_
inline void getBlendedValue_ScaleY_short(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62a1fc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE15getBlendedValueEPvPfiSD_
inline void getBlendedValue_ScaleY_char(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62a3c4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE15getBlendedValueEPvPfiSD_
inline void getBlendedValue_ScaleZ_short(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62a4a8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE15getBlendedValueEPvPfiSD_
inline void getBlendedValue_ScaleZ_char(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62a58c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE15getBlendedValueEPvPfiSD_
inline void getBlendedValue_Material_float3_0(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62a670 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE15getBlendedValueEPvPfiSD_
inline void getBlendedValue_Material_float3_m1(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62a754 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEEEE13getAddedValueEPvPfiSB_
inline void getAddedValue_Position_float(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62a838 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE13getAddedValueEPvPfiSB_
inline void getAddedValue_Position_short(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62a91c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE13getAddedValueEPvPfiSB_
inline void getAddedValue_Position_char(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62aa00 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_PositionX_float(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62aae4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_PositionX_short(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62abc8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_PositionX_char(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62acac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_PositionY_float(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62ad90 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_PositionY_short(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62ae74 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_PositionY_char(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62af58 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_PositionZ_float(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62b03c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_PositionZ_short(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62b120 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_PositionZ_char(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62b204 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEEEE13getAddedValueEPvPfiSB_
inline void getAddedValue_Scale_float(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62b2e8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE13getAddedValueEPvPfiSB_
inline void getAddedValue_Scale_short(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62b3cc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE13getAddedValueEPvPfiSB_
inline void getAddedValue_Scale_char(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62b4b0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_ScaleX_float(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62b594 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_ScaleX_short(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62b678 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_ScaleX_char(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62b75c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_ScaleY_float(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62b840 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_ScaleY_short(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62b924 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_ScaleY_char(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62ba08 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_ScaleZ_float(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62baec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_ScaleZ_short(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62bbd0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_ScaleZ_char(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62bcb4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_Material_float3_0(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62bd98 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE13getAddedValueEPvPfiSD_
inline void getAddedValue_Material_float3_m1(const float* keys, const float* weights, int count, float* out) {
    blendVec3(keys, weights, count, out);
}

// 0x62be7c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyBlendedValueEx_ScaleZ_char(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0x94 omitted
    static_cast<void>(target);
}

// 0x62bf90 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_ScaleZ_char(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0x94 omitted
    static_cast<void>(target);
}

// 0x62c0a4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyBlendedValueEx_ScaleZ_short(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0x94 omitted
    static_cast<void>(target);
}

// 0x62c1b8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_ScaleZ_short(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0x94 omitted
    static_cast<void>(target);
}

// 0x62c3e0 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_ScaleZ_float(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0x94 omitted
    static_cast<void>(target);
}

// 0x62c4f4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyBlendedValueEx_ScaleY_char(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0x94 omitted
    static_cast<void>(target);
}

// 0x62c608 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
inline void applyAddedValueEx_ScaleY_char(const float* keys, const float* weights, unsigned int count, void* target) {
    float sum[3];
    // Hex-Rays only sums when count is above 1 and never stores the sum
    sumVec3(keys, weights, count > 1 ? static_cast<int>(count) : 0, sum);
    // unresolved: virtual call on target at vtable +0x94 omitted
    static_cast<void>(target);
}

}  // namespace glitch::collada::recon_16
