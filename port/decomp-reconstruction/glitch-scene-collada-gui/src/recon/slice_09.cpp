// Slice 09: glitch::collada::animation_track key-based value getters.
//
// Each function's body follows its Hex-Rays pseudocode (address in the comment
// above it). Accessor, reader and default-value layouts are the stand-ins in
// include/glitch-scene-collada-gui/recon/slice_09.hpp. Original 32-bit member
// offsets are kept as comments; key data is indexed by element, not by raw
// byte offset.
#include "glitch-scene-collada-gui/recon/slice_09.hpp"

namespace glitch::collada::recon_09 {

template <class T, class F, int N>
CInputReader<T, F, N>::CInputReader(SAnimationAccessor* accessor)
    : output(accessor->getOutput(0)), scales(accessor->getScales()), offsets(accessor->getOffsets()) {}

namespace {

// Hex-Rays stores the reader in three DWORDs: output stream, scales, offsets.
// Component c of key 'key' (N components per key) is key * scale + offset.
template <class T, class F, int N>
F decodeComponent(const CInputReader<T, F, N>& reader, int key, int c) {
    return static_cast<F>(reader.keys()[N * key + c]) * reader.scales[c] + reader.offsets[c];
}

template <class T, class F, int N>
void decodeKey(const CInputReader<T, F, N>& reader, int key, F* out) {
    for (int c = 0; c < N; ++c) out[c] = decodeComponent(reader, key, c);
}

// Default value of the track if present, otherwise null.
const float* defaultOrNull(const SAnimationAccessor* self) {
    return self->hasDefaultValue() ? self->getDefaultValue() : nullptr;
}

// Byte blend used by the uchar interpolators: from + t * (to - from), as unsigned.
unsigned blendByte(std::uint8_t from, std::uint8_t to, float t) {
    return static_cast<unsigned>(static_cast<float>(from) + t * static_cast<float>(to - from));
}

// out[c] = blend(key a3, key a4, t)[c] - key a2[c], stored as bytes.
// Returns the last blended component (the Hex-Rays result register).
template <int N>
unsigned blendDeltaBytes(const std::uint8_t* data, int a2, int a3, int a4, float t, std::uint8_t* out) {
    const std::uint8_t* k2 = data + N * a2;
    const std::uint8_t* k3 = data + N * a3;
    const std::uint8_t* k4 = data + N * a4;
    unsigned result = 0;
    for (int c = 0; c < N; ++c) {
        result = blendByte(k3[c], k4[c], t);
        out[c] = static_cast<std::uint8_t>(result - k2[c]);
    }
    return result;
}

// out[c] = key a4[c] - key a3[c] (bytes).
template <int N>
void deltaBytes(const std::uint8_t* data, int a3, int a4, std::uint8_t* out) {
    const std::uint8_t* k3 = data + N * a3;
    const std::uint8_t* k4 = data + N * a4;
    for (int c = 0; c < N; ++c) out[c] = static_cast<std::uint8_t>(k4[c] - k3[c]);
}

// out[c] = key a3[c] (bytes).
template <int N>
void copyBytes(const std::uint8_t* data, int a3, std::uint8_t* out) {
    const std::uint8_t* k3 = data + N * a3;
    for (int c = 0; c < N; ++c) out[c] = k3[c];
}

// Quaternion helpers. The original negates the xyz words by adding 0x80000000
// to the float bit patterns, which is a sign flip.
void conjugateInPlace(float q[4]) {
    q[0] = -q[0];
    q[1] = -q[1];
    q[2] = -q[2];
}

void copyQuat(float* dst, const float* src) {
    for (int c = 0; c < 4; ++c) dst[c] = src[c];
}

// Quaternion read of key a2 in float form, reused by the interpolators below.
template <class T>
float quatKeyValues(SAnimationAccessor* self, int key, float* out) {
    CInputReader<T, float, 4> reader(self);
    decodeKey(reader, key, out);
    return out[1];
}

// Blend of two quaternion keys with weights {1 - t, t}, through CBlender.
template <class T>
bool quatBlendKeys(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    CInputReader<T, float, 4> reader(self);
    float values[8];
    decodeKey(reader, a2, values);
    decodeKey(reader, a3, values + 4);
    const float weights[2] = {1.0f - t, t};
    return ext::blender_quat_getBlendedValueEx(values, weights, 2, out);
}

// conj(key a2) * key a3.
template <class T>
float quatRelative(SAnimationAccessor* self, int a2, int a3, float* out) {
    float q3[4];
    float q2[4];
    quatKeyValues<T>(self, a3, q3);
    quatKeyValues<T>(self, a2, q2);
    conjugateInPlace(q2);
    float product[4];
    ext::quaternion_multiply(product, q2, q3);
    copyQuat(out, product);
    return product[0];
}

// conj(key a2) * slerp(key a3, key a4, t).
template <class T>
float quatSlerpRelative(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    float q3[4];
    float q4[4];
    float q2[4];
    quatKeyValues<T>(self, a3, q3);
    quatKeyValues<T>(self, a4, q4);
    quatKeyValues<T>(self, a2, q2);
    float slerped[4];
    ext::quaternion_slerp(slerped, q3, q4, t);
    conjugateInPlace(q2);
    float product[4];
    ext::quaternion_multiply(product, q2, slerped);
    copyQuat(out, product);
    return product[0];
}

// Quaternion-angle (1 animated component, components 0..2 from default).
template <class T>
const float* angleKey(SAnimationAccessor* self, int a2, float* out) {
    CInputReader<T, float, 1> reader(self);
    const float value = decodeComponent(reader, a2, 0);
    const float* def = defaultOrNull(self);
    if (def != nullptr) {
        out[0] = def[0];
        out[1] = def[1];
        out[2] = def[2];
        out[3] = value;
    } else {
        out[0] = value;
    }
    return def;
}

template <class T>
float angleLerp(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    CInputReader<T, float, 1> reader(self);
    const float v2 = decodeComponent(reader, a2, 0);
    const float v3 = decodeComponent(reader, a3, 0);
    const float result = t * (v3 - v2) + v2;
    const float* def = defaultOrNull(self);
    if (def != nullptr) {
        for (int i = 0; i < 3; ++i) out[i] = def[i];
        out[3] = result;
    } else {
        out[0] = result;
    }
    return result;
}

// Three-component position or scale key, read as floats (also the 'value' form).
template <class T>
float vec3Key(SAnimationAccessor* self, int key, float* out) {
    CInputReader<T, float, 3> reader(self);
    decodeKey(reader, key, out);
    return out[0];
}

// out = key a3 - key a2, component-wise.
template <class T>
float vec3Delta(SAnimationAccessor* self, int a2, int a3, float* out) {
    CInputReader<T, float, 3> reader(self);
    float key2[3];
    float key3[3];
    decodeKey(reader, a2, key2);
    decodeKey(reader, a3, key3);
    float result = 0.0f;
    for (int c = 0; c < 3; ++c) {
        result = key3[c] - key2[c];
        out[c] = result;
    }
    return result;
}

// out = (key a3 + t * (key a4 - key a3)) - key a2, component-wise.
template <class T>
float vec3Lerp(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    CInputReader<T, float, 3> reader(self);
    float key2[3];
    float key3[3];
    float key4[3];
    decodeKey(reader, a2, key2);
    decodeKey(reader, a3, key3);
    decodeKey(reader, a4, key4);
    float result = 0.0f;
    for (int c = 0; c < 3; ++c) {
        result = (key3[c] + (t * (key4[c] - key3[c]))) - key2[c];
        out[c] = result;
    }
    return result;
}

// X component: value of key a2 (one animated value); out[1..2] from the default.
template <class T>
const float* posXKey(SAnimationAccessor* self, int a2, float* out) {
    CInputReader<T, float, 1> reader(self);
    const float value = decodeComponent(reader, a2, 0);
    const float* def = defaultOrNull(self);
    out[0] = value;
    if (def != nullptr) {
        out[1] = def[1];
        out[2] = def[2];
    }
    return def;
}

template <class T>
float posXLerp(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    CInputReader<T, float, 1> reader(self);
    const float v2 = decodeComponent(reader, a2, 0);
    const float v3 = decodeComponent(reader, a3, 0);
    const float result = t * (v3 - v2) + v2;
    const float* def = defaultOrNull(self);
    out[0] = result;
    if (def != nullptr) {
        out[1] = def[1];
        out[2] = def[2];
    }
    return result;
}

template <class T>
const float* posXDelta(SAnimationAccessor* self, int a2, int a3, float* out) {
    CInputReader<T, float, 1> reader(self);
    const float v2 = decodeComponent(reader, a2, 0);
    const float v3 = decodeComponent(reader, a3, 0);
    const float* def = defaultOrNull(self);
    out[0] = v3 - v2;
    if (def != nullptr) {
        out[1] = def[1];
        out[2] = def[2];
    }
    return def;
}

template <class T>
float posXLerpBlend(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    CInputReader<T, float, 1> reader(self);
    const float v2 = decodeComponent(reader, a2, 0);
    const float v3 = decodeComponent(reader, a3, 0) - v2;
    const float v4 = decodeComponent(reader, a4, 0) - v2;
    const float result = v3 + (t * (v4 - v3));
    const float* def = defaultOrNull(self);
    out[0] = result;
    if (def != nullptr) {
        out[1] = def[1];
        out[2] = def[2];
    }
    return result;
}

// Y component: out[0] and out[2] from the default, out[1] animated.
const float* posYKey(SAnimationAccessor* self, int a2, float* out) {
    CInputReader<std::int16_t, float, 1> reader(self);
    const float value = decodeComponent(reader, a2, 0);
    const float* def = defaultOrNull(self);
    if (def != nullptr) {
        out[0] = def[0];
        out[1] = value;
        out[2] = def[2];
    } else {
        out[0] = value;
    }
    return def;
}

float posYLerp(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    CInputReader<std::int16_t, float, 1> reader(self);
    const float v2 = decodeComponent(reader, a2, 0);
    const float v3 = decodeComponent(reader, a3, 0);
    const float result = t * (v3 - v2) + v2;
    const float* def = defaultOrNull(self);
    if (def != nullptr) {
        out[0] = def[0];
        out[1] = result;
        out[2] = def[2];
    } else {
        out[0] = result;
    }
    return result;
}

}  // namespace

// ---- CInterpreter<CMixin<uchar,3,SLightColor,-1,uchar>,uchar,3,SUseDefaultLerp<uchar>> ----

// 0x6126c8 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi3ENS1_11SLightColorELin1EhEEhLi3ENS1_15SUseDefaultLerpIhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
unsigned interp_light_u8x3::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, std::uint8_t* out) {
    // v15[0] = 1 - t, v15[1] = t; the two keys are at element a2 and a2 + 1
    const float weights[2] = {1.0f - t, t};
    const std::uint8_t* data = static_cast<const std::uint8_t*>(self->getOutput(0)->data);
    const std::uint8_t* keys[2] = {data + 3 * a2, data + 3 * (a2 + 1)};
    float comp[3] = {0.0f, 0.0f, 0.0f};
    for (int s = 0; s < 2; ++s) {
        for (int c = 0; c < 3; ++c) {
            comp[c] = static_cast<float>(keys[s][c]) * weights[s] + comp[c];
        }
    }
    for (int c = 0; c < 3; ++c) out[c] = static_cast<std::uint8_t>(static_cast<unsigned>(comp[c]));
    return static_cast<unsigned>(comp[2]);
}

