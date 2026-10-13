// glitch::scene::IParticleSystemSceneNode: the base of the scene particle system node. Canonical header of the scene-owner wave. It
// is the glitch::scene counterpart of collada::IParticleSystemSceneNode (collada/IParticleSystemSceneNode.hpp, a different class in
// another namespace, kept as it is). It adds no members of its own: the object is the ISceneNode.
//
// Evidence (libDungeonHunter2.so, ARM): constructor 0x6c23f4 (C2), called by CParticleSystemSceneNode's constructor 0x6c2478 with
// the base's vtable (off_988614) and the position, rotation vector and scale. The rotation vector is converted to a quaternion by the
// original; the port passes the identity quaternion (PARTIAL, see the constructor).
#pragma once

#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"

namespace glitch::scene {

class IParticleSystemSceneNode : public ISceneNode {
public:
    // 0x6c23f4 (C2). PARTIAL: the rotation vector is not converted to a quaternion; the identity is used.
    IParticleSystemSceneNode(int id, const core::vector3d<float>& position, const core::vector3d<float>& rotation,
                             const core::vector3d<float>& scale);
    // Destructor: PARTIAL, empty (the base destruction is the ISceneNode destructor).
    ~IParticleSystemSceneNode() override;
};

}  // namespace glitch::scene
