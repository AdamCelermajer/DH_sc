// Slice 33 reconstruction: CColladaBinaryFileLoader constructors, the collada skin
// technique prepare hooks, SAnimationAccessor::findKeyFrameNo and scene node
// attribute (de)serialization, particle emitter and affector attribute code, the
// scene collision manager, and a few collada and terrain helpers.
//
// Every class here is a stand-in named after the original. The original classes are
// not declared in any header of this category with the members these bodies need, and
// the existing headers must not be edited by this slice, so every body is a free
// function that takes its object by reference. Field offsets are kept as comments.
// IAttributesStandIn method names come from call shape only: the vtable slot numbers
// are noted, but the slot-to-name mapping is not verified against the io interface.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

namespace glitch::collada::recon_33 {

// ---------------------------------------------------------------------------
// Stand-in interfaces
// ---------------------------------------------------------------------------

struct ITextureStandIn;
struct IMeshStandIn;
struct ISceneNodeStandIn;

// glitch::io::IAttributes. Slot numbers are the vtable offsets used by the original
// calls; the names are inferred from the call shape.
class IAttributesStandIn {
public:
    virtual ~IAttributesStandIn() = default;

    virtual void addBool(const char* name, bool value) = 0;                       // +0xd8
    virtual bool getBool(const char* name) = 0;                                   // +0xe4
    virtual void addInt(const char* name, std::int32_t value) = 0;                // +0x4c
    virtual std::int32_t getInt(const char* name) = 0;                            // +0x58
    virtual void addFloat(const char* name, float value) = 0;                     // +0x64
    virtual float getFloat(const char* name) = 0;                                 // +0x70
    virtual void addString(const char* name, const std::string& value) = 0;       // +0x7c
    virtual std::string getString(const char* name) = 0;                          // +0x84
    virtual void addEnum(const char* name, std::int32_t value, const char* const* names) = 0;  // +0xf4
    virtual std::int32_t getEnum(const char* name, const char* const* names) = 0; // +0x100
    virtual void addColor(const char* name, std::uint32_t value) = 0;             // +0x118
    virtual std::uint32_t getColor(const char* name) = 0;                         // +0x124
    virtual void addVector2d(const char* name, const core::position2d<std::int32_t>& value) = 0;  // +0x1d8
    virtual core::position2d<std::int32_t> getVector2d(const char* name) = 0;     // +0x1e4
    virtual void addVector3d(const char* name, const core::vector3d<float>& value) = 0;  // +0x1a8
    virtual core::vector3d<float> getVector3d(const char* name) = 0;              // +0x1b4
    virtual void addRect(const char* name, const core::rect<std::int32_t>& value) = 0;   // +0x1f0
    virtual core::rect<std::int32_t> getRect(const char* name) = 0;               // +0x1fc
    virtual void addQuaternion(const char* name, const float value[4]) = 0;       // +0x220
    virtual void getQuaternion(float out[4], const char* name) = 0;               // +0x22c
    virtual void addTexture(const char* name, ITextureStandIn* value) = 0;        // +0x2b0
    virtual ITextureStandIn* getTexture(const char* name) = 0;                    // +0x2bc

