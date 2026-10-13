// Track B batch e2-glitch-scene-collada-gui-pack2-52827d-0.
// glitch::collada::animation_track CVirtualEx members for the vec3 position and quaternion applicators, and the
// CInterpreter key-reading members for the quaternion, quaternion-angle and position-X interpreters.
// The IColladaSkinTechnique::initProxyBuffer row (0x670ec8) is already defined, as a partial stub, in
// src/tb_scene-collada-gui-glitch_collada_detail_IColladaSkinTechnique-0.cpp, and is not defined twice here.
//
// The key tables and default values are read through the canonical SAnimationAccessor (collada/SAnimationAccessor.hpp): the output
// record's data pointer is the key table and getDefaultValue is the default value. The control flow of each body follows the
// Hex-Rays pseudocode. Functions that call a virtual slot of the applicator info (not mapped) are partial.

#include <cstdint>
#include <cstring>

#include "glitch-scene-collada-gui/layouts/animation_track.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_PositionMixinVec3f.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_QuatMixinFloat.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CBlender_quaternion.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreterQuaternionAngle_QAngleFloat.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_CMixinPosFloat3.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_PosXExFloat3.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_QuatFloat4.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CVirtualEx_PositionMixinVec3f.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CVirtualEx_QuatAngleFloat.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CVirtualEx_QuatMixinFloat.hpp"

