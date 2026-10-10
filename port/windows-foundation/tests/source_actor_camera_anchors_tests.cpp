#include "source_actor_camera_anchors.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
int main(){try{
 std::string error;CameraVec3 point{7,8,9};SourceActorCameraAnchorInput input;input.actor_lease=std::make_shared<int>(1);input.actor_identity=17;input.position_admitted=true;input.position160={1090.75f,-212.202f,258};check(!source_actor_camera_anchor(input,point,error),"Absent actual source anchor slot authority silently assumed null");input.anchor_slot_admitted=true;
 unsigned calls=0;input.anchor_position_c=[&](auto id,CameraVec3& result,std::string&){++calls;check(id==31,"Wrong source anchor receiver selected");result={4,5,6};return true;};
 check(source_actor_camera_anchor(input,point,error),error);check(point.x==input.position160.x&&point.y==input.position160.y&&point.z==input.position160.z&&calls==0,"Source null-anchor actor160 fallback changed");
 input.position160={1159.23f,-210.049f,255};check(source_actor_camera_anchor(input,point,error),error);check(point.x==input.position160.x&&point.z==255,"Source admitted actor motion not reflected in null-anchor point");
 input.anchor2e0=31;check(source_actor_camera_anchor(input,point,error),error);check(point.x==4&&point.y==5&&point.z==6&&calls==1,"Actual attached AnchorBase+c not selected");
 input.anchor_position_c={};check(!source_actor_camera_anchor(input,point,error),"Missing attached anchor silently fell back to actor position");check(point.x==4&&point.z==6,"Failed anchor lookup mutated published point");
 input.anchor2e0=0;input.position_admitted=false;check(!source_actor_camera_anchor(input,point,error),"Unadmitted source position published");input.position_admitted=true;input.actor_lease.reset();check(!source_actor_camera_anchor(input,point,error),"Actor without source ownership lease accepted");input.actor_lease=std::make_shared<int>(1);input.actor_identity=0;check(!source_actor_camera_anchor(input,point,error),"Null source actor identity accepted");
 std::cout<<"PASS original known-null actor-position branch, admitted movement, attached-anchor receiver branch and unsupported input preservation\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
