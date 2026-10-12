// Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreterQuate-0.
// glitch::collada::animation_track::CInterpreterQuaternionAngle<CSceneNodeQuaternionAngleMixin<float>, float>::getKeyBasedValueEx,
// four overloads. Declarations are in
// glitch-scene-collada-gui/include/glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreterQuaternionAngle_QAngleFloat.hpp.
//
// Shared shape: the CInterpreter callees fill a four-word axis-angle block (axis x, y, z, angle at word 3). fromAngleAxis
// turns that block into a quaternion (glitch::core::quaternion::fromAngleAxis(angle, axis)). The conjugate of a quaternion is
// formed by flipping the sign of x, y and z, which is what the original does with its +0x80000000 integer adds.
//
// The stack words the original leaves unset (the spare word of the axis-angle block on the no-default path, and the output
// words the callee does not write) are zero-initialised here. Those bytes are only read on paths the callee leaves unwritten.

#include <cstring>

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreterQuaternionAngle_QAngleFloat.hpp"

namespace glitch::collada::animation_track {

namespace {

// fromAngleAxis(v[3], axis = v[0..2]): the quaternion of one axis-angle block.
glitch::core::quaternion quaternionFromAngleAxisWords(const float* v) {
    glitch::core::vector3d<float> axis;
    axis.X = v[0];
    axis.Y = v[1];
    axis.Z = v[2];
    return glitch::core::quaternion::fromAngleAxis(v[3], axis);
}

// Writes the four quaternion words (X, Y, Z, W) to out.
void storeQuaternionWords(float* out, const glitch::core::quaternion& q) {
    out[0] = q.X;
    out[1] = q.Y;
    out[2] = q.Z;
    out[3] = q.W;
}

// Conjugate: negates the vector part, as the original toggles the sign bit of words 0..2.
glitch::core::quaternion conjugateQuaternion(glitch::core::quaternion q) {
    q.X = -q.X;
    q.Y = -q.Y;
    q.Z = -q.Z;
    return q;
}

}  // namespace

// 0x61f594 glitch::collada::animation_track::CInterpreterQuaternionAngle<glitch::collada::animation_track::CSceneNodeQuaternionAngleMixin<float>, float>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, void*)
// Reads the axis-angle block of one key, then writes its quaternion to out and returns out.
// partial: the CInterpreter<CSceneNodeQuaternionAngleMixin<float>, float, 4, SUseDefaultValues<3, float>>::getKeyBasedValueEx
// (key, out) callee at 0x61f51c is not reconstructed yet, so the block it fills is not produced by this tree. The pseudocode
// ignores the callee's return value, and so does this body.
float* CInterpreterQuaternionAngleFloat::getKeyBasedValueEx(const SAnimationAccessor& acc, int key, float* out) {
    float value[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    CInterpreterSceneNodeQuaternionAngleFloat4::getKeyBasedValueEx(acc, key, value);
    storeQuaternionWords(out, quaternionFromAngleAxisWords(value));
    return out;
}

// 0x61f5e4 glitch::collada::animation_track::CInterpreterQuaternionAngle<glitch::collada::animation_track::CSceneNodeQuaternionAngleMixin<float>, float>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, int, void*)
// The original reads the "to" key first (into the block that becomes the to quaternion), then the "from" key. The result is
// conjugate(from) * to, written to out. The return value is product.X.
// partial: both key reads go through the CInterpreter (key, out) callee at 0x61f51c, which is not reconstructed yet.
float CInterpreterQuaternionAngleFloat::getKeyBasedValueEx(const SAnimationAccessor& acc, int from, int to, float* out) {
    float toValue[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float fromValue[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    CInterpreterSceneNodeQuaternionAngleFloat4::getKeyBasedValueEx(acc, to, toValue);
    CInterpreterSceneNodeQuaternionAngleFloat4::getKeyBasedValueEx(acc, from, fromValue);
    const glitch::core::quaternion toQuaternion = quaternionFromAngleAxisWords(toValue);
    const glitch::core::quaternion fromQuaternion = conjugateQuaternion(quaternionFromAngleAxisWords(fromValue));
    const glitch::core::quaternion product = fromQuaternion * toQuaternion;
    storeQuaternionWords(out, product);
    return product.X;
}

// 0x61f6f4 glitch::collada::animation_track::CInterpreterQuaternionAngle<glitch::collada::animation_track::CSceneNodeQuaternionAngleMixin<float>, float>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, int, int, float, void*)
// Order of reads: from (a3), to (a4), base (a2). slerp(from, to, t) is the blended quaternion; the base quaternion is
// conjugated and multiplied on the left: out = conjugate(base) * slerp(from, to, t). The return value is product.X.
// The unset-local initialisation the pseudocode performs (1.0 and 0.0 stores) is overwritten by fromAngleAxis and slerp and is
// not repeated.
// partial: the three key reads go through the CInterpreter (key, out) callee at 0x61f51c, which is not reconstructed yet.
float CInterpreterQuaternionAngleFloat::getKeyBasedValueEx(const SAnimationAccessor& acc, int base, int from, int to, float t,
                                                           float* out) {
    float fromValue[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float toValue[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float baseValue[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    CInterpreterSceneNodeQuaternionAngleFloat4::getKeyBasedValueEx(acc, from, fromValue);
    CInterpreterSceneNodeQuaternionAngleFloat4::getKeyBasedValueEx(acc, to, toValue);
    CInterpreterSceneNodeQuaternionAngleFloat4::getKeyBasedValueEx(acc, base, baseValue);
    const glitch::core::quaternion fromQuaternion = quaternionFromAngleAxisWords(fromValue);
    const glitch::core::quaternion toQuaternion = quaternionFromAngleAxisWords(toValue);
    const glitch::core::quaternion blended = glitch::core::quaternion::slerp(fromQuaternion, toQuaternion, t);
    const glitch::core::quaternion baseQuaternion = conjugateQuaternion(quaternionFromAngleAxisWords(baseValue));
    const glitch::core::quaternion product = baseQuaternion * blended;
    storeQuaternionWords(out, product);
    return product.X;
}

// 0x61f94c glitch::collada::animation_track::CInterpreterQuaternionAngle<glitch::collada::animation_track::CSceneNodeQuaternionAngleMixin<float>, float>::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, int, float, void*)
// Blends two keys with t through the CInterpreter (from, to, t, out) callee, then converts the blended block to a quaternion
// written to out. Returns out.
// partial: the blend goes through the CInterpreter (from, to, t, out) callee at 0x61f898, which is not reconstructed yet.
float* CInterpreterQuaternionAngleFloat::getKeyBasedValueEx(const SAnimationAccessor& acc, int from, int to, float t,
                                                            float* out) {
    float value[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    CInterpreterSceneNodeQuaternionAngleFloat4::getKeyBasedValueEx(acc, from, to, t, value);
    storeQuaternionWords(out, quaternionFromAngleAxisWords(value));
    return out;
}

}  // namespace glitch::collada::animation_track
