#include "glitch-scene-collada-gui/scene_simple_accessors.hpp"

#include <cstring>

namespace glitch::scene {

namespace {
// Four-character type tags, stored as the little-endian words the original returns.
constexpr std::uint32_t kTagSkyBox = 0x5f796b73;      // "sky_"
constexpr std::uint32_t kTagShadowVolume = 0x77646873;  // "shdw"
constexpr std::uint32_t kTagAnimatedMesh = 0x68736d61;  // "amsh"
constexpr std::uint32_t kTagParticleSystem = 0x6c637470;  // "ptcl"
}  // namespace

// 0x6f76ec _ZNK6glitch5scene16IParticleEmitter7getTypeEv
std::uint32_t IParticleEmitter::getType() const { return 0; }

// 0x6f76f4 _ZNK6glitch5scene37IParticleAnimatedMeshSceneNodeEmitter7getTypeEv
std::uint32_t IParticleAnimatedMeshSceneNodeEmitter::getType() const { return 1; }

// 0x6f8398 _ZNK6glitch5scene27IParticleAttractionAffector7getTypeEv
std::uint32_t IParticleAttractionAffector::getType() const { return 1; }

// 0x6f894c _ZNK6glitch5scene19IParticleBoxEmitter7getTypeEv
std::uint32_t IParticleBoxEmitter::getType() const { return 2; }

// 0x6f9a00 _ZNK6glitch5scene24IParticleCylinderEmitter7getTypeEv
std::uint32_t IParticleCylinderEmitter::getType() const { return 3; }

// 0x6fa47c _ZNK6glitch5scene24IParticleFadeOutAffector7getTypeEv
std::uint32_t IParticleFadeOutAffector::getType() const { return 2; }

// 0x6fa9f4 _ZNK6glitch5scene24IParticleGravityAffector7getTypeEv
std::uint32_t IParticleGravityAffector::getType() const { return 3; }

// 0x6fafcc _ZNK6glitch5scene20IParticleMeshEmitter7getTypeEv
std::uint32_t IParticleMeshEmitter::getType() const { return 4; }

// 0x6fc3c4 _ZNK6glitch5scene20IParticleRingEmitter7getTypeEv
std::uint32_t IParticleRingEmitter::getType() const { return 5; }

// 0x6fcd00 _ZNK6glitch5scene25IParticleRotationAffector7getTypeEv
std::uint32_t IParticleRotationAffector::getType() const { return 4; }

// 0x6fd1e4 _ZNK6glitch5scene21IParticleSizeAffector7getTypeEv
std::uint32_t IParticleSizeAffector::getType() const { return 5; }

// 0x6fd8f4 _ZNK6glitch5scene22IParticleSphereEmitter7getTypeEv
std::uint32_t IParticleSphereEmitter::getType() const { return 6; }

// 0x6fe2ac _ZNK6glitch5scene21IParticleSpinAffector7getTypeEv
std::uint32_t IParticleSpinAffector::getType() const { return 6; }

// 0x6d00b4 _ZNK6glitch5scene17CTerrainSceneNode8getScaleEv
const core::vector3d<float>& CTerrainSceneNode::getScale() const { return scale; }

// 0x6d00bc _ZNK6glitch5scene17CTerrainSceneNode11getRotationEv
const core::quaternion& CTerrainSceneNode::getRotation() const { return rotation; }

// 0x6d00c4 _ZNK6glitch5scene17CTerrainSceneNode11getPositionEv
const core::vector3d<float>& CTerrainSceneNode::getPosition() const { return position; }

// 0x6d00cc _ZNK6glitch5scene17CTerrainSceneNode7getTypeEv
E_SCENE_NODE_TYPE CTerrainSceneNode::getType() const { return ESNT_TERRAIN; }

// 0x6d01e8 _ZNK6glitch5scene17CTerrainSceneNode14getBoundingBoxEv
const core::aabbox3d<float>& CTerrainSceneNode::getBoundingBox() const { return boundingBox; }

// 0x6fe854 _ZNK6glitch5scene22CShadowVolumeSceneNode7getTypeEv
std::uint32_t CShadowVolumeSceneNode::getType() const { return kTagShadowVolume; }

// 0x6fe97c _ZNK6glitch5scene22CShadowVolumeSceneNode14getBoundingBoxEv
const core::aabbox3d<float>& CShadowVolumeSceneNode::getBoundingBox() const { return boundingBox; }

// 0x6fe984 _ZN6glitch5scene22CShadowVolumeSceneNode13SetBiasFactorEf
void CShadowVolumeSceneNode::SetBiasFactor(float factor) { biasFactor = factor; }

// 0x6fe974 _ZN6glitch5scene22CShadowVolumeSceneNode4loadEPNS_2io9IReadFileE
std::uint32_t CShadowVolumeSceneNode::load(io::IReadFile* /*file*/) {
    return 0;
}

// 0x6f5fc4 _ZNK6glitch5scene22CAnimatedMeshSceneNode7getTypeEv
std::uint32_t CAnimatedMeshSceneNode::getType() const { return kTagAnimatedMesh; }

// 0x6f5fd0 _ZNK6glitch5scene22CAnimatedMeshSceneNode10getFrameNrEv
float CAnimatedMeshSceneNode::getFrameNr() const { return frameNr; }

// 0x6f61d0 _ZNK6glitch5scene22CAnimatedMeshSceneNode13getStartFrameEv
std::int32_t CAnimatedMeshSceneNode::getStartFrame() const { return startFrame; }

// 0x6f61d8 _ZNK6glitch5scene22CAnimatedMeshSceneNode11getEndFrameEv
std::int32_t CAnimatedMeshSceneNode::getEndFrame() const { return endFrame; }

// 0x6f63b0 _ZN6glitch5scene22CAnimatedMeshSceneNode11setLoopModeEb
void CAnimatedMeshSceneNode::setLoopMode(bool loop) { loopMode = loop; }

// 0x6f6450 _ZN6glitch5scene22CAnimatedMeshSceneNode21setRenderFromIdentityEb
void CAnimatedMeshSceneNode::setRenderFromIdentity(bool identity) { renderFromIdentity = identity; }

// 0x6f62ec _ZN6glitch5scene22CAnimatedMeshSceneNode17setAnimationSpeedEf
void CAnimatedMeshSceneNode::setAnimationSpeed(float framesPerSecond) {
    animationSpeed = framesPerSecond * 0.001f;
}

// 0x6c0dc4 _ZNK6glitch5scene24CParticleSystemSceneNode7getTypeEv
std::uint32_t CParticleSystemSceneNode::getType() const { return kTagParticleSystem; }

// 0x6c0df8 _ZN6glitch5scene24CParticleSystemSceneNode10getEmitterEv
IParticleEmitter* CParticleSystemSceneNode::getEmitter() { return emitter; }

// 0x6c0e58 _ZNK6glitch5scene24CParticleSystemSceneNode16getMaterialCountEv
std::uint32_t CParticleSystemSceneNode::getMaterialCount() const { return 1; }

// 0x6c0ed8 _ZN6glitch5scene24CParticleSystemSceneNode15setTransparencyEf
void CParticleSystemSceneNode::setTransparency(float alpha) { transparency = alpha; }

// 0x6c0ee0 _ZN6glitch5scene24CParticleSystemSceneNode6renderEPv
void CParticleSystemSceneNode::render(void* /*unused*/) {}

// 0x6c0ee4 _ZNK6glitch5scene24CParticleSystemSceneNode14getBoundingBoxEv
const core::aabbox3d<float>& CParticleSystemSceneNode::getBoundingBox() const { return boundingBox; }

// 0x6c0eec _ZN6glitch5scene24CParticleSystemSceneNode21setParticlesAreGlobalEb
void CParticleSystemSceneNode::setParticlesAreGlobal(bool global) { particlesAreGlobal = global; }

// 0x6c0f08 _ZN6glitch5scene24CParticleSystemSceneNode17reallocateBuffersEv
void CParticleSystemSceneNode::reallocateBuffers() {}


// 0x6c0dd0 _ZNK6glitch5scene24CParticleSystemSceneNode20getNumberOfParticlesEv
std::int32_t CParticleSystemSceneNode::getNumberOfParticles() const {
    return static_cast<std::int32_t>(particles.size());
}

// 0x6c0ef4 _ZN6glitch5scene24CParticleSystemSceneNode15setParticleSizeERKNS_4core11dimension2dIfEE
void CParticleSystemSceneNode::setParticleSize(const core::dimension2d<float>& size) {
    particleSize = size;
}

// 0x6c0f0c _ZN6glitch5scene24CParticleSystemSceneNode16setBillboardTypeENS0_25E_PARTICLE_BILLBOARD_TYPEE
void CParticleSystemSceneNode::setBillboardType(std::uint32_t type) {
    billboardType = type;
}

// 0x6d01c4 _ZN6glitch5scene17CTerrainSceneNode16setRotationPivotERKNS_4core8vector3dIfEE
void CTerrainSceneNode::setRotationPivot(const core::vector3d<float>& pivot) {
    rotationPivotFlag = 0;
    rotationPivot = pivot;
}

// 0x6d0200 _ZNK6glitch5scene17CTerrainSceneNode14getBoundingBoxEii
const core::aabbox3d<float>& CTerrainSceneNode::getBoundingBox(std::int32_t row, std::int32_t column) const {
    return patches[static_cast<std::size_t>(patchesPerSide * row + column)].boundingBox;
}

// 0x6d021c _ZN6glitch5scene17CTerrainSceneNode13setLODOfPatchEiii
void CTerrainSceneNode::setLODOfPatch(std::int32_t row, std::int32_t column, std::int32_t lod) {
    patches[static_cast<std::size_t>(patchesPerSide * row + column)].lod = lod;
}

// 0x6d03f0 _ZN6glitch5scene17CTerrainSceneNode22setCurrentLODOfPatchesEi
// The original loops patchesPerSide squared times; the patch vector holds that many entries.
void CTerrainSceneNode::setCurrentLODOfPatches(std::int32_t lod) {
    for (STerrainPatch& patch : patches) {
        patch.lod = lod;
    }
}

// 0x6f836c _ZN6glitch5scene17IParticleAffector10setEnabledEb
void IParticleAffector::setEnabled(bool value) {
    enabled = value ? 1 : 0;
}

// 0x6f8374 _ZNK6glitch5scene17IParticleAffector10getEnabledEv
bool IParticleAffector::getEnabled() const {
    return enabled != 0;
}

}  // namespace glitch::scene

namespace glitch {

// 0x5a20bc _ZN6glitcheqERKSt4pairIjNS_4core8aabbox3dIfEEES6_
bool operator==(const std::pair<std::uint32_t, core::aabbox3d<float>>& lhs,
                const std::pair<std::uint32_t, core::aabbox3d<float>>& rhs) {
    return lhs.first == rhs.first;
}

}  // namespace glitch
