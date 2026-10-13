// Slice 33 reconstruction (see include/glitch-scene-collada-gui/recon/slice_33.hpp).
// Each function keeps the original address and mangled symbol in its comment.
// Bodies marked "not reconstructed" keep the signature and return a neutral value; the
// original body is a larger routine whose parts depend on unmapped classes.

#include "glitch-scene-collada-gui/recon/slice_33.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstring>

namespace glitch::collada::recon_33 {

namespace {

// Attribute constants of the particle affector and emitter index deserializers.
// The original compares the attribute name with the expected key and stops at the
// first mismatch; each helper below does the same and returns the next index.
bool nameIs(IAttributesStandIn& attrs, std::int32_t index, const char* expected) {
    const char* name = attrs.attributeNameAt(index);
    return name != nullptr && std::strcmp(name, expected) == 0;
}

}  // namespace

// ---------------------------------------------------------------------------
// glitch::collada::detail skin techniques
// ---------------------------------------------------------------------------

// 0x66b948 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique27onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
int prepareMatrixSkinBuffer(HardwareSkinTechniqueStandIn& self, int step,
                            IVideoDriverStandIn* driver, SkinBufferStandIn& skinBuffer,
                            MeshBufferStandIn* meshBuffer) {
    (void)driver;
    if (step == kPrepareBufferStepUpload) {
        self.prepareSkinBuffer(skinBuffer, meshBuffer);  // vtable +0x18
    }
    return kPrepareBufferReturnHandled;
}

// 0x66d088 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique27onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
int prepareQuatSkinBuffer(HardwareSkinTechniqueStandIn& self, int step,
                          IVideoDriverStandIn* driver, SkinBufferStandIn& skinBuffer,
                          MeshBufferStandIn* meshBuffer) {
    (void)driver;
    if (step == kPrepareBufferStepUpload) {
        self.prepareSkinBuffer(skinBuffer, meshBuffer);  // vtable +0x18
    }
    return kPrepareBufferReturnHandled;
}

// 0x66e780 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique27onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
int prepareTextureSkinBuffer(HardwareSkinTechniqueStandIn& self, int step,
                             IVideoDriverStandIn* driver, SkinBufferStandIn& skinBuffer,
                             MeshBufferStandIn* meshBuffer) {
    (void)driver;
    if (step == kPrepareBufferStepUpload) {
        self.prepareSkinBuffer(skinBuffer, meshBuffer);  // vtable +0x18
    }
    return kPrepareBufferReturnHandled;
}

// 0x66f7a8 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique27onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
// Partial: the material technique lookup that yields the flag mask is passed in as
// `technique`; the original reads it through the material chain. The skin buffer
// reference count that the original takes and drops around the driver call is not
// modelled.
int prepareSoftwareSkinBuffer(SoftwareSkinTechniqueStandIn& self, int step,
                              IVideoDriverStandIn* driver, SkinBufferStandIn& skinBuffer,
                              MeshBufferStandIn* meshBuffer, std::uint32_t technique) {
    // The original uses 0x60001 when the byte at +0x18 is set and 0x20001 otherwise.
    const std::uint32_t flags = self.hasSkinningFlag ? 393217u : 131073u;
    const std::uint32_t processBuffer =
        self.getProcessBuffer(driver, step == 0, flags & technique);
    if ((processBuffer & 4u) != 0) {
        self.processSkinBuffer(skinBuffer, meshBuffer);  // vtable +0x18
    }
    return static_cast<int>(processBuffer);
}


// ---------------------------------------------------------------------------
// glitch::scene::CSceneCollisionManager
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// glitch::scene animators
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// glitch::scene skybox, terrain, text, shadow volume
// ---------------------------------------------------------------------------

// 0x6cebc8 _ZN6glitch5scene16CSkyBoxSceneNodeC2EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS2_8ITextureEEESA_SA_SA_SA_SA_i
// Not reconstructed (14012 byte body).
void constructSkyBoxC2(void* self, void* driver) {
    (void)self;
    (void)driver;
}

// 0x6cf564 _ZN6glitch5scene16CSkyBoxSceneNodeC1EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS2_8ITextureEEESA_SA_SA_SA_SA_i
// Not reconstructed (14086 byte body).
void constructSkyBoxC1(void* self, void* driver) {
    (void)self;
    (void)driver;
}

// 0x6d042c _ZN6glitch5scene17CTerrainSceneNode22setCurrentLODOfPatchesERKSt6vectorIiNS_4core10SAllocatorIiLNS_6memory13E_MEMORY_HINTE0EEEE
// The patch count is the square of the patch dimension; each patch takes one LOD value.
// The stand-in keeps the patch table as a vector, so the loop is also bounded by its
// size and by the size of the input; the original trusts the table to be large enough.
void setCurrentLODOfPatches(TerrainStandIn& self, const std::vector<std::int32_t>& lods) {
    const std::int64_t dim = self.patchDimension;
    const std::int64_t count = dim * dim;
    if (count <= 0) {
        return;
    }
    const std::size_t n = std::min({static_cast<std::size_t>(count), self.patches.size(), lods.size()});
    for (std::size_t i = 0; i != n; ++i) {
        self.patches[i].lod = lods[i];
    }
}

// 0x6d083c _ZNK6glitch5scene17CTerrainSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Base ISceneNode serialization is omitted (not mapped).
void serializeTerrain(void* self, IAttributesStandIn& attrs, const std::uint32_t* options) {
    (void)self;
    (void)attrs;
    (void)options;
}

// 0x6d5268 _ZN6glitch5scene17CTerrainSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed.
void deserializeTerrain(void* self, IAttributesStandIn& attrs) {
    (void)self;
    (void)attrs;
}

// 0x6d21ac _ZNK6glitch5scene17CTerrainSceneNode22getCurrentLODOfPatchesERSt6vectorIiNS_4core10SAllocatorIiLNS_6memory13E_MEMORY_HINTE0EEEE
// Not reconstructed (2033 byte body).
void getCurrentLODOfPatches(void* self, std::vector<std::int32_t>& lods) {
    (void)self;
    (void)lods;
}

// 0x6d4454 _ZN6glitch5scene17CTerrainSceneNode13loadHeightMapEPNS_2io9IReadFileENS_5video6SColorEi
// Not reconstructed (17835 byte body).
void loadHeightMap(void* self, void* readFile, std::uint32_t colorKey, std::int32_t mode) {
    (void)self;
    (void)readFile;
    (void)colorKey;
    (void)mode;
}

// 0x6ffb1c _ZN6glitch5scene22CShadowVolumeSceneNodeC1EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrIKNS0_5IMeshEEEiNS0_20E_SHADOW_VOLUME_TYPEEf
// Not reconstructed (2001 byte body).
void constructShadowVolumeC1(void* self, void* driver, void* mesh, std::int32_t id, int type, float f) {
    (void)self;
    (void)driver;
    (void)mesh;
    (void)id;
    (void)type;
    (void)f;
}

// 0x6ffc70 _ZN6glitch5scene22CShadowVolumeSceneNodeC2EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrIKNS0_5IMeshEEEiNS0_20E_SHADOW_VOLUME_TYPEEf
// Not reconstructed (1930 byte body).
void constructShadowVolumeC2(void* self, void* driver, void* mesh, std::int32_t id, int type, float f) {
    (void)self;
    (void)driver;
    (void)mesh;
    (void)id;
    (void)type;
    (void)f;
}

// 0x701358 _ZN6glitch5scene22CShadowVolumeSceneNode11SSilhouette16createSilhouetteENS_4core8vector3dIfEENS_5video12E_LIGHT_TYPEERKN5boost13intrusive_ptrIKNS0_11CMeshBufferEEE
// Not reconstructed (8712 byte body).
void createSilhouette(void* self, const core::vector3d<float>& light, int lightType, void* mesh) {
    (void)self;
    (void)light;
    (void)lightType;
    (void)mesh;
}

// ---------------------------------------------------------------------------
// glitch::collada coronas and glitch::scene animated mesh
// ---------------------------------------------------------------------------

// 0x6e5a90 _ZNK6glitch7collada17CCoronasSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeCoronas(const CoronasSceneNodeStandIn& self, IAttributesStandIn& attrs) {
    attrs.addFloat("Width", self.width);     // +0x148
    attrs.addFloat("Height", self.height);   // +0x14c
    attrs.addColor("VColor", self.vColor);   // +0x18c
}

