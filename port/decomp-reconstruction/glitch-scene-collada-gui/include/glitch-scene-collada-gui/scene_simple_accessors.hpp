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

class IText {
public:
    // 0x6d5838 _ZN6glitch5scene5ITextD1Ev (empty)
    ~IText() = default;
};

class CTextSceneNode {
public:
    // 0x6d5844 _ZNK6glitch5scene14CTextSceneNode7getTypeEv (tag "text")
    std::uint32_t getType() const;
    // 0x6d586c _ZNK6glitch5scene14CTextSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;
    // 0x6d5874 _ZN6glitch5scene14CTextSceneNode12setTextColorENS_5video6SColorE
    void setTextColor(video::SColor color);

    core::aabbox3d<float> boundingBox;  // +0x188
    video::SColor textColor;            // +0x17c (bytes +0x17c..+0x17f)
};

class CBillboardTextSceneNode {
public:
    // 0x6d5858 _ZNK6glitch5scene23CBillboardTextSceneNode7getTypeEv (tag "text")
    std::uint32_t getType() const;
    // 0x6d58a8 _ZN6glitch5scene23CBillboardTextSceneNode19onRegisterSceneNodeEv (returns 1)
    int onRegisterSceneNode();
    // 0x6d58b8 _ZNK6glitch5scene23CBillboardTextSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;
    // 0x6d59c4 _ZNK6glitch5scene23CBillboardTextSceneNode7getSizeEv
    const core::dimension2d<float>& getSize() const;
    // 0x6d589c _ZN6glitch5scene23CBillboardTextSceneNode7setTextEPKw (empty)
    void setText(const wchar_t* text);
    // 0x6d59fc _ZN6glitch5scene23CBillboardTextSceneNode8setColorERKNS_5video6SColorE (empty)
    void setColor(const glitch::video::SColor& color);
    // 0x6d5a08 _ZN6glitch5scene23CBillboardTextSceneNode8setColorERKNS_5video6SColorES5_ (empty)
    void setColor(const glitch::video::SColor& top, const glitch::video::SColor& bottom);
    // 0x6d59cc _ZN6glitch5scene23CBillboardTextSceneNode12setTextColorENS_5video6SColorE
    void setTextColor(video::SColor color);

    core::dimension2d<float> size;      // +0x188
    core::aabbox3d<float> boundingBox;  // +0x190
    video::SColor textColor;            // +0x180 (bytes +0x180..+0x183)
    void* font = nullptr;               // +0x184 IGUIFont* constructor argument; vtable slot 0x48 is queried for the dimensions
    std::uint32_t ctorArg1a8 = 0;       // +0x1a8 constructor argument (a9)
    video::SColor color;                // +0x1ac constructor argument (a10), written byte-wise
};

class CDummyTransformationSceneNode {
public:
    // 0x6bb528 _ZN6glitch5scene29CDummyTransformationSceneNode6renderEPv (empty)
    void render(void* /*unused*/);
    // 0x6bb52c _ZNK6glitch5scene29CDummyTransformationSceneNode7getTypeEv (tag "dmmy")
    std::uint32_t getType() const;
    // 0x6bb538 _ZNK6glitch5scene29CDummyTransformationSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;

    core::aabbox3d<float> boundingBox;  // +0x174
    float relativeTransformation[16] = {};  // +0x130 returned by getRelativeTransformationMatrix (CMatrix4; 16 floats assumed)
};

class CSkyBoxSceneNode {
public:
    // 0x6ce3bc _ZNK6glitch5scene16CSkyBoxSceneNode7getTypeEv (tag "sky_")
    std::uint32_t getType() const;
    // 0x6ce3c8 _ZNK6glitch5scene16CSkyBoxSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;
    // 0x6ce3f0 _ZNK6glitch5scene16CSkyBoxSceneNode16getMaterialCountEv (returns 6)
    std::uint32_t getMaterialCount() const;

    core::aabbox3d<float> boundingBox;  // +0x130
};

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

    core::aabbox3d<float> boundingBox;  // +0x184
    core::vector3d<float> rotationPivot;  // +0x150
    std::uint8_t rotationPivotFlag = 0;   // +0x1be (cleared by setRotationPivot)
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

