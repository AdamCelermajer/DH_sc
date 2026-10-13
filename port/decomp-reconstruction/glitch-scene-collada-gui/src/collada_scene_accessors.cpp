#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"

namespace glitch::collada {

// 0x6e5a38 _ZNK6glitch7collada17CCoronasSceneNode14getBoundingBoxEv
const core::aabbox3d<float>& CCoronasSceneNode::getBoundingBox() const {
    return boundingBox;
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
