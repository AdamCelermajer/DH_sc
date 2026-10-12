// Bodies for CInterpreter<CSceneNodeScaleXEx<float>, float, 3, SUseDefaultValues<0, float> >.
// Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreter_glit-0 (owner
// class selected by the batch file; the batch id is shared by 165 batches, so this file is named by the
// class instead). Declarations are in glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_ScaleXFloat.hpp.
//
// Layout of the accessor is taken from the pseudocode only: the key data is the word at +0x04 of
// SAnimationAccessor::getOutput(0), and each key is a 4-byte float. The default-value object has two
// more words read at +0x04 and +0x08. Those words are copied as raw 32-bit values, the way the ARM
// code moves them.

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_ScaleXFloat.hpp"

namespace glitch::collada::animation_track {

namespace {

// Byte offset of word index i (32-bit words, 4 bytes each).
constexpr std::ptrdiff_t wordOffset(int index) {
    return 4 * static_cast<std::ptrdiff_t>(index);
}

// Raw 32-bit word at byte offset off of base.
std::uint32_t loadWord(const void* base, std::ptrdiff_t off) {
    std::uint32_t value;
    std::memcpy(&value, static_cast<const std::uint8_t*>(base) + off, sizeof value);
    return value;
}

void storeWord(void* base, std::ptrdiff_t off, std::uint32_t value) {
    std::memcpy(static_cast<std::uint8_t*>(base) + off, &value, sizeof value);
}

float loadFloat(const void* base, std::ptrdiff_t off) {
    float value;
    std::memcpy(&value, static_cast<const std::uint8_t*>(base) + off, sizeof value);
    return value;
}

void storeFloat(void* base, std::ptrdiff_t off, float value) {
    std::memcpy(static_cast<std::uint8_t*>(base) + off, &value, sizeof value);
}

// Low 32 bits of a pointer: the word the 32-bit ARM code leaves in r0 when it returns a pointer.
std::uint32_t pointerWord(const void* p) {
    return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
}

// The float with the same bit pattern as the word (the ARM return register holds it as a float).
float floatFromWord(std::uint32_t bits) {
    float value;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

}  // namespace

// 0x61dc4c glitch::collada::animation_track::CInterpreter<glitch::collada::animation_track::CSceneNodeScaleXEx<float>,float,3,glitch::collada::animation_track::SUseDefaultValues<0,float>>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, void*)
// Hex-Rays: data = getOutput(0)->data; if (hasDefaultValue() && getDefaultValue() != 0) { out[0] = data[key];
// out[1] = default[+4]; out[2] = default[+8]; return getDefaultValue(); } else { out[0] = data[key]; return 0; }.
// Partial: the original returns the default-value pointer (r0 left by the second getDefaultValue call).
// That pointer is a 32-bit guest address, so this returns its low 32 bits, which is not the same
// value on a 64-bit host. The callers' use of that return value is not identified. The writes match.
int CInterpreter<CSceneNodeScaleXEx<float>, float, 3, SUseDefaultValues<0, float>>::getKeyBasedValueEx(
    const SAnimationAccessor& accessor, int key, void* out) {
    const void* data = tb_ext::accessorKeyData(accessor);
    if (tb_ext::accessorHasDefaultValue(accessor) && tb_ext::accessorDefaultValue(accessor) != nullptr) {
        const void* def = tb_ext::accessorDefaultValue(accessor);
        storeWord(out, wordOffset(0), loadWord(data, wordOffset(key)));
        storeWord(out, wordOffset(1), loadWord(def, wordOffset(1)));
        storeWord(out, wordOffset(2), loadWord(def, wordOffset(2)));
        return static_cast<int>(pointerWord(def));
    }
    storeWord(out, wordOffset(0), loadWord(data, wordOffset(key)));
    return 0;
}

// 0x61dccc glitch::collada::animation_track::CInterpreter<glitch::collada::animation_track::CSceneNodeScaleXEx<float>,float,3,glitch::collada::animation_track::SUseDefaultValues<0,float>>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, int, float, void*)
// Hex-Rays: value = data[from] + t * (data[to] - data[from]); if (hasDefaultValue()) { out[0] = value;
// out[1] = default[+4]; out[2] = default[+8]; return the second getDefaultValue() pointer, reinterpreted as
// float; } else { out[0] = value; return value; }. The disassembly (0x61dccc) confirms the default branch: the
// return register holds the second getDefaultValue() result when the function returns.
// Partial: in the default branch the original returns the pointer bits as a float, not the blend. This
// returns the same bits (low 32 bits of the pointer, which differs from the guest value on a 64-bit host).
// The callers' use of that return value is not identified.
float CInterpreter<CSceneNodeScaleXEx<float>, float, 3, SUseDefaultValues<0, float>>::getKeyBasedValueEx(
    const SAnimationAccessor& accessor, int from, int to, float t, void* out) {
    const void* data = tb_ext::accessorKeyData(accessor);
    const float begin = loadFloat(data, wordOffset(from));
    const float value = begin + t * (loadFloat(data, wordOffset(to)) - begin);
    if (tb_ext::accessorHasDefaultValue(accessor)) {
        storeFloat(out, wordOffset(0), value);
        storeWord(out, wordOffset(1), loadWord(tb_ext::accessorDefaultValue(accessor), wordOffset(1)));
        const void* def = tb_ext::accessorDefaultValue(accessor);
        storeWord(out, wordOffset(2), loadWord(def, wordOffset(2)));
        return floatFromWord(pointerWord(def));
    }
    storeFloat(out, wordOffset(0), value);
    return value;
}

// 0x61dda0 glitch::collada::animation_track::CInterpreter<glitch::collada::animation_track::CSceneNodeScaleXEx<float>,float,3,glitch::collada::animation_track::SUseDefaultValues<0,float>>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, int, void*)
// Hex-Rays: delta = data[to] - data[from]; if (hasDefaultValue()) { def = getDefaultValue(); out[0] = delta;
// out[1] = def[+4]; out[2] = def[+8]; return def; } else { out[0] = delta; return 0; }.
// Partial: the original returns the default-value pointer, as in 0x61dc4c, and it dereferences that pointer
// without a null check. This returns the low 32 bits of the pointer. The callers' use of the return is not
// identified.
int CInterpreter<CSceneNodeScaleXEx<float>, float, 3, SUseDefaultValues<0, float>>::getKeyBasedValueEx(
    const SAnimationAccessor& accessor, int from, int to, void* out) {
    const void* data = tb_ext::accessorKeyData(accessor);
    const float delta = loadFloat(data, wordOffset(to)) - loadFloat(data, wordOffset(from));
    if (tb_ext::accessorHasDefaultValue(accessor)) {
        const void* def = tb_ext::accessorDefaultValue(accessor);
        storeFloat(out, wordOffset(0), delta);
        storeWord(out, wordOffset(1), loadWord(def, wordOffset(1)));
        storeWord(out, wordOffset(2), loadWord(def, wordOffset(2)));
        return static_cast<int>(pointerWord(def));
    }
    storeFloat(out, wordOffset(0), delta);
    return 0;
}

// 0x61de20 glitch::collada::animation_track::CInterpreter<glitch::collada::animation_track::CSceneNodeScaleXEx<float>,float,3,glitch::collada::animation_track::SUseDefaultValues<0,float>>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, int, int, float, void*)
// Hex-Rays: origin = data[base]; begin = data[from] - origin; end = data[to] - origin; value = begin + t * (end - begin);
// if (hasDefaultValue()) { out[0] = value; out[1] = def[+4]; out[2] = def[+8]; } else { out[0] = value; }
// return value. Reconstructed: the control flow and the writes match Hex-Rays, the disassembly agrees on the
// blend order, and the default branch calls getDefaultValue once, as in the pseudocode.
float CInterpreter<CSceneNodeScaleXEx<float>, float, 3, SUseDefaultValues<0, float>>::getKeyBasedValueEx(
    const SAnimationAccessor& accessor, int base, int from, int to, float t, void* out) {
    const void* data = tb_ext::accessorKeyData(accessor);
    const float origin = loadFloat(data, wordOffset(base));
    const float begin = loadFloat(data, wordOffset(from)) - origin;
    const float end = loadFloat(data, wordOffset(to)) - origin;
    const float value = begin + t * (end - begin);
    storeFloat(out, wordOffset(0), value);
    if (tb_ext::accessorHasDefaultValue(accessor)) {
        const void* def = tb_ext::accessorDefaultValue(accessor);
        storeWord(out, wordOffset(1), loadWord(def, wordOffset(1)));
        storeWord(out, wordOffset(2), loadWord(def, wordOffset(2)));
    }
    return value;
}

}  // namespace glitch::collada::animation_track
