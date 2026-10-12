#pragma once
#include <array>
#include <cmath>

#include "../../../level-world/navigation_heading.hpp"

namespace dh::foundation {
// CSInteract::OnFocus 0x3c5548: look-at requested and !IsCharSitting ->
// Cmd_LookAt -> Ctrl_LookAt 0x3addbc -> GameObject::LookAt 0x393cec ->
// GameObject::LookTowards 0x393b1c, which writes the yaw (GameObject+376)
// immediately. The yaw is in the source heading convention used by every
// other writer of ActorState::transform.rotation[2] (movement kernel, source
// LookAt, rendering): yaw 0 faces -Y and +X is pi/2 (atan(x/-y) with quadrant
// branches), i.e. atan2(dy,dx)+pi/2, not atan2(dy,dx). B151: the earlier
// atan2 form turned the hero a quarter turn away from the vase/chest.
// Coincident XY leaves the yaw unchanged (source LookTowards).
inline bool interaction_facing_v1(const std::array<float,3>& owner,
    const std::array<float,3>& target, bool requested, bool sitting, float& yaw) noexcept {
    if(!requested||sitting)return false;
    const float direction[3]{target[0]-owner[0],target[1]-owner[1],target[2]-owner[2]};
    if(!std::isfinite(direction[0])||!std::isfinite(direction[1])||(direction[0]==0.f&&direction[1]==0.f))return false;
    float next=std::isfinite(yaw)?yaw:0.f;
    if(dh2_nav_look_towards(&next,direction)!=0)return false;
    yaw=next;return true;
}
} // namespace dh::foundation
