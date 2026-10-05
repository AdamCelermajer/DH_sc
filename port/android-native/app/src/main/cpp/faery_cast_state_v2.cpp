#include "faery_cast_state_v2.hpp"
namespace {using namespace dh2::android_ui;
bool call(dh2::character::State* state,std::uintptr_t character,const CastStateServicesV2* services,unsigned op,unsigned value){CastStateRequestV2 q{op,value,character};return !services->invoke(services->context,state,&q);}
}
extern "C" int dh2_faery_ctrl_allowed_v2(const dh2::character::State* state){return !state?-1:(state->attack_gate&4)?0:(state->attack_gate&2)?0:1;}
extern "C" int dh2_faery_command_allowed_v2(const dh2::character::ControllerCommandState32* state){return !state?-1:state->forced?1:(state->global_blocked||state->locked)?0:1;}
extern "C" int dh2_faery_cast_body_v2(dh2::character::State* state,std::uintptr_t character,unsigned op,const CastStateServicesV2* services){
 if(!state||!character||op>3||!services||!services->invoke)return -1;
 if(op>=2)return 0;
 if(op==1)return call(state,character,services,cast_raise_v2,0x21)?0:-2;
 state->flags=0x6301;
 if(!call(state,character,services,cast_raise_v2,0x20)||!call(state,character,services,cast_animation_v2,0xffffffffu)||!call(state,character,services,cast_speed_v2,0x3f800000)||!call(state,character,services,cast_heading_v2,0)||!call(state,character,services,cast_cancel_sneaking_v2,0))return -2;
 return 0;
}
extern "C" int dh2_faery_cast_step_v2(dh2::character::State* state,dh2::character::skills::SkillAIStateV3* fields,std::uintptr_t character,unsigned begin,unsigned step,unsigned mode,const CastStateServicesV2* services){
 if(!state||!fields||fields->reserved||!character||begin>1||!services||!services->invoke)return -1;
 if(step||mode!=1)return 0;
 if(begin)fields->continued=1;
 if(fields->last&&!call(state,character,services,cast_stop_loop_v2,1))return -2;
 return 0;
}
