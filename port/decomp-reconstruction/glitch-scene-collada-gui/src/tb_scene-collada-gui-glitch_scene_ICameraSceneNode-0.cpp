#include "glitch-scene-collada-gui/scene_nodes.hpp"

namespace glitch::scene {

namespace {

// ICameraSceneNode::ICameraSceneNode takes the rotation as three angles. The original builds the quaternion
// with quaternion::set(x, y, z) (0x35c9d8) before the ISceneNode constructor call.
core::quaternion rotationFromAngles(const core::vector3d<float>& angles) {
    core::quaternion rotation;
    rotation.set(angles.X, angles.Y, angles.Z);
    return rotation;
}

}  // namespace

// 0x582b18 _ZN6glitch5scene16ICameraSceneNodeC2EiRKNS_4core8vector3dIfEES6_S6_
// The rotation quaternion comes from quaternion::set(rotation.x, rotation.y, rotation.z); the base constructor
// ISceneNode(id, position, quaternion, scale) (0x5990c0) gets it. The constructor then writes the vtable words
// (+0x00, and +0x130 for the IEventReceiver sub-object, both implicit here) and clears the orthogonal byte (+0x134).
ICameraSceneNode::ICameraSceneNode(int id, const core::vector3d<float>& position,
                                   const core::vector3d<float>& rotation, const core::vector3d<float>& scale)
    : ISceneNode(id, position, rotationFromAngles(rotation), scale), orthogonal(false) {}

// 0x35bec0 _ZNK6glitch5scene16ICameraSceneNode12isOrthogonalEv
// Returns the flag byte at +0x134 as a truth value.
bool ICameraSceneNode::isOrthogonal() const {
    return orthogonal;
}

}  // namespace glitch::scene
