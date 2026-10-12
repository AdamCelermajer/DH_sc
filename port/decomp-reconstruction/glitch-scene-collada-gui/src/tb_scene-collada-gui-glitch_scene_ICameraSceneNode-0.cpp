#include "glitch-scene-collada-gui/scene_nodes.hpp"

namespace glitch::scene {

// 0x35bec0 _ZNK6glitch5scene16ICameraSceneNode12isOrthogonalEv
// Returns the flag byte at +0x134 as a truth value.
bool ICameraSceneNode::isOrthogonal() const {
    return orthogonal != 0;
}

}  // namespace glitch::scene
