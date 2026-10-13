// glitch::scene accessors and interface destructors whose bodies are constant
// returns, one-member reads or one-member writes. Classes map only the members
// those bodies touch; offsets are kept as comments and the code uses typed
// members only. Virtual dispatch is not modelled (no vtables in the stand-ins).
#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/scene/CBillboardSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/CMesh.hpp"
#include "glitch-scene-collada-gui/scene/IMesh.hpp"
#include "glitch-scene-collada-gui/scene/SAnimatedMesh.hpp"
#include "glitch-scene-collada-gui/scene/scene_types.hpp"
#include "glitch-scene-collada-gui/scene/SceneFactories.hpp"

namespace glitch::video {
class IVideoDriver;
class CMaterial;
class CMaterialVertexAttributeMap;
}

namespace glitch::io {
class IReadFile;
}

namespace glitch {

// 0x5a20bc _ZN6glitcheqERKSt4pairIjNS_4core8aabbox3dIfEEES6_
// Pairs of a buffer index and a box compare by their index only.
bool operator==(const std::pair<std::uint32_t, core::aabbox3d<float>>& lhs,
                const std::pair<std::uint32_t, core::aabbox3d<float>>& rhs);

}  // namespace glitch

namespace glitch::scene {

class ITriangleSelector;
class IParticleEmitter;

// ---- Particle interfaces: type tag per interface and an empty destructor. ----

class IParticleEmitter {
public:
    // 0x6f76ec _ZNK6glitch5scene16IParticleEmitter7getTypeEv (returns 0)
    std::uint32_t getType() const;
    // 0x6f77d8 _ZN6glitch5scene16IParticleEmitterD1Ev (empty)
    ~IParticleEmitter() = default;
};

class IParticleAnimatedMeshSceneNodeEmitter {
public:
    // 0x6f76f4 _ZNK6glitch5scene37IParticleAnimatedMeshSceneNodeEmitter7getTypeEv (returns 1)
    std::uint32_t getType() const;
    // 0x6f77fc _ZN6glitch5scene37IParticleAnimatedMeshSceneNodeEmitterD1Ev (empty)
    ~IParticleAnimatedMeshSceneNodeEmitter() = default;
};

class IParticleAffector {
public:
    // 0x6f8414 _ZN6glitch5scene17IParticleAffectorD1Ev (empty)
    ~IParticleAffector() = default;
    // 0x6f836c _ZN6glitch5scene17IParticleAffector10setEnabledEb
    void setEnabled(bool value);
    // 0x6f8374 _ZNK6glitch5scene17IParticleAffector10getEnabledEv
    bool getEnabled() const;

