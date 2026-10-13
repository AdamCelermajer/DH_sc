#include "glitch-scene-collada-gui/scene_node.hpp"

namespace glitch::scene {

// 0x5990c0 _ZN6glitch5scene10ISceneNodeC2EiRKNS_4core8vector3dIfEERKNS2_10quaternionES6_ (base object)
// 0x599268 _ZN6glitch5scene10ISceneNodeC1EiRKNS_4core8vector3dIfEERKNS2_10quaternionES6_ (complete object)
// PARTIAL. One C++ constructor covers both variants (they differ only in the virtual-base and vtable words). The
// mapped members are written as the pseudocode writes them: absolute and relative transforms start as
// identities (+0x24 and +0x68, identity flags set), then the position (+0xac), rotation (+0xb8) and scale (+0xc8)
// arguments, the transformed box (-1, -1, -1)..(1, 1, 1) (+0xd4), the parent (+0xec), the scene manager (+0x110),
// the id (+0x10c), the triangle selector (+0x114), the culling word (+0x118), the flag word 0x60f (+0x11c), the
// game data (+0x124), the camera offset (+0x128) and the rendering layer (+0x12c). The constructor ends with
// the base updateAbsolutePosition(false), which copies the relative matrix into the absolute one.
// Not modelled: the IObject base (words +0x04 and +0x08), the name string (inline buffer at +0x0c, begin and end
// pointers at +0x1c and +0x20), the three intrusive list sentinels at +0xf4, +0xfc and +0x104 (the vectors
// of this header stand in for them), the two flag bytes at +0x120 and +0x121 (both set to 1), and the vtable words
// and the IReferenceCounted virtual base (vptr at +0x130, reference count 1 at +0x134 in the complete object).
ISceneNode::ISceneNode(int nodeId, const core::vector3d<float>& nodePosition, const core::quaternion& nodeRotation,
                       const core::vector3d<float>& nodeScale)
    : position(nodePosition), rotation(nodeRotation), scale(nodeScale) {
    // +0x24..+0x63 identity (the flag byte +0x64 is not mapped for the absolute matrix)
    for (int i = 0; i < 16; ++i) {
        absoluteTransform[i] = 0.0f;
    }
    absoluteTransform[0] = 1.0f;
    absoluteTransform[5] = 1.0f;
    absoluteTransform[10] = 1.0f;
    absoluteTransform[15] = 1.0f;
    // +0x68..+0xa7 identity, flag byte +0xa8 = 1
    relativeTransform = core::CMatrix4<float>(core::CMatrix4<float>::EM4CONST_IDENTITY);
    transformedBoundingBox.MinEdge = core::vector3d<float>{-1.0f, -1.0f, -1.0f};  // +0xd4
    transformedBoundingBox.MaxEdge = core::vector3d<float>{1.0f, 1.0f, 1.0f};     // +0xe0
    parent = nullptr;             // +0xec
    sceneManager = nullptr;       // +0x110
    id = nodeId;                  // +0x10c
    triangleSelector = nullptr;   // +0x114
    automaticCulling = 0;         // +0x118
    flags = 0x60Fu;               // +0x11c (1551)
    gameData = nullptr;           // +0x124
    cameraOffset = 0.0f;          // +0x128
    renderingLayer = 0;           // +0x12c
    updateAbsolutePosition(false);
}

// 0x35a8e0 _ZNK6glitch5scene10ISceneNode25getAbsoluteTransformationEv
const float* ISceneNode::getAbsoluteTransformation() const {
    return absoluteTransform;
}

// 0x596e24 _ZNK6glitch5scene10ISceneNode20getRenderVertexCountEPv
std::uint32_t ISceneNode::getRenderVertexCount(void* /*unused*/) const {
    return 0xffffffffu;
}

// 0x596f5c _ZNK6glitch5scene10ISceneNode5getIDEv
int ISceneNode::getID() const {
    return id;
}

// 0x596f64 _ZN6glitch5scene10ISceneNode5setIDEi
void ISceneNode::setID(int newId) {
    id = newId;
}

// 0x596fe4 _ZNK6glitch5scene10ISceneNode6getUIDEv
const char* ISceneNode::getUID() const {
    return "";
}

// 0x596ff4 _ZNK6glitch5scene10ISceneNode10getScopeIDEv
const char* ISceneNode::getScopeID() const {
    return "";
}

// 0x597094 _ZNK6glitch5scene10ISceneNode12getAnimatorsEv
const std::vector<ISceneNodeAnimator*>& ISceneNode::getAnimators() const {
    return animators;
}

// 0x59709c _ZNK6glitch5scene10ISceneNode18getBindedAnimatorsEv
const std::vector<ISceneNodeAnimator*>& ISceneNode::getBindedAnimators() const {
    return bindedAnimators;
}

// 0x5970a4 _ZNK6glitch5scene10ISceneNode11getMaterialEj
// The base node has no materials: the original writes a null reference.
video::CMaterial* ISceneNode::getMaterial(unsigned /*index*/) const {
    return nullptr;
}

// 0x5970b0 _ZNK6glitch5scene10ISceneNode16getMaterialCountEv
std::uint32_t ISceneNode::getMaterialCount() const {
    return 0;
}

// 0x5970b8 _ZN6glitch5scene10ISceneNode21notifyMaterialChangedEv
void ISceneNode::notifyMaterialChanged() {}

// 0x5971a4 _ZN6glitch5scene10ISceneNode16setIsDebugObjectEb
void ISceneNode::setIsDebugObject(bool debug) {
    if (debug) {
        flags |= kSceneNodeDebugObjectBit;
    } else {
        flags &= ~kSceneNodeDebugObjectBit;
    }
}

// 0x5971bc _ZNK6glitch5scene10ISceneNode13isDebugObjectEv
bool ISceneNode::isDebugObject() const {
    return (flags & kSceneNodeDebugObjectBit) != 0;
}

// 0x5971c8 _ZNK6glitch5scene10ISceneNode11getChildrenEv
const std::vector<ISceneNode*>& ISceneNode::getChildren() const {
    return children;
}

// 0x5971d0 _ZNK6glitch5scene10ISceneNode15getUserPropertyEv
const void* ISceneNode::getUserProperty() const {
    return nullptr;
}

// 0x5971d8 _ZNK6glitch5scene10ISceneNode18getUserPropertyStrEv
const void* ISceneNode::getUserPropertyStr() const {
    return nullptr;
}

// 0x596ec0 _ZN6glitch5scene10ISceneNode21onChangedSceneManagerEv
void ISceneNode::onChangedSceneManager() {}

// 0x5974c4 _ZN6glitch5scene10ISceneNode5cloneEv
ISceneNode* ISceneNode::clone() {
    return nullptr;
}

// 0x5974cc _ZN6glitch5scene10ISceneNode11setGameDataEPv
void ISceneNode::setGameData(void* data) {
    gameData = data;
}

// 0x5974d4 _ZNK6glitch5scene10ISceneNode11getGameDataEv
void* ISceneNode::getGameData() const {
    return gameData;
}

// 0x5974dc _ZN6glitch5scene10ISceneNode15setCameraOffsetEf
void ISceneNode::setCameraOffset(float offset) {
    cameraOffset = offset;
}

// 0x5974e4 _ZNK6glitch5scene10ISceneNode15getCameraOffsetEv
float ISceneNode::getCameraOffset() const {
    return cameraOffset;
}

// 0x5974ec _ZN6glitch5scene10ISceneNode17setRenderingLayerEi
void ISceneNode::setRenderingLayer(int layer) {
    renderingLayer = layer;
}

// 0x5974f4 _ZNK6glitch5scene10ISceneNode17getRenderingLayerEv
int ISceneNode::getRenderingLayer() const {
    return renderingLayer;
}

// 0x5974fc _ZN6glitch5scene10ISceneNode17onMaterialChangedEv
void ISceneNode::onMaterialChanged() {}

// 0x597254 _ZNK6glitch5scene10ISceneNode19getTriangleSelectorEv
ITriangleSelector* ISceneNode::getTriangleSelector() const {
    return triangleSelector;
}

// 0x59719c _ZN6glitch5scene10ISceneNode19setAutomaticCullingENS0_14E_CULLING_TYPEE
void ISceneNode::setAutomaticCulling(std::uint32_t culling) {
    automaticCulling = culling;
}

// 0x597298 _ZNK6glitch5scene10ISceneNode7getTypeEv
// 0x6e6b6e75 is the four-character code "unkn" in the original.
std::uint32_t ISceneNode::getType() const {
    return 0x6e6b6e75u;
}

// 0x597544 _ZN6glitch5scene10ISceneNode14resetTransformEb
void ISceneNode::resetTransform(bool /*keepTransform*/) {}

}  // namespace glitch::scene