// 0x6e5b0c _ZN6glitch7collada17CCoronasSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// The final virtual call (slot +0xf4 on the node, with the size) is omitted.
void deserializeCoronas(CoronasSceneNodeStandIn& self, IAttributesStandIn& attrs) {
    self.width = attrs.getFloat("Width");
    self.height = attrs.getFloat("Height");
    self.vColor = attrs.getColor("VColor");
}

// 0x6f6478 _ZNK6glitch5scene22CAnimatedMeshSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// The mesh name is passed in by the caller; the original resolves it through the scene
// manager chain. The base ISceneNode serialization is omitted.
void serializeAnimatedMesh(const AnimatedMeshStandIn& self, const std::string& meshName,
                           IAttributesStandIn& attrs) {
    attrs.addString("Mesh", meshName);                     // name from the mesh object
    attrs.addBool("Looping", self.looping);                // +0x154
    attrs.addInt("FramesPerSecond", self.framesPerSecond); // +0x140
}

// 0x6f6d98 _ZN6glitch5scene22CAnimatedMeshSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the mesh lookup and the setMesh virtual call (slot +0x118 on the node) are
// omitted. Looping and FramesPerSecond are read.
void deserializeAnimatedMesh(AnimatedMeshStandIn& self, IAttributesStandIn& attrs) {
    (void)attrs.getString("Mesh");
    self.looping = attrs.getBool("Looping");
    self.framesPerSecond = attrs.getInt("FramesPerSecond");
}

