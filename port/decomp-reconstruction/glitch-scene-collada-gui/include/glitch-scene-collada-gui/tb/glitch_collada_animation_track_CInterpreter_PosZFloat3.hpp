// glitch::collada::animation_track::CInterpreter<
//     CSceneNodePositionZEx<float>, float, 3, SUseDefaultValues<2, float> >
//
// Declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreter_glit-0
// (the batch id is shared by several batches; the source file is named by the class).
//
// No header in this category declared this instantiation before. The class is stateless: every
// method takes the SAnimationAccessor as its first argument and has no this pointer, so the
// methods are static.
//
// Offsets are the original 32-bit offsets, kept as comments only.
#pragma once

#include <cstdint>

namespace glitch::collada {
struct SAnimationAccessor;  // real layout: glitch-scene-collada-gui/layouts/animation_track.hpp
}  // namespace glitch::collada

namespace glitch::collada::animation_track {

// Output table entry returned by SAnimationAccessor::getOutput. The key value array is at +0x04.
struct SKeyOutput {
    std::uint32_t reserved_00;  // +0x00 not read by these bodies
    const float* values;        // +0x04 key values (guest 32-bit address in the original)
};

// Externals the bodies call. They are SAnimationAccessor members whose definitions are not in
// the tree yet, so they are declared as free functions here and are not defined in this batch.
SKeyOutput* accessorGetOutput(const SAnimationAccessor& accessor, int index);  // SAnimationAccessor::getOutput (0x669e24)
bool accessorHasDefaultValue(const SAnimationAccessor& accessor);             // SAnimationAccessor::hasDefaultValue
const float* accessorGetDefaultValue(const SAnimationAccessor& accessor);     // SAnimationAccessor::getDefaultValue

// Forward declarations of the template arguments, so the instantiation can be named.
template <typename Value>
class CSceneNodePositionZEx;

template <int Index, typename Value>
struct SUseDefaultValues;

// Primary template: declared only, its interpolation body is not in this header.
template <typename Mixin, typename Value, int Dim, typename Defaults>
class CInterpreter;

template <>
class CInterpreter<CSceneNodePositionZEx<float>, float, 3, SUseDefaultValues<2, float>> {
public:
    // 0x615e6c getKeyBasedValueEx(SAnimationAccessor const&, int, int, void*)
    // Returns the default value pointer when the accessor has one (and writes the two default words
    // and the key difference), otherwise nullptr and writes the key difference.
    static const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int a2, int a3, void* out);

    // 0x61db54 getKeyBasedValueEx(SAnimationAccessor const&, int, int, int, float, void*)
    // Linear blend of the key at a2 toward the key at a3 by t, with the key at a4 as base.
    static float getKeyBasedValueEx(const SAnimationAccessor& accessor, int a2, int a3, int a4, float t, void* out);

    // 0x61ff3c getKeyBasedValueEx(SAnimationAccessor const&, int, void*)
    // Copies the key at a2 (and the default value words when present).
    static const float* getKeyBasedValueEx(const SAnimationAccessor& accessor, int a2, void* out);

    // 0x61ffbc getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*)
    // Linear blend of the key at a2 toward the key at a3 by t.
    static float getKeyBasedValueEx(const SAnimationAccessor& accessor, int a2, int a3, float t, void* out);
};

}  // namespace glitch::collada::animation_track