// 0x612874 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi3ENS1_11SLightColorELin1EhEEhLi3ENS1_15SUseDefaultLerpIhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
unsigned interp_light_u8x3::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, std::uint8_t* out) {
    const std::uint8_t* data = static_cast<const std::uint8_t*>(self->getOutput(0)->data);
    return blendDeltaBytes<3>(data, a2, a3, a4, t, out);
}

// 0x6127a4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
float apply_light_u8x3::applyKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, CApplicatorInfo* info) {
    std::uint8_t value[12] = {};  // original zeroes a 12-byte scratch; 3 bytes are used
    interp_light_u8x3::getKeyBasedValueEx(self, a2, a3, t, value);
    return ext::applicator_light_color_applyValueEx(info, value);
}

// 0x6127e0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
float virtual_light_u8x3::applyKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, CApplicatorInfo* info) {
    return apply_light_u8x3::applyKeyBasedValueEx(self, a2, a3, t, info);
}

// 0x612804 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
unsigned virtual_light_u8x3::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, std::uint8_t* out) {
    return interp_light_u8x3::getKeyBasedValueEx(self, a2, a3, t, out);
}

// 0x612820 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
SAnimationOutput* virtual_light_u8x3::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, std::uint8_t* out) {
    const std::uint8_t* data = static_cast<const std::uint8_t*>(self->getOutput(0)->data);
    deltaBytes<3>(data, a2, a3, out);
    return self->getOutput(0);
}

