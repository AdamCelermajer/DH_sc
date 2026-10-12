// glitch::collada::animation_track::CInterpreter<CSceneNodeQuaternionMixin<float>, float, 4, SUseDefaultLerp<float>>,
// declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreter_glit-0.
//
// The primary CInterpreter template is not declared in any header of this category, and only this one
// instantiation is in the batch. The class is therefore a named stand-in holding the single static member
// that the batch reconstructs. Its layout is empty: the original reads only the accessor and the arguments.
#pragma once

#include "glitch-scene-collada-gui/layouts/animation_track.hpp"

namespace glitch::collada::animation_track {

// CInterpreter<CSceneNodeQuaternionMixin<float>, float, 4, SUseDefaultLerp<float>>.
struct CInterpreterSceneNodeQuaternionFloat4 {
    // 0x613294 getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*) (static, no this).
    // a3 is not read by the original body. Writes the blended quaternion into a5 and returns the callee result.
    static bool getKeyBasedValueEx(const glitch::collada::SAnimationAccessor& a1, int a2, int a3, float a4, float* a5);
};

}  // namespace glitch::collada::animation_track