    std::uint8_t enabled = 0;  // +0x04
};

class IParticleAttractionAffector {
public:
    // 0x6f8398 _ZNK6glitch5scene27IParticleAttractionAffector7getTypeEv (returns 1)
    std::uint32_t getType() const;
    // 0x6f8438 _ZN6glitch5scene27IParticleAttractionAffectorD1Ev (empty)
    ~IParticleAttractionAffector() = default;
};

class IParticleBoxEmitter {
public:
    // 0x6f894c _ZNK6glitch5scene19IParticleBoxEmitter7getTypeEv (returns 2)
    std::uint32_t getType() const;
    // 0x6f8a24 _ZN6glitch5scene19IParticleBoxEmitterD1Ev (empty)
    ~IParticleBoxEmitter() = default;
};

class IParticleCylinderEmitter {
public:
    // 0x6f9a00 _ZNK6glitch5scene24IParticleCylinderEmitter7getTypeEv (returns 3)
    std::uint32_t getType() const;
    // 0x6f9b14 _ZN6glitch5scene24IParticleCylinderEmitterD1Ev (empty)
    ~IParticleCylinderEmitter() = default;
};

class IParticleFadeOutAffector {
public:
    // 0x6fa47c _ZNK6glitch5scene24IParticleFadeOutAffector7getTypeEv (returns 2)
    std::uint32_t getType() const;
    // 0x6fa4b0 _ZN6glitch5scene24IParticleFadeOutAffectorD1Ev (empty)
    ~IParticleFadeOutAffector() = default;
};

class IParticleGravityAffector {
public:
    // 0x6fa9f4 _ZNK6glitch5scene24IParticleGravityAffector7getTypeEv (returns 3)
    std::uint32_t getType() const;
    // 0x6faa30 _ZN6glitch5scene24IParticleGravityAffectorD1Ev (empty)
    ~IParticleGravityAffector() = default;
};

class IParticleMeshEmitter {
public:
    // 0x6fafcc _ZNK6glitch5scene20IParticleMeshEmitter7getTypeEv (returns 4)
    std::uint32_t getType() const;
    // 0x6fb0b0 _ZN6glitch5scene20IParticleMeshEmitterD1Ev (empty)
    ~IParticleMeshEmitter() = default;
};

class IParticleRingEmitter {
public:
    // 0x6fc3c4 _ZNK6glitch5scene20IParticleRingEmitter7getTypeEv (returns 5)
    std::uint32_t getType() const;
    // 0x6fc4a4 _ZN6glitch5scene20IParticleRingEmitterD1Ev (empty)
    ~IParticleRingEmitter() = default;
};

class IParticleRotationAffector {
public:
    // 0x6fcd00 _ZNK6glitch5scene25IParticleRotationAffector7getTypeEv (returns 4)
    std::uint32_t getType() const;
    // 0x6fcd50 _ZN6glitch5scene25IParticleRotationAffectorD1Ev (empty)
    ~IParticleRotationAffector() = default;
};

class IParticleSizeAffector {
public:
    // 0x6fd1e4 _ZNK6glitch5scene21IParticleSizeAffector7getTypeEv (returns 5)
    std::uint32_t getType() const;
    // 0x6fd22c _ZN6glitch5scene21IParticleSizeAffectorD1Ev (empty)
    ~IParticleSizeAffector() = default;
};

class IParticleSphereEmitter {
public:
    // 0x6fd8f4 _ZNK6glitch5scene22IParticleSphereEmitter7getTypeEv (returns 6)
    std::uint32_t getType() const;
    // 0x6fd9c4 _ZN6glitch5scene22IParticleSphereEmitterD1Ev (empty)
    ~IParticleSphereEmitter() = default;
};

class IParticleSpinAffector {
public:
    // 0x6fe2ac _ZNK6glitch5scene21IParticleSpinAffector7getTypeEv (returns 6)
    std::uint32_t getType() const;
    // 0x6fe2ec _ZN6glitch5scene21IParticleSpinAffectorD1Ev (empty)
    ~IParticleSpinAffector() = default;
};

// ---- Scene-node types ----

// IText, CTextSceneNode, CBillboardTextSceneNode and CDummyTransformationSceneNode: canonical classes in scene/CTextSceneNode.hpp
// and scene/CDummyTransformationSceneNode.hpp.

// CSkyBoxSceneNode: canonical class in scene/CSkyBoxSceneNode.hpp.

// One terrain patch record, stride 0x38. The LOD word is the first field and the
// bounding box follows it; the rest of the record is not mapped.
struct STerrainPatch {
    std::int32_t lod = 0;                    // +0x00
    core::aabbox3d<float> boundingBox;       // +0x04
};

// The 32-bit layout evidence of the earlier layouts/scene_members.hpp put the patch container at +0x178 / +0x17c and a material
// holder at +0x1ac; the members below follow the bodies of the accessors instead, so those two are not repeated.
class CTerrainSceneNode {
public:
    // 0x6d00b4 _ZNK6glitch5scene17CTerrainSceneNode8getScaleEv
    const core::vector3d<float>& getScale() const;
    // 0x6d00bc _ZNK6glitch5scene17CTerrainSceneNode11getRotationEv
    const core::vector3d<float>& getRotation() const;
    // 0x6d00c4 _ZNK6glitch5scene17CTerrainSceneNode11getPositionEv
    const core::vector3d<float>& getPosition() const;
    // 0x6d00cc _ZNK6glitch5scene17CTerrainSceneNode7getTypeEv (tag "terr")
    std::uint32_t getType() const;
    // 0x6d01e8 _ZNK6glitch5scene17CTerrainSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;