// ---------------------------------------------------------------------------
// glitch::scene particle system node, emitters and affectors
// ---------------------------------------------------------------------------

// 0x6c1554 _ZNK6glitch5scene24CParticleSystemSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed: the emitter and affector type enums depend on the emitter vtable
// and the type name tables (ParticleEmitterTypeNames, ParticleAffectorTypeNames).
void serializeParticleSystemNode(void* self, IAttributesStandIn& attrs, const std::uint32_t* options) {
    (void)self;
    (void)attrs;
    (void)options;
}

// 0x6c1790 _ZN6glitch5scene24CParticleSystemSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed (6187 byte body).
void deserializeParticleSystemNode(void* self, IAttributesStandIn& attrs) {
    (void)self;
    (void)attrs;
}

// 0x6c23f4 _ZN6glitch5scene24IParticleSystemSceneNodeC2EiRKNS_4core8vector3dIfEES6_S6_
// The rotation vector is converted to a quaternion; the base ISceneNode constructor and
// the vtable writes are omitted.
void constructIParticleSystemSceneNode(void* self, void* parent, std::int32_t id,
                                       const core::vector3d<float>& rotation) {
    (void)self;
    (void)parent;
    (void)id;
    (void)rotation;
}

// 0x6f76d0 _ZNK6glitch5scene16IParticleEmitter19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Empty in the original.
void serializeParticleEmitterBase() {}

// 0x6f76e4 _ZN6glitch5scene16IParticleEmitter21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Empty in the original: returns 0 and leaves the index alone.
std::int32_t deserializeParticleEmitterBase(IAttributesStandIn& attrs, std::int32_t index) {
    (void)attrs;
    (void)index;
    return 0;
}

