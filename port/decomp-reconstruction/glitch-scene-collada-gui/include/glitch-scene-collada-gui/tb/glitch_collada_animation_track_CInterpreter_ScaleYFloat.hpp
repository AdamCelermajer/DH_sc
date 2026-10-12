// glitch::collada::animation_track::CInterpreter<CSceneNodeScaleYEx<float>, float, 3,
//     SUseDefaultValues<1, float>>, declared for Track B batch
// e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreter_glit-0 (the ScaleY float entry).
//
// Only this one instantiation is declared. The primary template is not defined anywhere in the tree.
// The accessor members the bodies call (getOutput, hasDefaultValue, getDefaultValue) have no declaration
// in the tree yet, so they are declared below as unattached helpers. The bodies compile against them
// and link once the accessor is mapped.
//
// The functions are static: the original takes no `this` (the accessor is the first argument and the
// pseudocode never reads a member of the interpreter).
#pragma once

#include <cstdint>

namespace glitch::collada {

struct SAnimationAccessor;

// Unattached. Host view of the key words of accessor output 0: the guest word at getOutput(acc, 0) + 4,
// mapped to host memory. The guest-to-host mapping is not in the tree.
const std::uint32_t* animationAccessorKeyWords(const SAnimationAccessor& acc);

// Unattached. SAnimationAccessor::hasDefaultValue().
bool animationAccessorHasDefaultValue(const SAnimationAccessor& acc);

// Unattached. SAnimationAccessor::getDefaultValue(): three words, or null when there is no default.
const std::uint32_t* animationAccessorDefaultValue(const SAnimationAccessor& acc);

}  // namespace glitch::collada

namespace glitch::collada::animation_track {

template <typename T>
struct CSceneNodeScaleYEx;

template <int Component, typename Key>
struct SUseDefaultValues;

// Primary template: not defined in this tree. Only the instantiation below is declared.
template <typename Mixin, typename T, int Dim, typename Defaults>
class CInterpreter;

template <>
class CInterpreter<CSceneNodeScaleYEx<float>, float, 3, SUseDefaultValues<1, float>> {
public:
    // 0x61df18 getKeyBasedValueEx(acc, key, out): one key. Returns the default-value pointer, or null.
    static const std::uint32_t* getKeyBasedValueEx(const SAnimationAccessor& acc, int key, void* out);

    // 0x61df98 getKeyBasedValueEx(acc, from, to, t, out): linear blend between two keys.
    static float getKeyBasedValueEx(const SAnimationAccessor& acc, int from, int to, float t, void* out);

    // 0x61e06c getKeyBasedValueEx(acc, from, to, out): the difference between two keys.
    static const std::uint32_t* getKeyBasedValueEx(const SAnimationAccessor& acc, int from, int to, void* out);

    // 0x61e0ec getKeyBasedValueEx(acc, base, from, to, t, out): blend of two key deltas from the base key.
    static float getKeyBasedValueEx(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);
};

}  // namespace glitch::collada::animation_track
