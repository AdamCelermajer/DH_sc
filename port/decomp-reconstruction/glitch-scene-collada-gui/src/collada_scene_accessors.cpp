#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"
#include <utility>

#include "glitch-scene-collada-gui/collada/CCoronasSceneNode.hpp"

namespace glitch::collada {

// CCoronasSceneNode bodies (canonical header collada/CCoronasSceneNode.hpp). They live in this file because the source list
// of the library is generated and a new translation unit would need a CMake change.

// 0x6e5a2c _ZNK6glitch7collada17CCoronasSceneNode7getTypeEv
scene::E_SCENE_NODE_TYPE CCoronasSceneNode::getType() const {
    return static_cast<scene::E_SCENE_NODE_TYPE>(1936618083);
}

// 0x6e5a38 _ZNK6glitch7collada17CCoronasSceneNode14getBoundingBoxEv
const core::aabbox3d<float>& CCoronasSceneNode::getBoundingBox() const {
    return boundingBox;
}

// 0x6e5a40 _ZN6glitch7collada17CCoronasSceneNode7setSizeERKNS_4core11dimension2dIfEE. A zero component becomes 1.
void CCoronasSceneNode::setSize(const core::dimension2d<float>& size) {
    width = size.Width;
    height = size.Height;
    if (width == 0.0f) width = 1.0f;
    if (height == 0.0f) height = 1.0f;
}

// 0x6e5bc8 _ZN6glitch7collada17CCoronasSceneNode9isBlockedEv. PARTIAL: the camera lookup through the scene manager (+0x110) is not
// reconstructed, so the segment is not built and the callback is not called.
std::int32_t CCoronasSceneNode::isBlocked() {
    return 0;
}

// 0x6e5c40 _ZN6glitch7collada17CCoronasSceneNode19onRegisterSceneNodeEv. PARTIAL: the scene manager registration is not
// reconstructed; returns 1 as the original does.
int CCoronasSceneNode::onRegisterSceneNode() {
    return 1;
}

// 0x6e5d14. PARTIAL: declared only; the draw path is not reconstructed and does nothing.
void CCoronasSceneNode::render(void* renderContext) {
    (void)renderContext;
}

// 0x6e703c / 0x6e70e4. PARTIAL: the vertex stream count is not dropped here.
CCoronasSceneNode::~CCoronasSceneNode() {}

// 0x6e7164 / 0x6e7488. PARTIAL: the base is built with an identity transform (the original passes its parent and the position
// through the ISceneNode base argument, not ported); the vertex stream allocation is not reconstructed.
CCoronasSceneNode::CCoronasSceneNode(video::IVideoDriver* driver, int id, const core::vector3d<float>& position,
                                     boost::intrusive_ptr<video::CMaterial> material,
                                     const core::dimension2d<float>& size, video::SColor color)
    : scene::ISceneNode(id, core::vector3d<float>{0.0f, 0.0f, 0.0f}, core::quaternion(), core::vector3d<float>{1.0f, 1.0f, 1.0f}) {
    (void)driver;
    (void)position;
    blockCallback = nullptr;
    setSize(size);
    boundingBox.MinEdge = core::vector3d<float>{-1.0f, -1.0f, -1.0f};
    boundingBox.MaxEdge = core::vector3d<float>{1.0f, 1.0f, 1.0f};
    this->material = std::move(material);
    for (SCoronaCorner& corner : corners) {
        corner.color = color.color;
    }
    unk_208 = 1;
}

// 0x6e7778 / 0x6e7a80. PARTIAL: declared only, no body (the database record and the factory slots are not ported).
CCoronasSceneNode::CCoronasSceneNode(const CColladaDatabase& db, video::IVideoDriver* driver, SCoronas& coronas,
                                     CRootSceneNode* root)
    : scene::ISceneNode(-1, core::vector3d<float>{0.0f, 0.0f, 0.0f}, core::quaternion(), core::vector3d<float>{1.0f, 1.0f, 1.0f}) {
    (void)db;
    (void)driver;
    (void)coronas;
    (void)root;
}

// 0x6e5a90 / 0x6e5b0c. PARTIAL: the attribute slots are not reconstructed; nothing is read or written.
void CCoronasSceneNode::serializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) const {
    (void)attributes;
    (void)options;
}

void CCoronasSceneNode::deserializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) {
    (void)attributes;
    (void)options;
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

}  // namespace animation_track


}  // namespace glitch::collada
