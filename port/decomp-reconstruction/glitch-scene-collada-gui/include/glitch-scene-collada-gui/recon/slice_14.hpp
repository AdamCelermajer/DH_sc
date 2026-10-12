// Slice 14 reconstruction: glitch::collada::animation_track applicator wrappers
// (CVirtualEx / CApplyValueEx / CInterpreter instantiations for float[2], unsigned
// char[4] and vector3d<float> value types).
//
// Layouts of the applicator, the animation accessor and the material/scene-node
// targets are not mapped in this repository. Everything that depends on them is
// declared here and left undefined: accessorOutputData, applicatorParameterIndex,
// setParameterCvt, invokeVirtualSlot and the CInterpreter templates. The bodies in
// slice_14.cpp keep the control flow and the arithmetic of the pseudocode.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace glitch::video {
struct SColor;
}

namespace glitch::core {
template <class T>
struct vector2d;
}

namespace glitch::collada::recon_14 {

// Opaque stand-ins for unmapped types.
struct SAnimationAccessor;   // glitch::collada::SAnimationAccessor
struct SApplicatorInfo;      // glitch::collada::animation_track::CApplicatorInfo
struct SSceneNodePosition;   // animation_track::SSceneNodePosition

// Declared, defined by the owning slice or the engine layout.
// Data pointer of output block 'slot' of the accessor (word at +0x04 of the output).
const std::uint8_t* accessorOutputData(const SAnimationAccessor* accessor, unsigned slot);
// u16 parameter index of the applicator info (+0x08).
std::uint16_t applicatorParameterIndex(const SApplicatorInfo* info);
// IMaterialParameters<CMaterial, ...>::setParameterCvt<T>(target, index, flags, value).
template <class T>
int setParameterCvt(void* target, std::uint16_t index, unsigned flags, const void* value);
// Virtual dispatch through the vtable slot at byte offset SlotByteOffset of the target.
template <int SlotByteOffset>
int invokeVirtualSlot(void* self);
template <int SlotByteOffset>
int invokeVirtualSlot(void* self, const void* value);

// Stand-ins for the animation_track template names used as CInterpreter arguments.
template <class Scalar, unsigned N, class Target, int Index, class Value>
struct CMixin;
template <class T>
struct CSceneNodeScaleMixin;
template <class T>
struct CSceneNodePositionXEx;
template <class T>
struct CSceneNodePositionYEx;
template <class T>
struct CSceneNodePositionZEx;
template <class T>
struct CSceneNodeScaleXEx;
template <class T>
struct CSceneNodeScaleYEx;
template <class T>
struct CSceneNodeScaleZEx;
template <class... Ts>
struct SMaterialSetParam;
template <class... Ts>
struct SAnimationTypes;
template <int Index, class T>
struct SUseDefaultValues;
template <class T>
struct SUseDefaultLerp;

// CInterpreter<Mixin, Value, Components, Defaults>: key-based value lookup.
// Only the instantiations listed in slice_14.cpp are referenced from this slice.
template <class Mixin, class Value, unsigned Components, class Defaults>
struct CInterpreter {
    static void getKeyBasedValueEx(const SAnimationAccessor* accessor, int key, Value* out);
    static void getKeyBasedValueEx(const SAnimationAccessor* accessor, int key, int unused,
                                   float t, Value* out);
};

// Interpreter of CMixin<unsigned char, 4, ..., Index, unsigned char>.
template <int Index>
using UChar4Interp = CInterpreter<CMixin<unsigned char, 4, SMaterialSetParam<SAnimationTypes<unsigned char[4], glitch::video::SColor>>, Index, unsigned char>,
                                  unsigned char, 4, SUseDefaultValues<Index, unsigned char>>;

// Weighted sum of float[2] values (getAddedValue, applyAddedValueEx, applyBlendedValueEx).
// A single key is copied; otherwise each component is the sum of value * weight.
inline void weightedSumFloat2(const float* values, const float* weights, int count, float* out) {
    if (count == 1) {
        out[0] = values[0];
        out[1] = values[1];
        return;
    }
    float sx = 0.0f;
    float sy = 0.0f;
    for (int i = 0; i < count; ++i) {
        sx = sx + weights[i] * values[2 * i];
        sy = sy + weights[i] * values[2 * i + 1];
    }
    out[1] = sy;
    out[0] = sx;
}

// Weighted sum of four unsigned char channels per key, truncated back to bytes.
inline void addWeightedUChar4(const std::uint8_t* values, const float* weights, int count,
                              std::uint8_t* out) {
    if (count == 1) {
        for (int k = 0; k < 4; ++k) {
            out[k] = values[k];
        }
        return;
    }
    float acc[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    for (int i = 0; i < count; ++i) {
        for (int k = 0; k < 4; ++k) {
            acc[k] = static_cast<float>(static_cast<float>(values[4 * i + k]) * weights[i]) + acc[k];
        }
    }
    for (int k = 0; k < 4; ++k) {
        out[k] = static_cast<std::uint8_t>(static_cast<unsigned>(acc[k]));
    }
}

// applyValue of unsigned char[4]: the four bytes are passed through to parameter 0.
inline int applyUChar4Value(const std::uint8_t* value, void* target) {
    const std::uint8_t copy[4] = {value[0], value[1], value[2], value[3]};
    return setParameterCvt<glitch::video::SColor>(target, 0, 0, copy);
}

// applyAddedValue of unsigned char[4] (mixin 0).
inline int applyAddedUChar4(const std::uint8_t* values, const float* weights, int count,
                            void* target, const SApplicatorInfo* info) {
    std::uint8_t out[4] = {0, 0, 0, 0};
    addWeightedUChar4(values, weights, count, out);
    return setParameterCvt<glitch::video::SColor>(target, applicatorParameterIndex(info), 0, out);
}

// Key-based unsigned char[4] through a 3-argument interpreter call.
template <class Interp>
int applyKeyBasedUChar4(const SAnimationAccessor* accessor, int key, void* target,
                        const SApplicatorInfo* info) {
    std::uint8_t value[4] = {0, 0, 0, 0};
    Interp::getKeyBasedValueEx(accessor, key, value);
    return setParameterCvt<glitch::video::SColor>(target, applicatorParameterIndex(info), 0, value);
}

// Key-based unsigned char[4] with an explicit time (5-argument interpreter call).
template <class Interp>
int applyKeyBasedUChar4Ex(const SAnimationAccessor* accessor, int key, int unused, float t,
                          void* target, const SApplicatorInfo* info) {
    std::uint8_t value[4] = {0, 0, 0, 0};
    Interp::getKeyBasedValueEx(accessor, key, unused, t, value);
    return setParameterCvt<glitch::video::SColor>(target, applicatorParameterIndex(info), 0, value);
}

// Key-based unsigned char[4] read straight from the output block (4-byte keys).
inline int applyKeyBasedUChar4Direct(const SAnimationAccessor* accessor, int key, void* target,
                                     const SApplicatorInfo* info) {
    const std::uint8_t* value = accessorOutputData(accessor, 0) + 4 * key;
    return setParameterCvt<glitch::video::SColor>(target, applicatorParameterIndex(info), 0, value);
}

// Key-based vector3d<float> through a 3-argument interpreter call (3 words zeroed first).
template <class Interp, int Slot>
int applyKeyBasedVec3(const SAnimationAccessor* accessor, int key, void* target) {
    float value[3] = {0.0f, 0.0f, 0.0f};
    Interp::getKeyBasedValueEx(accessor, key, value);
    return invokeVirtualSlot<Slot>(target, value);
}

// Key-based vector3d<float> with an explicit time (5-argument interpreter call).
template <class Interp, int Slot>
int applyKeyBasedVec3Ex(const SAnimationAccessor* accessor, int key, int unused, float t,
                        void* target) {
    float value[3] = {0.0f, 0.0f, 0.0f};
    Interp::getKeyBasedValueEx(accessor, key, unused, t, value);
    return invokeVirtualSlot<Slot>(target, value);
}

// Key-based vector3d<float> read from the output block (12-byte keys, position mixin float).
template <int Slot>
int applyKeyBasedVec3Direct(const SAnimationAccessor* accessor, int key, void* target) {
    float value[3];
    std::memcpy(value, accessorOutputData(accessor, 0) + 12 * key, sizeof value);
    return invokeVirtualSlot<Slot>(target, value);
}

// Forward declarations of the instantiated free functions (one per row of the slice).
void CVirtualEx_f2_m1_getAddedValue_a5(const void* self, const float* values, const float* weights, int count, float* out);
void CVirtualEx_f2_mn1_getAddedValue_a5(const void* self, const float* values, const float* weights, int count, float* out);
float CInterpreter_f2mn1_getKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, float* out);
float CVirtualEx_f2_mn1_getKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, float* out);
int CApplyValueEx_f2_mn1_applyKeyBasedValueEx_a6(const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info);
int CVirtualEx_f2_mn1_applyKeyBasedValue_a7(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info);
int CApplyValueEx_f2_m0_applyAddedValueEx_a5(const float* values, const float* weights, int count, void* target, const SApplicatorInfo* info);
int CVirtualEx_f2_m0_applyAddedValue_a6(const void* self, const float* values, const float* weights, int count, void* target, const SApplicatorInfo* info);
int CApplyValueEx_f2_m0_applyBlendedValueEx_a5(const float* values, const float* weights, int count, void* target, const SApplicatorInfo* info);
int CVirtualEx_f2_m0_applyBlendedValue_a6(const void* self, const float* values, const float* weights, int count, void* target, const SApplicatorInfo* info);
int CVirtualEx_u4_m0_applyValue_a3(const void* self, const std::uint8_t* value, void* target);
int CVirtualEx_u4_m0_applyKeyBasedValue_a5(const void* self, const SAnimationAccessor* accessor, int key, void* target, const SApplicatorInfo* info);
int CApplyValueEx_u4_m0_applyKeyBasedValueEx_a6(const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info);
int CVirtualEx_u4_m0_applyKeyBasedValue_a7(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info);
int CVirtualEx_u4_m0_applyAddedValue_a6(const void* self, const std::uint8_t* values, const float* weights, int count, void* target, const SApplicatorInfo* info);
int CVirtualEx_u4_m1_applyValue_a3(const void* self, const std::uint8_t* value, void* target);
int CVirtualEx_u4_m1_applyKeyBasedValue_a5(const void* self, const SAnimationAccessor* accessor, int key, void* target, const SApplicatorInfo* info);
int CApplyValueEx_u4_m1_applyKeyBasedValueEx_a6(const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info);
int CVirtualEx_u4_m1_applyKeyBasedValue_a7(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info);
int CVirtualEx_u4_m2_applyValue_a3(const void* self, const std::uint8_t* value, void* target);
int CVirtualEx_u4_m2_applyKeyBasedValue_a5(const void* self, const SAnimationAccessor* accessor, int key, void* target, const SApplicatorInfo* info);
int CApplyValueEx_u4_m2_applyKeyBasedValueEx_a6(const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info);
int CVirtualEx_u4_m2_applyKeyBasedValue_a7(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info);
int CVirtualEx_u4_m3_applyValue_a3(const void* self, const std::uint8_t* value, void* target);
int CVirtualEx_u4_m3_applyKeyBasedValue_a5(const void* self, const SAnimationAccessor* accessor, int key, void* target, const SApplicatorInfo* info);
int CApplyValueEx_u4_m3_applyKeyBasedValueEx_a6(const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info);
int CVirtualEx_u4_m3_applyKeyBasedValue_a7(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info);
int CVirtualEx_u4_mn1_applyValue_a3(const void* self, const std::uint8_t* value, void* target);
int CApplyValueEx_u4_mn1_applyKeyBasedValueEx_a4(const SAnimationAccessor* accessor, int key, void* target, const SApplicatorInfo* info);
int CVirtualEx_u4_mn1_applyKeyBasedValue_a5(const void* self, const SAnimationAccessor* accessor, int key, void* target, const SApplicatorInfo* info);
int CVirtualEx_v3_ScaleZ_char_applyValue_a3(const void* self, const void* value, void* target);
int CVirtualEx_v3_ScaleZ_short_applyValue_a3(const void* self, const void* value, void* target);
int CVirtualEx_v3_ScaleY_char_applyValue_a3(const void* self, const void* value, void* target);
int CVirtualEx_v3_ScaleY_short_applyValue_a3(const void* self, const void* value, void* target);
int CVirtualEx_v3_ScaleX_char_applyValue_a3(const void* self, const void* value, void* target);
int CVirtualEx_v3_ScaleX_short_applyValue_a3(const void* self, const void* value, void* target);
int CVirtualEx_v3_PositionZ_char_applyValue_a3(const void* self, const void* value, void* target);
int CVirtualEx_v3_PositionZ_short_applyValue_a3(const void* self, const void* value, void* target);
int CVirtualEx_v3_PositionY_char_applyValue_a3(const void* self, const void* value, void* target);
int CApplyValueEx_v3_PositionMixin_float_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_PositionMixin_float_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_PositionMixin_short_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_PositionMixin_short_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_PositionMixin_char_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_PositionMixin_char_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_PositionX_float_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_PositionX_float_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_PositionX_float_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CVirtualEx_v3_PositionX_float_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CApplyValueEx_v3_PositionX_short_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_PositionX_short_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_PositionX_short_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CVirtualEx_v3_PositionX_short_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CApplyValueEx_v3_PositionX_char_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_PositionX_char_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_PositionX_char_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CVirtualEx_v3_PositionX_char_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CApplyValueEx_v3_PositionY_float_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_PositionY_float_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_PositionY_float_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CVirtualEx_v3_PositionY_float_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CApplyValueEx_v3_PositionY_short_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_PositionY_short_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_PositionY_short_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CVirtualEx_v3_PositionY_short_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CApplyValueEx_v3_ScaleZ_short_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_ScaleZ_short_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_ScaleZ_short_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CVirtualEx_v3_ScaleZ_short_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CApplyValueEx_v3_ScaleZ_char_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_ScaleZ_char_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_ScaleZ_char_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CVirtualEx_v3_ScaleZ_char_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CApplyValueEx_v3_ScaleMixin_char_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_ScaleMixin_char_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_ScaleX_float_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_ScaleX_float_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_ScaleX_float_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CVirtualEx_v3_ScaleX_float_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CApplyValueEx_v3_ScaleX_short_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_ScaleX_short_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_ScaleX_short_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CVirtualEx_v3_ScaleX_short_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CApplyValueEx_v3_ScaleX_char_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_ScaleX_char_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_ScaleX_char_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CVirtualEx_v3_ScaleX_char_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CApplyValueEx_v3_ScaleY_float_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_ScaleY_float_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_ScaleY_float_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CVirtualEx_v3_ScaleY_float_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CApplyValueEx_v3_ScaleY_short_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_ScaleY_short_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_ScaleY_short_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CVirtualEx_v3_ScaleY_short_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CApplyValueEx_v3_ScaleY_char_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);
int CVirtualEx_v3_ScaleY_char_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target);
int CApplyValueEx_v3_ScaleY_char_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CVirtualEx_v3_ScaleY_char_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target);
int CApplyValueEx_v3_ScaleZ_float_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target);

}  // namespace glitch::collada::recon_14
