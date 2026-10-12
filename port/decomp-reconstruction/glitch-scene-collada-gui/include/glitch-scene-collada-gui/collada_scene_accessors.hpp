// glitch::collada scene-node, mesh, animation-set and value-track accessors that
// read one member or forward to one other accessor. Each class maps only the
// members these accessors touch; the rest of the layout is a pad. Offsets are
// kept as comments, and the code uses typed members only.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/animation_set.hpp"
#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/modular_skinned_mesh.hpp"

namespace glitch::scene {
class ISceneNode;
}

namespace glitch::collada {

class IMesh;

// Uid holder: the first word of the object it points to is the uid string.
struct SUidHolder {
    const char* uid = nullptr;  // word 0
};

// Named mesh record; the uid word is at +0x08 in the original.
struct SNamedMeshData {
    std::uint32_t reserved[2] = {0, 0};  // +0x00..+0x07
    const char* uid = nullptr;           // +0x08
};

// One channel record of a dynamic animation set (16 bytes in the original).
struct SChannelRecord {
    std::uint32_t words[4] = {0, 0, 0, 0};
};

class CDynamicAnimationSet {
public:
    // 0x62dba0 _ZN6glitch7collada20CDynamicAnimationSet10getChannelEj
    SChannelRecord* getChannel(unsigned index);
    // 0x62dbac _ZNK6glitch7collada20CDynamicAnimationSet10getChannelEj
    const SChannelRecord* getChannel(unsigned index) const;

    SChannelRecord* channels = nullptr;  // +0x74
};

class CCoronasSceneNode {
public:
    // 0x6e5a38 _ZNK6glitch7collada17CCoronasSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;

    core::aabbox3d<float> boundingBox;  // +0x150
};

class CMesh {
public:
    // 0x644b80 _ZNK6glitch7collada5CMesh14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;
    // 0x644b88 _ZN6glitch7collada5CMesh14setBoundingBoxERKNS_4core8aabbox3dIfEE
    void setBoundingBox(const core::aabbox3d<float>& box);

    core::aabbox3d<float> boundingBox;  // +0x24
};

class CMorphingMesh {
public:
    // 0x64990c _ZNK6glitch7collada13CMorphingMesh5cloneEv
    // Returns a null handle (the original stores 0 into the result slot).
    boost::intrusive_ptr<IMesh> clone() const;
};

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

class CSceneNodeAnimatorSet {
public:
    // 0x65f0fc _ZN6glitch7collada21CSceneNodeAnimatorSet18getAnimationLengthEi
    std::uint32_t getAnimationLength(int index);
    // 0x65f104 _ZN6glitch7collada21CSceneNodeAnimatorSet17getAnimationStartEi
    std::uint32_t getAnimationStart(int index);
    // 0x65f10c _ZN6glitch7collada21CSceneNodeAnimatorSet15getAnimationEndEi
    std::uint32_t getAnimationEnd(int index);
    // 0x65f17c _ZN6glitch7collada21CSceneNodeAnimatorSet14getTargetCountEv
    std::uint32_t getTargetCount();

    CAnimationSet* animationSet = nullptr;  // +0x24
};

// Scene-node property block: the user property word is at +0x48.
struct SNodeUserProperties {
    std::uint8_t pad_0000[0x48];
    std::int32_t value = 0;  // +0x48
};

class CSceneNode {
public:
    // 0x65cd70 _ZNK6glitch7collada10CSceneNode15getUserPropertyEv
    // Returns 0 when the node has no property block.
    std::int32_t getUserProperty() const;

    SNodeUserProperties* userProperties = nullptr;  // +0x154
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
