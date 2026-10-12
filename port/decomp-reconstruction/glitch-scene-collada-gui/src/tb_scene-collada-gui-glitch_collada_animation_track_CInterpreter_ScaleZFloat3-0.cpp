// Bodies for the CInterpreter<CSceneNodeScaleZEx<float>, float, 3, SUseDefaultValues<2, float>>
// instantiation. Batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreter_glit-0
// (class-named file: the batch-id file holds another batch's body).
// Declarations are in glitch-scene-collada-gui/include/glitch-scene-collada-gui/tb/
// glitch_collada_animation_track_CInterpreter_ScaleZFloat3.hpp.
//
// All four bodies are partial. They read the key table through SAnimationAccessor::getOutput and
// the default value through hasDefaultValue/getDefaultValue. Those accessor members are not
// defined in the tree, and the key table pointer at getOutput()+0x04 is a guest 32-bit address in
// the original. The bodies follow the Hex-Rays control flow against the declared accessor
// externals, so they assume host pointers from those externals. That mapping is not verified.
//
// Buffer writes follow the original word stores: the out buffer is written at the original
// offsets +0x00, +0x04 (default words) and +0x08 (key or blended value), each as its 32-bit pattern.

#include <cstring>

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_ScaleZFloat3.hpp"

namespace glitch::collada::animation_track {

namespace {

// Copies one 32-bit word (bit pattern) from src to dst. The original uses a DWORD load and store.
inline void copyWord(void* dst, const float* src) {
    std::memcpy(dst, src, sizeof(std::uint32_t));
}

// Stores one float at dst, as the original float store does.
inline void storeFloat(void* dst, float value) {
    std::memcpy(dst, &value, sizeof(float));
}

}  // namespace

// 0x61e1e4 glitch::collada::animation_track::CInterpreter<glitch::collada::animation_track::CSceneNodeScaleZEx<float>,float,3,glitch::collada::animation_track::SUseDefaultValues<2,float>>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, void*)
// partial: the key table and default value come from unattached accessor externals (see the file
// header). Returns the default pointer, or nullptr when the accessor has no default. The Hex-Rays
// body calls getDefaultValue a second time before copying word 1, which is kept here.
const float* CInterpreter<CSceneNodeScaleZEx<float>, float, 3, SUseDefaultValues<2, float>>::getKeyBasedValueEx(
    const SAnimationAccessor& accessor, int a2, void* out) {
    const float* values = accessorGetOutput(accessor, 0)->values;  // +0x04 of the output entry
    auto* dst = static_cast<unsigned char*>(out);
    const float* result = nullptr;
    if (accessorHasDefaultValue(accessor)) {
        result = accessorGetDefaultValue(accessor);
        if (result != nullptr) {
            result = accessorGetDefaultValue(accessor);
            copyWord(dst + 0x00, result + 0);  // default word 0
            copyWord(dst + 0x04, result + 1);  // default word 1
            copyWord(dst + 0x08, values + a2);  // key a2
            return result;
        }
    }
    copyWord(dst + 0x00, values + a2);  // key a2 as the only word
    return result;
}

// 0x61e264 glitch::collada::animation_track::CInterpreter<glitch::collada::animation_track::CSceneNodeScaleZEx<float>,float,3,glitch::collada::animation_track::SUseDefaultValues<2,float>>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, int, float, void*)
// partial: same accessor externals as 0x61e1e4. The blend is the Hex-Rays expression
// a2 + t * (a3 - a2) on the key values.
float CInterpreter<CSceneNodeScaleZEx<float>, float, 3, SUseDefaultValues<2, float>>::getKeyBasedValueEx(
    const SAnimationAccessor& accessor, int a2, int a3, float t, void* out) {
    const float* values = accessorGetOutput(accessor, 0)->values;
    auto* dst = static_cast<unsigned char*>(out);
    float result;
    if (accessorHasDefaultValue(accessor)) {
        copyWord(dst + 0x00, accessorGetDefaultValue(accessor) + 0);
        copyWord(dst + 0x04, accessorGetDefaultValue(accessor) + 1);
        result = values[a2] + (t * (values[a3] - values[a2]));
        storeFloat(dst + 0x08, result);
    } else {
        result = values[a2] + (t * (values[a3] - values[a2]));
        storeFloat(dst + 0x00, result);
    }
    return result;
}

// 0x61e334 glitch::collada::animation_track::CInterpreter<glitch::collada::animation_track::CSceneNodeScaleZEx<float>,float,3,glitch::collada::animation_track::SUseDefaultValues<2,float>>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, int, void*)
// partial: same accessor externals as 0x61e1e4. Writes the key difference (a3 - a2). As in the
// original there is no null check on getDefaultValue before the word copies.
const float* CInterpreter<CSceneNodeScaleZEx<float>, float, 3, SUseDefaultValues<2, float>>::getKeyBasedValueEx(
    const SAnimationAccessor& accessor, int a2, int a3, void* out) {
    const float* values = accessorGetOutput(accessor, 0)->values;
    const float diff = values[a3] - values[a2];
    const float* result = nullptr;
    auto* dst = static_cast<unsigned char*>(out);
    if (accessorHasDefaultValue(accessor)) {
        result = accessorGetDefaultValue(accessor);
        copyWord(dst + 0x00, result + 0);  // default word 0
        copyWord(dst + 0x04, result + 1);  // default word 1
        storeFloat(dst + 0x08, diff);
    } else {
        storeFloat(dst + 0x00, diff);
    }
    return result;
}

// 0x61e3b4 glitch::collada::animation_track::CInterpreter<glitch::collada::animation_track::CSceneNodeScaleZEx<float>,float,3,glitch::collada::animation_track::SUseDefaultValues<2,float>>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, int, int, float, void*)
// partial: same accessor externals as 0x61e1e4. The blend is the Hex-Rays expression
// base-relative a3 + t * (a4 - a3), with the key at a2 as the base.
float CInterpreter<CSceneNodeScaleZEx<float>, float, 3, SUseDefaultValues<2, float>>::getKeyBasedValueEx(
    const SAnimationAccessor& accessor, int a2, int a3, int a4, float t, void* out) {
    const float* values = accessorGetOutput(accessor, 0)->values;
    const float base = values[a2];
    const float v12 = values[a3] - base;
    const float v13 = values[a4] - base;
    auto* dst = static_cast<unsigned char*>(out);
    float result;
    if (accessorHasDefaultValue(accessor)) {
        const float* defaultValue = accessorGetDefaultValue(accessor);
        copyWord(dst + 0x00, defaultValue + 0);
        copyWord(dst + 0x04, defaultValue + 1);
        result = v12 + (t * (v13 - v12));
        storeFloat(dst + 0x08, result);
    } else {
        result = v12 + (t * (v13 - v12));
        storeFloat(dst + 0x00, result);
    }
    return result;
}

}  // namespace glitch::collada::animation_track