    core::vector3d<float> position;     // +0x134
    core::vector3d<float> rotation;     // +0x140
    core::vector3d<float> scale;        // +0x15c
    // The patch grid is row-major: index = row * patchesPerSide + column.
    // 0x6d01c4 _ZN6glitch5scene17CTerrainSceneNode16setRotationPivotERKNS_4core8vector3dIfEE
    void setRotationPivot(const core::vector3d<float>& pivot);
    // 0x6d0200 _ZNK6glitch5scene17CTerrainSceneNode14getBoundingBoxEii
    const core::aabbox3d<float>& getBoundingBox(std::int32_t row, std::int32_t column) const;
    // 0x6d021c _ZN6glitch5scene17CTerrainSceneNode13setLODOfPatchEiii
    void setLODOfPatch(std::int32_t row, std::int32_t column, std::int32_t lod);
    // 0x6d03f0 _ZN6glitch5scene17CTerrainSceneNode22setCurrentLODOfPatchesEi
    void setCurrentLODOfPatches(std::int32_t lod);
    // 0x6d042c _ZN6glitch5scene17CTerrainSceneNode22setCurrentLODOfPatchesERKSt6vectorIiNS_4core10SAllocatorIiLNS_6memory13E_MEMORY_HINTE0EEEE
    void setCurrentLODOfPatches(const std::vector<std::int32_t>& lods);
    // 0x6d21ac _ZNK6glitch5scene17CTerrainSceneNode22getCurrentLODOfPatchesERSt6vectorIiNS_4core10SAllocatorIiLNS_6memory13E_MEMORY_HINTE0EEEE
    void getCurrentLODOfPatches(std::vector<std::int32_t>& lods) const;
    // 0x6d0238 _ZN6glitch5scene17CTerrainSceneNode19overrideLODDistanceEid
    // Returns 1 when the index is inside the LOD table (the squared distance is stored), else 0.
    std::int32_t overrideLODDistance(std::int32_t index, double distance);

    core::aabbox3d<float> boundingBox;  // +0x184
    core::vector3d<float> rotationPivot;  // +0x150
    std::uint8_t rotationPivotFlag = 0;   // +0x1be (cleared by setRotationPivot)
    std::uint8_t lodDistanceOverridden = 0;  // +0x1bd (set to 1 by overrideLODDistance)
    std::int32_t lodDistanceCount = 0;       // +0x180 (entries of the squared LOD distance table)
    std::vector<double> lodDistanceSquared;  // +0x19c (squared LOD distances, lodDistanceCount entries)
    std::int32_t patchesPerSide = 0;      // +0x17c
    std::vector<STerrainPatch> patches;   // +0x1a8, patchesPerSide squared entries
};

class CShadowVolumeSceneNode {
public:
    // 0x6fe854 _ZNK6glitch5scene22CShadowVolumeSceneNode7getTypeEv (tag "shdw")
    std::uint32_t getType() const;
    // 0x6fe97c _ZNK6glitch5scene22CShadowVolumeSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;
    // 0x6fe984 _ZN6glitch5scene22CShadowVolumeSceneNode13SetBiasFactorEf
    void SetBiasFactor(float factor);
    // 0x6fe974 _ZN6glitch5scene22CShadowVolumeSceneNode4loadEPNS_2io9IReadFileE (returns 0)
    std::uint32_t load(io::IReadFile* file);

    core::aabbox3d<float> boundingBox;  // +0x168
    float biasFactor = 0.0f;            // +0x188
};

class CAnimatedMeshSceneNode {
public:
    // 0x6f5fc4 _ZNK6glitch5scene22CAnimatedMeshSceneNode7getTypeEv (tag "amsh")
    std::uint32_t getType() const;
    // 0x6f5fd0 _ZNK6glitch5scene22CAnimatedMeshSceneNode10getFrameNrEv
    float getFrameNr() const;
    // 0x6f61d0 _ZNK6glitch5scene22CAnimatedMeshSceneNode13getStartFrameEv
    std::int32_t getStartFrame() const;
    // 0x6f61d8 _ZNK6glitch5scene22CAnimatedMeshSceneNode11getEndFrameEv
    std::int32_t getEndFrame() const;
    // 0x6f63b0 _ZN6glitch5scene22CAnimatedMeshSceneNode11setLoopModeEb
    void setLoopMode(bool loop);
    // 0x6f6450 _ZN6glitch5scene22CAnimatedMeshSceneNode21setRenderFromIdentityEb
    void setRenderFromIdentity(bool identity);
    // 0x6f62ec _ZN6glitch5scene22CAnimatedMeshSceneNode17setAnimationSpeedEf
    // Stores framesPerSecond scaled by 0.001 (the original multiplies by 0x3a83126f).
    void setAnimationSpeed(float framesPerSecond);

