#include "character_skill_state_v4.hpp"
#include <cstring>
namespace {using namespace dh2::character::skills;
bool aligned(const void* p,std::size_t n){return p&&reinterpret_cast<std::uintptr_t>(p)%n==0;}
struct Run {
 SkillStateV4& s;const SkillStateServices16V4& cb;
 bool valid()const{return aligned(s.state,alignof(dh2::character::State))&&s.character&&!s.reserved[0]&&!s.reserved[1];}
 bool call(unsigned op,unsigned value=0,unsigned index=0,std::uintptr_t subject=0,std::uintptr_t payload=0,SkillStateResponse16V4* answer=nullptr){
  if(!valid())return false;SkillStateResponse16V4 r{};const SkillStateRequest32V4 q{op,value,index,0,subject?subject:s.character,payload};
  if(cb.invoke(cb.context,&s,&q,&r)||!valid()||r.reserved)return false;if(answer)*answer=r;return true;
 }
 bool debug(const char* name="isTracingCharState"){SkillStateResponse16V4 r;if(!call(skill_state_debug_load_v4)||!call(skill_state_debug_construct_v4,0,0,0,reinterpret_cast<std::uintptr_t>(name),&r)||!r.identity)return false;
  const auto captured=r.identity;return call(skill_state_debug_get_v4,0,0,captured)&&call(skill_state_debug_destroy_v4,0,0,captured);
 }
 bool monster(bool clear){SkillStateResponse16V4 r;if(!call(skill_state_monster_v4,0,0,0,0,&r))return false;if(!r.word)return true;
  if(!call(skill_state_miniboss_v4,0,0,0,0,&r))return false;if(r.word)return true;
  if(!call(skill_state_boss_v4,0,0,0,0,&r))return false;if(!r.word){if(clear)s.state->flags&=~0x10000u;else s.state->flags|=0x10000u;}return true;
 }
 bool focus(){if(!debug())return false;s.state->flags=0x6341;s.state->attack_gate&=~0x140u;
  if(!call(skill_state_raise_v4,0x1e)||!call(skill_state_animation_v4,UINT32_MAX)||!call(skill_state_speed_v4,0x3f800000))return false;
  s.heading_enabled=0;if(!call(skill_state_cancel_sneaking_v4))return false;
  const auto physical=s.physical;if(s.moving)s.state->attack_gate|=0x100u;
  if(physical&&!call(skill_state_unpin_v4,0,0,physical))return false;return monster(false);
 }
 bool blur(){if(!debug())return false;s.last_target=s.target;
  if(!call(skill_state_stop_v4)||!call(skill_state_raise_v4,0x1f))return false;
  if(s.state->attack_gate&0x100){if(!call(skill_state_timer_v4,10,0x30))return false;}
  else {const auto physical=s.physical;if(physical&&!call(skill_state_pin_v4,0,0,physical))return false;}return monster(true);
 }
 bool select(unsigned index,unsigned moving,std::uintptr_t payload,unsigned force){SkillStateResponse16V4 r;
  if(!call(skill_state_row_v4,0,index,0,0,&r)||!aligned(reinterpret_cast<const void*>(r.identity),alignof(dh2::data::SkillProjection76)))return false;
  const auto animation=reinterpret_cast<const dh2::data::SkillProjection76*>(r.identity)->words[1];
  if(!call(skill_state_constant_v4,0,0,0,0,&r))return false;unsigned stance=r.word&0x200000u;
  if(stance){if(!call(skill_state_stance_v4,0,0,0,0,&r))return false;stance=r.word;}
  const auto word=animation+stance;std::memcpy(&s.state->animation_override,&word,4);s.index=index;s.moving=static_cast<std::uint8_t>(moving);
  return call(force?skill_state_transition_v4:skill_state_state_event_v4,50005,force?6:0,0,payload);
 }
 bool animation_event(std::uintptr_t payload){if(!payload)return false;SkillStateResponse16V4 r;
  if(!call(skill_state_step_index_v4,0,0,0,0,&r))return false;const auto step=r.word;
  if(!call(skill_state_step_count_v4))return false;const auto* text=reinterpret_cast<const char*>(payload);
  const char* prefixes[]={"ev_","an_","fx_","sfx_"};
  for(unsigned i=0;i<4;++i){const auto n=i==3?4u:3u;if(!std::strncmp(text,prefixes[i],n))return call(skill_state_named_prefix_v4,i,step,0,payload+n);}
  if(!call(skill_state_current_v4,0,0,0,0,&r))return false;
  if(r.word==6)return std::strcmp(text,"do_skill")|| (debug("isTracingCharAI_AnimEvent")&&call(skill_state_use_v4));
  if(r.word==5||r.word==7||r.word==13)return call(skill_state_other_event_v4,r.word,step,0,payload);
  return true;
 }
};}
extern "C" int dh2_character_skill_state_v4(SkillStateV4* s,std::uint32_t op,std::uint32_t index,std::uint32_t moving,std::uintptr_t payload,std::uint32_t force,const SkillStateServices16V4* cb){
 if(!aligned(s,alignof(SkillStateV4))||!aligned(cb,alignof(SkillStateServices16V4))||!cb->invoke||op>skill_state_ai_event_v4||moving>255||force>1)return -1;Run run{*s,*cb};if(!run.valid())return -1;
 bool ok=true;switch(op){case skill_state_focus_v4:ok=run.focus();break;case skill_state_blur_v4:ok=run.blur();break;case skill_state_event_v4:
  if(index==0x28){if(!payload)return -2;if(!std::strcmp(reinterpret_cast<const char*>(payload),"is_stoppable"))s->state->flags|=0x8000u;}break;
 case skill_state_select_v4:ok=run.select(index,moving,payload,force);break;case skill_state_ai_event_v4:ok=run.animation_event(payload);break;default:break;}return ok?0:-2;
}
