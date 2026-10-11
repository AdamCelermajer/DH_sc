#pragma once
#include <array>
#include <cmath>

namespace dh::foundation {
// CSInteract::OnFocus 0x3c5548: look-at requested and !IsCharSitting.
// GameObject::LookAt/LookTowards 0x393cec/0x393b1c writes yaw immediately;
// coincident XY leaves it alone. Uses this port's +X-forward yaw convention.
inline bool interaction_facing_v1(const std::array<float,3>& owner,
    const std::array<float,3>& target, bool requested, bool sitting, float& yaw,
    bool preserve_coincident_heading=true) noexcept {
    if(!requested||sitting)return false;
    const float x=target[0]-owner[0],y=target[1]-owner[1];
    if(!std::isfinite(x)||!std::isfinite(y)||(preserve_coincident_heading&&x==0.f&&y==0.f))return false;
    yaw=std::atan2(y,x);return true;
}
} // namespace dh::foundation
