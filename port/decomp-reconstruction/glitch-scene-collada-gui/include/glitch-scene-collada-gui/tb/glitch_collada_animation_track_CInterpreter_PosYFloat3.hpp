// glitch::collada::animation_track::CInterpreter<
//     CSceneNodePositionYEx<float>, float, 3, SUseDefaultValues<1, float> >
//
// Declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreter_glit-0
// (the batch id is shared by several batches in e1.json, so the source file is named by the class).
//
// No header in this category declared this instantiation before. The class is stateless: every method
// takes the SAnimationAccessor as its first argument and has no this pointer, so the methods are static.
//
// The shared accessor externals (SKeyOutput, accessorGetOutput, accessorHasDefaultValue,
// accessorGetDefaultValue) and the primary CInterpreter declaration come from the sibling PosZ header,
// so both instantiations use the same externals.
//
// Offsets are the original 32-bit offsets, kept as comments only.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_PosZFloat3.hpp"

namespace glitch::collada::animation_track {

// Forward declaration of the template argument, so the instantiation can be named.
template <typename Value>
class CSceneNodePositionYEx;

template <>
class CInterpreter<CSceneNodePositionYEx<float>, float, 3, SUseDefaultValues<1, float> > {
public:
    // 0x61fc70 getKeyBasedValueEx(SAnimationAccessor const&, int, void*)
    // Copies the key at a2 into the out words. When the accessor has a default value, the default's
    // word 0 goes to out[0], the key goes to out[1] and the default's word 2 goes to out[2]. Returns the
    // default pointer in that case, otherwise nullptr.
    static const float* getKeyBasedValueEx(const SAnimationAccessor& a1, int a2, void* a3);

    // 0x61fcf0 getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*)
    // Linear blend of the key at a2 toward the key at a3 by t. Writes the blend as out[0] (no default) or
    // as out[1] with the default's words 0 and 2 in out[0] and out[2]. The return value is the default
    // pointer bit pattern in the default branch (r0 in the original), otherwise the blended value.
    static float getKeyBasedValueEx(const SAnimationAccessor& a1, int a2, int a3, float a4, void* a5);

    // 0x61fdc4 getKeyBasedValueEx(SAnimationAccessor const&, int, int, void*)
    // Writes the key difference key[a3] - key[a2]. With a default value the default words 0 and 2 are
    // written around it (out[0], out[2]) and the default pointer is returned. Otherwise nullptr.
    static const float* getKeyBasedValueEx(const SAnimationAccessor& a1, int a2, int a3, void* a4);

    // 0x61fe44 getKeyBasedValueEx(SAnimationAccessor const&, int, int, int, float, void*)
    // Linear blend v12 + t * (v13 - v12) where v12 = key[a3] - key[a2] and v13 = key[a4] - key[a2].
    static float getKeyBasedValueEx(const SAnimationAccessor& a1, int a2, int a3, int a4, float a5, void* a6);
};

}  // namespace glitch::collada::animation_track
