#include "gameplay_camera_level_v4.hpp"
#include <cmath>
#include <cstring>
namespace dh2::camera {
bool GameplayCameraLevelV4::play_animation(std::int32_t id,std::int32_t clip_index,bool hold,std::string& e){
 if(!services_.controller_present)return true;
 if(!services_.controller_clip_indexc||!services_.controller_byte10){e="Required same source AnimSetController fields";return false;}
 *services_.controller_clip_indexc=clip_index;*services_.controller_byte10=0;
 if(!services_.controller_play){e="Required same source AnimSetController Play";return false;}bool played;if(!services_.controller_play(id,played,e))return false;if(!played)return true;
 state_.shake84=true;if(!hold){state_.requested_zoom88=0;state_.automatic_zoom8c=1;}state_.hold_zoom_a4=hold;return true;
}
bool GameplayCameraLevelV4::handle_zoom(std::string& e){
 if(!services_.debug_infinite_zoom){e="Required actual Debug InfiniteZoom";return false;}bool infinite;if(!services_.debug_infinite_zoom(infinite,e))return false;
 if(infinite){state_.applied_zoom90=state_.requested_zoom88;return true;}if(state_.zoom_locked86)return true;
 if(!services_.zoom_bounds){e="Required actual camera configuration zoom bounds";return false;}float lower,upper;if(!services_.zoom_bounds(state_.mode85,lower,upper,e))return false;
 auto clamp=[&](float value){if(!(value>lower))value=lower;if(!(value<upper))value=upper;return value;};
 state_.requested_zoom88=clamp(state_.requested_zoom88);state_.automatic_zoom8c=clamp(state_.automatic_zoom8c);
 state_.applied_zoom90=state_.requested_zoom88>state_.automatic_zoom8c?state_.automatic_zoom8c:state_.requested_zoom88;return true;
}
bool GameplayCameraLevelV4::update(std::string& e){
 if(!services_.active_is_this){e="Required actual CameraBase active camera identity";return false;}bool active;if(!services_.active_is_this(active,e))return false;
 auto& f=target_.fields();if(!active||!services_.target.root_present||!services_.camera_present||!f.target)return true;
 bool transitioned;if(!target_.transition(transitioned,e))return false;if(transitioned)return true;
 PointV2 p;
 if(state_.mode85){if(!services_.actor_position){e="Required same target Character position160";return false;}if(!services_.actor_position(f.target,p,e))return false;}
 else{if(!services_.target.actor_anchor){e="Required same target camera anchor";return false;}if(!services_.target.actor_anchor(f.target,p,e))return false;}
 if(!target_.offset(p,e))return false;
 if(!services_.handle_centering){e="Required actual local-player/Level camera centering";return false;}if(!services_.handle_centering(p,e))return false;
 auto offset=state_.offset98;
 if(state_.mode85){std::uint32_t raw=0x3f3504f3;float cos45;std::memcpy(&cos45,&raw,4);const float xcos=offset[0]*cos45,ysin=offset[1]*cos45,ycos=offset[1]*cos45,xsin=offset[0]*cos45;offset[0]=xcos-ysin;offset[1]=ycos+xsin;}
 for(unsigned n=0;n<3;++n)p[n]=p[n]+offset[n];
 if(!state_.shake84||state_.hold_zoom_a4){if(!handle_zoom(e))return false;}
 else{if(std::fabs(state_.applied_zoom90)<0.1f)state_.applied_zoom90=0;else state_.applied_zoom90=state_.applied_zoom90*0.75f;}
 // Source sets child camera local position(0,0,-zoom90*distance94).
 PointV2 camera{0,0,(-state_.applied_zoom90)*state_.default_distance94};if(!services_.camera_set_position){e="Required same authored camera SetPosition";return false;}if(!services_.camera_set_position(camera,e))return false;
 if(!target_.ghost(p)||!target_.damping(p,e))return false;
 if(!services_.target.root_set_position){e="Required same source camera root SetPosition";return false;}if(!services_.target.root_set_position(p,e))return false;
 if(!services_.actor_disabled81){e="Required same target disabled81 lifecycle field";return false;}bool disabled;if(!services_.actor_disabled81(f.target,disabled,e))return false;if(disabled)f.target=0;return true;
}
}
