#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"
#include <utility>

#include "glitch-core-io/io/attributes.hpp"
#include "glitch-scene-collada-gui/collada/CCoronasSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/CSceneManager.hpp"
#include "glitch-video/vertex_streams.hpp"

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

// 0x6e5bc8 _ZN6glitch7collada17CCoronasSceneNode9isBlockedEv
// The original returns 0 when the blocking callback (+0x130) is null. Otherwise it asks the active camera (+0x110 scene
// manager, +0xe4) for its absolute position and this node for its own, packs the six floats into {camera, this} - the
// listing writes v5 (the camera's) last - and returns whatever the callback returns.
// One host deviation, named: the camera is tested for null. The original loads it and calls getAbsolutePosition on it
// unconditionally, which crashes when no camera is registered; the port cannot reproduce a crash.
std::int32_t CCoronasSceneNode::isBlocked() {
    if (blockCallback == nullptr) {
        return 0;
    }
    scene::ISceneNode* camera = sceneManager != nullptr ? static_cast<scene::ISceneNode*>(sceneManager->activeCamera) : nullptr;
    if (camera == nullptr) {
        return 0;
    }
    const core::vector3d<float> cameraPosition = camera->getAbsolutePosition();
    const core::vector3d<float> nodePosition = getAbsolutePosition();
    const float segment[6] = {nodePosition.X, nodePosition.Y, nodePosition.Z,
                              cameraPosition.X, cameraPosition.Y, cameraPosition.Z};
    return blockCallback(segment);
}

// 0x6e5c40 _ZN6glitch7collada17CCoronasSceneNode19onRegisterSceneNodeEv
// When isBlocked() is 0 (true is 1 and does not compare, as written), the node is registered with the scene manager:
// registerNodeForRendering(this, the material at +0x168, null user data, pass 3 = ESNRP_AUTOMATIC, no position override,
// sortOrder 0x7fffffff). Always returns 1.
int CCoronasSceneNode::onRegisterSceneNode() {
    if (!isBlocked()) {
        sceneManager->registerNodeForRendering(this, material, nullptr, scene::ESNRP_AUTOMATIC, nullptr, 0x7FFFFFFF);
    }
    return 1;
}

// 0x6e5d14. PARTIAL: declared only; the draw path is not reconstructed and does nothing.
void CCoronasSceneNode::render(void* renderContext) {
    (void)renderContext;
}

// 0x6e703c (D1) / 0x6e70e4 (D2). Verified against the decompilation: the two vtable words (the primary one and the
// secondary at +0x204), then the reference count of the vertex-stream table at +0x1fc is decremented and the table is
// deleted when it was the last use, then the material handle and the scene-node base.
// The listing's decrement is `if ((*(_DWORD*)v1)-- == 1) { ~CVertexStreams(v1); LC_API_FREE_0(v1); }`. The port's
// CVertexStreams keeps that count in m_refCount (+0x00) and glitch-video/vertex_streams.hpp exposes it through the
// intrusive_ptr hooks, which run exactly that body (delete, after the destructor), so the drop is written as
// intrusive_ptr_release. The material handle and the base are destroyed by the members here.
// The null test is the port's: the driver constructor never allocates the streams yet (see its PARTIAL note), and the
// original's own object always had one.
CCoronasSceneNode::~CCoronasSceneNode() {
    if (vertexStreams != nullptr) {
        video::intrusive_ptr_release(vertexStreams);
        vertexStreams = nullptr;
    }
}

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

// 0x6e5a90 _ZNK6glitch7collada17CCoronasSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Three stores (vtable slots measured off the listing; the port's IAttributes declares the same slots):
// addFloat("Width", width, false) at +0x64, addFloat("Height", height, false) at +0x64, addColor("VColor",
// the vertex colour, false) at +0x118. The float is passed as the raw bit pattern of +0x148 / +0x14c, and the colour is
// the word at +0x18c, i.e. corners[0].color.
void CCoronasSceneNode::serializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) const {
    (void)options;
    attributes->addFloat("Width", width, false);
    attributes->addFloat("Height", height, false);
    attributes->addColor("VColor", video::SColor{corners[0].color}, false);
}

// 0x6e5b0c _ZN6glitch7collada17CCoronasSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// getFloat("Width") into +0x148 and getFloat("Height") into +0x14c (slot +0x70 each), then getColor("VColor") at +0x124
// whose four bytes are stored separately at +0x18c..+0x18f (byte 0 to +0x18c, byte 3 to +0x18f, i.e. the low byte of the
// packed colour lands at +0x18c, which is corners[0].color). The listing then ends with the node's own vtable slot +0xf4
// and returns its register. That slot is NOT part of this class: CCoronasSceneNode derives from scene::ISceneNode and
// declares no computeBoundingBox (the only CCoronas functions in the database are 0x6e5a2c .. 0x6e7a80, listed above),
// and in the port's ISceneNode K 0xf4 is onChangedSceneManager - another ISceneNode method the port does not declare.
// The call is therefore dropped rather than replaced by a wrong method; it is the one deviation on this row.
void CCoronasSceneNode::deserializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) {
    (void)options;
    width = attributes->getFloat("Width");
    height = attributes->getFloat("Height");
    corners[0].color = attributes->getColor("VColor").color;
}

// 0x64bb78 _ZNK6glitch7collada24CParticleSystemSceneNode6getUIDEv
// LDR R3,[R0,#0x18C]; LDR R0,[R3]: the uid holder pointer, then its first word (the string).
const char* CParticleSystemSceneNode::getUID() const {
    return uidHolder->uid;
}

// 0x64bbc0 _ZNK6glitch7collada24CParticleSystemSceneNode25getTransformedBoundingBoxEv
// LDR R0,[R0,#0x14C]: one load of the member.
const core::aabbox3d<float>* CParticleSystemSceneNode::getTransformedBoundingBox() const {
    return transformedBoundingBox;
}

// 0x637b3c _ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode6getUIDEv
// LDR R3,[R0,#0x17C]; LDR R0,[R3]. Note the +0x17C, not the sibling's +0x18C.
const char* CGlitchNewParticleSystemSceneNode::getUID() const {
    return uidHolder->uid;
}

// 0x637b84 _ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode25getTransformedBoundingBoxEv
// LDR R0,[R0,#0x14C]: one load of the member.
const core::aabbox3d<float>* CGlitchNewParticleSystemSceneNode::getTransformedBoundingBox() const {
    return transformedBoundingBox;
}

namespace animation_track {

}  // namespace animation_track


}  // namespace glitch::collada
