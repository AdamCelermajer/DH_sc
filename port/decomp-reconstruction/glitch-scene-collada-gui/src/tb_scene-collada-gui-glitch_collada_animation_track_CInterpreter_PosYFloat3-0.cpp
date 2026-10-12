// Bodies for the CInterpreter<CSceneNodePositionYEx<float>, float, 3, SUseDefaultValues<1, float> >
// instantiation. Batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreter_glit-0
// (the batch id is shared with other batches in e1.json, so this file is named by the class, as the
// PosZ sibling is). Declarations are in glitch-scene-collada-gui/include/glitch-scene-collada-gui/tb/
// glitch_collada_animation_track_CInterpreter_PosYFloat3.hpp.
//
// All four bodies are partial. They read the key table through SAnimationAccessor::getOutput and the
// default value through hasDefaultValue/getDefaultValue. Those accessor members are declared as externals
// in the PosZ header and are not defined in the tree yet. The key table pointer at getOutput()+0x04 is a
// guest 32-bit address in the original. These bodies assume host pointers from those externals, and that
// mapping is not verified.
//
// The out buffer is three 32-bit words (the original stores at +0x00, +0x04, +0x08). Each word is written
// as its bit pattern, so float and word stores both go through memcpy.

#include <cstdint>
#include <cstring>

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_PosYFloat3.hpp"

namespace glitch::collada::animation_track {

namespace {

// Bit pattern of the 32-bit word at p (the original DWORD load).
inline std::uint32_t wordAt(const float* p) {
    std::uint32_t w;
    std::memcpy(&w, p, sizeof w);
    return w;
}

// Stores one float as its bit pattern (the original float store).
inline void storeFloat(std::uint32_t* dst, int index, float value) {
    std::memcpy(&dst[index], &value, sizeof value);
}

// The original returns the default pointer in r0 from a function declared to return float. The
// pointer bits are reproduced from the 32-bit guest address, which is valid when guest memory sits
// below 4 GiB on the host (the convention used by the recon_v2 helpers).
inline float guestAddressBitsAsFloat(const float* p) {
    const std::uint32_t addr = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
    float f;
    std::memcpy(&f, &addr, sizeof f);
    return f;
}

}  // namespace

// 0x61fc70 glitch::collada::animation_track::CInterpreter<glitch::collada::animation_track::CSceneNodePositionYEx<float>,float,3,glitch::collada::animation_track::SUseDefaultValues<1,float>>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, void*)
// partial: the key table and default value come from accessor externals that are not defined in the tree
// (see the file header). The Hex-Rays control flow is kept, including the three getDefaultValue calls
// of the original. The return value is the default pointer, or nullptr when the accessor has none.
const float* CInterpreter<CSceneNodePositionYEx<float>, float, 3, SUseDefaultValues<1, float> >::getKeyBasedValueEx(
    const SAnimationAccessor& a1, int a2, void* a3) {
    const float* values = accessorGetOutput(a1, 0)->values;  // +0x04 of the output entry
    auto* out = static_cast<std::uint32_t*>(a3);
    const float* result = nullptr;
    if (accessorHasDefaultValue(a1)) {
        result = accessorGetDefaultValue(a1);
    }
    if (result != nullptr) {
        result = accessorGetDefaultValue(a1);
        out[0] = wordAt(result + 0);       // default word 0
        out[1] = wordAt(&values[a2]);      // key a2
        out[2] = wordAt(result + 2);       // default word 2
        return result;
    }
    out[0] = wordAt(&values[a2]);          // key a2
    return nullptr;
}

// 0x61fcf0 glitch::collada::animation_track::CInterpreter<glitch::collada::animation_track::CSceneNodePositionYEx<float>,float,3,glitch::collada::animation_track::SUseDefaultValues<1,float>>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, int, float, void*)
// partial: same accessor externals as 0x61fc70. The return value in the default branch is the default
// pointer's bit pattern (Hex-Rays COERCE_FLOAT of the last getDefaultValue call, r0 in the assembly); the
// caller's use of that value is not confirmed, so the row stays partial.
float CInterpreter<CSceneNodePositionYEx<float>, float, 3, SUseDefaultValues<1, float> >::getKeyBasedValueEx(
    const SAnimationAccessor& a1, int a2, int a3, float a4, void* a5) {
    const float* values = accessorGetOutput(a1, 0)->values;  // +0x04 of the output entry
    auto* out = static_cast<std::uint32_t*>(a5);
    const float blended = values[a2] + (a4 * (values[a3] - values[a2]));
    if (accessorHasDefaultValue(a1)) {
        out[0] = wordAt(accessorGetDefaultValue(a1));  // default word 0
        storeFloat(out, 1, blended);
        const float* defaultValue = accessorGetDefaultValue(a1);
        out[2] = wordAt(defaultValue + 2);             // default word 2
        return guestAddressBitsAsFloat(defaultValue);
    }
    storeFloat(out, 0, blended);
    return blended;
}

// 0x61fdc4 glitch::collada::animation_track::CInterpreter<glitch::collada::animation_track::CSceneNodePositionYEx<float>,float,3,glitch::collada::animation_track::SUseDefaultValues<1,float>>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, int, void*)
// partial: same accessor externals as 0x61fc70. The original dereferences the default pointer without a
// null check, and this body does the same.
const float* CInterpreter<CSceneNodePositionYEx<float>, float, 3, SUseDefaultValues<1, float> >::getKeyBasedValueEx(
    const SAnimationAccessor& a1, int a2, int a3, void* a4) {
    const float* values = accessorGetOutput(a1, 0)->values;  // +0x04 of the output entry
    const float diff = values[a3] - values[a2];
    auto* out = static_cast<std::uint32_t*>(a4);
    if (accessorHasDefaultValue(a1)) {
        const float* result = accessorGetDefaultValue(a1);
        out[0] = wordAt(result + 0);  // default word 0
        storeFloat(out, 1, diff);
        out[2] = wordAt(result + 2);  // default word 2
        return result;
    }
    storeFloat(out, 0, diff);
    return nullptr;
}

// 0x61fe44 glitch::collada::animation_track::CInterpreter<glitch::collada::animation_track::CSceneNodePositionYEx<float>,float,3,glitch::collada::animation_track::SUseDefaultValues<1,float>>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, int, int, float, void*)
// partial: same accessor externals as 0x61fc70. The blend is v12 + t * (v13 - v12), with the key at a2 as
// the base (Hex-Rays v11), as in the original.
float CInterpreter<CSceneNodePositionYEx<float>, float, 3, SUseDefaultValues<1, float> >::getKeyBasedValueEx(
    const SAnimationAccessor& a1, int a2, int a3, int a4, float a5, void* a6) {
    const float* values = accessorGetOutput(a1, 0)->values;  // +0x04 of the output entry
    const float base = values[a2];
    const float v12 = values[a3] - base;
    const float v13 = values[a4] - base;
    const float result = v12 + (a5 * (v13 - v12));
    auto* out = static_cast<std::uint32_t*>(a6);
    if (accessorHasDefaultValue(a1)) {
        const float* defaultValue = accessorGetDefaultValue(a1);
        out[0] = wordAt(defaultValue + 0);  // default word 0
        storeFloat(out, 1, result);
        out[2] = wordAt(defaultValue + 2);  // default word 2
        return result;
    }
    storeFloat(out, 0, result);
    return result;
}

}  // namespace glitch::collada::animation_track
