#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"

namespace glitch::collada {

// 0x62dba0 _ZN6glitch7collada20CDynamicAnimationSet10getChannelEj
SChannelRecord* CDynamicAnimationSet::getChannel(unsigned index) {
    return channels + index;
}

// 0x62dbac _ZNK6glitch7collada20CDynamicAnimationSet10getChannelEj
const SChannelRecord* CDynamicAnimationSet::getChannel(unsigned index) const {
    return channels + index;
}

// 0x6e5a38 _ZNK6glitch7collada17CCoronasSceneNode14getBoundingBoxEv
const core::aabbox3d<float>& CCoronasSceneNode::getBoundingBox() const {
    return boundingBox;
}

// 0x644b80 _ZNK6glitch7collada5CMesh14getBoundingBoxEv
const core::aabbox3d<float>& CMesh::getBoundingBox() const {
    return boundingBox;
}

// 0x64990c _ZNK6glitch7collada13CMorphingMesh5cloneEv
boost::intrusive_ptr<IMesh> CMorphingMesh::clone() const {
    return boost::intrusive_ptr<IMesh>();
}

// 0x64943c _ZNK6glitch7collada28CModularSkinnedMeshSceneNode22getCategoryModuleCountEi
std::uint32_t CModularSkinnedMeshSceneNode::getCategoryModuleCount(int category) const {
    return mesh->getCategoryModuleCount(category);
}

// 0x649444 _ZNK6glitch7collada28CModularSkinnedMeshSceneNode16getCategoryCountEv
std::uint32_t CModularSkinnedMeshSceneNode::getCategoryCount() const {
    return mesh->getCategoryCount();
}

// 0x64945c _ZNK6glitch7collada28CModularSkinnedMeshSceneNode18getCurrentModuleIdEi
std::int32_t CModularSkinnedMeshSceneNode::getCurrentModuleId(int category) const {
    return mesh->getCurrentModuleId(category);
}

// 0x64944c _ZNK6glitch7collada28CModularSkinnedMeshSceneNode13getModuleNameEii
const char* CModularSkinnedMeshSceneNode::getModuleName(int category, int module) const {
    return mesh->getModuleName(category, module);
}

// 0x649454 _ZNK6glitch7collada28CModularSkinnedMeshSceneNode20getCurrentModuleNameEi
const char* CModularSkinnedMeshSceneNode::getCurrentModuleName(int category) const {
    return mesh->getCurrentModuleName(category);
}

// 0x649464 _ZNK6glitch7collada28CModularSkinnedMeshSceneNode11getModuleIdEPKc
std::int32_t CModularSkinnedMeshSceneNode::getModuleId(const char* name) const {
    return mesh->getModuleId(name);
}

// 0x64946c _ZNK6glitch7collada28CModularSkinnedMeshSceneNode15getCategoryNameEi
const char* CModularSkinnedMeshSceneNode::getCategoryName(int category) const {
    return mesh->getCategoryName(category);
}

// 0x6494e0 _ZNK6glitch7collada28CModularSkinnedMeshSceneNode13getCategoryIdEPKc
std::int32_t CModularSkinnedMeshSceneNode::getCategoryId(const char* name) const {
    return mesh->getCategoryId(name);
}

// 0x65f0fc _ZN6glitch7collada21CSceneNodeAnimatorSet18getAnimationLengthEi
// The original tail-calls CAnimationSet::getAnimationLength with the index unchanged.
std::uint32_t CSceneNodeAnimatorSet::getAnimationLength(int index) {
    return ::glitch::collada::getAnimationLength(animationSet, static_cast<unsigned>(index));
}

// 0x65f104 _ZN6glitch7collada21CSceneNodeAnimatorSet17getAnimationStartEi
std::uint32_t CSceneNodeAnimatorSet::getAnimationStart(int index) {
    return ::glitch::collada::getAnimationStart(animationSet, static_cast<unsigned>(index));
}

// 0x65f10c _ZN6glitch7collada21CSceneNodeAnimatorSet15getAnimationEndEi
std::uint32_t CSceneNodeAnimatorSet::getAnimationEnd(int index) {
    return ::glitch::collada::getAnimationEnd(animationSet, static_cast<unsigned>(index));
}

// 0x65f17c _ZN6glitch7collada21CSceneNodeAnimatorSet14getTargetCountEv
std::uint32_t CSceneNodeAnimatorSet::getTargetCount() {
    return animationSet->animatedTrackCount;
}

// 0x65cd70 _ZNK6glitch7collada10CSceneNode15getUserPropertyEv
std::int32_t CSceneNode::getUserProperty() const {
    return userProperties ? userProperties->value : 0;
}

// 0x64bb78 _ZNK6glitch7collada24CParticleSystemSceneNode6getUIDEv
const char* CParticleSystemSceneNode::getUID() const {
    return uidHolder->uid;
}

// 0x64bbc0 _ZNK6glitch7collada24CParticleSystemSceneNode25getTransformedBoundingBoxEv
const core::aabbox3d<float>* CParticleSystemSceneNode::getTransformedBoundingBox() const {
    return transformedBoundingBox;
}

// 0x637b3c _ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode6getUIDEv
const char* CGlitchNewParticleSystemSceneNode::getUID() const {
    return uidHolder->uid;
}

// 0x637b84 _ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode25getTransformedBoundingBoxEv
const core::aabbox3d<float>* CGlitchNewParticleSystemSceneNode::getTransformedBoundingBox() const {
    return transformedBoundingBox;
}

// 0x6e5160 _ZNK6glitch7collada16CCameraSceneNode13getTargetNodeEv
glitch::scene::ISceneNode* CCameraSceneNode::getTargetNode() const {
    return targetNode;
}

// 0x6e5174 _ZNK6glitch7collada16CCameraSceneNode6getUIDEv
const char* CCameraSceneNode::getUID() const {
    return uidHolder->uid;
}

// 0x646148 _ZNK6glitch7collada14CMeshSceneNode6getUIDEv
const char* CMeshSceneNode::getUID() const {
    if (uidHolder == nullptr) {
        return namedMesh->uid;
    }
    return uidHolder->uid;
}

namespace animation_track {

// 0x6e3118 _ZNK6glitch7collada15animation_track8CFloatEx16getIdentityValueEPv
void CFloatEx::getIdentityValue(void* out) const {
    *static_cast<float*>(out) = 0.0f;
}

// 0x6e4dbc _ZNK6glitch7collada15animation_track9CWeightEx16getIdentityValueEPv
void CWeightEx::getIdentityValue(void* out) const {
    *static_cast<float*>(out) = 0.0f;
}

}  // namespace animation_track


// 0x644b88 _ZN6glitch7collada5CMesh14setBoundingBoxERKNS_4core8aabbox3dIfEE
void CMesh::setBoundingBox(const core::aabbox3d<float>& box) {
    boundingBox = box;
}

}  // namespace glitch::collada
