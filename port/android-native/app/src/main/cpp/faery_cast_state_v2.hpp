#pragma once
#include "character_state.hpp"
#include "character_controller_commands.hpp"
#include "character_skill_ai_v3.hpp"
namespace dh2::android_ui {
enum CastStateServiceV2:unsigned {cast_raise_v2=1,cast_animation_v2,cast_speed_v2,cast_heading_v2,cast_cancel_sneaking_v2,cast_stop_loop_v2};
struct CastStateRequestV2 {unsigned operation,value;std::uintptr_t character;};
struct CastStateServicesV2 {void* context;int(*invoke)(void*,dh2::character::State*,const CastStateRequestV2*);};
}
extern "C" {
int dh2_faery_ctrl_allowed_v2(const dh2::character::State*);
int dh2_faery_command_allowed_v2(const dh2::character::ControllerCommandState32*);
// Source semantic body only; profiler calls excluded. op0Focus,1Blur,
//2OnEvent,3OnUpdate. Same State pointer remains live during callbacks.
int dh2_faery_cast_body_v2(dh2::character::State*,std::uintptr_t,unsigned,
 const dh2::android_ui::CastStateServicesV2*);
// Same shared skill/spell +d0/+d1. Actual Animator step/mode supplied.
int dh2_faery_cast_step_v2(dh2::character::State*,dh2::character::skills::SkillAIStateV3*,std::uintptr_t,unsigned begin,unsigned step,unsigned mode,const dh2::android_ui::CastStateServicesV2*);
}
