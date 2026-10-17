#include "../camera_animset_playback_v1.hpp"

#include <cmath>
#include <stdexcept>

namespace {
void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
struct BackendState {
    dh2::camera_level_runtime_v1::Owner* camera=nullptr;
    bool accepted=true;
    bool saw_pre_start_zoom=false;
    int calls=0;
};
bool start_resource(void* context,const dh2::camera_animset_v1::PlayRequest& request){
    auto& state=*static_cast<BackendState*>(context);++state.calls;
    state.saw_pre_start_zoom=state.camera->current_zoom()==0.25f&&
                             state.camera->target_zoom()==0.75f;
    return state.accepted&&request.present&&request.resource.registration_order==4;
}
dh2::camera_animset_v1::PlayRequest request(bool present,bool preserve){
    dh2::camera_animset_v1::PlayRequest value;value.present=present;
    value.resource.registration_order=4;value.resource.clip_id=17;value.resource.path="camera/test.bdae";
    value.preserve_zoom=preserve;value.speed=1.0f;value.loop=false;return value;
}
}

int main(){
    using namespace dh2;
    camera_level_runtime_v1::Owner camera;camera.set_zoom(0.25f,0.75f);
    BackendState state{&camera,true,false,0};
    camera_animset_playback_v1::Backend backend{&state,start_resource};std::string error;

    require(camera_animset_playback_v1::start(camera,request(false,true),backend,error)==
                camera_animset_playback_v1::Status::no_request&&state.calls==0,
            "Absent camera request called the controller");
    require(camera_animset_playback_v1::start(camera,request(true,true),backend,error)==
                camera_animset_playback_v1::Status::started&&state.calls==1&&
                state.saw_pre_start_zoom&&camera.current_zoom()==0.25f&&camera.target_zoom()==0.75f,
            "Successful preserveZoom camera play changed ZoomHandler state/order");

    state.accepted=false;state.saw_pre_start_zoom=false;
    require(camera_animset_playback_v1::start(camera,request(true,false),backend,error)==
                camera_animset_playback_v1::Status::play_rejected&&state.calls==2&&
                camera.current_zoom()==0.25f&&camera.target_zoom()==0.75f,
            "Rejected PlayAnim mutated CameraLevel state");

    state.accepted=true;
    require(camera_animset_playback_v1::start(camera,request(true,false),backend,error)==
                camera_animset_playback_v1::Status::started&&camera.current_zoom()==0.0f&&
                camera.target_zoom()==1.0f,
            "Successful preserveZoom=false camera play did not reset zoom");

    require(camera_animset_playback_v1::start(camera,request(true,true),{},error)==
                camera_animset_playback_v1::Status::invalid_request&&!error.empty(),
            "Missing mandatory AnimSetController backend was accepted");
    return 0;
}
