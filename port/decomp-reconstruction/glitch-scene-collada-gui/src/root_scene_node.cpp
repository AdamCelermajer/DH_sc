#include "glitch-scene-collada-gui/root_scene_node.hpp"

namespace glitch::collada {

// 0x65adc4 _ZNK6glitch7collada14CRootSceneNode6getUIDEv
std::uint32_t CRootSceneNode::getUID() const {
    return uidOwner ? uidOwner->uid : 0u;
}

// 0x65add4 _ZN6glitch7collada14CRootSceneNode14removeMaterialERKN5boost13intrusive_ptrINS_5video9CMaterialEEE
// Original body is empty.
void CRootSceneNode::removeMaterial(const void* /*material*/) {}

// 0x65ae38 _ZNK6glitch7collada14CRootSceneNode16getMaterialCountEv
std::uint32_t CRootSceneNode::getMaterialCount() const {
    return materials.count();
}

// 0x65ae64 _ZNK6glitch7collada14CRootSceneNode7getTypeEv
// Returns the four-character tag 'read' (0x72656164 in the original).
std::uint32_t CRootSceneNode::getType() const {
    return 0x72656164u;
}

// 0x65b210 _ZN6glitch7collada14CRootSceneNode17addParticleSystemEPNS0_24IParticleSystemSceneNodeE
void CRootSceneNode::addParticleSystem(void* particleSystem) {
    particleSystems.pushBack(particleSystem);
}

// 0x65b244 _ZN6glitch7collada14CRootSceneNode8addLightEPNS0_15CLightSceneNodeE
void CRootSceneNode::addLight(void* light) {
    lights.pushBack(light);
}

// 0x65b278 _ZN6glitch7collada14CRootSceneNode15addMorphingMeshEPNS0_13CMorphingMeshE
void CRootSceneNode::addMorphingMesh(void* morphingMesh) {
    morphingMeshes.pushBack(morphingMesh);
}

// 0x65b2ac _ZN6glitch7collada14CRootSceneNode9addCameraEPNS0_16CCameraSceneNodeE
void CRootSceneNode::addCamera(void* camera) {
    cameras.pushBack(camera);
}

}  // namespace glitch::collada
