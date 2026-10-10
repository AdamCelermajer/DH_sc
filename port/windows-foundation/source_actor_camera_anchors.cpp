#include "source_actor_camera_anchors.hpp"
#include "../level-world/gameplay_camera_anchor_v6.hpp"
namespace dh::foundation {
bool source_actor_camera_anchor(const SourceActorCameraAnchorInput& input,CameraVec3& output,std::string& error) {
    if(!input.position_admitted) {error="Required admitted source actor position160";return false;}
    if(!input.anchor_slot_admitted) {error="Required actual borrowed source actor anchor2e0 slot";return false;}
    float position[]{input.position160.x,input.position160.y,input.position160.z};
    float attached[3]{};auto anchor=input.anchor2e0;
    dh2::camera::CameraAnchorBorrowV6 borrow;borrow.actor_lease=input.actor_lease;borrow.identity=input.actor_identity;borrow.anchor2e0=&anchor;borrow.position160=position;
    if(input.anchor_position_c)borrow.anchor_position=[&](auto id,const float*& point,std::string& problem){
        CameraVec3 result;if(!input.anchor_position_c(id,result,problem))return false;
        attached[0]=result.x;attached[1]=result.y;attached[2]=result.z;point=attached;return true;
    };
    dh2::camera::PointV2 point;if(!dh2::camera::source_camera_anchor_v6(borrow,point,error))return false;
    output={point[0],point[1],point[2]};error.clear();return true;
}
}
