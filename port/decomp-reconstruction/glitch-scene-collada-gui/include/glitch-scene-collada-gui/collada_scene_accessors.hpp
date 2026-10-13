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
#include "glitch-scene-collada-gui/modular_skinned_mesh.hpp"

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

// Named mesh record; the uid word is at +0x08 in the original.
struct SNamedMeshData {
    std::uint32_t reserved[2] = {0, 0};  // +0x00..+0x07
    const char* uid = nullptr;           // +0x08
};

// CDynamicAnimationSet: canonical in collada/CDynamicAnimationSet.hpp.

class CCoronasSceneNode {
public:
    // 0x6e5a38 _ZNK6glitch7collada17CCoronasSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;

    core::aabbox3d<float> boundingBox;  // +0x150
};

// CMesh, CMorphingMesh and IMesh: canonical definitions in collada/IMesh.hpp.

class CModularSkinnedMeshSceneNode {
public:
    // 0x64943c _ZNK6glitch7collada28CModularSkinnedMeshSceneNode22getCategoryModuleCountEi
    std::uint32_t getCategoryModuleCount(int category) const;
    // 0x649444 _ZNK6glitch7collada28CModularSkinnedMeshSceneNode16getCategoryCountEv
    std::uint32_t getCategoryCount() const;
    // 0x64945c _ZNK6glitch7collada28CModularSkinnedMeshSceneNode18getCurrentModuleIdEi
    std::int32_t getCurrentModuleId(int category) const;
    // The five accessors below are tail calls into the mesh (thunks in the original).
    // 0x64944c _ZNK6glitch7collada28CModularSkinnedMeshSceneNode13getModuleNameEii
    const char* getModuleName(int category, int module) const;
    // 0x649454 _ZNK6glitch7collada28CModularSkinnedMeshSceneNode20getCurrentModuleNameEi
    const char* getCurrentModuleName(int category) const;
    // 0x649464 _ZNK6glitch7collada28CModularSkinnedMeshSceneNode11getModuleIdEPKc
    std::int32_t getModuleId(const char* name) const;
    // 0x64946c _ZNK6glitch7collada28CModularSkinnedMeshSceneNode15getCategoryNameEi
    const char* getCategoryName(int category) const;
    // 0x6494e0 _ZNK6glitch7collada28CModularSkinnedMeshSceneNode13getCategoryIdEPKc
    std::int32_t getCategoryId(const char* name) const;

    CModularSkinnedMesh* mesh = nullptr;  // +0x134
};

namespace animation_track {
class CApplicatorInfo;
}  // namespace animation_track

// CSceneNodeAnimatorSet: canonical in collada/CSceneNodeAnimatorSet.hpp.

// Scene-node property block (pointed to by +0x154 of the node). The uid string is the
// word at +0x00, the scope id the word at +0x08 and the user property word at +0x48.
struct SNodeUserProperties {
    const char* uid = nullptr;         // +0x00
    std::uint8_t pad_0004[0x04] = {};  // +0x04..+0x07 (not mapped)
    const char* scopeID = nullptr;     // +0x08
    std::uint8_t pad_0010[0x38] = {};  // +0x10..+0x47 (not mapped)
    std::int32_t value = 0;            // +0x48
};

class CSceneNode {
public:
    // 0x65cd70 _ZNK6glitch7collada10CSceneNode15getUserPropertyEv
    // Returns 0 when the node has no property block.
    std::int32_t getUserProperty() const;
    // 0x65cd28 _ZNK6glitch7collada10CSceneNode6getUIDEv (returns "" when there is no block)
    const char* getUID() const;
    // 0x65cd4c _ZNK6glitch7collada10CSceneNode10getScopeIDEv (returns "" when there is no block)
    const char* getScopeID() const;
    // 0x65cd80 _ZNK6glitch7collada10CSceneNode18getUserPropertyStrEv
    // PARTIAL: returns the uid word of the holder that the +0x48 word points to. The pointee cannot be
    // followed on host pointer width (see the .cpp); the body returns nullptr.
    const char* getUserPropertyStr() const;
    // 0x65cda8 _ZN6glitch7collada10CSceneNode18computeBoundingBoxEv
    // PARTIAL: child walk needs virtual slots and the intrusive child list (see the .cpp). Body is a no-op.
    void computeBoundingBox();
    // 0x65cf8c _ZNK6glitch7collada10CSceneNode18computeBoundingBoxERNS_4core8aabbox3dIfEE
    // PARTIAL: same child walk as above, recursive for node children. Body returns 0 (see the .cpp).
    int computeBoundingBox(core::aabbox3d<float>& box) const;

    core::aabbox3d<float> boundingBox;              // +0x130 (24 bytes, written by computeBoundingBox)
    SNodeUserProperties* userProperties = nullptr;  // +0x154
};

// Light scene node. The word at +0x160 is read two ways: getLightID returns it as a
// value and getUID follows it as a pointer to the uid word.
class CLightSceneNode {
public:
    // 0x644404 _ZNK6glitch7collada15CLightSceneNode10getLightIDEv
    std::uint32_t getLightID() const;
    // 0x64440c _ZNK6glitch7collada15CLightSceneNode6getUIDEv
    // Partial: the pointee type of +0x160 is inferred from the two accesses.
    std::uint32_t getUID() const;

    union {
        std::uint32_t lightID;           // +0x160 (value read by getLightID)
        const std::uint32_t* uidSource;  // +0x160 (pointer followed by getUID)
    } slot160{};
};

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

class CCameraSceneNode {
public:
    // 0x6e5160 _ZNK6glitch7collada16CCameraSceneNode13getTargetNodeEv
    glitch::scene::ISceneNode* getTargetNode() const;
    // 0x6e5174 _ZNK6glitch7collada16CCameraSceneNode6getUIDEv
    const char* getUID() const;

    glitch::scene::ISceneNode* targetNode = nullptr;  // +0x390
    SUidHolder* uidHolder = nullptr;                  // +0x394
};

class CMeshSceneNode {
public:
    // 0x646148 _ZNK6glitch7collada14CMeshSceneNode6getUIDEv
    // The holder at +0x130 is used when set; otherwise the uid of the mesh
    // record at +0x134.
    const char* getUID() const;
    // 0x646730 _ZN6glitch7collada14CMeshSceneNode6renderEPv
    // The original returns the leftover r0 value; no caller uses it, so void here.
    void render(void* context);

    SUidHolder* uidHolder = nullptr;      // +0x130
    SNamedMeshData* namedMesh = nullptr;  // +0x134
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