// 0x6f837c _ZNK6glitch5scene17IParticleAffector19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Empty in the original.
void serializeParticleAffectorBase() {}

// 0x6f8390 _ZN6glitch5scene17IParticleAffector21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Empty in the original: returns 0 and leaves the index alone.
std::int32_t deserializeParticleAffectorBase(IAttributesStandIn& attrs, std::int32_t index) {
    (void)attrs;
    (void)index;
    return 0;
}

// 0x6f8d5c _ZNK6glitch5scene19CParticleBoxEmitter19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// The box attribute is the half extent of the box: (max - min) * 0.5 per axis.
void serializeBoxEmitter(const ParticleBoxEmitterStandIn& self, IAttributesStandIn& attrs) {
    const core::vector3d<float> halfExtent{
        (self.maxEdge.X - self.minEdge.X) * 0.5f,
        (self.maxEdge.Y - self.minEdge.Y) * 0.5f,
        (self.maxEdge.Z - self.minEdge.Z) * 0.5f,
    };
    attrs.addVector3d("Box", halfExtent);
    attrs.addVector3d("Direction", self.direction);                              // +0x28
    attrs.addInt("MinParticlesPerSecond", self.minParticlesPerSecond);           // +0x34
    attrs.addInt("MaxParticlesPerSecond", self.maxParticlesPerSecond);           // +0x38
    attrs.addColor("MinStartColor", self.minStartColor);                         // +0x3c
    attrs.addColor("MaxStartColor", self.maxStartColor);                         // +0x40
    attrs.addInt("MinLifeTime", self.minLifeTime);                               // +0x44
    attrs.addInt("MaxLifeTime", self.maxLifeTime);                               // +0x48
    attrs.addInt("MaxAngleDegrees", self.maxAngleDegrees);                       // +0x54
}

// 0x6f8fd4 _ZN6glitch5scene19CParticleBoxEmitter21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed (3463 byte body).
void deserializeBoxEmitter(ParticleBoxEmitterStandIn& self, std::int32_t index, IAttributesStandIn& attrs) {
    (void)self;
    (void)index;
    (void)attrs;
}

// 0x6fbcd8 _ZNK6glitch5scene21CParticlePointEmitter19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializePointEmitter(const ParticlePointEmitterStandIn& self, IAttributesStandIn& attrs) {
    attrs.addVector3d("Direction", self.direction);                          // +0x48
    attrs.addInt("MinParticlesPerSecond", self.minParticlesPerSecond);       // +0x54
    attrs.addInt("MaxParticlesPerSecond", self.maxParticlesPerSecond);       // +0x58
    attrs.addColor("MinStartColor", self.minStartColor);                     // +0x5c
    attrs.addColor("MaxStartColor", self.maxStartColor);                     // +0x60
    attrs.addInt("MinLifeTime", self.minLifeTime);                           // +0x64
    attrs.addInt("MaxLifeTime", self.maxLifeTime);                           // +0x68
    attrs.addInt("MaxAngleDegrees", self.maxAngleDegrees);                   // +0x6c
}

// 0x6fbe4c _ZN6glitch5scene21CParticlePointEmitter21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed (2739 byte body). Recorded: the direction is normalized (a zero
// vector becomes the default), the per second counts are clamped to 1..200, and the
// final call on the attribute object is omitted.
void deserializePointEmitter(ParticlePointEmitterStandIn& self, std::int32_t index, IAttributesStandIn& attrs) {
    (void)self;
    (void)index;
    (void)attrs;
}

// 0x6fa750 _ZNK6glitch5scene24CParticleFadeOutAffector19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeFadeOutAffector(const ParticleFadeOutAffectorStandIn& self, IAttributesStandIn& attrs) {
    attrs.addColor("TargetColor", self.targetColor);   // +0x05
    attrs.addFloat("FadeOutTime", self.fadeOutTime);   // +0x0c
}