// 0x612908 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
unsigned virtual_light_u8x3::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, std::uint8_t* out) {
    return interp_light_u8x3::getKeyBasedValueEx(self, a2, a3, a4, t, out);
}

// ---- CApplyValueEx<uchar[3],CMixin<uchar,3,SMaterialSetParam...>> ----

// 0x61292c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
SAnimationOutput* virtual_material_u8x3::getKeyBasedValue(SAnimationAccessor* self, int a2, std::uint8_t* out) {
    const std::uint8_t* data = static_cast<const std::uint8_t*>(self->getOutput(0)->data);
    copyBytes<3>(data, a2, out);
    return self->getOutput(0);
}

// 0x612970 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
SAnimationOutput* virtual_material_u8x3::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, std::uint8_t* out) {
    const std::uint8_t* data = static_cast<const std::uint8_t*>(self->getOutput(0)->data);
    deltaBytes<3>(data, a2, a3, out);
    return self->getOutput(0);
}

// 0x612a58 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
unsigned virtual_material_u8x3::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, std::uint8_t* out) {
    return interp_material_u8x3::getKeyBasedValueEx(self, a2, a3, a4, t, out);
}

// 0x6129c4 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA3_hNS_5video6SColorEEEEELin1EhEEhLi3ENS1_15SUseDefaultLerpIhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
unsigned interp_material_u8x3::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, std::uint8_t* out) {
    const std::uint8_t* data = static_cast<const std::uint8_t*>(self->getOutput(0)->data);
    return blendDeltaBytes<3>(data, a2, a3, a4, t, out);
}