// Animator types: the type tag is the constant each class returns.
class CSceneNodeAnimatorCameraFPS {
public:
    // 0x6c77e4 _ZNK6glitch5scene27CSceneNodeAnimatorCameraFPS22isEventReceiverEnabledEv (returns true)
    bool isEventReceiverEnabled() const;
    // 0x6c77ec _ZNK6glitch5scene27CSceneNodeAnimatorCameraFPS7getTypeEv (returns 7)
    std::uint32_t getType() const;
    // 0x6c7948 _ZN6glitch5scene27CSceneNodeAnimatorCameraFPS14setRotateSpeedEf
    void setRotateSpeed(float speed);
    // 0x6c7950 _ZN6glitch5scene27CSceneNodeAnimatorCameraFPS12setMoveSpeedEf
    void setMoveSpeed(float speed);
    // 0x6c7958 _ZNK6glitch5scene27CSceneNodeAnimatorCameraFPS14getRotateSpeedEv
    float getRotateSpeed() const;
    // 0x6c7960 _ZNK6glitch5scene27CSceneNodeAnimatorCameraFPS12getMoveSpeedEv
    float getMoveSpeed() const;

    // 0x6c7928 _ZN6glitch5scene27CSceneNodeAnimatorCameraFPS9allKeysUpEv
    void allKeysUp();
    // 0x6c7968 _ZN6glitch5scene27CSceneNodeAnimatorCameraFPS19setVerticalMovementEb
    // Stores the inverse of the argument, as the original does.
    void setVerticalMovement(bool allowed);

    float moveSpeed = 0.0f;    // +0x14
    float rotateSpeed = 0.0f;  // +0x18
    bool keyDown[6] = {};      // +0x4c..+0x51 (cleared by allKeysUp)
    bool noVerticalMovement = false;  // +0x53
};

class CSceneNodeAnimatorCameraMaya {
public:
    // 0x6c94d8 _ZNK6glitch5scene28CSceneNodeAnimatorCameraMaya22isEventReceiverEnabledEv (returns true)
    bool isEventReceiverEnabled() const;
    // 0x6c94e0 _ZNK6glitch5scene28CSceneNodeAnimatorCameraMaya7getTypeEv (returns 8)
    std::uint32_t getType() const;
    // 0x6c95ec _ZN6glitch5scene28CSceneNodeAnimatorCameraMaya14setRotateSpeedEf
    void setRotateSpeed(float speed);
    // 0x6c95f4 _ZN6glitch5scene28CSceneNodeAnimatorCameraMaya12setMoveSpeedEf
    void setMoveSpeed(float speed);
    // 0x6c95fc _ZN6glitch5scene28CSceneNodeAnimatorCameraMaya12setZoomSpeedEf
    void setZoomSpeed(float speed);
    // 0x6c9604 _ZNK6glitch5scene28CSceneNodeAnimatorCameraMaya14getRotateSpeedEv
    float getRotateSpeed() const;
    // 0x6c960c _ZNK6glitch5scene28CSceneNodeAnimatorCameraMaya12getMoveSpeedEv
    float getMoveSpeed() const;
    // 0x6c9614 _ZNK6glitch5scene28CSceneNodeAnimatorCameraMaya12getZoomSpeedEv
    float getZoomSpeed() const;

    // 0x6c95cc _ZN6glitch5scene28CSceneNodeAnimatorCameraMaya14isMouseKeyDownEi
    bool isMouseKeyDown(int key) const;
    // 0x6c95d8 _ZN6glitch5scene28CSceneNodeAnimatorCameraMaya9allKeysUpEv
    void allKeysUp();

    // 0x6c97f4 _ZN6glitch5scene28CSceneNodeAnimatorCameraMaya20updateAnimationStateEv
    // Partial: the original tail-calls __aeabi_fsub(+0x14, +0x54), so the difference is returned.
    float updateAnimationState() const;

    bool mouseKeyDown[3] = {};         // +0x0c..+0x0e (cleared by allKeysUp)
    std::uint8_t pad_000f[0x05] = {};  // +0x0f..+0x13 (not mapped)
    float stateA = 0.0f;               // +0x14 (minuend of updateAnimationState)
    std::uint8_t pad_0018[0x0c] = {};  // +0x18..+0x23 (not mapped)
    float zoomSpeed = 0.0f;            // +0x24
    float rotateSpeed = 0.0f;          // +0x28
    float moveSpeed = 0.0f;            // +0x2c
    std::uint8_t pad_0030[0x24] = {};  // +0x30..+0x53 (not mapped)
    float stateB = 0.0f;               // +0x54 (subtrahend of updateAnimationState)
};