    float animationSpeed = 0.0f;  // +0x140
    std::int32_t startFrame = 0;  // +0x138
    std::int32_t endFrame = 0;    // +0x13c
    float frameNr = 0.0f;         // +0x144
    bool loopMode = false;        // +0x154
    bool renderFromIdentity = false;  // +0x170
    void* meshHolder = nullptr;   // +0x130 a counted reference around the mesh name query (serialize 0x6f6478, deserialize 0x6f6d98)
};

class CParticleSystemSceneNode {
public:
    // 0x6c0dc4 _ZNK6glitch5scene24CParticleSystemSceneNode7getTypeEv (tag "ptcl")
    std::uint32_t getType() const;
    // 0x6c0df8 _ZN6glitch5scene24CParticleSystemSceneNode10getEmitterEv
    IParticleEmitter* getEmitter();
    // 0x6c0e58 _ZNK6glitch5scene24CParticleSystemSceneNode16getMaterialCountEv (returns 1)
    std::uint32_t getMaterialCount() const;
    // 0x6c0ed8 _ZN6glitch5scene24CParticleSystemSceneNode15setTransparencyEf
    void setTransparency(float alpha);
    // 0x6c0ee0 _ZN6glitch5scene24CParticleSystemSceneNode6renderEPv (empty)
    void render(void* /*unused*/);
    // 0x6c0ee4 _ZNK6glitch5scene24CParticleSystemSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;
    // 0x6c0eec _ZN6glitch5scene24CParticleSystemSceneNode21setParticlesAreGlobalEb
    void setParticlesAreGlobal(bool global);
    // 0x6c0f08 _ZN6glitch5scene24CParticleSystemSceneNode17reallocateBuffersEv (empty)
    void reallocateBuffers();

    // 0x6c0dd0 _ZNK6glitch5scene24CParticleSystemSceneNode20getNumberOfParticlesEv
    std::int32_t getNumberOfParticles() const;
    // 0x6c0ef4 _ZN6glitch5scene24CParticleSystemSceneNode15setParticleSizeERKNS_4core11dimension2dIfEE
    void setParticleSize(const core::dimension2d<float>& size);
    // 0x6c0f0c _ZN6glitch5scene24CParticleSystemSceneNode16setBillboardTypeENS0_25E_PARTICLE_BILLBOARD_TYPEE
    void setBillboardType(std::uint32_t type);

    // One particle record, 0x44 bytes in the original. The fields are not mapped.
    struct SParticleRecord {
        std::uint8_t bytes[0x44] = {};
    };

    std::vector<SParticleRecord> particles;   // +0x13c (begin) .. +0x140 (end)
    IParticleEmitter* emitter = nullptr;      // +0x138
    float transparency = 0.0f;                // +0x184
    core::aabbox3d<float> boundingBox;        // +0x158
    core::dimension2d<float> particleSize;    // +0x148
    std::uint32_t billboardType = 0;          // +0x17c
    bool particlesAreGlobal = false;          // +0x180
};

// glitch::scene::CMesh: canonical definition in scene/CMesh.hpp.

// Animator classes: canonical classes in scene/CSceneNodeAnimator*.hpp and scene/CDefaultSceneNodeAnimatorFactory.hpp; ISceneCollisionManager in
// scene/ISceneCollisionManager.hpp.
// E_SCENE_NODE_ANIMATOR_TYPE (the factory name-table ids) is defined once in scene/scene_types.hpp.

// CDefaultSceneNodeFactory: canonical class in scene/CDefaultSceneNodeFactory.hpp.

// ---- Remaining interface destructors (empty D1 bodies). ----

// IBillboard: canonical class in scene/CBillboardSceneNode.hpp.

// IEndOfBatchCallback: canonical class in scene/CAppendMeshBuffer.hpp.

class ISegmentCompileCallback {
public:
    // 0x3ef17c _ZN6glitch5scene23ISegmentCompileCallbackD1Ev (empty)
    ~ISegmentCompileCallback() = default;
};

class SDefaultSegmentCompileCallback {
public:
    // 0x5891a8 _ZN6glitch5scene30SDefaultSegmentCompileCallbackD1Ev (empty)
    ~SDefaultSegmentCompileCallback() = default;
};

// SAnimatedMesh: canonical class in scene/SAnimatedMesh.hpp.

// CAppendMeshBuffer: canonical class in scene/CAppendMeshBuffer.hpp.

// CColladaBinaryFileLoader: canonical class in scene/CColladaBinaryFileLoader.hpp.

}  // namespace glitch::scene
