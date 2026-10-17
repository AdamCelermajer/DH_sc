#include "camera_animset_v1.hpp"

#include <algorithm>
#include <stdexcept>

namespace dh2::camera_animset_v1 {
namespace {
bool append(const data::Dictionary& clips,std::int32_t id,Resource::Slot slot,
            std::vector<Resource>& out,std::string& error) {
    if(id==-1)return true;
    if(id<0||static_cast<std::size_t>(id)>=clips.values.size()){
        error="CamAnimSet clip reference is outside AnimDict";return false;
    }
    const auto& path=clips.values[static_cast<std::size_t>(id)];
    if(path.empty()||path.size()>4096||path.find('\0')!=std::string::npos){
        error="CamAnimSet clip path is invalid";return false;
    }
    out.push_back({slot,static_cast<std::int32_t>(out.size()),id,path});return true;
}
}

bool select(const data::AnimationTables& tables,const data::Dictionary& clips,
            const std::string& name,Selection& out,std::string& error) {
    out={};error.clear();
    if(name.empty()||tables.camera_names.size()!=tables.cameras.size()||
       clips.values.empty()||clips.names.size()!=clips.values.size()){
        error="Invalid CamAnimSet or AnimDict dimensions";return false;
    }
    const auto found=std::find(tables.camera_names.begin(),tables.camera_names.end(),name);
    if(found==tables.camera_names.end()){
        error="Configured CamAnimSet is absent";return false;
    }
    const auto row=static_cast<std::size_t>(found-tables.camera_names.begin());
    const auto& source=tables.cameras[row];
    Selection next;next.name=name;next.row=row;next.idle_clip_id=source.idle;
    if(source.idle<0){error="Selected CamAnimSet has no source idle animation";return false;}

    // These calls mirror CameraLevel::Load 0x41068c and its AnimSetManager
    // registration order. The entry camera starts the Idle slot from
    // Level::_LoadCamera 0x3f1008 / CameraLevel::PlayAnim 0x40f904.
    if(!append(clips,source.template_id,Resource::Slot::template_animation,next.resources,error)||
       !append(clips,source.idle,Resource::Slot::idle,next.resources,error)||
       !append(clips,source.shake,Resource::Slot::shake,next.resources,error)||
       !append(clips,source.crit,Resource::Slot::crit,next.resources,error))return false;
    for(const auto id:source.cam_anims)
        if(!append(clips,id,Resource::Slot::cam_animation,next.resources,error))return false;

    const auto idle=std::find_if(next.resources.begin(),next.resources.end(),
        [](const Resource& item){return item.slot==Resource::Slot::idle;});
    if(idle==next.resources.end()){
        error="Selected CamAnimSet idle failed to resolve";return false;
    }
    next.idle_path=idle->path;
    out=std::move(next);return true;
}

bool PlaybackOwner::bind(const Selection& selection) {
    selection_={};bound_=false;
    if(selection.name.empty()||selection.idle_clip_id<0||selection.resources.empty())return false;
    for(std::size_t i=0;i<selection.resources.size();++i){
        const auto& resource=selection.resources[i];
        if(resource.registration_order!=static_cast<std::int32_t>(i)||resource.clip_id<0||
           resource.path.empty())return false;
    }
    const auto idle=std::find_if(selection.resources.begin(),selection.resources.end(),
        [](const Resource& resource){return resource.slot==Resource::Slot::idle;});
    if(idle==selection.resources.end()||idle->clip_id!=selection.idle_clip_id||
       idle->path!=selection.idle_path)return false;
    selection_=selection;bound_=true;return true;
}

bool PlaybackOwner::request(std::int32_t clip_id,Resource::Slot required_slot,Trigger trigger,
                            bool preserve_zoom,PlayRequest& out,std::string& error) const {
    out={};error.clear();
    if(!bound_){error="Camera AnimSet is not bound";return false;}
    const auto found=std::find_if(selection_.resources.begin(),selection_.resources.end(),
        [clip_id,required_slot](const Resource& resource){
            return resource.clip_id==clip_id&&resource.slot==required_slot;
        });
    if(found==selection_.resources.end()){error="Camera clip is not registered in selected AnimSet";return false;}
    out.present=true;out.trigger=trigger;out.resource=*found;out.preserve_zoom=preserve_zoom;
    out.loop=false;out.speed=1.0f;return true;
}

bool PlaybackOwner::request_clip(std::int32_t clip_id,Trigger trigger,
                                 bool preserve_zoom,PlayRequest& out,
                                 std::string& error) const {
    out={};error.clear();
    if(!bound_){error="Camera AnimSet is not bound";return false;}
    const auto found=std::find_if(selection_.resources.begin(),selection_.resources.end(),
        [clip_id](const Resource& resource){return resource.clip_id==clip_id;});
    if(found==selection_.resources.end()){
        error="Camera AnimDict clip is not registered in selected AnimSet";return false;
    }
    out.present=true;out.trigger=trigger;out.resource=*found;out.preserve_zoom=preserve_zoom;
    out.loop=false;out.speed=1.0f;return true;
}

bool PlaybackOwner::level_idle(PlayRequest& out,std::string& error) const {
    return request(selection_.idle_clip_id,Resource::Slot::idle,Trigger::level_idle,false,out,error);
}

bool PlaybackOwner::animation_step(const data::AnimationStep& step,
                                   std::int32_t selected_random_clip,
                                   PlayRequest& out,std::string& error) const {
    out={};error.clear();
    std::int32_t clip=step.cam;
    if(selected_random_clip>=0){
        if(std::find(step.random_cam.begin(),step.random_cam.end(),selected_random_clip)==step.random_cam.end()){
            error="Selected random camera clip is not in this animation step";return false;
        }
        clip=selected_random_clip;
    }
    if(clip<0)return true; // No authored camera action on this step.
    // Cam and RandomCam contain AnimDict IDs, independently registered in the
    // selected camera AnimSet; their IDs are not the resource registration ordinal.
    return request_clip(clip,Trigger::animation_step,true,out,error);
}

bool PlaybackOwner::object_event(const std::string& event,const data::Dictionary& clips,
                                 bool shake_allowed,PlayRequest& out,std::string& error) const {
    out={};error.clear();
    constexpr const char prefix[]="camera/";
    if(event.compare(0,sizeof(prefix)-1,prefix)!=0){error="Object animation event is not a camera event";return false;}
    if(!shake_allowed)return true; // CanPlayShakeAnim denied this source event.
    const auto name=event.substr(sizeof(prefix)-1);
    const auto found=std::find(clips.names.begin(),clips.names.end(),name);
    if(found==clips.names.end()){error="Camera event name is absent from AnimDict";return false;}
    const auto id=static_cast<std::int32_t>(found-clips.names.begin());
    const auto resource=std::find_if(selection_.resources.begin(),selection_.resources.end(),
        [id](const Resource& candidate){return candidate.clip_id==id;});
    if(!bound_||resource==selection_.resources.end()){
        error="Camera event clip is not registered in selected AnimSet";return false;
    }
    return request(id,resource->slot,Trigger::object_event,true,out,error);
}

bool PlaybackOwner::combat_shake(bool critical_result,bool requester_is_player,
                                 bool shake_allowed,PlayRequest& out,std::string& error) const {
    out={};error.clear();
    if(!critical_result||!requester_is_player||!shake_allowed)return true;
    const auto crit=std::find_if(selection_.resources.begin(),selection_.resources.end(),
        [](const Resource& resource){return resource.slot==Resource::Slot::crit;});
    if(crit==selection_.resources.end())return true; // Source set has no Crit clip.
    // Character::F_ApplyResult calls CanPlayShakeAnim, then reads the selected
    // CamAnimSet row's +0x0c Crit field and calls PlayAnim(id, 0, true).
    return request(crit->clip_id,Resource::Slot::crit,Trigger::combat_crit,true,out,error);
}

bool PlaybackOwner::script_play_camera(bool use_crit,std::int32_t external_clip,
                                       PlayRequest& out,std::string& error) const {
    out={};error.clear();
    if(use_crit){
        const auto crit=std::find_if(selection_.resources.begin(),selection_.resources.end(),
            [](const Resource& resource){return resource.slot==Resource::Slot::crit;});
        if(crit==selection_.resources.end())return true; // Crit is optional in the source set.
        return request(crit->clip_id,Resource::Slot::crit,Trigger::script_crit,false,out,error);
    }
    if(external_clip<0){error="Script_PlayCamera external clip ID is invalid";return false;}
    return request_clip(external_clip,Trigger::script_external,false,out,error);
}

} // namespace dh2::camera_animset_v1
