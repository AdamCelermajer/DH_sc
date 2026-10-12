#include "glitch-scene-collada-gui/scene_nodes.hpp"

namespace glitch::scene {

// 0x599868 _ZNK6glitch5scene18ISceneNodeAnimator15getTimelineCtrlEv
// glitch::scene::ISceneNodeAnimator::getTimelineCtrl() const
// Returns the timeline controller word at +0x08 (ldr r0, [r0, #8]).
ITimelineController* ISceneNodeAnimator::getTimelineCtrl() const {
    return timelineCtrl;
}

// 0x599870 _ZN6glitch5scene18ISceneNodeAnimator15getTimelineCtrlEv
// glitch::scene::ISceneNodeAnimator::getTimelineCtrl()
// Non-const overload with the same body: returns the word at +0x08 (ldr r0, [r0, #8]).
ITimelineController* ISceneNodeAnimator::getTimelineCtrl() {
    return timelineCtrl;
}

}  // namespace glitch::scene