class CSceneNodeAnimatorCollisionResponse {
public:
    // 0x6ca694 _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse7getTypeEv (returns 6)
    std::uint32_t getType() const;
    // 0x6ca69c _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse9isFallingEv
    bool isFalling() const;
    // 0x6ca74c _ZN6glitch5scene35CSceneNodeAnimatorCollisionResponse15setSlidingSpeedEf
    void setSlidingSpeed(float speed);
    // 0x6ca754 _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse15getSlidingSpeedEv
    float getSlidingSpeed() const;
    // 0x6ca75c _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse8getWorldEv
    ITriangleSelector* getWorld() const;

    // 0x6ca6a4 _ZN6glitch5scene35CSceneNodeAnimatorCollisionResponse18setEllipsoidRadiusERKNS_4core8vector3dIfEE
    void setEllipsoidRadius(const core::vector3d<float>& radius);
    // 0x6ca6c0 _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse18getEllipsoidRadiusEv
    core::vector3d<float> getEllipsoidRadius() const;
    // 0x6ca6dc _ZN6glitch5scene35CSceneNodeAnimatorCollisionResponse10setGravityERKNS_4core8vector3dIfEE
    void setGravity(const core::vector3d<float>& gravity);
    // 0x6ca6f8 _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse10getGravityEv
    core::vector3d<float> getGravity() const;
    // 0x6ca714 _ZN6glitch5scene35CSceneNodeAnimatorCollisionResponse23setEllipsoidTranslationERKNS_4core8vector3dIfEE
    void setEllipsoidTranslation(const core::vector3d<float>& translation);
    // 0x6ca730 _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse23getEllipsoidTranslationEv
    core::vector3d<float> getEllipsoidTranslation() const;

    core::vector3d<float> ellipsoidRadius;       // +0x18
    core::vector3d<float> gravity;               // +0x24
    core::vector3d<float> ellipsoidTranslation;  // +0x30
    ITriangleSelector* world = nullptr;  // +0x3c
    bool falling = false;                // +0x54
    float slidingSpeed = 0.0f;           // +0x50
    bool animateCameraTarget = false;    // +0x56 serialize attribute "AnimateCameraTarget" (serialize 0x6ca764, deserialize 0x6ca804)
};

class CSceneNodeAnimatorDelete {
public:
    // 0x6cb418 _ZNK6glitch5scene24CSceneNodeAnimatorDelete7getTypeEv (returns 5)
    std::uint32_t getType() const;

    std::uint32_t deleteTime = 0;           // +0x0c constructor argument, passed back to createClone
    CSceneManager* sceneManager = nullptr;  // +0x10 constructor argument, passed to createClone
};

class CSceneNodeAnimatorFlyCircle {
public:
    // 0x6cb730 _ZNK6glitch5scene27CSceneNodeAnimatorFlyCircle7getTypeEv (returns 0)
    std::uint32_t getType() const;

    core::vector3d<float> center;     // +0x0c constructor argument
    core::vector3d<float> direction;  // +0x18 constructor argument, normalized by init()
    core::vector3d<float> vecU;       // +0x24 written by init()
    core::vector3d<float> vecV;       // +0x30 written by init()
    float radius = 0.0f;              // +0x3c constructor argument
    float speed = 0.0f;               // +0x40 constructor argument, multiplies the time delta
    std::uint32_t startTime = 0;      // +0x44 constructor argument, subtracted from the animate time
};

class CSceneNodeAnimatorFlyStraight {
public:
    // 0x6cc070 _ZNK6glitch5scene29CSceneNodeAnimatorFlyStraight7getTypeEv (returns 1)
    std::uint32_t getType() const;
    // 0x6cc330 _ZN6glitch5scene29CSceneNodeAnimatorFlyStraight25recalculateImidiateValuesEv
    // Partial: the original tail-calls __aeabi_fsub(+0x18, +0x0c), so the difference of the X components of end and start is
    // returned; the vector, length and time factor are not stored here.
    float recalculateImidiateValues() const;

