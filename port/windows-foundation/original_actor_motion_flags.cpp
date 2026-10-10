#include "original_actor_motion_flags.hpp"
#include <exception>
namespace dh::foundation {
bool original_character_motion_ctor_prefix(std::optional<std::uint32_t>& boundary,std::string& error){
    // CharacterC1 3aa5ec / CharacterC2 3a9778 STRB fp,[owner,#1c4], fp=1.
    boundary=1;error.clear();return true;
}
bool original_motion_boundary_validation(const std::optional<std::uint32_t>& boundary,
    bool& enabled,std::string& error){
    if(!boundary||*boundary>255){error="Actual source Character validation byte452 is unbound/invalid";return false;}
    enabled=*boundary!=0;error.clear();return true;
}
bool original_motion_focus_prefix(OriginalMotionFocusPrefix focus,
    std::optional<std::uint32_t>& flags,std::optional<std::uint32_t>& movement_type,
    std::uint8_t suppressed,OriginalMotionPrefixResult& output,std::string& error){
    if(&flags==&movement_type){error="Source flags/movement-type cells must be distinct";return false;}
    OriginalMotionPrefixResult result;
    switch(focus){
    case OriginalMotionFocusPrefix::move:{
        std::uint32_t raw_flags=0,raw_type=0;
        if(dh2_move_focus_begin(&raw_flags,&raw_type)){error="Original Move focus prefix failed";return false;}
        flags=raw_flags;movement_type=raw_type;result={true,true};break;
    }
    case OriginalMotionFocusPrefix::idle:
        // CSIdle::OnFocus source prefix in character_state.cpp: suppression
        // returns BEFORE NewFlags(2380). Movement type is not touched.
        if(suppressed==0){flags=0x2380;result.flags_written=true;}break;
    default:error="Unsupported source focus prefix; actual owner required";return false;
    }
    output=result;error.clear();return true;
}
bool original_motion_source_set_flags(std::optional<std::uint32_t>& flags,
    std::uint32_t actual,std::string& error){flags=actual;error.clear();return true;}
bool original_motion_policy(const std::optional<std::uint32_t>& flags,
    const std::array<std::int32_t,224>& resolved,OriginalMotionPolicy& output,std::string& error){
    if(!flags){error="Actual source Character flags520 are unbound";return false;}
    OriginalMotionPolicy result;
    if(dh2_move_policy(&result.decoded,&*flags)||
       dh2_move_rotation_speed(&result.rotation_speed,&*flags,resolved.data())){
        error="Original source motion policy getter failed";return false;
    }
    output=result;error.clear();return true;
}
bool original_motion_attack_focus_prefix(std::optional<std::uint32_t>& flags,
    std::optional<std::uint32_t>& gate,const OriginalMotionAttackDelayGetter& delay_getter,
    OriginalAttackFocusPrefixResult& result,std::string& error){
    if(&flags==&gate){error="Source flags/attack-gate cells must be distinct";return false;}
    error.clear();result={};flags=0x2341;result.flags_written=true;
    try{
        if(!delay_getter){error="Reached Attack focus GetAttackDelay owner unavailable";return false;}
        std::uint32_t delay=0;if(!delay_getter(delay,error)){if(error.empty())error="Attack focus GetAttackDelay delivery failed";return false;}
        if(delay){if(!gate){error="Reached actual source attack-gate528 cell unavailable";return false;}
            *gate|=1u;result.attack_gate_written=true;}
        error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
bool original_motion_blur_body(OriginalMotionBlurBody body,std::optional<std::uint32_t>& suppressed,
    const OriginalMotionBlurServices& services,OriginalMotionBlurResult& result,std::string& error){
    if(body!=OriginalMotionBlurBody::idle&&body!=OriginalMotionBlurBody::move&&body!=OriginalMotionBlurBody::attack){
        error="Unsupported original motion blur body";return false;}
    error.clear();result={};
    const auto missing=[&](const char* message){error=message;return false;};
    const auto delivery_failed=[&](const char* message){if(error.empty())error=message;return false;};
    try{
        if(body==OriginalMotionBlurBody::idle){result.phase=OriginalMotionBlurPhase::idle_store;
            suppressed=0;result.idle_suppressed_written=true;result.phase=OriginalMotionBlurPhase::completed;error.clear();return true;}
        if(body==OriginalMotionBlurBody::move){result.phase=OriginalMotionBlurPhase::stop;
            if(!services.stop)return missing("Reached original Move blur GameObject.Stop owner unavailable");
            ++result.service_calls;if(!services.stop(error))return delivery_failed("Original Move blur Stop failed");
        }else{
            result.phase=OriginalMotionBlurPhase::delay_query;
            if(!services.attack_delay)return missing("Reached original Attack blur GetAttackDelay owner unavailable");
            std::uint32_t delay=0;++result.service_calls;
            if(!services.attack_delay(delay,error))return delivery_failed("Original Attack blur delay query failed");
            if(delay){
                // ASM3c4010 re-calls getter. Do not substitute captured first value.
                ++result.service_calls;if(!services.attack_delay(delay,error))return delivery_failed("Original Attack blur second delay query failed");
                result.phase=OriginalMotionBlurPhase::timer;
                if(!services.start_timer)return missing("Reached original Attack blur timer owner unavailable");
                ++result.service_calls;if(!services.start_timer(delay,0,0x2a,0,error))return delivery_failed("Original Attack blur timer failed");
            }
        }
        result.phase=OriginalMotionBlurPhase::body_query;
        if(!services.body_present)return missing("Reached original blur live body+2dc getter unavailable");
        bool present=false;++result.service_calls;if(!services.body_present(present,error))return delivery_failed("Original blur body getter failed");
        if(present){result.phase=OriginalMotionBlurPhase::pin;
            if(!services.pin)return missing("Reached original blur body pin owner unavailable");
            ++result.service_calls;if(!services.pin(error))return delivery_failed("Original blur body pin failed");}
        result.phase=OriginalMotionBlurPhase::completed;error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
} // namespace dh::foundation
