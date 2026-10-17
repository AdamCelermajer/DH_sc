#include "camera_animset_playback_v1.hpp"

#include <cmath>

namespace dh2::camera_animset_playback_v1 {

Status start(camera_level_runtime_v1::Owner& camera,
             const camera_animset_v1::PlayRequest& request,
             Backend backend,std::string& error) {
    error.clear();
    if(!request.present)return Status::no_request;
    if(request.resource.registration_order<0||request.resource.clip_id<0||
       request.resource.path.empty()||!std::isfinite(request.speed)||request.speed<=0.0f||
       !backend.start_resource){
        error="Camera playback request or required backend is invalid";
        return Status::invalid_request;
    }
    // CameraLevel::PlayAnim changes active/preserve/zoom state only after its
    // AnimSetController accepted PlayAnim. Keep that observable order.
    if(!backend.start_resource(backend.context,request))return Status::play_rejected;
    camera.play_animation_started(request.preserve_zoom);
    return Status::started;
}

} // namespace dh2::camera_animset_playback_v1