// ---- CApplyValueEx<float[2],CMixin<float,2,SMaterialSetParam...>> ----

// 0x612a7c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
SAnimationOutput* virtual_float2::getKeyBasedValue(SAnimationAccessor* self, int a2, float* out) {
    const float* data = static_cast<const float*>(self->getOutput(0)->data);
    out[0] = data[2 * a2];
    out[1] = data[2 * a2 + 1];
    return self->getOutput(0);
}

// 0x612ab0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float virtual_float2::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out) {
    const float* data = static_cast<const float*>(self->getOutput(0)->data);
    out[0] = data[2 * a3] - data[2 * a2];
    const float result = data[2 * a3 + 1] - data[2 * a2 + 1];
    out[1] = result;
    return result;
}

// 0x612afc _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA2_fS6_EEEELin1EfEEfLi2ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float interp_float2::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    const float* data = static_cast<const float*>(self->getOutput(0)->data);
    out[0] = (data[2 * a3] + (t * (data[2 * a4] - data[2 * a3]))) - data[2 * a2];
    const float result = (data[2 * a3 + 1] + (t * (data[2 * a4 + 1] - data[2 * a3 + 1]))) - data[2 * a2 + 1];
    out[1] = result;
    return result;
}

// 0x612b98 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float virtual_float2::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return interp_float2::getKeyBasedValueEx(self, a2, a3, a4, t, out);
}

// ---- CApplyValueEx<uchar[4],CMixin<uchar,4,SMaterialSetParam...>> ----

// 0x612bbc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
SAnimationOutput* virtual_u8x4::getKeyBasedValue(SAnimationAccessor* self, int a2, std::uint8_t* out) {
    const std::uint8_t* data = static_cast<const std::uint8_t*>(self->getOutput(0)->data);
    copyBytes<4>(data, a2, out);
    return self->getOutput(0);
}

// 0x612c08 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
SAnimationOutput* virtual_u8x4::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, std::uint8_t* out) {
    const std::uint8_t* data = static_cast<const std::uint8_t*>(self->getOutput(0)->data);
    deltaBytes<4>(data, a2, a3, out);
    return self->getOutput(0);
}

// 0x612c54 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELin1EhEEhLi4ENS1_15SUseDefaultLerpIhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
unsigned interp_u8x4::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, std::uint8_t* out) {
    const std::uint8_t* data = static_cast<const std::uint8_t*>(self->getOutput(0)->data);
    return blendDeltaBytes<4>(data, a2, a3, a4, t, out);
}

// 0x612cdc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
unsigned virtual_u8x4::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, std::uint8_t* out) {
    return interp_u8x4::getKeyBasedValueEx(self, a2, a3, a4, t, out);
}

