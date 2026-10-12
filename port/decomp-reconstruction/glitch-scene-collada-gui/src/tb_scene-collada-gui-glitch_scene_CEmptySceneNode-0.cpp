// Track B batch e1-glitch-scene-collada-gui-glitch_scene_CEmptySceneNode-0.
#include "glitch-scene-collada-gui/scene_nodes.hpp"

namespace glitch::scene {

// 0x5839b0 _ZNK6glitch5scene15CEmptySceneNode14getBoundingBoxEv glitch::scene::CEmptySceneNode::getBoundingBox() const
// Asm: ADD R0, R0, #0x130; BX LR. Returns this + 0x130, the box stored in the node.
const core::aabbox3d<float>& CEmptySceneNode::getBoundingBox() const {
    return boundingBox;  // +0x130
}

}  // namespace glitch::scene
