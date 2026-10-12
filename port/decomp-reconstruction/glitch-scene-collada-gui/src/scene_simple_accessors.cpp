#include "glitch-scene-collada-gui/scene_simple_accessors.hpp"

#include <cstring>

namespace glitch::scene {

namespace {
// Four-character type tags, stored as the little-endian words the original returns.
constexpr std::uint32_t kTagText = 0x74786574;        // "text"
constexpr std::uint32_t kTagDummy = 0x796d6d64;       // "dmmy"
constexpr std::uint32_t kTagSkyBox = 0x5f796b73;      // "sky_"
constexpr std::uint32_t kTagTerrain = 0x72726574;     // "terr"
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

// 0x6d5844 _ZNK6glitch5scene14CTextSceneNode7getTypeEv
// 0x6d586c _ZNK6glitch5scene14CTextSceneNode14getBoundingBoxEv
std::uint32_t CTextSceneNode::getType() const { return kTagText; }
const core::aabbox3d<float>& CTextSceneNode::getBoundingBox() const { return boundingBox; }

// 0x6d5858 _ZNK6glitch5scene23CBillboardTextSceneNode7getTypeEv
std::uint32_t CBillboardTextSceneNode::getType() const { return kTagText; }

// 0x6d58a8 _ZN6glitch5scene23CBillboardTextSceneNode19onRegisterSceneNodeEv
int CBillboardTextSceneNode::onRegisterSceneNode() { return 1; }

// 0x6d58b8 _ZNK6glitch5scene23CBillboardTextSceneNode14getBoundingBoxEv
const core::aabbox3d<float>& CBillboardTextSceneNode::getBoundingBox() const { return boundingBox; }

// 0x6d59c4 _ZNK6glitch5scene23CBillboardTextSceneNode7getSizeEv
const core::dimension2d<float>& CBillboardTextSceneNode::getSize() const { return size; }

// 0x6d589c _ZN6glitch5scene23CBillboardTextSceneNode7setTextEPKw
void CBillboardTextSceneNode::setText(const wchar_t* /*text*/) {}

// 0x6d59fc _ZN6glitch5scene23CBillboardTextSceneNode8setColorERKNS_5video6SColorE
void CBillboardTextSceneNode::setColor(const glitch::video::SColor& /*color*/) {}

// 0x6d5a08 _ZN6glitch5scene23CBillboardTextSceneNode8setColorERKNS_5video6SColorES5_
void CBillboardTextSceneNode::setColor(const glitch::video::SColor& /*top*/,
                                       const glitch::video::SColor& /*bottom*/) {}

// 0x6bb528 _ZN6glitch5scene29CDummyTransformationSceneNode6renderEPv
void CDummyTransformationSceneNode::render(void* /*unused*/) {}

// 0x6bb52c _ZNK6glitch5scene29CDummyTransformationSceneNode7getTypeEv
std::uint32_t CDummyTransformationSceneNode::getType() const { return kTagDummy; }

// 0x6bb538 _ZNK6glitch5scene29CDummyTransformationSceneNode14getBoundingBoxEv
const core::aabbox3d<float>& CDummyTransformationSceneNode::getBoundingBox() const { return boundingBox; }

// 0x6ce3bc _ZNK6glitch5scene16CSkyBoxSceneNode7getTypeEv
std::uint32_t CSkyBoxSceneNode::getType() const { return kTagSkyBox; }

// 0x6ce3c8 _ZNK6glitch5scene16CSkyBoxSceneNode14getBoundingBoxEv
const core::aabbox3d<float>& CSkyBoxSceneNode::getBoundingBox() const { return boundingBox; }

// 0x6ce3f0 _ZNK6glitch5scene16CSkyBoxSceneNode16getMaterialCountEv
std::uint32_t CSkyBoxSceneNode::getMaterialCount() const { return 6; }

// 0x6d00b4 _ZNK6glitch5scene17CTerrainSceneNode8getScaleEv
const core::vector3d<float>& CTerrainSceneNode::getScale() const { return scale; }

// 0x6d00bc _ZNK6glitch5scene17CTerrainSceneNode11getRotationEv
const core::vector3d<float>& CTerrainSceneNode::getRotation() const { return rotation; }

// 0x6d00c4 _ZNK6glitch5scene17CTerrainSceneNode11getPositionEv
const core::vector3d<float>& CTerrainSceneNode::getPosition() const { return position; }

// 0x6d00cc _ZNK6glitch5scene17CTerrainSceneNode7getTypeEv
std::uint32_t CTerrainSceneNode::getType() const { return kTagTerrain; }

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

// 0x6bba70 _ZN6glitch5scene5CMeshC2Ev
// 0x6bbad4 _ZN6glitch5scene5CMeshC1Ev
CMesh::CMesh() {
    boundingBox.MinEdge = {-1.0f, -1.0f, -1.0f};
    boundingBox.MaxEdge = {1.0f, 1.0f, 1.0f};
}

// 0x6bbc68 _ZNK6glitch5scene5CMesh14getBoundingBoxEv
const core::aabbox3d<float>& CMesh::getBoundingBox() const { return boundingBox; }

// 0x6bbc70 _ZN6glitch5scene5CMesh14setBoundingBoxERKNS_4core8aabbox3dIfEE
void CMesh::setBoundingBox(const core::aabbox3d<float>& box) { boundingBox = box; }

// 0x6bbedc _ZN6glitch5scene5CMesh5clearEv
void CMesh::clear() {
    buffers.clear();
}

// 0x6c77e4 _ZNK6glitch5scene27CSceneNodeAnimatorCameraFPS22isEventReceiverEnabledEv
bool CSceneNodeAnimatorCameraFPS::isEventReceiverEnabled() const { return true; }

// 0x6c77ec _ZNK6glitch5scene27CSceneNodeAnimatorCameraFPS7getTypeEv
std::uint32_t CSceneNodeAnimatorCameraFPS::getType() const { return 7; }

// 0x6c7948 _ZN6glitch5scene27CSceneNodeAnimatorCameraFPS14setRotateSpeedEf
void CSceneNodeAnimatorCameraFPS::setRotateSpeed(float speed) { rotateSpeed = speed; }

// 0x6c7950 _ZN6glitch5scene27CSceneNodeAnimatorCameraFPS12setMoveSpeedEf
void CSceneNodeAnimatorCameraFPS::setMoveSpeed(float speed) { moveSpeed = speed; }

// 0x6c7958 _ZNK6glitch5scene27CSceneNodeAnimatorCameraFPS14getRotateSpeedEv
float CSceneNodeAnimatorCameraFPS::getRotateSpeed() const { return rotateSpeed; }

// 0x6c7960 _ZNK6glitch5scene27CSceneNodeAnimatorCameraFPS12getMoveSpeedEv
float CSceneNodeAnimatorCameraFPS::getMoveSpeed() const { return moveSpeed; }

// 0x6c94d8 _ZNK6glitch5scene28CSceneNodeAnimatorCameraMaya22isEventReceiverEnabledEv
bool CSceneNodeAnimatorCameraMaya::isEventReceiverEnabled() const { return true; }

// 0x6c94e0 _ZNK6glitch5scene28CSceneNodeAnimatorCameraMaya7getTypeEv
std::uint32_t CSceneNodeAnimatorCameraMaya::getType() const { return 8; }

// 0x6c95ec _ZN6glitch5scene28CSceneNodeAnimatorCameraMaya14setRotateSpeedEf
void CSceneNodeAnimatorCameraMaya::setRotateSpeed(float speed) { rotateSpeed = speed; }

// 0x6c95f4 _ZN6glitch5scene28CSceneNodeAnimatorCameraMaya12setMoveSpeedEf
void CSceneNodeAnimatorCameraMaya::setMoveSpeed(float speed) { moveSpeed = speed; }

// 0x6c95fc _ZN6glitch5scene28CSceneNodeAnimatorCameraMaya12setZoomSpeedEf
void CSceneNodeAnimatorCameraMaya::setZoomSpeed(float speed) { zoomSpeed = speed; }

// 0x6c9604 _ZNK6glitch5scene28CSceneNodeAnimatorCameraMaya14getRotateSpeedEv
float CSceneNodeAnimatorCameraMaya::getRotateSpeed() const { return rotateSpeed; }

// 0x6c960c _ZNK6glitch5scene28CSceneNodeAnimatorCameraMaya12getMoveSpeedEv
float CSceneNodeAnimatorCameraMaya::getMoveSpeed() const { return moveSpeed; }

// 0x6c9614 _ZNK6glitch5scene28CSceneNodeAnimatorCameraMaya12getZoomSpeedEv
float CSceneNodeAnimatorCameraMaya::getZoomSpeed() const { return zoomSpeed; }

// 0x6ca694 _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse7getTypeEv
std::uint32_t CSceneNodeAnimatorCollisionResponse::getType() const { return 6; }

// 0x6ca69c _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse9isFallingEv
bool CSceneNodeAnimatorCollisionResponse::isFalling() const { return falling; }

// 0x6ca74c _ZN6glitch5scene35CSceneNodeAnimatorCollisionResponse15setSlidingSpeedEf
void CSceneNodeAnimatorCollisionResponse::setSlidingSpeed(float speed) { slidingSpeed = speed; }

// 0x6ca754 _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse15getSlidingSpeedEv
float CSceneNodeAnimatorCollisionResponse::getSlidingSpeed() const { return slidingSpeed; }

// 0x6ca75c _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse8getWorldEv
ITriangleSelector* CSceneNodeAnimatorCollisionResponse::getWorld() const { return world; }

// 0x6cb418 _ZNK6glitch5scene24CSceneNodeAnimatorDelete7getTypeEv
std::uint32_t CSceneNodeAnimatorDelete::getType() const { return 5; }

// 0x6cb730 _ZNK6glitch5scene27CSceneNodeAnimatorFlyCircle7getTypeEv
std::uint32_t CSceneNodeAnimatorFlyCircle::getType() const { return 0; }

// 0x6cc070 _ZNK6glitch5scene29CSceneNodeAnimatorFlyStraight7getTypeEv
std::uint32_t CSceneNodeAnimatorFlyStraight::getType() const { return 1; }

// 0x6cc330 _ZN6glitch5scene29CSceneNodeAnimatorFlyStraight25recalculateImidiateValuesEv
float CSceneNodeAnimatorFlyStraight::recalculateImidiateValues() const {
    return minuend - subtrahend;
}

// 0x6cc754 _ZNK6glitch5scene30CSceneNodeAnimatorFollowSpline7getTypeEv
std::uint32_t CSceneNodeAnimatorFollowSpline::getType() const { return 2; }

// 0x6cd58c _ZNK6glitch5scene26CSceneNodeAnimatorRotation7getTypeEv
std::uint32_t CSceneNodeAnimatorRotation::getType() const { return 3; }

// 0x6cd9d8 _ZNK6glitch5scene25CSceneNodeAnimatorTexture7getTypeEv
std::uint32_t CSceneNodeAnimatorTexture::getType() const { return 4; }

// 0x6cda34 _ZN6glitch5scene25CSceneNodeAnimatorTexture10updateTimeEj
void CSceneNodeAnimatorTexture::updateTime(std::uint32_t /*time*/) {}

// 0x6b96a8 _ZNK6glitch5scene32CDefaultSceneNodeAnimatorFactory38getCreatableSceneNodeAnimatorTypeCountEv
std::uint32_t CDefaultSceneNodeAnimatorFactory::getCreatableSceneNodeAnimatorTypeCount() const { return 9; }

// 0x6b96b0 _ZNK6glitch5scene32CDefaultSceneNodeAnimatorFactory34getCreateableSceneNodeAnimatorTypeEj
std::uint32_t CDefaultSceneNodeAnimatorFactory::getCreateableSceneNodeAnimatorType(std::uint32_t index) const {
    return index > 8 ? 10 : index;
}

// Name table of glitch::scene::SceneNodeAnimatorTypeNames (0x957ba4 in the binary, nine
// words). Entry 4 ("texture") is a tail-merged literal inferred from the suffix of
// "GL_EXT_paletted_texture" in the binary's string data.
static const char* const kSceneNodeAnimatorTypeNames[9] = {
    "flyCircle", "flyStraight", "followSpline", "rotation", "texture",
    "deletion", "collisionResponse", "cameraFPS", "cameraMaya",
};

// 0x6b96c0 _ZNK6glitch5scene32CDefaultSceneNodeAnimatorFactory38getCreateableSceneNodeAnimatorTypeNameEj
const char* CDefaultSceneNodeAnimatorFactory::getCreateableSceneNodeAnimatorTypeName(std::uint32_t index) const {
    return index < 9 ? kSceneNodeAnimatorTypeNames[index] : nullptr;
}

// 0x6b96e0 _ZNK6glitch5scene32CDefaultSceneNodeAnimatorFactory38getCreateableSceneNodeAnimatorTypeNameENS0_26E_SCENE_NODE_ANIMATOR_TYPEE
const char* CDefaultSceneNodeAnimatorFactory::getCreateableSceneNodeAnimatorTypeName(E_SCENE_NODE_ANIMATOR_TYPE type) const {
    return static_cast<std::uint32_t>(type) < 9 ? kSceneNodeAnimatorTypeNames[type] : nullptr;
}

// 0x6b9cf0 _ZNK6glitch5scene24CDefaultSceneNodeFactory30getCreatableSceneNodeTypeCountEv
std::int32_t CDefaultSceneNodeFactory::getCreatableSceneNodeTypeCount() const {
    return static_cast<std::int32_t>(types.size());
}

// 0x6b9d1c _ZNK6glitch5scene24CDefaultSceneNodeFactory26getCreateableSceneNodeTypeEj
std::uint32_t CDefaultSceneNodeFactory::getCreateableSceneNodeType(std::uint32_t index) const {
    return index < types.size() ? types[index].type : 0x6e6b6e75u;  // 'unkn'
}

// 0x6b9d60 _ZNK6glitch5scene24CDefaultSceneNodeFactory30getCreateableSceneNodeTypeNameEj
const char* CDefaultSceneNodeFactory::getCreateableSceneNodeTypeName(std::uint32_t index) const {
    return index < types.size() ? types[index].name : nullptr;
}

// 0x599efc _ZNK6glitch5scene13IAnimatedMesh11getMeshTypeEv
std::uint32_t IAnimatedMesh::getMeshType() const { return 0; }

// 0x599f88 _ZNK6glitch5scene13SAnimatedMesh11getMeshTypeEv
std::uint32_t SAnimatedMesh::getMeshType() const { return meshType; }


// 0x6d5874 _ZN6glitch5scene14CTextSceneNode12setTextColorENS_5video6SColorE
void CTextSceneNode::setTextColor(video::SColor color) { textColor = color; }

// 0x6d59cc _ZN6glitch5scene23CBillboardTextSceneNode12setTextColorENS_5video6SColorE
void CBillboardTextSceneNode::setTextColor(video::SColor color) { textColor = color; }

// 0x6c7928 _ZN6glitch5scene27CSceneNodeAnimatorCameraFPS9allKeysUpEv
void CSceneNodeAnimatorCameraFPS::allKeysUp() {
    for (bool& key : keyDown) {
        key = false;
    }
}

// 0x6c7968 _ZN6glitch5scene27CSceneNodeAnimatorCameraFPS19setVerticalMovementEb
void CSceneNodeAnimatorCameraFPS::setVerticalMovement(bool allowed) {
    noVerticalMovement = !allowed;
}

// 0x6c95cc _ZN6glitch5scene28CSceneNodeAnimatorCameraMaya14isMouseKeyDownEi
bool CSceneNodeAnimatorCameraMaya::isMouseKeyDown(int key) const {
    return mouseKeyDown[key];
}

// 0x6c97f4 _ZN6glitch5scene28CSceneNodeAnimatorCameraMaya20updateAnimationStateEv
float CSceneNodeAnimatorCameraMaya::updateAnimationState() const {
    return stateA - stateB;
}

// 0x6c95d8 _ZN6glitch5scene28CSceneNodeAnimatorCameraMaya9allKeysUpEv
void CSceneNodeAnimatorCameraMaya::allKeysUp() {
    for (bool& key : mouseKeyDown) {
        key = false;
    }
}

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

// 0x6ca6a4 _ZN6glitch5scene35CSceneNodeAnimatorCollisionResponse18setEllipsoidRadiusERKNS_4core8vector3dIfEE
void CSceneNodeAnimatorCollisionResponse::setEllipsoidRadius(const core::vector3d<float>& radius) {
    ellipsoidRadius = radius;
}

// 0x6ca6c0 _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse18getEllipsoidRadiusEv
core::vector3d<float> CSceneNodeAnimatorCollisionResponse::getEllipsoidRadius() const {
    return ellipsoidRadius;
}

// 0x6ca6dc _ZN6glitch5scene35CSceneNodeAnimatorCollisionResponse10setGravityERKNS_4core8vector3dIfEE
void CSceneNodeAnimatorCollisionResponse::setGravity(const core::vector3d<float>& value) {
    gravity = value;
}

// 0x6ca6f8 _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse10getGravityEv
core::vector3d<float> CSceneNodeAnimatorCollisionResponse::getGravity() const {
    return gravity;
}

// 0x6ca714 _ZN6glitch5scene35CSceneNodeAnimatorCollisionResponse23setEllipsoidTranslationERKNS_4core8vector3dIfEE
void CSceneNodeAnimatorCollisionResponse::setEllipsoidTranslation(const core::vector3d<float>& translation) {
    ellipsoidTranslation = translation;
}

// 0x6ca730 _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse23getEllipsoidTranslationEv
core::vector3d<float> CSceneNodeAnimatorCollisionResponse::getEllipsoidTranslation() const {
    return ellipsoidTranslation;
}

// 0x6f836c _ZN6glitch5scene17IParticleAffector10setEnabledEb
void IParticleAffector::setEnabled(bool value) {
    enabled = value ? 1 : 0;
}

// 0x6f8374 _ZNK6glitch5scene17IParticleAffector10getEnabledEv
bool IParticleAffector::getEnabled() const {
    return enabled != 0;
}

// 0x599f04 _ZNK6glitch5scene13SAnimatedMesh13getFrameCountEv
std::int32_t SAnimatedMesh::getFrameCount() const {
    return static_cast<std::int32_t>(meshes.size());
}

// 0x599f4c _ZNK6glitch5scene13SAnimatedMesh14getBoundingBoxEv
const core::aabbox3d<float>& SAnimatedMesh::getBoundingBox() const {
    return boundingBox;
}

// 0x599f54 _ZN6glitch5scene13SAnimatedMesh14setBoundingBoxERKNS_4core8aabbox3dIfEE
void SAnimatedMesh::setBoundingBox(const core::aabbox3d<float>& box) {
    boundingBox = box;
}

// 0x6be504 _ZNK6glitch5scene10CMeshCache12getMeshCountEv
std::int32_t CMeshCache::getMeshCount() const {
    return static_cast<std::int32_t>(entries.size());
}

// 0x6be60c _ZNK6glitch5scene10CMeshCache15getMeshFilenameEj
const char* CMeshCache::getMeshFilename(std::uint32_t index) const {
    return index < entries.size() ? entries[index].filename : nullptr;
}

// 0x6be64c _ZNK6glitch5scene10CMeshCache15getMeshFilenameERKN5boost13intrusive_ptrIKNS0_13IAnimatedMeshEEE
const char* CMeshCache::getMeshFilename(const boost::intrusive_ptr<const IAnimatedMesh>& mesh) const {
    for (const SMeshEntry& entry : entries) {
        if (entry.mesh == mesh.get()) {
            return entry.filename;
        }
    }
    return nullptr;
}
// 0x6be530 _ZNK6glitch5scene10CMeshCache12getMeshIndexERKN5boost13intrusive_ptrIKNS0_13IAnimatedMeshEEE
std::int32_t CMeshCache::getMeshIndex(const boost::intrusive_ptr<const IAnimatedMesh>& mesh) const {
    for (std::size_t i = 0; i < entries.size(); ++i) {
        if (entries[i].mesh == mesh.get()) {
            return static_cast<std::int32_t>(i);
        }
    }
    return -1;
}

// 0x6bea64 _ZN6glitch5scene10CMeshCache5clearEv
void CMeshCache::clear() {
    entries.clear();
}

// 0x6be474 _ZN6glitch5scene10CMeshCacheC2EPNS_5video12IVideoDriverE
// 0x6be4bc _ZN6glitch5scene10CMeshCacheC1EPNS_5video12IVideoDriverE
CMeshCache::CMeshCache(video::IVideoDriver* driver) : videoDriver(driver) {}

// 0x6b8878 _ZN6glitch5scene17CAppendMeshBuffer5clearEv
void CAppendMeshBuffer::clear() {
    fillCounterB = 0;
    fillCounterA = 0;
}

// 0x6b9538 _ZNK6glitch5scene24CColladaBinaryFileLoader24isALoadableFileExtensionEPKc
bool CColladaBinaryFileLoader::isALoadableFileExtension(const char* filename) const {
    return std::strstr(filename, ".bdae") != nullptr;
}

}  // namespace glitch::scene

namespace glitch {

// 0x5a20bc _ZN6glitcheqERKSt4pairIjNS_4core8aabbox3dIfEEES6_
bool operator==(const std::pair<std::uint32_t, core::aabbox3d<float>>& lhs,
                const std::pair<std::uint32_t, core::aabbox3d<float>>& rhs) {
    return lhs.first == rhs.first;
}

}  // namespace glitch
