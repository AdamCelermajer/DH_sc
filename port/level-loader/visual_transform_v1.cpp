#include "visual_transform_v1.hpp"
#include "../engine-math/math.hpp"
#include <cmath>
#include <cstring>
namespace dh2::loader {
namespace {float literal(unsigned bits){float f;std::memcpy(&f,&bits,4);return f;}}
bool project_visual_transform_v1(VisualTransformV1& value,std::string& error) {
    error.clear();
    for(const auto* a:{&value.position,&value.rotation_degrees,&value.scale})
        for(float x:*a)if(!std::isfinite(x)){error="Nonfinite authored visual transform";return false;}
    auto next=value;
    // 0x38be8c..0x38beec: per-component near-zero scale replacement.
    for(float& x:next.scale)if(std::abs(x)<literal(0x38d1b717))x=1;
    const float conversion=literal(0x3c8efa35);
    const float x=next.rotation_degrees[0]*conversion;
    const float y=next.rotation_degrees[1]*conversion;
    const float z=next.rotation_degrees[2]*conversion;
    // 0x47288c..0x4728a4: ARM soft-float r1=Y,r2=-X,r3=-Z.
    math::Quaternion q{};dh2_quat_from_euler(&q,y,-x,-z);
    next.quaternion={q.x,q.y,q.z,q.w};
    for(float v:next.quaternion)if(!std::isfinite(v)){error="Authored rotation overflow";return false;}
    // SyncScaling calls the Point3D overload, not the uniform float overload.
    value=next;return true;
}
}