// ---- quaternion blend and add forwards ----

// 0x6132e4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
bool quat_mixin_float::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    return ext::interp_quat_float_getKeyBasedValueEx(self, a2, a3, t, out);
}

// 0x613588 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE13getAddedValueEPvPfiSA_
void* quat_mixin_float::getAddedValue(const void* values, const float* weights, int count, void* out) {
    return ext::blender_quat_getAddedValueEx(values, weights, count, out);
}

// 0x613314 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE15getBlendedValueEPvPfiSA_
bool quat_mixin_short::getBlendedValue(const void* values, const float* weights, int count, void* out) {
    return ext::blender_quat_getBlendedValueEx(values, weights, count, out);
}

// 0x61359c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE13getAddedValueEPvPfiSA_
void* quat_mixin_short::getAddedValue(const void* values, const float* weights, int count, void* out) {
    return ext::blender_quat_getAddedValueEx(values, weights, count, out);
}

// 0x6136d0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
float quat_mixin_short::getKeyBasedValue(SAnimationAccessor* self, int a2, float* out) {
    return interp_quat_short::getKeyBasedValueEx(self, a2, out);
}

// 0x61379c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float quat_mixin_short::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out) {
    return interpq_quat_short::getKeyBasedValueEx(self, a2, a3, out);
}

// 0x6138d0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float quat_mixin_short::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return interpq_quat_short::getKeyBasedValueEx(self, a2, a3, a4, t, out);
}

// 0x6139b8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
bool quat_mixin_short::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    return interp_quat_short::getKeyBasedValueEx(self, a2, a3, t, out);
}

// 0x613328 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE15getBlendedValueEPvPfiSA_
bool quat_mixin_char::getBlendedValue(const void* values, const float* weights, int count, void* out) {
    return ext::blender_quat_getBlendedValueEx(values, weights, count, out);
}

// 0x6135b0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE13getAddedValueEPvPfiSA_
void* quat_mixin_char::getAddedValue(const void* values, const float* weights, int count, void* out) {
    return ext::blender_quat_getAddedValueEx(values, weights, count, out);
}

// 0x613aa4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
float quat_mixin_char::getKeyBasedValue(SAnimationAccessor* self, int a2, float* out) {
    return interp_quat_char::getKeyBasedValueEx(self, a2, out);
}

// 0x613b70 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float quat_mixin_char::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out) {
    return interpq_quat_char::getKeyBasedValueEx(self, a2, a3, out);
}

// 0x613ca4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float quat_mixin_char::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return interpq_quat_char::getKeyBasedValueEx(self, a2, a3, a4, t, out);
}

// 0x613d8c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
bool quat_mixin_char::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    return interp_quat_char::getKeyBasedValueEx(self, a2, a3, t, out);
}

// 0x61333c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEEEE15getBlendedValueEPvPfiSA_
bool quat_angle_float::getBlendedValue(const void* values, const float* weights, int count, void* out) {
    return ext::blender_quat_getBlendedValueEx(values, weights, count, out);
}

// 0x6135c4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEEEE13getAddedValueEPvPfiSA_
void* quat_angle_float::getAddedValue(const void* values, const float* weights, int count, void* out) {
    return ext::blender_quat_getAddedValueEx(values, weights, count, out);
}

// 0x613350 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE15getBlendedValueEPvPfiSA_
bool quat_angle_short::getBlendedValue(const void* values, const float* weights, int count, void* out) {
    return ext::blender_quat_getBlendedValueEx(values, weights, count, out);
}

// 0x6135d8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE13getAddedValueEPvPfiSA_
void* quat_angle_short::getAddedValue(const void* values, const float* weights, int count, void* out) {
    return ext::blender_quat_getAddedValueEx(values, weights, count, out);
}

// 0x613364 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE15getBlendedValueEPvPfiSA_
bool quat_angle_char::getBlendedValue(const void* values, const float* weights, int count, void* out) {
    return ext::blender_quat_getBlendedValueEx(values, weights, count, out);
}

// 0x6135ec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE13getAddedValueEPvPfiSA_
void* quat_angle_char::getAddedValue(const void* values, const float* weights, int count, void* out) {
    return ext::blender_quat_getAddedValueEx(values, weights, count, out);
}

