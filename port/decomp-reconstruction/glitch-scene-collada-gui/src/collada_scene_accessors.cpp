#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"

namespace glitch::collada {

// 0x6e5a38 _ZNK6glitch7collada17CCoronasSceneNode14getBoundingBoxEv
const core::aabbox3d<float>& CCoronasSceneNode::getBoundingBox() const {
    return boundingBox;
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

// 0x65cd70 _ZNK6glitch7collada10CSceneNode15getUserPropertyEv
std::int32_t CSceneNode::getUserProperty() const {
    return userProperties ? userProperties->value : 0;
}

// 0x65cd28 _ZNK6glitch7collada10CSceneNode6getUIDEv
const char* CSceneNode::getUID() const {
    return userProperties ? userProperties->uid : "";
}

// 0x65cd4c _ZNK6glitch7collada10CSceneNode10getScopeIDEv
const char* CSceneNode::getScopeID() const {
    return userProperties ? userProperties->scopeID : "";
}

// 0x644404 _ZNK6glitch7collada15CLightSceneNode10getLightIDEv
std::uint32_t CLightSceneNode::getLightID() const {
    return slot160.lightID;
}

// 0x64440c _ZNK6glitch7collada15CLightSceneNode6getUIDEv
std::uint32_t CLightSceneNode::getUID() const {
    return *slot160.uidSource;
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


}  // namespace glitch::collada
