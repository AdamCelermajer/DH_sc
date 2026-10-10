#include "campaign_camera_adapter.hpp"
#include "../level-world/gameplay_camera_target_v2.hpp"
#include <cmath>
#include <cstring>
#include <utility>

namespace dh::foundation {
namespace {
std::int32_t signedWord(std::uint32_t bits){std::int32_t value;std::memcpy(&value,&bits,4);return value;}
bool finite(CameraVec3 p){return std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z);}
}
CampaignCameraAdapter::CampaignCameraAdapter(CampaignCameraProviders providers):providers_(std::move(providers)){}
void CampaignCameraAdapter::bind(CampaignCameraProviders providers){providers_=std::move(providers);}
bool CampaignCameraAdapter::seed_target(std::uint64_t identity,std::string& error){return set_target(identity,0,error);}
bool CampaignCameraAdapter::set_target(std::uint64_t identity,std::int32_t duration,std::string& error){
    // Original CameraTarget::SetTarget ignores NULL, preserving prior state.
    if(!identity){error.clear();return true;}
    CameraVec3 start{};
    if(duration>0&&target_){
        if(!providers_.anchor){error="Missing original previous camera-target anchor provider";return false;}
        if(!providers_.anchor(target_,start,error))return false;
        if(!finite(start)){error="Nonfinite original camera-target anchor";return false;}
    }
    transitionStart_=start;duration_=duration>0?duration:0;remaining_=duration_;target_=identity;
    error.clear();return true;
}
bool CampaignCameraAdapter::command(CampaignCommandPhase phase,const OriginalCampaignCommand& command,bool skip,
                                    bool& handled,bool& blocking,std::string& error){
    handled=command.kind==4||command.kind==8;blocking=false;
    if(!handled){error.clear();return true;}
    // Script_SetCamera::Execute455678 is BX LR; no guessed asset switching.
    if(command.kind==4||phase==CampaignCommandPhase::update){error.clear();return true;}
    if(phase==CampaignCommandPhase::is_blocking){
        const auto wait=command.scalars.find(20);
        if(wait==command.scalars.end()||wait->second>1){error="Missing/invalid original camera target wait byte20";return false;}
        blocking=wait->second!=0&&remaining_>0;error.clear();return true;
    }
    const auto name=command.strings.find(16);const auto duration=command.scalars.find(8);
    if(name==command.strings.end()||duration==command.scalars.end()){
        error="Missing original SetCameraTarget name16/duration8";return false;
    }
    std::uint64_t identity=0;
    if(name->second.empty()){
        if(!providers_.local_player){error="Missing original local-player camera target provider";return false;}
        if(!providers_.local_player(identity,error))return false;
    }else{
        if(!providers_.named_target){error="Missing original named camera target provider";return false;}
        bool found=false;if(!providers_.named_target(name->second,identity,found,error))return false;
        if(!found){error.clear();return true;} // Original unresolved name is a no-op.
    }
    return set_target(identity,skip?0:signedWord(duration->second),error);
}
bool CampaignCameraAdapter::tick(std::uint32_t dt,CampaignCameraFrame& output,std::string& error){
    CampaignCameraFrame frame;frame.hasTarget=target_!=0;
    if(!target_){output=frame;error.clear();return true;}
    if(!providers_.anchor){error="Missing current camera-target anchor provider";return false;}
    // Source HandleTransition checks remaining<0 BEFORE decrement, subtracts
    // unsigned Application dt with signed word reinterpretation, then queries
    // the current destination anchor. This is intentionally not a float timer.
    const bool transition=remaining_>=0;
    if(transition)remaining_=signedWord(std::uint32_t(remaining_)-dt);
    CameraVec3 anchor;if(!providers_.anchor(target_,anchor,error))return false;
    if(!finite(anchor)){error="Nonfinite original camera-target destination";return false;}
    if(transition){
        const float start[3]{transitionStart_.x,transitionStart_.y,transitionStart_.z};
        const float end[3]{anchor.x,anchor.y,anchor.z};float out[3];
        dh2_gameplay_camera_transition_v2(out,start,end,remaining_,duration_);
        frame.anchor={out[0],out[1],out[2]};frame.applyDamping=false;
    }else frame.anchor=anchor;
    output=frame;error.clear();return true;
}
} // namespace dh::foundation