// 0x6fa804 _ZN6glitch5scene24CParticleFadeOutAffector21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
std::int32_t deserializeFadeOutAffector(ParticleFadeOutAffectorStandIn& self, std::int32_t index,
                                        IAttributesStandIn& attrs) {
    if (!nameIs(attrs, index, "TargetColor")) {
        return index;
    }
    self.targetColor = attrs.colorAt(index);
    ++index;
    if (!nameIs(attrs, index, "FadeOutTime")) {
        return index;
    }
    self.fadeOutTime = attrs.floatAt(index);
    return index + 1;
}

// 0x6fad3c _ZNK6glitch5scene24CParticleGravityAffector19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeGravityAffector(const ParticleGravityAffectorStandIn& self, IAttributesStandIn& attrs) {
    attrs.addVector3d("Gravity", self.gravity);            // +0x0c
    attrs.addFloat("TimeForceLost", self.timeForceLost);   // +0x08
}

// 0x6fadd8 _ZN6glitch5scene24CParticleGravityAffector21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
std::int32_t deserializeGravityAffector(ParticleGravityAffectorStandIn& self, std::int32_t index,
                                        IAttributesStandIn& attrs) {
    if (!nameIs(attrs, index, "Gravity")) {
        return index;
    }
    self.gravity = attrs.vector3dAt(index);
    ++index;
    if (!nameIs(attrs, index, "TimeForceLost")) {
        return index;
    }
    self.timeForceLost = attrs.floatAt(index);
    return index + 1;
}

// 0x6fd400 _ZNK6glitch5scene21CParticleSizeAffector19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeSizeAffector(const ParticleSizeAffectorStandIn& self, IAttributesStandIn& attrs) {
    attrs.addFloat("TargetSize", self.targetSize);       // +0x08
    attrs.addFloat("Variation", self.variation);         // +0x0c
    attrs.addFloat("GrowForTime", self.growForTime);     // +0x10
    attrs.addFloat("FadeForTime", self.fadeForTime);     // +0x14
}

// 0x6fd4e4 _ZN6glitch5scene21CParticleSizeAffector21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
std::int32_t deserializeSizeAffector(ParticleSizeAffectorStandIn& self, std::int32_t index,
                                     IAttributesStandIn& attrs) {
    if (!nameIs(attrs, index, "TargetSize")) {
        return index;
    }
    self.targetSize = attrs.floatAt(index);
    ++index;
    if (!nameIs(attrs, index, "Variation")) {
        return index;
    }
    self.variation = attrs.floatAt(index);
    ++index;
    if (!nameIs(attrs, index, "GrowForTime")) {
        return index;
    }
    self.growForTime = attrs.floatAt(index);
    ++index;
    if (!nameIs(attrs, index, "FadeForTime")) {
        return index;
    }
    self.fadeForTime = attrs.floatAt(index);
    return index + 1;
}

// 0x6fe498 _ZNK6glitch5scene21CParticleSpinAffector19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeSpinAffector(const ParticleSpinAffectorStandIn& self, IAttributesStandIn& attrs) {
    attrs.addFloat("SpinTime", static_cast<float>(self.spinTime));  // +0x08 (unsigned to float)
    attrs.addFloat("Variation", self.variation);                    // +0x0c
}

// 0x6fe540 _ZN6glitch5scene21CParticleSpinAffector21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
std::int32_t deserializeSpinAffector(ParticleSpinAffectorStandIn& self, std::int32_t index,
                                     IAttributesStandIn& attrs) {
    if (!nameIs(attrs, index, "SpinTime")) {
        return index;
    }
    self.spinTime = static_cast<std::uint32_t>(attrs.floatAt(index));
    ++index;
    if (!nameIs(attrs, index, "Variation")) {
        return index;
    }
    self.variation = attrs.floatAt(index);
    return index + 1;
}

}  // namespace glitch::collada::recon_33
