// Bodies of the scene classes that have no canonical header yet (CLightSceneNode, CBillboardSceneNode).
// The bodies of ISceneNodeAnimator, ITimeController, ITimelineController, ICameraSceneNode, CCameraSceneNode, CEmptySceneNode,
// CSceneManagerRootNode, CMeshSceneNode, CSceneManager and IMesh moved to src/scene/ with their canonical classes.
#include "glitch-scene-collada-gui/scene_nodes.hpp"

namespace glitch::scene {

// 0x583bec _ZNK6glitch5scene15CLightSceneNode7getTypeEv
std::uint32_t CLightSceneNode::getType() const {
    return 0x7468676c;  // "lght"
}

// 0x580a2c _ZNK6glitch5scene19CBillboardSceneNode7getTypeEv
std::uint32_t CBillboardSceneNode::getType() const {
    return 0x6c6c6962;  // "bill"
}

// 0x580a98 _ZNK6glitch5scene19CBillboardSceneNode14getBoundingBoxEv
const core::aabbox3d<float>& CBillboardSceneNode::getBoundingBox() const {
    return boundingBox;
}

// 0x580b34 _ZNK6glitch5scene19CBillboardSceneNode16getMaterialCountEv
std::uint32_t CBillboardSceneNode::getMaterialCount() const {
    return 1;
}

// 0x580b3c _ZNK6glitch5scene19CBillboardSceneNode7getSizeEv
const core::dimension2d<float>& CBillboardSceneNode::getSize() const {
    return size;
}

}  // namespace glitch::scene