    core::vector3d<float> start;   // +0x0c constructor argument
    core::vector3d<float> end;     // +0x18 constructor argument
    core::vector3d<float> vector;  // +0x24 written by recalculateImidiateValues
    float wayLength = 0.0f;        // +0x30 written by recalculateImidiateValues
    float timeFactor = 0.0f;       // +0x34 written by recalculateImidiateValues
    std::uint32_t startTime = 0;   // +0x38 constructor argument, subtracted from the animate time
    std::uint32_t timeForWay = 0;  // +0x3c constructor argument
    bool loop = false;             // +0x40 constructor argument
};

class CSceneNodeAnimatorFollowSpline {
public:
    // 0x6cc754 _ZNK6glitch5scene30CSceneNodeAnimatorFollowSpline7getTypeEv (returns 2)
    std::uint32_t getType() const;

    std::vector<core::vector3d<float>> waypoints;  // +0x0c (begin) .. +0x14 (capacity), 12-byte elements
    float speed = 0.0f;                            // +0x18 constructor argument, scales the time delta
    float tightness = 0.0f;                        // +0x1c not confirmed
    std::uint32_t startTime = 0;                   // +0x20 constructor argument, subtracted from the animate time
};

class CSceneNodeAnimatorRotation {
public:
    // 0x6cd58c _ZNK6glitch5scene26CSceneNodeAnimatorRotation7getTypeEv (returns 3)
    std::uint32_t getType() const;

    float rotation[4] = {};       // +0x0c quaternion copied from the constructor argument
    std::uint32_t startTime = 0;  // +0x1c constructor argument, subtracted from the animate time
};

class CSceneNodeAnimatorTexture {
public:
    // 0x6cd9d8 _ZNK6glitch5scene25CSceneNodeAnimatorTexture7getTypeEv (returns 4)
    std::uint32_t getType() const;
    // 0x6cda34 _ZN6glitch5scene25CSceneNodeAnimatorTexture10updateTimeEj (empty)
    void updateTime(std::uint32_t time);

    std::vector<video::ITexture*> textures;  // +0x0c (begin) .. +0x14 (capacity); counted intrusive_ptr<ITexture> in the original
    std::uint32_t timePerFrame = 0;          // +0x18 divisor in animateNode (time / period)
    std::uint32_t startTime = 0;             // +0x1c subtracted from the animate time
    bool loop = false;                       // +0x24 read in animateNode and createClone
};

// E_SCENE_NODE_ANIMATOR_TYPE (the factory name-table ids) is defined once in scene/scene_types.hpp.

class CDefaultSceneNodeAnimatorFactory {
public:
    // 0x6b96a8 _ZNK6glitch5scene32CDefaultSceneNodeAnimatorFactory38getCreatableSceneNodeAnimatorTypeCountEv (returns 9)
    std::uint32_t getCreatableSceneNodeAnimatorTypeCount() const;
    // 0x6b96b0 _ZNK6glitch5scene32CDefaultSceneNodeAnimatorFactory34getCreateableSceneNodeAnimatorTypeEj
    // Indices above 8 are clamped to 10, as in the original.
    std::uint32_t getCreateableSceneNodeAnimatorType(std::uint32_t index) const;
    // 0x6b96c0 _ZNK6glitch5scene32CDefaultSceneNodeAnimatorFactory38getCreateableSceneNodeAnimatorTypeNameEj (null when index is 9 or more)
    const char* getCreateableSceneNodeAnimatorTypeName(std::uint32_t index) const;
    // 0x6b96e0 _ZNK6glitch5scene32CDefaultSceneNodeAnimatorFactory38getCreateableSceneNodeAnimatorTypeNameENS0_26E_SCENE_NODE_ANIMATOR_TYPEE (null when out of range)
    const char* getCreateableSceneNodeAnimatorTypeName(E_SCENE_NODE_ANIMATOR_TYPE type) const;

    glitch::IReferenceCounted* cursorControl = nullptr;  // +0x0c a counted reference, dropped in the destructor
};