    // Index based access used by the particle deserializers. The first name lookup in
    // the pseudocode passes no index; it is treated here as the current index.
    virtual const char* attributeNameAt(std::int32_t index) = 0;                  // +0x10
    virtual std::int32_t intAt(std::int32_t index) = 0;                           // +0x74 (typed by use)
    virtual float floatAt(std::int32_t index) = 0;                                // +0x74 (typed by use)
    virtual std::uint32_t colorAt(std::int32_t index) = 0;                        // +0x128
    virtual core::vector3d<float> vector3dAt(std::int32_t index) = 0;             // +0x1b8
};

// Minimal stand-in for a reference-counted object (glitch::IReferenceCounted at +0x04).
struct IReferenceCountedStandIn {
    std::int32_t refCount = 1;  // +0x04
};

struct IFileSystemStandIn : IReferenceCountedStandIn {};
struct ITextureStandIn : IReferenceCountedStandIn {};
struct IVideoDriverStandIn : IReferenceCountedStandIn {};

// ---------------------------------------------------------------------------
// glitch::collada::detail skin technique prepare hooks
// ---------------------------------------------------------------------------
// E_PREPARE_BUFFER_STEP value that forwards to the technique hook.
inline constexpr int kPrepareBufferStepUpload = 1;
// Value the hardware technique hooks return for every step.
inline constexpr int kPrepareBufferReturnHandled = 16;

struct SkinBufferStandIn {};
struct MeshBufferStandIn {};

// Virtual hook forwarded from the hardware techniques (vtable +0x18).
class HardwareSkinTechniqueStandIn {
public:
    virtual ~HardwareSkinTechniqueStandIn() = default;
    virtual void prepareSkinBuffer(SkinBufferStandIn& skinBuffer,
                                   MeshBufferStandIn* meshBuffer) = 0;  // +0x18
    // +0x14 attached scene node (not modelled here).
    ISceneNodeStandIn* attachedNode = nullptr;
};

struct SoftwareSkinTechniqueStandIn {
    std::uint8_t pad_0000[0x18] = {};          // +0x00..+0x17 (not mapped)
    bool hasSkinningFlag = false;              // +0x18
    virtual ~SoftwareSkinTechniqueStandIn() = default;
    // Virtual hook called when the buffer must be processed (vtable +0x18).
    virtual void processSkinBuffer(SkinBufferStandIn& skinBuffer,
                                   MeshBufferStandIn* meshBuffer) = 0;
    // Driver-side query used by the software prepare path (not reconstructed here).
    virtual std::uint32_t getProcessBuffer(IVideoDriverStandIn* driver, bool firstStep,
                                           std::uint32_t flags) = 0;
};

// glitch::collada::SAnimationAccessor: canonical class in collada/SAnimationAccessor.hpp.

// ---------------------------------------------------------------------------
// glitch::scene::CAnimatedMeshSceneNode (the mesh file name is resolved by the caller)
// ---------------------------------------------------------------------------
struct AnimatedMeshStandIn {
    std::uint8_t pad_0000[0x140] = {};          // +0x000..+0x13f (not mapped)
    std::int32_t framesPerSecond = 0;           // +0x140 (320 decimal)
    bool looping = false;                       // +0x154 (340 decimal)
};

// ---------------------------------------------------------------------------
// glitch::collada::CCoronasSceneNode
// ---------------------------------------------------------------------------
struct CoronasSceneNodeStandIn {
    std::uint8_t pad_0000[0x148] = {};          // +0x000..+0x147 (not mapped)
    float width = 0.0f;                         // +0x148
    float height = 0.0f;                        // +0x14c
    std::uint32_t vColor = 0;                   // +0x18c
};

// ---------------------------------------------------------------------------
// glitch::scene particle emitters and affectors
// ---------------------------------------------------------------------------
struct ParticleBoxEmitterStandIn {
    std::uint8_t pad_0000[0x10] = {};           // +0x00..+0x0f (not mapped)
    core::vector3d<float> minEdge;              // +0x10 (x, y, z at +0x10, +0x14, +0x18)
    core::vector3d<float> maxEdge;              // +0x1c (x, y, z at +0x1c, +0x20, +0x24)
    core::vector3d<float> direction;            // +0x28
    std::int32_t minParticlesPerSecond = 0;     // +0x34
    std::int32_t maxParticlesPerSecond = 0;     // +0x38
    std::uint32_t minStartColor = 0;            // +0x3c
    std::uint32_t maxStartColor = 0;            // +0x40
    std::int32_t minLifeTime = 0;               // +0x44
    std::int32_t maxLifeTime = 0;               // +0x48
    std::int32_t maxAngleDegrees = 0;           // +0x54
};

struct ParticlePointEmitterStandIn {
    std::uint8_t pad_0000[0x48] = {};           // +0x00..+0x47 (not mapped)
    core::vector3d<float> direction;            // +0x48
    std::int32_t minParticlesPerSecond = 0;     // +0x54
    std::int32_t maxParticlesPerSecond = 0;     // +0x58
    std::uint32_t minStartColor = 0;            // +0x5c
    std::uint32_t maxStartColor = 0;            // +0x60
    std::int32_t minLifeTime = 0;               // +0x64
    std::int32_t maxLifeTime = 0;               // +0x68
    std::int32_t maxAngleDegrees = 0;           // +0x6c
};

struct ParticleFadeOutAffectorStandIn {
    std::uint8_t pad_0000[0x05] = {};           // +0x00..+0x04 (not mapped)
    std::uint32_t targetColor = 0;              // +0x05 (four bytes, read unaligned)
    float fadeOutTime = 0.0f;                   // +0x0c
};

struct ParticleGravityAffectorStandIn {
    std::uint8_t pad_0000[0x08] = {};           // +0x00..+0x07 (not mapped)
    float timeForceLost = 0.0f;                 // +0x08
    core::vector3d<float> gravity;              // +0x0c
};

struct ParticleSizeAffectorStandIn {
    std::uint8_t pad_0000[0x08] = {};           // +0x00..+0x07 (not mapped)
    float targetSize = 0.0f;                    // +0x08
    float variation = 0.0f;                     // +0x0c
    float growForTime = 0.0f;                   // +0x10
    float fadeForTime = 0.0f;                   // +0x14
};

struct ParticleSpinAffectorStandIn {
    std::uint8_t pad_0000[0x08] = {};           // +0x00..+0x07 (not mapped)
    std::uint32_t spinTime = 0;                 // +0x08
    float variation = 0.0f;                     // +0x0c
};

// ---------------------------------------------------------------------------
// Declarations of the free functions in src/recon/slice_33.cpp
// ---------------------------------------------------------------------------

// Skin technique prepare hooks.
int prepareMatrixSkinBuffer(HardwareSkinTechniqueStandIn& self, int step,
                            IVideoDriverStandIn* driver, SkinBufferStandIn& skinBuffer,
                            MeshBufferStandIn* meshBuffer);
int prepareQuatSkinBuffer(HardwareSkinTechniqueStandIn& self, int step,
                          IVideoDriverStandIn* driver, SkinBufferStandIn& skinBuffer,
                          MeshBufferStandIn* meshBuffer);
int prepareTextureSkinBuffer(HardwareSkinTechniqueStandIn& self, int step,
                             IVideoDriverStandIn* driver, SkinBufferStandIn& skinBuffer,
                             MeshBufferStandIn* meshBuffer);
int prepareSoftwareSkinBuffer(SoftwareSkinTechniqueStandIn& self, int step,
                              IVideoDriverStandIn* driver, SkinBufferStandIn& skinBuffer,
                              MeshBufferStandIn* meshBuffer, std::uint32_t technique);


// glitch::scene skybox, terrain, text, shadow volume.
void constructSkyBoxC2(void* self, void* driver);                                                         // 0x6cebc8
void constructSkyBoxC1(void* self, void* driver);                                                         // 0x6cf564
void constructShadowVolumeC1(void* self, void* driver, void* mesh, std::int32_t id, int type, float f);   // 0x6ffb1c
void constructShadowVolumeC2(void* self, void* driver, void* mesh, std::int32_t id, int type, float f);   // 0x6ffc70
void createSilhouette(void* self, const core::vector3d<float>& light, int lightType, void* mesh);         // 0x701358

// glitch::collada coronas and animated mesh.
void serializeCoronas(const CoronasSceneNodeStandIn& self, IAttributesStandIn& attrs);                    // 0x6e5a90
void deserializeCoronas(CoronasSceneNodeStandIn& self, IAttributesStandIn& attrs);                        // 0x6e5b0c
void serializeAnimatedMesh(const AnimatedMeshStandIn& self, const std::string& meshName,
                           IAttributesStandIn& attrs);                                                     // 0x6f6478
void deserializeAnimatedMesh(AnimatedMeshStandIn& self, IAttributesStandIn& attrs);                       // 0x6f6d98

// glitch::scene particle system node, emitters and affectors.
void serializeParticleSystemNode(void* self, IAttributesStandIn& attrs, const std::uint32_t* options);   // 0x6c1554
void deserializeParticleSystemNode(void* self, IAttributesStandIn& attrs);                               // 0x6c1790
void constructIParticleSystemSceneNode(void* self, void* parent, std::int32_t id,
                                       const core::vector3d<float>& rotation);                           // 0x6c23f4
void serializeParticleEmitterBase();                                                                      // 0x6f76d0
std::int32_t deserializeParticleEmitterBase(IAttributesStandIn& attrs, std::int32_t index);              // 0x6f76e4
void serializeParticleAffectorBase();                                                                     // 0x6f837c
std::int32_t deserializeParticleAffectorBase(IAttributesStandIn& attrs, std::int32_t index);             // 0x6f8390
void serializeBoxEmitter(const ParticleBoxEmitterStandIn& self, IAttributesStandIn& attrs);              // 0x6f8d5c
void deserializeBoxEmitter(ParticleBoxEmitterStandIn& self, std::int32_t index, IAttributesStandIn& attrs);  // 0x6f8fd4
void serializePointEmitter(const ParticlePointEmitterStandIn& self, IAttributesStandIn& attrs);          // 0x6fbcd8
void deserializePointEmitter(ParticlePointEmitterStandIn& self, std::int32_t index, IAttributesStandIn& attrs);  // 0x6fbe4c
void serializeFadeOutAffector(const ParticleFadeOutAffectorStandIn& self, IAttributesStandIn& attrs);    // 0x6fa750
std::int32_t deserializeFadeOutAffector(ParticleFadeOutAffectorStandIn& self, std::int32_t index,
                                        IAttributesStandIn& attrs);                                       // 0x6fa804
void serializeGravityAffector(const ParticleGravityAffectorStandIn& self, IAttributesStandIn& attrs);    // 0x6fad3c
std::int32_t deserializeGravityAffector(ParticleGravityAffectorStandIn& self, std::int32_t index,
                                        IAttributesStandIn& attrs);                                       // 0x6fadd8
void serializeSizeAffector(const ParticleSizeAffectorStandIn& self, IAttributesStandIn& attrs);          // 0x6fd400
std::int32_t deserializeSizeAffector(ParticleSizeAffectorStandIn& self, std::int32_t index,
                                     IAttributesStandIn& attrs);                                          // 0x6fd4e4
void serializeSpinAffector(const ParticleSpinAffectorStandIn& self, IAttributesStandIn& attrs);          // 0x6fe498
std::int32_t deserializeSpinAffector(ParticleSpinAffectorStandIn& self, std::int32_t index,
                                     IAttributesStandIn& attrs);                                          // 0x6fe540

}  // namespace glitch::collada::recon_33
