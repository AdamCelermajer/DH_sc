#include "camera_animset_bank_playback_v1.hpp"

namespace dh2::camera_animset_bank_playback_v1 {
namespace {
struct Dispatch {
    const camera_animset_bank_v1::Owner* bank=nullptr;
    Backend backend{};
};

struct RigDispatch {
    player_camera_rig_v1::Rig* rig=nullptr;
    player_camera_rig_v1::Playback* playback=nullptr;
    std::string* error=nullptr;
};

bool start_selected(void* context,const camera_animset_v1::PlayRequest& request) {
    auto& dispatch=*static_cast<Dispatch*>(context);
    const auto* clip=dispatch.bank->resolve(request);
    if(!clip||!dispatch.backend.start_bdae)return false;
    return dispatch.backend.start_bdae(dispatch.backend.context,request,
                                       clip->bdae.data(),clip->bdae.size());
}

bool start_rig_clip(void* context,const camera_animset_v1::PlayRequest&,
                    const std::uint8_t* bytes,std::size_t size) {
    auto& dispatch=*static_cast<RigDispatch*>(context);
    if(!dispatch.rig||!dispatch.playback||!dispatch.error)return false;
    if(!dispatch.rig->load_animation(bytes,size,*dispatch.error))return false;
    return dispatch.playback->start(*dispatch.rig,*dispatch.error);
}
}

Status start(camera_level_runtime_v1::Owner& camera,
             const camera_animset_bank_v1::Owner& bank,
             const camera_animset_v1::PlayRequest& request,
             Backend backend,std::string& error) {
    error.clear();
    if(!request.present)return Status::no_request;
    if(!backend.start_bdae||request.resource.clip_id<0||
       request.resource.registration_order<0||request.resource.path.empty()){
        error="Camera BDAE request or required backend is invalid";
        return Status::invalid_request;
    }
    if(!bank.resolve(request)){
        error="Camera request does not resolve in the selected BDAE bank";
        return Status::bank_resource_missing;
    }

    Dispatch dispatch{&bank,backend};
    const camera_animset_playback_v1::Backend playback_backend{&dispatch,start_selected};
    const auto status=camera_animset_playback_v1::start(camera,request,playback_backend,error);
    switch(status){
        case camera_animset_playback_v1::Status::no_request:return Status::no_request;
        case camera_animset_playback_v1::Status::invalid_request:return Status::invalid_request;
        case camera_animset_playback_v1::Status::play_rejected:return Status::playback_rejected;
        case camera_animset_playback_v1::Status::started:return Status::started;
    }
    error="Unknown camera playback result";
    return Status::invalid_request;
}

Status start_rig(camera_level_runtime_v1::Owner& camera,
                 const camera_animset_bank_v1::Owner& bank,
                 const camera_animset_v1::PlayRequest& request,
                 player_camera_rig_v1::Rig& rig,
                 player_camera_rig_v1::Playback& playback,
                 std::string& error) {
    RigDispatch dispatch{&rig,&playback,&error};
    return start(camera,bank,request,Backend{&dispatch,start_rig_clip},error);
}

bool advance_rig(camera_level_runtime_v1::Owner& camera,
                 player_camera_rig_v1::Rig& rig,
                 player_camera_rig_v1::Playback& playback,
                 std::uint32_t dt_ms,player_camera_rig_v1::Pose* pose,
                 std::string& error) {
    if(!playback.advance(rig,dt_ms,pose,error))return false;
    // The original CameraLevel registers __Callback on PlayAnim's timeline;
    // that callback clears +0x84 when this non-looping clip ends. Keep this
    // transition beside the sole playback clock so zoom mode cannot get stuck.
    if(playback.completed())camera.end_animation();
    return true;
}

} // namespace dh2::camera_animset_bank_playback_v1