// ---- quaternion interpolators ----

// 0x61363c _ZN6glitch7collada15animation_track12CInterpreterINS1_25CSceneNodeQuaternionMixinIsEEfLi4ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
float interp_quat_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out) {
    return quatKeyValues<std::int16_t>(self, a2, out);
}

// 0x6138f4 _ZN6glitch7collada15animation_track12CInterpreterINS1_25CSceneNodeQuaternionMixinIsEEfLi4ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
bool interp_quat_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    return quatBlendKeys<std::int16_t>(self, a2, a3, t, out);
}

// 0x6136e0 _ZN6glitch7collada15animation_track22CInterpreterQuaternionINS1_25CSceneNodeQuaternionMixinIsEEsE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float interpq_quat_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out) {
    return quatRelative<std::int16_t>(self, a2, a3, out);
}

// 0x6137b0 _ZN6glitch7collada15animation_track22CInterpreterQuaternionINS1_25CSceneNodeQuaternionMixinIsEEsE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float interpq_quat_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return quatSlerpRelative<std::int16_t>(self, a2, a3, a4, t, out);
}

// 0x613a10 _ZN6glitch7collada15animation_track12CInterpreterINS1_25CSceneNodeQuaternionMixinIcEEfLi4ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
float interp_quat_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out) {
    return quatKeyValues<std::int8_t>(self, a2, out);
}

// 0x613cc8 _ZN6glitch7collada15animation_track12CInterpreterINS1_25CSceneNodeQuaternionMixinIcEEfLi4ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
bool interp_quat_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    return quatBlendKeys<std::int8_t>(self, a2, a3, t, out);
}

// 0x613ab4 _ZN6glitch7collada15animation_track22CInterpreterQuaternionINS1_25CSceneNodeQuaternionMixinIcEEcE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float interpq_quat_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out) {
    return quatRelative<std::int8_t>(self, a2, a3, out);
}

// 0x613b84 _ZN6glitch7collada15animation_track22CInterpreterQuaternionINS1_25CSceneNodeQuaternionMixinIcEEcE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float interpq_quat_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return quatSlerpRelative<std::int8_t>(self, a2, a3, a4, t, out);
}

// ---- quaternion-angle interpolators ----

// 0x614a90 _ZN6glitch7collada15animation_track12CInterpreterINS1_30CSceneNodeQuaternionAngleMixinIsEEfLi4ENS1_17SUseDefaultValuesILi3EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const float* interp_quat_angle_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out) {
    return angleKey<std::int16_t>(self, a2, out);
}

// 0x614b38 _ZN6glitch7collada15animation_track12CInterpreterINS1_30CSceneNodeQuaternionAngleMixinIsEEfLi4ENS1_17SUseDefaultValuesILi3EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float interp_quat_angle_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    return angleLerp<std::int16_t>(self, a2, a3, t, out);
}

// 0x614c44 _ZN6glitch7collada15animation_track12CInterpreterINS1_30CSceneNodeQuaternionAngleMixinIcEEfLi4ENS1_17SUseDefaultValuesILi3EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const float* interp_quat_angle_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out) {
    return angleKey<std::int8_t>(self, a2, out);
}

// 0x614ce8 _ZN6glitch7collada15animation_track12CInterpreterINS1_30CSceneNodeQuaternionAngleMixinIcEEfLi4ENS1_17SUseDefaultValuesILi3EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float interp_quat_angle_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    return angleLerp<std::int8_t>(self, a2, a3, t, out);
}

// ---- CInterpreter<CMixin<float,3,SSceneNodePosition>> ----

// 0x613e5c _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_18SSceneNodePositionELin1EfEEfLi3ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
float interp_pos_f3_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out) {
    return vec3Key<std::int16_t>(self, a2, out);
}

// 0x613efc _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_18SSceneNodePositionELin1EfEEfLi3ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float interp_pos_f3_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out) {
    return vec3Delta<std::int16_t>(self, a2, a3, out);
}

// 0x613fec _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_18SSceneNodePositionELin1EfEEfLi3ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float interp_pos_f3_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return vec3Lerp<std::int16_t>(self, a2, a3, a4, t, out);
}

// 0x613eec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
float virtual_pos_f3_short::getKeyBasedValue(SAnimationAccessor* self, int a2, float* out) {
    return interp_pos_f3_short::getKeyBasedValueEx(self, a2, out);
}