// Creatable scene-node types. Each record is 0x1c bytes in the original: the type word
// at +0x00 and the name pointer at +0x18; the words between are not mapped.
class CDefaultSceneNodeFactory {
public:
    struct SCreatableType {
        std::uint32_t type = 0;          // +0x00
        std::uint32_t reserved[5] = {};  // +0x04..+0x17
        const char* name = nullptr;      // +0x18
    };

    // 0x6b9cf0 _ZNK6glitch5scene24CDefaultSceneNodeFactory30getCreatableSceneNodeTypeCountEv
    std::int32_t getCreatableSceneNodeTypeCount() const;
    // 0x6b9d1c _ZNK6glitch5scene24CDefaultSceneNodeFactory26getCreateableSceneNodeTypeEj
    // Out-of-range indices return the four-character id 'unkn'.
    std::uint32_t getCreateableSceneNodeType(std::uint32_t index) const;
    // 0x6b9d60 _ZNK6glitch5scene24CDefaultSceneNodeFactory30getCreateableSceneNodeTypeNameEj (null when out of range)
    const char* getCreateableSceneNodeTypeName(std::uint32_t index) const;

    std::vector<SCreatableType> types;  // +0x08 (begin) .. +0x0c (end)
};

// ---- Remaining interface destructors (empty D1 bodies). ----

class ISceneCollisionManager {
public:
    // 0x6c3be8 _ZN6glitch5scene22ISceneCollisionManagerD1Ev (empty)
    ~ISceneCollisionManager() = default;
};

class IMeshCache {
public:
    // 0x6be470 _ZN6glitch5scene10IMeshCacheD1Ev (empty)
    ~IMeshCache() = default;
};

// Mesh cache. Each entry is 0x1c bytes: the file name at +0x14 and the mesh at +0x18.
class CMeshCache {
public:
    struct SMeshEntry {
        std::uint8_t reserved[0x14] = {};  // +0x00..+0x13 (not mapped)
        const char* filename = nullptr;    // +0x14
        const IAnimatedMesh* mesh = nullptr;  // +0x18
    };

    // 0x6be504 _ZNK6glitch5scene10CMeshCache12getMeshCountEv
    std::int32_t getMeshCount() const;
    // 0x6be60c _ZNK6glitch5scene10CMeshCache15getMeshFilenameEj (null when out of range)
    const char* getMeshFilename(std::uint32_t index) const;
    // 0x6be64c _ZNK6glitch5scene10CMeshCache15getMeshFilenameERKN5boost13intrusive_ptrIKNS0_13IAnimatedMeshEEE
    // The first entry whose mesh matches is used; null when none matches.
    const char* getMeshFilename(const boost::intrusive_ptr<const IAnimatedMesh>& mesh) const;
    // 0x6be530 _ZNK6glitch5scene10CMeshCache12getMeshIndexERKN5boost13intrusive_ptrIKNS0_13IAnimatedMeshEEE
    // Index of the first entry whose mesh matches, or -1 when none does.
    std::int32_t getMeshIndex(const boost::intrusive_ptr<const IAnimatedMesh>& mesh) const;
    // 0x6bea64 _ZN6glitch5scene10CMeshCache5clearEv
    // Partial: the entries are dropped without releasing their mesh references.
    void clear();

    CMeshCache() = default;
    // 0x6be474 _ZN6glitch5scene10CMeshCacheC2EPNS_5video12IVideoDriverE
    // 0x6be4bc _ZN6glitch5scene10CMeshCacheC1EPNS_5video12IVideoDriverE
    explicit CMeshCache(video::IVideoDriver* driver);

    std::uint32_t referenceCount = 1;           // +0x04 (IReferenceCounted base, starts at 1)
    std::vector<SMeshEntry> entries;            // +0x08 (begin) .. +0x0c (end)
    video::IVideoDriver* videoDriver = nullptr;  // +0x14
};

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

// Binary COLLADA file loader. Only the extension test is mapped.
class CColladaBinaryFileLoader {
public:
    // 0x6b9538 _ZNK6glitch5scene24CColladaBinaryFileLoader24isALoadableFileExtensionEPKc
    // The original returns a char* that is 1 when the name contains ".bdae" and null
    // otherwise; here that is a bool.
    bool isALoadableFileExtension(const char* filename) const;
};

}  // namespace glitch::scene
