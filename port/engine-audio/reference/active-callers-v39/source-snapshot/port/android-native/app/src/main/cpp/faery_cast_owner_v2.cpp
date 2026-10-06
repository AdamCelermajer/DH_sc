#include "faery_cast_owner_v2.hpp"
#include "faery_cast_state_v2.hpp"
#include <cstring>
namespace dh2::android_ui {
namespace {bool same(const model_renderer::PlayerGameplayBinding& p){return p.active&&p.character&&p.state&&p.state_owner&&p.skills&&p.save&&&p.state_owner->state()==p.state&&p.state_owner->native_fsm().character==p.character&&p.skills->state().owner==p.character&&p.save->character()==p.character&&p.skills->native_savegame()==p.save;}}
int FaeryCastOwnerV2::selected(const model_renderer::PlayerGameplayBinding& p,int& id,unsigned& type){
 if(!same(p)||p.difficulty<0||p.difficulty>2){error_="Same player cast authority unavailable";return -1;}
 id=p.save->current_faery(unsigned(p.difficulty));
 // The retained player CurrentSpell validation resolves original Faery list
 // and asserts Type==index. Its public same-session native binding is already
 // registered, but raw row access stays an exact source table requirement.
 if(id<0||id>=5){error_="Original selected faery assertion domain invalid";return -1;}
 if(!p.properties||!p.properties->resolved){error_="Actual FaeryList backing unavailable";return -1;}
 if(!services_.faery_type||services_.faery_type(services_.context,p.character,unsigned(id),&type)){error_="Original selected GetCharFaery SpellType unavailable";return -2;}
 return 0;
}
int FaeryCastOwnerV2::network(const model_renderer::PlayerGameplayBinding& p,bool origin,unsigned operation,unsigned id){
 bool enabled=false;if(!services_.network_enabled||services_.network_enabled(services_.context,&enabled)){error_="Actual network-enabled source field unavailable";return -2;}
 if(!enabled||origin)return 0;
 if(!services_.network_spell||services_.network_spell(services_.context,p.character,operation,id)){error_="Original network spell command unavailable";return -2;}return 0;
}
int FaeryCastOwnerV2::controller_allowed(const model_renderer::PlayerGameplayBinding& p,bool* out){if(!same(p)||!out)return -1;*out=dh2_faery_ctrl_allowed_v2(p.state)==1;return 0;}
int FaeryCastOwnerV2::can_begin_casting(const model_renderer::PlayerGameplayBinding& p,bool* out){auto code=controller_allowed(p,out);if(code||!*out)return code;return faery_spell_usable_v1(p,out,error_);}
int FaeryCastOwnerV2::begin(const model_renderer::PlayerGameplayBinding& p,bool origin,bool* accepted){
 if(!accepted)return -1;*accepted=false;int id;unsigned type;auto code=selected(p,id,type);if(code)return code;
 bool usable=false;code=faery_spell_usable_v1(p,&usable,error_);if(code||!usable)return code;
 auto& flags=p.skills->skill_ai();flags.continued=flags.last=0;
 // SM_SetCastState first validates CharacterModel and its CastAnim list. The
 // provider returns1 for actual out-of-range model/list source no-op,0 for
 // an authored animation; all other returns are required failures.
 int animation=-1;if(!services_.cast_animation){error_="Original CharacterModel CastAnim list unavailable";return -2;}
 auto result=services_.cast_animation(services_.context,p.character,unsigned(id),&animation);
 if(result!=0&&result!=1){error_="Original CharacterModel CastAnim delivery failed";return -2;}
 if(!result){
  int constant;if(p.skills->session().constant("AnimStancedAnim","SL__LIST_IPHONE",constant)){error_="Original AnimStancedAnim constant unavailable";return -2;}
  unsigned stance=unsigned(constant)&0x400000u;
  if(stance){if(!services_.stance||services_.stance(services_.context,p.character,&stance)){error_="Original Character stance unavailable";return -2;}}
  const auto bits=unsigned(animation)+stance;std::memcpy(&p.state->animation_override,&bits,4);
  if(!services_.state_services){error_="Same StateOwner cast transition services unavailable";return -2;}
  auto state=p.state_owner->event(50006,0,*services_.state_services);if(state<0){error_="Original cast-state event failed after animation selection";return -2;}
 }
 code=network(p,origin,3,unsigned(id));if(code)return code;
 *accepted=p.state->current==7;return 0;
}
int FaeryCastOwnerV2::end(const model_renderer::PlayerGameplayBinding& p,bool origin){
 if(!same(p))return -1;if(p.state->current!=7)return 0;
 int id;unsigned type;auto code=selected(p,id,type);if(code)return code;
 // SpellType2 continuous handling is resolved by the source-row successor
 // below. Type0/1 have no EndSpell state effects.
 if(type!=2)return 0;
 auto& flags=p.skills->skill_ai();if(!flags.continued)flags.last=1;
 else if(!services_.animation_stop_loop||services_.animation_stop_loop(services_.context,p.character,true)){error_="Original Animator StopLoop unavailable";return -2;}
 return network(p,origin,4,0);
}
int FaeryCastOwnerV2::command(const model_renderer::PlayerGameplayBinding& p,bool start,bool origin){
 if(!same(p))return -1;
 if(dh2_faery_command_allowed_v2(&p.controller)!=1)return 0;
 if(start){bool accepted;return begin(p,origin,&accepted);}return end(p,origin);
}
int FaeryCastOwnerV2::state_body(const model_renderer::PlayerGameplayBinding& p,CastBodyV2 body){
 if(!same(p))return -1;
 CastStateServicesV2 source{this,[](void* c,dh2::character::State*,const CastStateRequestV2* q){
  auto& owner=*static_cast<FaeryCastOwnerV2*>(c);auto& s=owner.services_;int result=-1;
  switch(q->operation){
   case cast_raise_v2:if(s.raise_event)result=s.raise_event(s.context,q->character,q->value,0);break;
   case cast_animation_v2:if(s.animation)result=s.animation(s.context,q->character,-1);break;
   case cast_speed_v2:if(s.speed)result=s.speed(s.context,q->character,1.f);break;
   case cast_heading_v2:if(s.disable_heading)result=s.disable_heading(s.context,q->character);break;
   case cast_cancel_sneaking_v2:if(s.cancel_sneaking)result=s.cancel_sneaking(s.context,q->character);break;
  }
  if(result)owner.error_="Original cast state service failed: "+std::to_string(q->operation);return result;
 }};
 return dh2_faery_cast_body_v2(p.state,p.character,unsigned(body),&source);
}
int FaeryCastOwnerV2::character_event(const model_renderer::PlayerGameplayBinding& p,unsigned event){
 if(!same(p)||p.difficulty<0||p.difficulty>2||(event!=0x20&&event!=0x21))return -1;
 auto selected=p.save->current_faery(unsigned(p.difficulty));const auto& slots=p.skills->state().spells;
 if(selected<0||unsigned(selected)>=slots.count||!slots.items||!slots.items[selected])return 0;
 unsigned ignored=0;
 return faery_spell_callback_v1(p,event==0x20?0:2,&ignored,error_);
}
int FaeryCastOwnerV2::animation_event(const model_renderer::PlayerGameplayBinding& p,const char* name){
 if(!same(p)||!name)return -1;if(p.state->current!=7||std::strcmp(name,"do_spell"))return 0;
 if(p.difficulty<0||p.difficulty>2)return -1;
 const auto selected=p.save->current_faery(unsigned(p.difficulty));const auto& slots=p.skills->state().spells;
 if(selected<0||unsigned(selected)>=slots.count||!slots.items||!slots.items[selected])return 0;
 unsigned ignored=0;return faery_spell_callback_v1(p,1,&ignored,error_);
}
int FaeryCastOwnerV2::animation_step(const model_renderer::PlayerGameplayBinding& p,bool start,unsigned step,unsigned mode){
 if(!same(p))return -1;if(step||mode!=1)return 0;
 auto& flags=p.skills->skill_ai();if(start)flags.continued=1;
 if(!flags.last)return 0;
 if(!services_.animation_stop_loop||services_.animation_stop_loop(services_.context,p.character,true)){error_="Original deferred Animator StopLoop failed";return -2;}return 0;
}
}
