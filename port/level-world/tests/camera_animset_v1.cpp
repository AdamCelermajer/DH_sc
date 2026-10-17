#include "../camera_animset_v1.hpp"
#include "../player_camera_rig_v1.hpp"
#include "../../game-data/data.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
std::vector<std::uint8_t> read(const char* path){
    std::ifstream file(path,std::ios::binary);
    if(!file)throw std::runtime_error(std::string("Cannot open ")+path);
    return {std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};
}
void require(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
}

int main(int argc,char** argv){
    try{
        if(argc!=8){std::cerr<<"usage: camera_animset_v1_audit records names fields clip_names clip_values camera_scene idle_asset\n";return 2;}
        const auto records=read(argv[1]),names=read(argv[2]),fields=read(argv[3]);
        const auto clip_names=read(argv[4]),clip_values=read(argv[5]);
        const auto camera=read(argv[6]),idle=read(argv[7]);
        dh2::data::Dictionary clips;dh2::data::AnimationTables tables;std::string error;
        require(dh2::data::load_dictionary({clip_names.data(),clip_names.size()},
                    {clip_values.data(),clip_values.size()},clips,error),error);
        require(dh2::data::load_animation_tables({records.data(),records.size()},
                    {names.data(),names.size()},{fields.data(),fields.size()},clips,tables,error),error);
        dh2::camera_animset_v1::Selection selected;
        require(dh2::camera_animset_v1::select(tables,clips,"Default",selected,error),error);
        require(selected.row==0,"Default CamAnimSet row changed");
        require(selected.idle_clip_id>=0&&selected.idle_path==
                    "data/3D/camera/animations/common/camera_idle.bdae",
                "Default idle no longer resolves through the selected CamAnimSet");
        require(!selected.resources.empty()&&selected.resources[0].slot==
                    dh2::camera_animset_v1::Resource::Slot::template_animation,
                "Default CamAnimSet template is not registered before its playable clips");
        const auto idle_resource=std::find_if(selected.resources.begin(),selected.resources.end(),
            [](const auto& item){return item.slot==dh2::camera_animset_v1::Resource::Slot::idle;});
        require(idle_resource!=selected.resources.end()&&idle_resource->clip_id==selected.idle_clip_id,
                "CamAnimSet idle registration differs from the clip selected by _LoadCamera");
        for(const auto& resource:selected.resources)
            require(!resource.path.empty()&&resource.clip_id>=0,
                    "Resolved CamAnimSet contains an invalid source resource");
        for(std::size_t i=0;i<selected.resources.size();++i)
            require(selected.resources[i].registration_order==static_cast<std::int32_t>(i),
                    "Camera AnimSet local IDs do not follow source registration order");

        dh2::camera_animset_v1::PlaybackOwner camera_owner;
        dh2::camera_animset_v1::PlayRequest request;
        require(camera_owner.bind(selected),"Valid source CamAnimSet did not bind");
        require(camera_owner.level_idle(request,error)&&request.present&&
                    request.trigger==dh2::camera_animset_v1::Trigger::level_idle&&
                    request.resource.clip_id==selected.idle_clip_id&&!request.preserve_zoom&&
                    !request.loop&&request.speed==1.0f,
                "Level entry did not select source Idle with PlayAnim(idle,0,false)");

        std::size_t camera_steps=0;
        for(const auto& sequence:tables.sequences)for(const auto& step:sequence.steps){
            if(step.cam<0)continue;
            require(camera_owner.animation_step(step,-1,request,error)&&request.present&&
                        request.resource.clip_id==step.cam&&request.preserve_zoom,
                    "Shipped AnimationStep Cam ID did not resolve in the selected source AnimSet");
            ++camera_steps;
        }
        require(camera_steps>0,"Shipped animation tables contain no camera step for regression");

        const auto shake_source=std::find_if(selected.resources.begin(),selected.resources.end(),
            [](const auto& item){return item.slot==dh2::camera_animset_v1::Resource::Slot::shake;});
        if(shake_source!=selected.resources.end()){
            const auto event_name=clips.names[static_cast<std::size_t>(shake_source->clip_id)];
            require(camera_owner.object_event("camera/"+event_name,clips,true,request,error)&&
                        request.present&&request.resource.clip_id==shake_source->clip_id&&
                        request.preserve_zoom,
                    "Shipped camera/<Shake name> event did not resolve its selected resource");
            require(camera_owner.script_play_camera(false,shake_source->clip_id,request,error)&&
                        request.present&&request.resource.clip_id==shake_source->clip_id&&
                        !request.preserve_zoom,
                    "Shipped external Script_PlayCamera ID did not resolve through selected AnimSet");
        }

        const auto camera_anim=std::find_if(selected.resources.begin(),selected.resources.end(),
            [](const auto& item){return item.slot==dh2::camera_animset_v1::Resource::Slot::cam_animation;});
        if(camera_anim!=selected.resources.end()){
            dh2::data::AnimationStep step;step.cam=camera_anim->clip_id;
            require(camera_owner.animation_step(step,-1,request,error)&&request.present&&
                        request.trigger==dh2::camera_animset_v1::Trigger::animation_step&&
                        request.resource.clip_id==step.cam&&request.preserve_zoom,
                    "Authored Cam step did not resolve its registered camera clip");
            step.cam=-1;step.random_cam={camera_anim->clip_id};
            require(camera_owner.animation_step(step,camera_anim->clip_id,request,error)&&
                        request.present&&request.resource.clip_id==camera_anim->clip_id,
                    "Selected RandomCam value did not resolve through the source AnimSet");
            require(!camera_owner.animation_step(step,camera_anim->clip_id+100,request,error),
                    "RandomCam value outside the authored group was accepted");
            require(camera_owner.script_play_camera(false,camera_anim->clip_id,request,error)&&
                        request.present&&request.trigger==dh2::camera_animset_v1::Trigger::script_external&&
                        !request.preserve_zoom,
                    "Script_PlayCamera external ID did not preserve its source reset behavior");
        }

        const auto event_resource=std::find_if(selected.resources.begin(),selected.resources.end(),
            [](const auto& item){return item.slot==dh2::camera_animset_v1::Resource::Slot::cam_animation;});
        if(event_resource!=selected.resources.end()){
            const auto event_name=clips.names[static_cast<std::size_t>(event_resource->clip_id)];
            require(camera_owner.object_event("camera/"+event_name,clips,true,request,error)&&
                        request.present&&request.trigger==dh2::camera_animset_v1::Trigger::object_event&&
                        request.resource.clip_id==event_resource->clip_id&&request.preserve_zoom,
                    "camera/<AnimDict name> event did not resolve the selected resource");
            require(camera_owner.object_event("camera/"+event_name,clips,false,request,error)&&
                        !request.present,
                    "CanPlayShakeAnim denial did not suppress the camera event");
        }
        const auto combat_crit=std::find_if(selected.resources.begin(),selected.resources.end(),
            [](const auto& item){return item.slot==dh2::camera_animset_v1::Resource::Slot::crit;});
        if(combat_crit!=selected.resources.end()){
            require(camera_owner.combat_shake(true,true,true,request,error)&&request.present&&
                        request.trigger==dh2::camera_animset_v1::Trigger::combat_crit&&
                        request.resource.clip_id==combat_crit->clip_id&&
                        request.resource.slot==dh2::camera_animset_v1::Resource::Slot::crit&&
                        request.preserve_zoom,
                    "Character::F_ApplyResult did not select Crit with preserveZoom=true");
            require(camera_owner.combat_shake(false,true,true,request,error)&&!request.present&&
                        camera_owner.combat_shake(true,true,false,request,error)&&!request.present,
                    "Combat camera request bypassed its result or CanPlayShakeAnim gate");
        }
        const auto crit=std::find_if(selected.resources.begin(),selected.resources.end(),
            [](const auto& item){return item.slot==dh2::camera_animset_v1::Resource::Slot::crit;});
        if(crit!=selected.resources.end())
            require(camera_owner.script_play_camera(true,-1,request,error)&&request.present&&
                        request.trigger==dh2::camera_animset_v1::Trigger::script_crit&&
                        request.resource.clip_id==crit->clip_id&&!request.preserve_zoom,
                    "Script_PlayCamera Crit request did not select the configured Crit clip");

        dh2::player_camera_rig_v1::Rig rig;
        require(rig.load({camera.data(),camera.size(),idle.data(),idle.size()},error),error);
        dh2::player_camera_rig_v1::Playback playback;
        require(playback.start(rig,error),error);
        dh2::player_camera_rig_v1::Pose pose{};
        require(playback.advance(rig,0,&pose,error),error);
        const auto start=playback.current_time_ms();
        require(playback.advance(rig,8,&pose,error),error);
        require(playback.current_time_ms()==start+8&&std::isfinite(pose.camera[12]),
                "Selected CamAnimSet idle failed to advance from a game-frame delta");
        std::cout<<"PASS: CamAnimSet Default row="<<selected.row<<" idle="
                 <<selected.idle_clip_id<<" resources="<<selected.resources.size()
                 <<" path="<<selected.idle_path<<" timeline="<<start<<".."
                 <<playback.current_time_ms()<<" ms\n";
        for(const auto& resource:selected.resources)
            std::cout<<"  clip["<<resource.clip_id<<"]="<<resource.path<<"\n";
        return 0;
    }catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<"\n";return 1;}
}
