#include "../original_actor_motion_flags.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh::foundation;
static void check(bool value,const char* error){if(!value)throw std::runtime_error(error);}
int main(){try{
    std::string error;std::optional<std::uint32_t> flags,type;OriginalMotionPrefixResult prefix;
    std::optional<std::uint32_t> boundary;bool boundary_enabled=false;
    check(!original_motion_boundary_validation(boundary,boundary_enabled,error)&&!boundary_enabled,"unknown validation cell invented default");
    check(original_character_motion_ctor_prefix(boundary,error)&&boundary==1&&
        original_motion_boundary_validation(boundary,boundary_enabled,error)&&boundary_enabled&&!flags&&!type,
        "actual Character ctor override initialized wrong runtime fields");
    boundary=256;check(!original_motion_boundary_validation(boundary,boundary_enabled,error)&&boundary_enabled,
        "invalid source byte changed output");
    boundary=0;check(original_motion_boundary_validation(boundary,boundary_enabled,error)&&!boundary_enabled,
        "actual validation zero confused with unknown");
    std::array<std::int32_t,224> resolved{};resolved[47]=25600;
    OriginalMotionPolicy decoded;decoded.rotation_speed=71;
    check(!original_motion_policy(flags,resolved,decoded,error)&&decoded.rotation_speed==71,"unknown source flags invented a policy");
    check(original_motion_focus_prefix(OriginalMotionFocusPrefix::idle,flags,type,255,prefix,error)&&
        !flags&&!type&&!prefix.flags_written,"suppressed Idle created previously unknown flags");
    check(original_motion_focus_prefix(OriginalMotionFocusPrefix::move,flags,type,0,prefix,error)&&
        flags==0x23c1u&&type==0u&&prefix.flags_written&&prefix.movement_type_written,"source Move prefix differs");
    type=2;check(original_motion_focus_prefix(OriginalMotionFocusPrefix::idle,flags,type,1,prefix,error)&&
        flags==0x23c1u&&type==2u&&!prefix.flags_written,"suppressed Idle overwrote Move flags/type");
    check(original_motion_focus_prefix(OriginalMotionFocusPrefix::idle,flags,type,0,prefix,error)&&
        flags==0x2380u&&type==2u&&prefix.flags_written&&!prefix.movement_type_written,"Idle prefix altered movement type");
    check(original_motion_policy(flags,resolved,decoded,error)&&decoded.rotation_speed==2&&
        decoded.decoded.position_from_visual==0&&decoded.decoded.position_from_physics==0&&
        decoded.decoded.visual_with_rotation==1&&decoded.decoded.update_path==1&&decoded.decoded.validate_floor==1,
        "source Idle decode/rotation percent differs");
    const auto retained_flags=flags,retained_type=type;const auto retained_output=prefix;
    check(!original_motion_focus_prefix(static_cast<OriginalMotionFocusPrefix>(91),flags,type,0,prefix,error)&&
        flags==retained_flags&&type==retained_type&&prefix.flags_written==retained_output.flags_written,
        "unsupported focus mutated fields/output");
    check(!original_motion_focus_prefix(OriginalMotionFocusPrefix::move,flags,flags,0,prefix,error)&&
        flags==retained_flags,"aliased source cells accepted");
    check(original_motion_source_set_flags(flags,0x2341,error)&&original_motion_policy(flags,resolved,decoded,error)&&
        decoded.decoded.position_from_visual==1&&decoded.decoded.position_from_physics==0&&
        decoded.decoded.update_path==0&&decoded.decoded.validate_floor==0,"explicit source setter lost raw flags");
    check(original_motion_source_set_flags(flags,0x2341|0x20,error)&&original_motion_policy(flags,resolved,decoded,error)&&
        decoded.rotation_speed==-1,"actual source instant-rotation override lost");
    resolved[47]=-25600;check(original_motion_source_set_flags(flags,0x23c1,error)&&
        original_motion_policy(flags,resolved,decoded,error)&&decoded.rotation_speed==0,"original nonpositive rotation clamp differs");
    check(original_motion_source_set_flags(flags,0,error)&&flags.has_value()&&
        original_motion_policy(flags,resolved,decoded,error),"actual flags0 incorrectly treated as unknown");
    std::optional<std::uint32_t> gate;OriginalAttackFocusPrefixResult attack_prefix;
    unsigned delay_queries=0;
    check(original_motion_attack_focus_prefix(flags,gate,[&](std::uint32_t& delay,std::string&){
        check(flags==0x2341u,"Attack delay queried before original flag store");++delay_queries;delay=0;return true;},attack_prefix,error)&&
        flags==0x2341u&&!gate&&attack_prefix.flags_written&&!attack_prefix.attack_gate_written,
        "zero delay Attack focus read/created unknown gate");
    check(!original_motion_attack_focus_prefix(flags,gate,[](std::uint32_t& delay,std::string&){delay=7;return true;},
        attack_prefix,error)&&flags==0x2341u&&attack_prefix.flags_written&&!gate,"reached missing attack gate rolled back flags or fabricated gate");
    gate=6;check(original_motion_attack_focus_prefix(flags,gate,[](std::uint32_t& delay,std::string&){delay=7;return true;},
        attack_prefix,error)&&gate==7u&&attack_prefix.attack_gate_written,"Attack gate OR lost other source bits");
    check(!original_motion_attack_focus_prefix(flags,gate,{},attack_prefix,error)&&flags==0x2341u&&
        attack_prefix.flags_written,"missing reached delay owner rolled back source store");
    std::optional<std::uint32_t> suppressed=255;OriginalMotionBlurResult blur;
    OriginalMotionBlurServices callbacks;std::vector<std::string> calls;bool body_present=true;
    callbacks.stop=[&](std::string&){calls.push_back("stop");return true;};
    callbacks.body_present=[&](bool& present,std::string&){calls.push_back("body");present=body_present;return true;};
    callbacks.pin=[&](std::string&){calls.push_back("pin");return true;};
    const auto flags_before_blur=flags;
    check(original_motion_blur_body(OriginalMotionBlurBody::move,suppressed,callbacks,blur,error)&&
        calls==std::vector<std::string>{"stop","body","pin"}&&flags==flags_before_blur&&suppressed==255u,
        "Move blur order changed flags/suppression or skipped actual Stop");
    calls.clear();callbacks.stop=[&](std::string&){calls.push_back("stop");body_present=false;return true;};
    check(original_motion_blur_body(OriginalMotionBlurBody::move,suppressed,callbacks,blur,error)&&
        calls==std::vector<std::string>{"stop","body"},"Move blur used stale body presence before Stop");
    calls.clear();unsigned stop_effects=0;
    callbacks.stop=[&](std::string&){calls.push_back("stop");++stop_effects;return true;};callbacks.body_present={};
    check(!original_motion_blur_body(OriginalMotionBlurBody::move,suppressed,callbacks,blur,error)&&stop_effects==1&&
        blur.phase==OriginalMotionBlurPhase::body_query&&blur.service_calls==1,"missing body getter rolled back source Stop/failed phase");
    check(original_motion_blur_body(OriginalMotionBlurBody::idle,suppressed,{},blur,error)&&suppressed==0u&&
        blur.idle_suppressed_written&&blur.service_calls==0&&flags==flags_before_blur,"Idle blur altered motion flags or required unused callbacks");
    calls.clear();unsigned attack_delay_call=0;
    callbacks.attack_delay=[&](std::uint32_t& delay,std::string&){calls.push_back("delay");delay=++attack_delay_call==1?31:7;return true;};
    callbacks.start_timer=[&](std::uint32_t delay,std::uint32_t repeat,std::uint32_t event,std::uintptr_t payload,std::string&){
        check(delay==7&&repeat==0&&event==0x2a&&payload==0,"Attack blur timer used captured first delay/wrong params");calls.push_back("timer");body_present=true;return true;};
    callbacks.body_present=[&](bool& present,std::string&){calls.push_back("body");present=body_present;return true;};
    check(original_motion_blur_body(OriginalMotionBlurBody::attack,suppressed,callbacks,blur,error)&&
        calls==std::vector<std::string>{"delay","delay","timer","body","pin"}&&blur.service_calls==5,
        "Attack blur did not re-read delay before timer/body/pin");
    calls.clear();callbacks.attack_delay=[&](std::uint32_t& delay,std::string&){calls.push_back("delay");delay=0;return true;};callbacks.start_timer={};body_present=false;
    check(original_motion_blur_body(OriginalMotionBlurBody::attack,suppressed,callbacks,blur,error)&&
        calls==std::vector<std::string>{"delay","body"},"zero delay/absent body required unused timer/pin owners");
    calls.clear();unsigned timer_effects=0;attack_delay_call=0;
    callbacks.attack_delay=[&](std::uint32_t& delay,std::string&){calls.push_back("delay");delay=++attack_delay_call==1?31:0;return true;};
    callbacks.start_timer=[&](std::uint32_t delay,std::uint32_t repeat,std::uint32_t event,std::uintptr_t payload,std::string&){
        check(delay==0&&repeat==0&&event==0x2a&&payload==0,"second delay0 incorrectly skipped source timer branch");++timer_effects;calls.push_back("timer");return true;};
    callbacks.body_present={};
    check(!original_motion_blur_body(OriginalMotionBlurBody::attack,suppressed,callbacks,blur,error)&&timer_effects==1&&
        calls==std::vector<std::string>{"delay","delay","timer"}&&blur.phase==OriginalMotionBlurPhase::body_query,
        "Attack failure rolled back reached timer prefix");
    calls.clear();callbacks.attack_delay=[&](std::uint32_t& delay,std::string&){calls.push_back("delay");delay=0;return true;};
    callbacks.body_present=[&](bool& present,std::string&){calls.push_back("body");present=true;return true;};callbacks.pin={};
    check(!original_motion_blur_body(OriginalMotionBlurBody::attack,suppressed,callbacks,blur,error)&&
        calls==std::vector<std::string>{"delay","body"}&&blur.phase==OriginalMotionBlurPhase::pin&&blur.service_calls==2,
        "present body silently skipped missing pin owner");
    std::cout<<"source_motion_flags v14 PASS: Attack focus prefix and actual blur callbacks/re-reads/partial failures verified\n";
    std::cout<<"source_motion_flags PASS: actual source prefixes, Idle suppression, unknown versus zero, exact policy/rotation, atomic unsupported/alias rejection\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
