// glitch::collada scene-node, mesh, animation-set and value-track accessors that
// read one member or forward to one other accessor. Each class maps only the
// members these accessors touch; the rest of the layout is a pad. Offsets are
// kept as comments, and the code uses typed members only.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/collada/CAnimationSet.hpp"
#include "glitch-scene-collada-gui/collada/CDynamicAnimationSet.hpp"
#include "glitch-scene-collada-gui/collada/CSceneNodeAnimatorSet.hpp"
#include "glitch-scene-collada-gui/collada/IMesh.hpp"
#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/collada/CCameraSceneNode.hpp"
#include "glitch-scene-collada-gui/collada/CLightSceneNode.hpp"
#include "glitch-scene-collada-gui/collada/CMeshSceneNode.hpp"
#include "glitch-scene-collada-gui/collada/CModularSkinnedMesh.hpp"
#include "glitch-scene-collada-gui/collada/CSceneNode.hpp"

namespace glitch::scene {
class ISceneNode;
class CMeshBuffer;
}

namespace glitch::video {
class CMaterial;
class CMaterialVertexAttributeMap;
}

namespace glitch {
class IReferenceCounted;
}

namespace glitch::video {
class CMaterial;
class CMaterialVertexAttributeMap;
}  // namespace glitch::video

namespace glitch::collada {

// Uid holder: the first word of the object it points to is the uid string.
struct SUidHolder {
    const char* uid = nullptr;  // word 0
};

// CDynamicAnimationSet: canonical in collada/CDynamicAnimationSet.hpp.

class CCoronasSceneNode {
public:
    // 0x6e5a38 _ZNK6glitch7collada17CCoronasSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;

    core::aabbox3d<float> boundingBox;  // +0x150
};

// CMesh, CMorphingMesh and IMesh: canonical definitions in collada/IMesh.hpp.

// CSceneNode, CMeshSceneNode, CModularSkinnedMeshSceneNode, CLightSceneNode and CCameraSceneNode: canonical in collada/*.hpp (included above).

namespace animation_track {
class CApplicatorInfo;
}  // namespace animation_track

// CSceneNodeAnimatorSet: canonical in collada/CSceneNodeAnimatorSet.hpp.

class CParticleSystemSceneNode {
public:
    // 0x64bb78 _ZNK6glitch7collada24CParticleSystemSceneNode6getUIDEv
    const char* getUID() const;
    // 0x64bbc0 _ZNK6glitch7collada24CParticleSystemSceneNode25getTransformedBoundingBoxEv
    const core::aabbox3d<float>* getTransformedBoundingBox() const;

    const core::aabbox3d<float>* transformedBoundingBox = nullptr;  // +0x14c
    SUidHolder* uidHolder = nullptr;                                // +0x18c
};

class CGlitchNewParticleSystemSceneNode {
public:
    // 0x637b3c _ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode6getUIDEv
    const char* getUID() const;
    // 0x637b84 _ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode25getTransformedBoundingBoxEv
    const core::aabbox3d<float>* getTransformedBoundingBox() const;

    const core::aabbox3d<float>* transformedBoundingBox = nullptr;  // +0x14c
    SUidHolder* uidHolder = nullptr;                                // +0x17c
};

namespace animation_track {

// Float-valued track: identity value is 0.0.
class CFloatEx {
public:
    // 0x6e3118 _ZNK6glitch7collada15animation_track8CFloatEx16getIdentityValueEPv
    void getIdentityValue(void* out) const;
    // 0x6e31e4 _ZN6glitch7collada15animation_track8CFloatExD1Ev (empty)
    ~CFloatEx() = default;
};

// Weight-valued track: identity value is 0.0.
class CWeightEx {
public:
    // 0x6e4dbc _ZNK6glitch7collada15animation_track9CWeightEx16getIdentityValueEPv
    void getIdentityValue(void* out) const;
    // 0x6e4f48 _ZN6glitch7collada15animation_track9CWeightExD1Ev (empty)
    ~CWeightEx() = default;
};

}  // namespace animation_track

}  // namespace glitch::collada
