#include "glitch-scene-collada-gui/scene_node_animator.hpp"

namespace glitch::collada {

// 0x65d3e8 _ZN6glitch7collada18CSceneNodeAnimator19getAnimationTrackExEi
std::uint32_t CSceneNodeAnimator::getAnimationTrackEx(int index) const {
    return targets[index].trackEx;
}

// 0x65d3f8 _ZN6glitch7collada18CSceneNodeAnimator17getAnimationTrackEi
std::uint32_t CSceneNodeAnimator::getAnimationTrack(int index) const {
    return targets[index].track;
}

// 0x65d494 _ZN6glitch7collada18CSceneNodeAnimator14getTargetCountEv
std::uint32_t CSceneNodeAnimator::getTargetCount() const {
    return static_cast<std::uint32_t>(targets.size());
}

// 0x65d544 _ZN6glitch7collada18CSceneNodeAnimator11createCloneEv
void* CSceneNodeAnimator::createClone() const {
    return nullptr;
}

}  // namespace glitch::collada
