// Batch r3a-glitch-scene-collada-gui-glitch_scene_CSceneNodeAnimatorFlyStraig-7b1009-0.
// Bodies of glitch::scene::CSceneNodeAnimatorFlyStraight (the stand-in class of scene_simple_accessors.hpp, whose members
// sit at the original offsets). The class is not derived from the canonical scene::ISceneNodeAnimator yet, so the
// animator base calls (dtors, clone, IAttributes serialization) stay partial; see the list at the end.
#include <cmath>

#include "glitch-scene-collada-gui/scene_simple_accessors.hpp"

namespace glitch::scene {

// 0x6cc330 _ZN6glitch5scene29CSceneNodeAnimatorFlyStraight25recalculateImidiateValuesEv
// The way vector is end - start (+0x18 - +0x0c, componentwise); wayLength is its length; vector is normalized;
// timeFactor is wayLength / timeForWay (the time the way takes, as a speed factor).
float CSceneNodeAnimatorFlyStraight::recalculateImidiateValues() {
    const float dx = end.X - start.X;
    const float dy = end.Y - start.Y;
    const float dz = end.Z - start.Z;
    vector.X = dx;
    vector.Y = dy;
    vector.Z = dz;
    wayLength = std::sqrt((dx * dx + dy * dy) + dz * dz);
    vector.normalize();
    timeFactor = wayLength / static_cast<float>(timeForWay);
    return timeFactor;
}

// 0x6cc138 _ZN6glitch5scene29CSceneNodeAnimatorFlyStraight11animateNodeEPNS0_10ISceneNodeEj
// The position starts at the start point. A node that is not looping is set to the end point once the elapsed time
// (timeMs - startTime) reaches timeForWay. Otherwise the elapsed time, taken modulo timeForWay, moves the position
// along the way vector scaled by timeFactor.
void CSceneNodeAnimatorFlyStraight::animateNode(ISceneNode* node, std::uint32_t timeMs) {
    if (node == nullptr) {
        return;
    }
    core::vector3d<float> position = start;
    const std::uint32_t elapsed = timeMs - startTime;
    if (!loop) {
        if (elapsed >= timeForWay) {
            position = end;
            node->setPosition(position);
            return;
        }
    }
    const float t = std::fmod(static_cast<float>(elapsed), static_cast<float>(timeForWay));
    position.X = position.X + timeFactor * (t * vector.X);
    position.Y = position.Y + timeFactor * (t * vector.Y);
    position.Z = position.Z + timeFactor * (t * vector.Z);
    node->setPosition(position);
}

// PARTIAL rows of this batch (no body written here):
//   0x6cc078 serializeAttributes(...) const: writes Start, End, TimeForWay and Loop through IAttributes vtable slots
//            (+0x1a8, +0x4c, +0xd8); the slot-to-method mapping of io::IAttributes is not confirmed.
//   0x6cc26c, 0x6cc2e4 ~CSceneNodeAnimatorFlyStraight (D1/D2): the glitch::scene::ISceneNodeAnimator base is not derived here.
//   0x6cc3ec deserializeAttributes(...): reads Start, End, TimeForWay and Loop through IAttributes slots (same mapping gap).
//   0x6cc4b8, 0x6cc61c constructors (C1, C2): the IObject / IEventReceiver / IReferenceCounted base construction.
//   0x6cc5d4 createClone(): needs the copy constructor over the base animator.

}  // namespace glitch::scene