// 0x613fd8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float virtual_pos_f3_short::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out) {
    return interp_pos_f3_short::getKeyBasedValueEx(self, a2, a3, out);
}

// 0x614130 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float virtual_pos_f3_short::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return interp_pos_f3_short::getKeyBasedValueEx(self, a2, a3, a4, t, out);
}

// 0x614488 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_18SSceneNodePositionELin1EfEEfLi3ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
float interp_pos_f3_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out) {
    return vec3Key<std::int8_t>(self, a2, out);
}

// 0x614528 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_18SSceneNodePositionELin1EfEEfLi3ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float interp_pos_f3_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out) {
    return vec3Delta<std::int8_t>(self, a2, a3, out);
}

// 0x61461c _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_18SSceneNodePositionELin1EfEEfLi3ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float interp_pos_f3_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return vec3Lerp<std::int8_t>(self, a2, a3, a4, t, out);
}

// 0x614518 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
float virtual_pos_f3_char::getKeyBasedValue(SAnimationAccessor* self, int a2, float* out) {
    return interp_pos_f3_char::getKeyBasedValueEx(self, a2, out);
}

// 0x614608 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float virtual_pos_f3_char::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out) {
    return interp_pos_f3_char::getKeyBasedValueEx(self, a2, a3, out);
}

// 0x614768 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float virtual_pos_f3_char::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return interp_pos_f3_char::getKeyBasedValueEx(self, a2, a3, a4, t, out);
}

// ---- CInterpreter<CSceneNodeScaleMixin<short|char>,float,3,...> ----

// 0x614154 _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIsEEfLi3ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
float interp_scale_f3_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out) {
    return vec3Key<std::int16_t>(self, a2, out);
}

// 0x6141f4 _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIsEEfLi3ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float interp_scale_f3_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out) {
    return vec3Delta<std::int16_t>(self, a2, a3, out);
}

// 0x6142e4 _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIsEEfLi3ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float interp_scale_f3_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return vec3Lerp<std::int16_t>(self, a2, a3, a4, t, out);
}

// 0x6141e4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
float virtual_scale_f3_short::getKeyBasedValue(SAnimationAccessor* self, int a2, float* out) {
    return interp_scale_f3_short::getKeyBasedValueEx(self, a2, out);
}

// 0x6142d0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float virtual_scale_f3_short::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out) {
    return interp_scale_f3_short::getKeyBasedValueEx(self, a2, a3, out);
}

// 0x614428 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float virtual_scale_f3_short::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return interp_scale_f3_short::getKeyBasedValueEx(self, a2, a3, a4, t, out);
}

// 0x61478c _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIcEEfLi3ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
float interp_scale_f3_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out) {
    return vec3Key<std::int8_t>(self, a2, out);
}

// 0x61482c _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIcEEfLi3ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float interp_scale_f3_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out) {
    return vec3Delta<std::int8_t>(self, a2, a3, out);
}

// 0x614920 _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIcEEfLi3ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float interp_scale_f3_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return vec3Lerp<std::int8_t>(self, a2, a3, a4, t, out);
}

// 0x61481c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
float virtual_scale_f3_char::getKeyBasedValue(SAnimationAccessor* self, int a2, float* out) {
    return interp_scale_f3_char::getKeyBasedValueEx(self, a2, out);
}

// 0x61490c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float virtual_scale_f3_char::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out) {
    return interp_scale_f3_char::getKeyBasedValueEx(self, a2, a3, out);
}

// 0x614a6c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float virtual_scale_f3_char::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return interp_scale_f3_char::getKeyBasedValueEx(self, a2, a3, a4, t, out);
}

// ---- CInterpreter<CSceneNodePositionXEx<short|char>,float,3,SUseDefaultValues<0,...>> ----

// 0x614dec _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionXExIsEEfLi3ENS1_17SUseDefaultValuesILi0EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const float* interp_pos_x_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out) {
    return posXKey<std::int16_t>(self, a2, out);
}

// 0x614e9c _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionXExIsEEfLi3ENS1_17SUseDefaultValuesILi0EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float interp_pos_x_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    return posXLerp<std::int16_t>(self, a2, a3, t, out);
}

// 0x614fc8 _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionXExIsEEfLi3ENS1_17SUseDefaultValuesILi0EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const float* interp_pos_x_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out) {
    return posXDelta<std::int16_t>(self, a2, a3, out);
}

