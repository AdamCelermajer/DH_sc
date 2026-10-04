#include "character_skill_context_v4.hpp"
namespace dh2::character::skills {
CharacterSkillContextV4::CharacterSkillContextV4(CharacterPlayerSkillsV3& p,
 NativeFsm24& f,CharacterAnimationInstance& a,SkillStateV4& s,
 const SkillStateServices16V4& required):player_(p),fsm_(f),animation_(a),fields_(s),required_(required){}
bool CharacterSkillContextV4::coherent()const noexcept {
 return fsm_.state&&fields_.state==fsm_.state&&fields_.character==fsm_.character&&
  fsm_.character&&!fsm_.reserved&&fsm_.current_present<=1;
}
int CharacterSkillContextV4::service(void* context,SkillStateV4* fields,
 const SkillStateRequest32V4* q,SkillStateResponse16V4* r){
 auto& self=*static_cast<CharacterSkillContextV4*>(context);
 if(fields!=&self.fields_||!self.coherent())return -1;
 switch(q->operation){
 case skill_state_current_v4:{
  std::int32_t id;
  if(dh2_character_native_fsm_get_integer(&id,&self.fsm_,0)!=1)return -1;
  r->word=static_cast<std::uint32_t>(id);return 0;
 }
 case skill_state_timer_v4:
  return self.player_.session().start_timer(q->value,0,static_cast<std::int32_t>(q->index))<0?-1:0;
 case skill_state_use_v4:{
  std::uint32_t result=0;
  return self.player_.skill_ai(skill_ai_use_v3,0,&result);
 }
 default:
  return self.required_.invoke?self.required_.invoke(self.required_.context,fields,q,r):-1;
 }
}
int CharacterSkillContextV4::execute(std::uint32_t operation,std::uint32_t index,
 std::uint32_t moving,std::uintptr_t payload,std::uint32_t force){
 if(!coherent())return status_=-1;
 const SkillStateServices16V4 services{this,service};
 status_=dh2_character_skill_state_v4(&fields_,operation,index,moving,payload,force,&services);
 if(status_==0&&!coherent())status_=-2;
 return status_;
}
int CharacterSkillContextV4::animation_event(const actor::BlendedPlaybackEvent& event){
 if(event.event.reserved)return status_=-1;
 if(event.event.handoff.event_id!=0x28)return status_=0;
 return execute(skill_state_ai_event_v4,0,0,
  reinterpret_cast<std::uintptr_t>(event.event.handoff.payload));
}
}
