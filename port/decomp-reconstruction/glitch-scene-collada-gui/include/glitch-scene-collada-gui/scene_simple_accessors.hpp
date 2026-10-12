// glitch::scene accessors and interface destructors whose bodies are constant
// returns, one-member reads or one-member writes. Classes map only the members
// those bodies touch; offsets are kept as comments and the code uses typed
// members only. Virtual dispatch is not modelled (no vtables in the stand-ins).
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

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

    core::aabbox3d<float> boundingBox;  // +0x188
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

    core::dimension2d<float> size;      // +0x188
    core::aabbox3d<float> boundingBox;  // +0x190
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
    core::aabbox3d<float> boundingBox;  // +0x184
};

class CShadowVolumeSceneNode {
public:
    // 0x6fe854 _ZNK6glitch5scene22CShadowVolumeSceneNode7getTypeEv (tag "shdw")
    std::uint32_t getType() const;
    // 0x6fe97c _ZNK6glitch5scene22CShadowVolumeSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;
    // 0x6fe984 _ZN6glitch5scene22CShadowVolumeSceneNode13SetBiasFactorEf
    void SetBiasFactor(float factor);

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

    IParticleEmitter* emitter = nullptr;      // +0x138
    float transparency = 0.0f;                // +0x184
    core::aabbox3d<float> boundingBox;        // +0x158
    bool particlesAreGlobal = false;          // +0x180
};

class CMesh {
public:
    // 0x6bbc68 _ZNK6glitch5scene5CMesh14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;

    core::aabbox3d<float> boundingBox;  // +0x14
};

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

    float moveSpeed = 0.0f;    // +0x14
    float rotateSpeed = 0.0f;  // +0x18
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

    float zoomSpeed = 0.0f;    // +0x24
    float rotateSpeed = 0.0f;  // +0x28
    float moveSpeed = 0.0f;    // +0x2c
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

    ITriangleSelector* world = nullptr;  // +0x3c
    bool falling = false;                // +0x54
    float slidingSpeed = 0.0f;           // +0x50
};

class CSceneNodeAnimatorDelete {
public:
    // 0x6cb418 _ZNK6glitch5scene24CSceneNodeAnimatorDelete7getTypeEv (returns 5)
    std::uint32_t getType() const;
};

class CSceneNodeAnimatorFlyCircle {
public:
    // 0x6cb730 _ZNK6glitch5scene27CSceneNodeAnimatorFlyCircle7getTypeEv (returns 0)
    std::uint32_t getType() const;
};

class CSceneNodeAnimatorFlyStraight {
public:
    // 0x6cc070 _ZNK6glitch5scene29CSceneNodeAnimatorFlyStraight7getTypeEv (returns 1)
    std::uint32_t getType() const;
};

class CSceneNodeAnimatorFollowSpline {
public:
    // 0x6cc754 _ZNK6glitch5scene30CSceneNodeAnimatorFollowSpline7getTypeEv (returns 2)
    std::uint32_t getType() const;
};

class CSceneNodeAnimatorRotation {
public:
    // 0x6cd58c _ZNK6glitch5scene26CSceneNodeAnimatorRotation7getTypeEv (returns 3)
    std::uint32_t getType() const;
};

class CSceneNodeAnimatorTexture {
public:
    // 0x6cd9d8 _ZNK6glitch5scene25CSceneNodeAnimatorTexture7getTypeEv (returns 4)
    std::uint32_t getType() const;
    // 0x6cda34 _ZN6glitch5scene25CSceneNodeAnimatorTexture10updateTimeEj (empty)
    void updateTime(std::uint32_t time);
};

class CDefaultSceneNodeAnimatorFactory {
public:
    // 0x6b96a8 _ZNK6glitch5scene32CDefaultSceneNodeAnimatorFactory38getCreatableSceneNodeAnimatorTypeCountEv (returns 9)
    std::uint32_t getCreatableSceneNodeAnimatorTypeCount() const;
    // 0x6b96b0 _ZNK6glitch5scene32CDefaultSceneNodeAnimatorFactory34getCreateableSceneNodeAnimatorTypeEj
    // Indices above 8 are clamped to 10, as in the original.
    std::uint32_t getCreateableSceneNodeAnimatorType(std::uint32_t index) const;
};

// ---- Remaining interface destructors (empty D1 bodies). ----

class ISceneCollisionManager {
public:
    // 0x6c3be8 _ZN6glitch5scene22ISceneCollisionManagerD1Ev (empty)
    ~ISceneCollisionManager() = default;
};

class IMeshLoader {
public:
    // 0x6b930c _ZN6glitch5scene11IMeshLoaderD1Ev (empty)
    ~IMeshLoader() = default;
};

class ISceneNodeAnimatorFactory {
public:
    // 0x6b958c _ZN6glitch5scene25ISceneNodeAnimatorFactoryD1Ev (empty)
    ~ISceneNodeAnimatorFactory() = default;
};

class ISceneNodeFactory {
public:
    // 0x6b9cec _ZN6glitch5scene17ISceneNodeFactoryD1Ev (empty)
    ~ISceneNodeFactory() = default;
};

class IMeshCache {
public:
    // 0x6be470 _ZN6glitch5scene10IMeshCacheD1Ev (empty)
    ~IMeshCache() = default;
};

class IBillboard {
public:
    // 0x580a20 _ZN6glitch5scene10IBillboardD1Ev (empty)
    ~IBillboard() = default;
};

class IEndOfBatchCallback {
public:
    // 0x588f80 _ZN6glitch5scene19IEndOfBatchCallbackD1Ev (empty)
    ~IEndOfBatchCallback() = default;
};

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

class IAnimatedMesh {
public:
    // 0x599ef8 _ZN6glitch5scene13IAnimatedMeshD1Ev (empty)
    ~IAnimatedMesh() = default;
    // 0x599efc _ZNK6glitch5scene13IAnimatedMesh11getMeshTypeEv (returns 0)
    std::uint32_t getMeshType() const;
};

class SAnimatedMesh {
public:
    // 0x599f88 _ZNK6glitch5scene13SAnimatedMesh11getMeshTypeEv
    std::uint32_t getMeshType() const;

    std::uint32_t meshType = 0;  // +0x2c
};

}  // namespace glitch::scene
