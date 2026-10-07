#include "gameplay_camera_target_v2.hpp"
#include <cstring>
extern "C" void dh2_gameplay_camera_transition_v2(float out[3],const float start[3],const float end[3],std::int32_t remaining,std::int32_t duration){
 if(remaining<=0){for(unsigned n=0;n<3;++n)out[n]=end[n];return;}
 const float division=static_cast<float>(remaining)/static_cast<float>(duration);const float t=1.f-division;
 for(unsigned n=0;n<3;++n){const float delta=end[n]-start[n];const float move=t*delta;out[n]=start[n]+move;}
}
namespace dh2::camera {
bool GameplayCameraTargetV2::set_target(std::uintptr_t id,std::int32_t ms,std::string& e){
 if(!id)return true;PointV2 start{};
 if(ms>0){if(state_.target){if(!services_.actor_anchor){e="Required same actor camera anchor";return false;}if(!services_.actor_anchor(state_.target,start,e))return false;}else{if(!services_.source_origin){e="Required original camera transition Point3D ZERO";return false;}start=*services_.source_origin;}}
 state_.transition_start=start;state_.transition_duration=ms>0?ms:0;state_.transition_remaining=state_.transition_duration;state_.target=id;state_.ghost={};return true;
}
bool GameplayCameraTargetV2::transition(bool& handled,std::string& e){
 handled=false;if(state_.transition_remaining<0||!services_.root_present||!state_.target)return true;
 if(!services_.scene_lease||!services_.application_dt){e="Required retained camera scene/Application dt";return false;}
 std::uint32_t dt;if(!services_.application_dt(dt,e))return false;
 const std::uint32_t raw=static_cast<std::uint32_t>(state_.transition_remaining)-dt;std::memcpy(&state_.transition_remaining,&raw,4);
 if(!services_.actor_anchor){e="Required same target camera anchor after transition countdown";return false;}PointV2 anchor,out;if(!services_.actor_anchor(state_.target,anchor,e))return false;
 dh2_gameplay_camera_transition_v2(out.data(),state_.transition_start.data(),anchor.data(),state_.transition_remaining,state_.transition_duration);
 if(!services_.root_set_position){e="Required same root SetPosition transition";return false;}if(!services_.root_set_position(out,e))return false;handled=true;return true;
}
bool GameplayCameraTargetV2::offset(PointV2& p,std::string& e){
 if(!state_.offset_enabled)return true;
 if(!services_.center_offset){e="Required actual CameraBase GetCenterOffset";return false;}PointV2 delta{};if(!services_.center_offset(p[2],delta,e))return false;for(unsigned n=0;n<3;++n)p[n]=p[n]+delta[n];return true;
}
bool GameplayCameraTargetV2::ghost(PointV2& p)noexcept{for(unsigned n=0;n<3;++n)p[n]=p[n]+state_.ghost[n];return true;}
bool GameplayCameraTargetV2::damping(PointV2& p,std::string& e){
 DampingServicesV1 s;
 if(services_.root_position)s.root_position=[this](float v[3],std::string& error){PointV2 position;if(!services_.root_position(position,error))return false;for(unsigned n=0;n<3;++n)v[n]=position[n];return true;};
 s.application_dt=services_.application_dt;bool applied;return handle_damping_v1(state_.damping,services_.root_present,p.data(),s,applied,e);
}
bool GameplayCameraTargetV2::update(std::string& e){
 if(!services_.root_present||!state_.target)return true;
 bool handled;if(!transition(handled,e))return false;if(handled)return true;
 if(!services_.actor_anchor){e="Required same target GetCameraAnchorPosition";return false;}PointV2 p;if(!services_.actor_anchor(state_.target,p,e)||!offset(p,e)||!ghost(p)||!damping(p,e))return false;
 if(!services_.root_set_position){e="Required same root SetPosition";return false;}return services_.root_set_position(p,e);
}
}
