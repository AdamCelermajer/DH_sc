// glitch::collada::animation_track::CInterpreter<CMixin<float, 3, SSceneNodePosition, -1, float>, float, 3, SUseDefaultLerp<float>>,
// declared for the Track B batch that owns 0x6286cc.
//
// The class is declared nowhere else in glitch-scene-collada-gui (the slice_08/09 declarations are free
// functions or comments). This header holds only the explicit specialization that the batch reconstructs.
// The primary template, CMixin and SUseDefaultLerp are forward declared; their layouts belong to their own
// batches.
//
// Offsets are the original 32-bit ARM offsets, kept as comments only.
#pragma once

#include <cstdint>

namespace glitch::collada {

struct SAnimationAccessor;  // full layout in layouts/animation_track.hpp; not included here

namespace animation_track {

struct SSceneNodePosition;

template <typename T, int Dim, typename Component, int Index, typename Value>
class CMixin;

template <typename T>
struct SUseDefaultLerp;

// Primary template: layout and other specializations are declared by their own batches.
template <typename Mixin, typename Key, int Dim, typename Default>
class CInterpreter;

template <>
class CInterpreter<CMixin<float, 3, SSceneNodePosition, -1, float>, float, 3, SUseDefaultLerp<float>> {
public:
    // 0x6286cc getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*) (static, no this).
    // a3 is not read by the original body. Writes the three blended components (x, y, z) into a5 and
    // returns the x component, which the original leaves in r0.
    static float getKeyBasedValueEx(const glitch::collada::SAnimationAccessor& a1, int a2, int a3, float a4,
                                    float* a5);
};

}  // namespace animation_track

namespace tb_ext {

// SAnimationAccessor::getOutput(0) (0x669e24), followed by the word at output +0x04 that the original
// reads as the key data base. The key data holds float triples (x, y, z), 12 bytes per key, at the
// original offsets. Defined outside this batch: the body of getOutput is not reconstructed in this tree,
// and the host width of the data pointer is not verified (the layout comments call it a guest 32-bit word).
const float* accessorOutputKeys(const glitch::collada::SAnimationAccessor& accessor);

}  // namespace tb_ext

}  // namespace glitch::collada