namespace glitch::collada::animation_track {

namespace {

// Reinterprets a 32-bit word as the float it encodes.
float wordToFloat(std::uint32_t bits) {
    float value;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

// Float at index of the key table.
float keyFloat(const float* keyBase, int index) {
    return keyBase[index];
}

// Key table of accessor output 0. The pseudocode reads the word at (getOutput(a1, 0) + 4), the data pointer.
const float* keyBaseOf(const glitch::collada::SAnimationAccessor& accessor) {
    return static_cast<const float*>(accessor.getOutput(0)->data);
}

// The accessor's default value as words.
const std::uint32_t* defaultValueWords(const glitch::collada::SAnimationAccessor& accessor) {
    return static_cast<const std::uint32_t*>(accessor.getDefaultValue());
}

// Float at index of a default value, or zero when the host view is unmapped.
float defaultFloat(const std::uint32_t* defaults, int index) {
    return defaults != nullptr ? wordToFloat(defaults[index]) : 0.0f;
}

}  // namespace

// The vec3 position applicator and the quaternion applicators, as named in the pseudocode.
using PositionVec3Applicator = CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodePositionMixin<float>>;
using QuatMixinApplicator = CApplyValueEx<glitch::core::quaternion, CSceneNodeQuaternionMixin<float>>;
using QuatAngleApplicator = CApplyValueEx<glitch::core::quaternion, CSceneNodeQuaternionAngleMixin<float>>;

// 0x60eff8 glitch::collada::animation_track::CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodePositionMixin<float>>>::getValueSize() const
// Constant: one position vector of three floats.
int CVirtualEx<PositionVec3Applicator>::getValueSize() const noexcept {
    return 12;
}

// 0x623dd8 glitch::collada::animation_track::CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodePositionMixin<float>>>::applyValue(void*, void*, CApplicatorInfo*) const
// partial: the original tail-calls virtual slot +0xa4 (byte 164) of the applicator info, passing the info object. The
// vtable of CApplicatorInfo is not mapped, so the call is not written and the function returns 0. The value argument
// is not read by the pseudocode.
int CVirtualEx<PositionVec3Applicator>::applyValue(void* value, void* target, CApplicatorInfo* info) const {
    (void)value;
    (void)target;
    (void)info;
    return 0;
}

// 0x626b4c glitch::collada::animation_track::CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodePositionMixin<float>>>::getBlendedValue(void*, float*, int, void*) const
// A count of 1 copies the first vector. Otherwise each component of out is the sum over the keys of weights[i] times the
// component of key i, accumulated from 0.0f in the original order. A count of 0 writes zeros. The pseudocode loops on a
// decrementing counter; a non-positive count is not reached by the game, so it is treated as zero keys here.
void CVirtualEx<PositionVec3Applicator>::getBlendedValue(void* valuesArg, float* weights, int count, void* outArg) const {
    const auto* values = static_cast<const float*>(valuesArg);
    auto* out = static_cast<float*>(outArg);
    if (count == 1) {
        out[0] = values[0];
        out[1] = values[1];
        out[2] = values[2];
        return;
    }
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    for (int i = 0; i < count; ++i) {
        const float w = weights[i];
        x = x + w * values[3 * i];
        y = y + w * values[3 * i + 1];
        z = z + w * values[3 * i + 2];
    }
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

// 0x62859c glitch::collada::animation_track::CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodePositionMixin<float>>>::applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*) const
// Forwards values, weights, count, target and info to CApplyValueEx::applyBlendedValueEx and returns its result. The
// assembly moves the stack word at sp+4 (info) into sp+0 and tail-calls with it as the fifth argument.
int CVirtualEx<PositionVec3Applicator>::applyBlendedValue(void* values, float* weights, int count, void* target,
                                                          CApplicatorInfo* info) const {
    return PositionVec3Applicator::applyBlendedValueEx(values, weights, count, target, info);
}

// 0x628834 glitch::collada::animation_track::CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodePositionMixin<float>>>::getKeyBasedValue(SAnimationAccessor const&, int, int, float, void*) const
// Forwards the accessor, the two key indices, t and out to CInterpreter<CMixin<float, 3, SSceneNodePosition, -1, float>,
// float, 3, SUseDefaultLerp<float>>::getKeyBasedValueEx. The callee is defined in its own row (0x6286cc).
float CVirtualEx<PositionVec3Applicator>::getKeyBasedValue(const glitch::collada::SAnimationAccessor& accessor,
                                                            int from, int to, float t, float* out) const {
    return CInterpreter<CMixin<float, 3, SSceneNodePosition, -1, float>, float, 3,
                        SUseDefaultLerp<float>>::getKeyBasedValueEx(accessor, from, to, t, out);
}

// 0x60f0e8 glitch::collada::animation_track::CVirtualEx<CApplyValueEx<quaternion, CSceneNodeQuaternionMixin<float>>>::getValueSize() const
// Constant: one quaternion of four floats.
int CVirtualEx<QuatMixinApplicator>::getValueSize() const noexcept {
    return 16;
}

// 0x60ff20 glitch::collada::animation_track::CVirtualEx<CApplyValueEx<quaternion, CSceneNodeQuaternionMixin<float>>>::getInstance()
// partial: the original stores (int)off_978AE8 into the function-local static s_Instance under the C++ guard, then
// registers the empty destructor with atexit. off_978AE8 is not identified, and a guest address cannot be carried into a
// host pointer, so s_Instance stays 0. The atexit registration is omitted because the destructor is empty.
int* CVirtualEx<QuatMixinApplicator>::getInstance() {
    static std::int32_t s_Instance = 0;
    return &s_Instance;
}

// 0x613300 glitch::collada::animation_track::CVirtualEx<CApplyValueEx<quaternion, CSceneNodeQuaternionMixin<float>>>::getBlendedValue(void*, float*, int, void*) const
// Forwards to CBlender<quaternion, 1, quaternion>::getBlendedValueEx. The callee's result is the Hex-Rays _BOOL4 word; its
// low byte is returned as the bool, as ARM returns a bool in a register.
bool CVirtualEx<QuatMixinApplicator>::getBlendedValue(void* values, float* weights, int count, void* out) const {
    const std::uint32_t result =
        CBlender<glitch::core::quaternion, 1, glitch::core::quaternion>::getBlendedValueEx(values, weights, count, out);
    return (result & 0xffu) != 0u;
}

// 0x62084c glitch::collada::animation_track::CVirtualEx<CApplyValueEx<quaternion, CSceneNodeQuaternionMixin<float>>>::applyValue(void*, void*, CApplicatorInfo*) const
// partial: the original tail-calls virtual slot +0x9c (byte 156) of the applicator info. That vtable is not mapped, so the
// call is not written and the function returns 0. The value argument is not read by the pseudocode.
int CVirtualEx<QuatMixinApplicator>::applyValue(void* value, void* target, CApplicatorInfo* info) const {
    (void)value;
    (void)target;
    (void)info;
    return 0;
}

// 0x6209b4 glitch::collada::animation_track::CVirtualEx<CApplyValueEx<quaternion, CSceneNodeQuaternionMixin<float>>>::applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*) const
// Forwards values, weights, count, target and info to CApplyValueEx::applyBlendedValueEx and returns its result. Same
// shape as 0x62859c: the assembly moves the stack word at sp+4 (info) into sp+0 and tail-calls with it as the fifth
// argument. Checked against the 0x6209b4 disassembly (MOV/LDR/STR/B sequence).
int CVirtualEx<QuatMixinApplicator>::applyBlendedValue(void* values, float* weights, int count, void* target,
                                                       CApplicatorInfo* info) const {
    return QuatMixinApplicator::applyBlendedValueEx(values, weights, count, target, info);
}

// 0x6100dc glitch::collada::animation_track::CVirtualEx<CApplyValueEx<quaternion, CSceneNodeQuaternionAngleMixin<float>>>::getInstance()
// partial: same shape as 0x60ff20. The original stores (int)off_978D30 into the function-local static s_Instance under the
// C++ guard and registers the destructor with atexit. off_978D30 is not identified, so s_Instance stays 0.
int* CVirtualEx<QuatAngleApplicator>::getInstance() {
    static std::int32_t s_Instance = 0;
    return &s_Instance;
}

// 0x61cbf4 glitch::collada::animation_track::CVirtualEx<CApplyValueEx<quaternion, CSceneNodeQuaternionAngleMixin<float>>>::retrieveValue(void*, void*) const
// partial: the original calls virtual slot +0x98 (byte 152) of the source object, which returns a pointer to four words.
// Those words are copied to out and the pointer is returned. The source object type and the method behind the slot are
// not identified, so no copy is made and nullptr is returned.
std::uint32_t* CVirtualEx<QuatAngleApplicator>::retrieveValue(void* source, std::uint32_t* out) const {
    (void)source;
    (void)out;
    return nullptr;
}

// 0x613294 glitch::collada::animation_track::CInterpreter<CSceneNodeQuaternionMixin<float>, float, 4, SUseDefaultLerp<float>>::getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*)
// Blends the quaternion key a2 with key a2 + 1 using the weights (1 - t, t) through CBlender<quaternion, 1, quaternion>.
// The key table is read through the guest seam, so both quaternions read as zero. a3 is not read by the original.
// partial: key data is not available in this tree (see the guest seam above).
bool CInterpreterSceneNodeQuaternionFloat4::getKeyBasedValueEx(const glitch::collada::SAnimationAccessor& a1, int a2,
                                                               int a3, float a4, float* a5) {
    (void)a3;
    const float* keyBase = keyBaseOf(a1);
    float keys[8] = {};
    for (int i = 0; i < 8; ++i) {
        keys[i] = keyFloat(keyBase, 4 * a2 + i);
    }
    float weights[2] = {1.0f - a4, a4};
    const std::uint32_t result =
        CBlender<glitch::core::quaternion, 1, glitch::core::quaternion>::getBlendedValueEx(keys, weights, 2, a5);
    return (result & 0xffu) != 0u;
}

// 0x61f51c glitch::collada::animation_track::CInterpreter<CSceneNodeQuaternionAngleMixin<float>, float, 4, SUseDefaultValues<3, float>>::getKeyBasedValueEx(SAnimationAccessor const&, int, void*)
// Writes the default value's first three words and the key's fourth word when the accessor has a default value, otherwise
// the key word alone to out[0]. Returns the default value pointer, or nullptr.
// partial: key and default words are read through the guest seam and read as zero; no default value has a host pointer.
const std::uint32_t* CInterpreterSceneNodeQuaternionAngleFloat4::getKeyBasedValueEx(
    const glitch::collada::SAnimationAccessor& acc, int key, float* out) {
    const float* keyBase = keyBaseOf(acc);
    const std::uint32_t* defaults = acc.hasDefaultValue() ? defaultValueWords(acc) : nullptr;
    if (defaults != nullptr) {
        out[0] = defaultFloat(defaults, 0);
        out[1] = defaultFloat(defaults, 1);
        out[2] = defaultFloat(defaults, 2);
        out[3] = keyFloat(keyBase, key);
    } else {
        out[0] = keyFloat(keyBase, key);
    }
    return defaults;
}

// 0x61f898 glitch::collada::animation_track::CInterpreter<CSceneNodeQuaternionAngleMixin<float>, float, 4, SUseDefaultValues<3, float>>::getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*)
// Linear blend of key from and key to with t: value = from + t * (to - from). With a default value the first three words
// of out are the default value and out[3] is the blend; otherwise out[0] is the blend. Returns the blend.
// partial: key and default words are read through the guest seam and read as zero.
float CInterpreterSceneNodeQuaternionAngleFloat4::getKeyBasedValueEx(const glitch::collada::SAnimationAccessor& acc,
                                                                     int from, int to, float t, float* out) {
    const float* keyBase = keyBaseOf(acc);
    const float fromValue = keyFloat(keyBase, from);
    const float blended = fromValue + (t * (keyFloat(keyBase, to) - fromValue));
    if (acc.hasDefaultValue()) {
        const std::uint32_t* defaults = defaultValueWords(acc);
        for (int i = 0; i < 3; ++i) {
            out[i] = defaultFloat(defaults, i);
        }
        out[3] = blended;
    } else {
        out[0] = blended;
    }
    return blended;
}

// 0x61f9a4 glitch::collada::animation_track::CInterpreter<CSceneNodePositionXEx<float>, float, 3, SUseDefaultValues<0, float>>::getKeyBasedValueEx(SAnimationAccessor const&, int, void*)
// Writes the key's x value to a3[0]. With a default value, a3[1] and a3[2] are its y and z words. Returns the default
// value pointer, or nullptr when there is none.
// partial: key and default words are read through the guest seam and read as zero.
const std::uint32_t* CInterpreter<CSceneNodePositionXEx<float>, float, 3, SUseDefaultValues<0, float>>::getKeyBasedValueEx(
    const glitch::collada::SAnimationAccessor& a1, int a2, float* a3) {
    const float* keyBase = keyBaseOf(a1);
    const std::uint32_t* defaults = a1.hasDefaultValue() ? defaultValueWords(a1) : nullptr;
    a3[0] = keyFloat(keyBase, a2);
    if (defaults != nullptr) {
        a3[1] = defaultFloat(defaults, 1);
        a3[2] = defaultFloat(defaults, 2);
    }
    return defaults;
}

// 0x61fa24 glitch::collada::animation_track::CInterpreter<CSceneNodePositionXEx<float>, float, 3, SUseDefaultValues<0, float>>::getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*)
// Blends key a2 with key a3 by a4: value = from + a4 * (to - from), written to a5[0]. With a default value, a5[1] and a5[2]
// are its y and z words.
// partial: (1) key and default words are read through the guest seam and read as zero. (2) In the default branch the
// original returns the bit pattern of the default value pointer as a float (Hex-Rays COERCE_FLOAT). A host pointer has no
// such float, so this returns the blended value in both branches.
float CInterpreter<CSceneNodePositionXEx<float>, float, 3, SUseDefaultValues<0, float>>::getKeyBasedValueEx(
    const glitch::collada::SAnimationAccessor& a1, int a2, int a3, float a4, float* a5) {
    const float* keyBase = keyBaseOf(a1);
    const float fromValue = keyFloat(keyBase, a2);
    const float blended = fromValue + (a4 * (keyFloat(keyBase, a3) - fromValue));
    a5[0] = blended;
    if (a1.hasDefaultValue()) {
        const std::uint32_t* defaults = defaultValueWords(a1);
        a5[1] = defaultFloat(defaults, 1);
        a5[2] = defaultFloat(defaults, 2);
    }
    return blended;
}

// 0x61faf8 glitch::collada::animation_track::CInterpreter<CSceneNodePositionXEx<float>, float, 3, SUseDefaultValues<0, float>>::getKeyBasedValueEx(SAnimationAccessor const&, int, int, void*)
// Writes the x difference key a3 - key a2 to a4[0]. With a default value, a4[1] and a4[2] are its y and z words and the
// default value pointer is returned; otherwise nullptr is returned.
// partial: key and default words are read through the guest seam and read as zero.
const std::uint32_t* CInterpreter<CSceneNodePositionXEx<float>, float, 3, SUseDefaultValues<0, float>>::getKeyBasedValueEx(
    const glitch::collada::SAnimationAccessor& a1, int a2, int a3, float* a4) {
    const float* keyBase = keyBaseOf(a1);
    const float diff = keyFloat(keyBase, a3) - keyFloat(keyBase, a2);
    if (a1.hasDefaultValue()) {
        const std::uint32_t* defaults = defaultValueWords(a1);
        a4[0] = diff;
        a4[1] = defaultFloat(defaults, 1);
        a4[2] = defaultFloat(defaults, 2);
        return defaults;
    }
    a4[0] = diff;
    return nullptr;
}

// 0x61fb78 glitch::collada::animation_track::CInterpreter<CSceneNodePositionXEx<float>, float, 3, SUseDefaultValues<0, float>>::getKeyBasedValueEx(SAnimationAccessor const&, int, int, int, float, void*)
// Blends three keys measured from the base key a2: span a3 = key a3 - base, span a4 = key a4 - base, and
// result = span a3 + a5 * (span a4 - span a3). Writes result to a6[0]; with a default value, a6[1] and a6[2] are its y and
// z words. Returns result.
// partial: key and default words are read through the guest seam and read as zero.
float CInterpreter<CSceneNodePositionXEx<float>, float, 3, SUseDefaultValues<0, float>>::getKeyBasedValueEx(
    const glitch::collada::SAnimationAccessor& a1, int a2, int a3, int a4, float a5, float* a6) {
    const float* keyBase = keyBaseOf(a1);
    const float base = keyFloat(keyBase, a2);
    const float spanA = keyFloat(keyBase, a3) - base;
    const float spanB = keyFloat(keyBase, a4) - base;
    const float result = spanA + (a5 * (spanB - spanA));
    a6[0] = result;
    if (a1.hasDefaultValue()) {
        const std::uint32_t* defaults = defaultValueWords(a1);
        a6[1] = defaultFloat(defaults, 1);
        a6[2] = defaultFloat(defaults, 2);
    }
    return result;
}

}  // namespace glitch::collada::animation_track
