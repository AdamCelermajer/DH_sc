#include "item_frame_virtual_policy_v4.hpp"
#include <cmath>
namespace dh2::character {
bool item_frame_virtual_policy_v4(RetainedWorldItemObjectV1& item,
 actor::GenericRuntimeRequestV4& request,actor::RuntimePolicy& policy,std::string& e){
 auto* stat=item.base().byte(0x84);
 if(!stat||!std::isfinite(item.fields().speed3b0)){e="Required same Item static84/speed3b0 source fields";return false;}
 // 34005c/64/6c/74/7c/84/8c are genuine inherited receiver methods.
 request.actual_virtual_policy={0,1,0,0,1,1,*stat?2u:0u};
 // GetRotationSpeed3400f0 returns the source float bits bf800000 (-1).
 request.actual_rotation_speed=-1.f;
 policy.path.update_path=1;policy.path.avoid_obstacles=0;
 policy.path.update_physics=1;policy.validating_camera=0;
 policy.virtual_speed=item.fields().speed3b0; // original ItemGetSpeed3ebc04
 request.actor.policy=&policy;request.actor.resolved224=nullptr;
 request.actor.binding=nullptr;request.actor.scene=nullptr;
 return true;
}
}
