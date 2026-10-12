// Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreter_glit-0:
// CInterpreter<CSceneNodeScaleYEx<float>, float, 3, SUseDefaultValues<1, float>>::getKeyBasedValueEx, four overloads.
// Declarations are in glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_ScaleYFloat.hpp.
//
// Status of every body in this file: partial. The control flow and the word stores follow the Hex-Rays
// pseudocode. The key words and the default value come from SAnimationAccessor, whose members are not
// mapped (animationAccessorKeyWords, animationAccessorHasDefaultValue, animationAccessorDefaultValue are
// declared only), so nothing here links yet.
//
// Output words: the original writes 32-bit words at out + 4 * i. They are written with memcpy at word
// index i, so no guest address is used for the output.

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_ScaleYFloat.hpp"

namespace glitch::collada::animation_track {

namespace {

float bitsToFloat(std::uint32_t bits) {
    float value = 0.0f;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

std::uint32_t floatToBits(float value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof bits);
    return bits;
}

// Stores one 32-bit word at word index `index` of the output buffer.
void storeWord(void* out, std::size_t index, std::uint32_t bits) {
    std::memcpy(static_cast<unsigned char*>(out) + index * sizeof bits, &bits, sizeof bits);
}

void storeFloat(void* out, std::size_t index, float value) {
    storeWord(out, index, floatToBits(value));
}

}  // namespace

// 0x61df18 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIfEEfLi3ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
// partial: body follows the pseudocode. Missing: animationAccessorKeyWords, animationAccessorHasDefaultValue
// and animationAccessorDefaultValue (SAnimationAccessor members, not mapped). Without a default value the
// result is null, as in the original.
const std::uint32_t* CInterpreter<CSceneNodeScaleYEx<float>, float, 3, SUseDefaultValues<1, float>>::getKeyBasedValueEx(
    const SAnimationAccessor& acc, int key, void* out) {
    const std::uint32_t* keys = animationAccessorKeyWords(acc);
    const std::uint32_t* defaults = nullptr;
    if (animationAccessorHasDefaultValue(acc)) {
        defaults = animationAccessorDefaultValue(acc);
    }
    if (defaults != nullptr) {
        // The original calls getDefaultValue again for the returned pointer; the value is the same.
        const std::uint32_t* result = animationAccessorDefaultValue(acc);
        storeWord(out, 0, result[0]);
        storeWord(out, 1, keys[key]);
        storeWord(out, 2, result[2]);
        return result;
    }
    storeWord(out, 0, keys[key]);
    return nullptr;
}

// 0x61df98 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIfEEfLi3ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
// partial: missing the three accessor helpers (see 0x61df18). The default-value path also returns the bit
// pattern of the getDefaultValue pointer (Hex-Rays COERCE_FLOAT on the result). That pointer is guest data
// and has no float value on a 64-bit host, so this body returns 0.0f there. Callers are not known yet.
float CInterpreter<CSceneNodeScaleYEx<float>, float, 3, SUseDefaultValues<1, float>>::getKeyBasedValueEx(
    const SAnimationAccessor& acc, int from, int to, float t, void* out) {
    const std::uint32_t* keys = animationAccessorKeyWords(acc);
    const float begin = bitsToFloat(keys[from]);
    const float value = begin + t * (bitsToFloat(keys[to]) - begin);
    if (animationAccessorHasDefaultValue(acc)) {
        const std::uint32_t* defaults = animationAccessorDefaultValue(acc);
        storeWord(out, 0, defaults[0]);
        storeFloat(out, 1, value);
        storeWord(out, 2, defaults[2]);
        return 0.0f;  // partial: original returns the default-value pointer bits (see above)
    }
    storeFloat(out, 0, value);
    return value;
}

// 0x61e06c _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIfEEfLi3ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
// partial: missing the three accessor helpers (see 0x61df18). Returns the default-value pointer, or null.
const std::uint32_t* CInterpreter<CSceneNodeScaleYEx<float>, float, 3, SUseDefaultValues<1, float>>::getKeyBasedValueEx(
    const SAnimationAccessor& acc, int from, int to, void* out) {
    const std::uint32_t* keys = animationAccessorKeyWords(acc);
    const float delta = bitsToFloat(keys[to]) - bitsToFloat(keys[from]);
    if (animationAccessorHasDefaultValue(acc)) {
        const std::uint32_t* defaults = animationAccessorDefaultValue(acc);
        storeWord(out, 0, defaults[0]);
        storeFloat(out, 1, delta);
        storeWord(out, 2, defaults[2]);
        return defaults;
    }
    storeFloat(out, 0, delta);
    return nullptr;
}

// 0x61e0ec _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIfEEfLi3ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
// partial: missing the three accessor helpers (see 0x61df18). Both paths return the blended value.
float CInterpreter<CSceneNodeScaleYEx<float>, float, 3, SUseDefaultValues<1, float>>::getKeyBasedValueEx(
    const SAnimationAccessor& acc, int base, int from, int to, float t, void* out) {
    const std::uint32_t* keys = animationAccessorKeyWords(acc);
    const float origin = bitsToFloat(keys[base]);
    const float begin = bitsToFloat(keys[from]) - origin;
    const float end = bitsToFloat(keys[to]) - origin;
    const float value = begin + t * (end - begin);
    if (animationAccessorHasDefaultValue(acc)) {
        const std::uint32_t* defaults = animationAccessorDefaultValue(acc);
        storeWord(out, 0, defaults[0]);
        storeFloat(out, 1, value);
        storeWord(out, 2, defaults[2]);
        return value;
    }
    storeFloat(out, 0, value);
    return value;
}

}  // namespace glitch::collada::animation_track
