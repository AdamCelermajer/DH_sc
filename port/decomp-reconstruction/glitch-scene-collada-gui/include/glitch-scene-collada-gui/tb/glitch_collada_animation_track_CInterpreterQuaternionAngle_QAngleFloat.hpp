// glitch::collada::animation_track::CInterpreterQuaternionAngle<CSceneNodeQuaternionAngleMixin<float>, float>,
// declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreterQuate-0.
//
// No header in this category declares this instantiation (the only CInterpreterQuaternionAngle template is the
// recon_10 stand-in for the short and char keys). The class is therefore declared here with its four static
// getKeyBasedValueEx overloads, one per batch row, in the order of their addresses.
//
// The primary CInterpreter instantiation it forwards to, CInterpreter<CSceneNodeQuaternionAngleMixin<float>, float,
// 4, SUseDefaultValues<3, float>>, is queued at 0x61f51c (key overload) and 0x61f898 (blend overload) and is not
// reconstructed yet. It is declared below as a named stand-in with the two static members the batch calls. Its
// bodies belong to their own rows.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/layouts/animation_track.hpp"
#include "glitch-shared/glitch_core.hpp"

namespace glitch::collada::animation_track {

// CInterpreter<CSceneNodeQuaternionAngleMixin<float>, float, 4, SUseDefaultValues<3, float>>. Declarations only;
// the definitions are at 0x61f51c and 0x61f898 and are not in the tree yet. Both are static (no this argument).
struct CInterpreterSceneNodeQuaternionAngleFloat4 {
    // 0x61f51c getKeyBasedValueEx(SAnimationAccessor const&, int, void*). Returns the default value pointer (null when
    // the accessor has none); the caller ignores it.
    static const std::uint32_t* getKeyBasedValueEx(const SAnimationAccessor& acc, int key, float* out);

    // 0x61f898 getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*). Returns the blended key value.
    static float getKeyBasedValueEx(const SAnimationAccessor& acc, int from, int to, float t, float* out);
};

// CInterpreterQuaternionAngle<CSceneNodeQuaternionAngleMixin<float>, float>. The value is an axis-angle block of four
// words (axis x, y, z then the angle at word 3), converted to a quaternion with glitch::core::quaternion::fromAngleAxis.
// Static members only; the object is never constructed by these paths.
struct CInterpreterQuaternionAngleFloat {
    // 0x61f594 getKeyBasedValueEx(SAnimationAccessor const&, int, void*) -> float*. Writes the quaternion for one key to
    // out and returns out.
    static float* getKeyBasedValueEx(const SAnimationAccessor& acc, int key, float* out);

    // 0x61f5e4 getKeyBasedValueEx(SAnimationAccessor const&, int, int, void*) -> float. out receives
    // conjugate(from) * to; returns the first product component (product.X).
    static float getKeyBasedValueEx(const SAnimationAccessor& acc, int from, int to, float* out);

    // 0x61f6f4 getKeyBasedValueEx(SAnimationAccessor const&, int, int, int, float, void*) -> float. out receives
    // conjugate(base) * slerp(from, to, t); returns product[0].
    static float getKeyBasedValueEx(const SAnimationAccessor& acc, int base, int from, int to, float t, float* out);

    // 0x61f94c getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*) -> float*. Writes the quaternion of
    // the blend between two keys to out and returns out.
    static float* getKeyBasedValueEx(const SAnimationAccessor& acc, int from, int to, float t, float* out);
};

}  // namespace glitch::collada::animation_track