// 0x6150ac _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionXExIsEEfLi3ENS1_17SUseDefaultValuesILi0EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float interp_pos_x_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return posXLerpBlend<std::int16_t>(self, a2, a3, a4, t, out);
}

// 0x614e8c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const float* virtual_pos_x_short::getKeyBasedValue(SAnimationAccessor* self, int a2, float* out) {
    return interp_pos_x_short::getKeyBasedValueEx(self, a2, out);
}

// 0x614fac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float virtual_pos_x_short::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    return interp_pos_x_short::getKeyBasedValueEx(self, a2, a3, t, out);
}

// 0x615098 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const float* virtual_pos_x_short::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out) {
    return interp_pos_x_short::getKeyBasedValueEx(self, a2, a3, out);
}

// 0x6151f8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float virtual_pos_x_short::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return interp_pos_x_short::getKeyBasedValueEx(self, a2, a3, a4, t, out);
}

// 0x61521c _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionXExIcEEfLi3ENS1_17SUseDefaultValuesILi0EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const float* interp_pos_x_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out) {
    return posXKey<std::int8_t>(self, a2, out);
}

// 0x6152c8 _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionXExIcEEfLi3ENS1_17SUseDefaultValuesILi0EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float interp_pos_x_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    return posXLerp<std::int8_t>(self, a2, a3, t, out);
}

// 0x6153ec _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionXExIcEEfLi3ENS1_17SUseDefaultValuesILi0EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const float* interp_pos_x_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out) {
    return posXDelta<std::int8_t>(self, a2, a3, out);
}

// 0x6154c8 _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionXExIcEEfLi3ENS1_17SUseDefaultValuesILi0EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float interp_pos_x_char::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return posXLerpBlend<std::int8_t>(self, a2, a3, a4, t, out);
}

// 0x6152b8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const float* virtual_pos_x_char::getKeyBasedValue(SAnimationAccessor* self, int a2, float* out) {
    return interp_pos_x_char::getKeyBasedValueEx(self, a2, out);
}

// 0x6153d0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float virtual_pos_x_char::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    return interp_pos_x_char::getKeyBasedValueEx(self, a2, a3, t, out);
}

// 0x6154b4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const float* virtual_pos_x_char::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out) {
    return interp_pos_x_char::getKeyBasedValueEx(self, a2, a3, out);
}

// 0x615608 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float virtual_pos_x_char::getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out) {
    return interp_pos_x_char::getKeyBasedValueEx(self, a2, a3, a4, t, out);
}

// ---- CInterpreter<CSceneNodePositionYEx<short>,float,3,SUseDefaultValues<1,short>> ----

// 0x61562c _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionYExIsEEfLi3ENS1_17SUseDefaultValuesILi1EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const float* interp_pos_y_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out) {
    return posYKey(self, a2, out);
}

// 0x6156dc _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionYExIsEEfLi3ENS1_17SUseDefaultValuesILi1EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float interp_pos_y_short::getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out) {
    return posYLerp(self, a2, a3, t, out);
}

// 0x6156cc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const float* virtual_pos_y_short::getKeyBasedValue(SAnimationAccessor* self, int a2, float* out) {
    return interp_pos_y_short::getKeyBasedValueEx(self, a2, out);
}

// ---- CInputReader constructors (one template; see the table in the header) ----
// 0x613600 _ZN6glitch7collada15animation_track12CInputReaderIsfLi4EEC1ERKNS0_18SAnimationAccessorE
// 0x6139d4 _ZN6glitch7collada15animation_track12CInputReaderIcfLi4EEC1ERKNS0_18SAnimationAccessorE
// 0x613da8 _ZN6glitch7collada15animation_track12CInputReaderIsfLi1EEC1ERKNS0_18SAnimationAccessorE
// 0x613de4 _ZN6glitch7collada15animation_track12CInputReaderIcfLi1EEC1ERKNS0_18SAnimationAccessorE
// 0x613e20 _ZN6glitch7collada15animation_track12CInputReaderIsfLi3EEC1ERKNS0_18SAnimationAccessorE
// 0x61444c _ZN6glitch7collada15animation_track12CInputReaderIcfLi3EEC1ERKNS0_18SAnimationAccessorE

}  // namespace glitch::collada::recon_09
