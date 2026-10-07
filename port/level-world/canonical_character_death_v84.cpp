#include "canonical_character_death_v84.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "character_ais_death_vm_v2.hpp"
#include "character_ais_death_vm_v56.hpp"
#include <algorithm>
#include <stdexcept>
namespace dh2::character {
ScriptLifecycleState64* CanonicalCharacterDeathV84::lifecycle()noexcept{
 if(record_.player_script_owner_v62)return &record_.player_script_owner_v62->session().owner().lifecycle();
 return record_.actor&&record_.actor->session?&record_.actor->session->owner().lifecycle():nullptr;
}
data::PropertyView* CanonicalCharacterDeathV84::properties()noexcept{
 if(record_.player_script_owner_v62)return &record_.player_script_owner_v62->session().property_view();
 return record_.actor&&record_.actor->session?&record_.actor->session->property_view():nullptr;
}
bool CanonicalCharacterDeathV84::constant(const char* group,const char* key,std::int32_t& value,std::string& e){
 auto design=record_.design.design();if(!design||!design->lookup||design->lookup(design->context,0,group,key,&value)){e="Required actual canonical death constant";return false;}e.clear();return true;
}
int CanonicalCharacterDeathV84::dead_constant(void* raw,const char* group,const char* key,int* out){auto& self=*static_cast<CanonicalCharacterDeathV84*>(raw);return out&&self.constant(group,key,*out,self.error_)?0:-1;}
int CanonicalCharacterDeathV84::dead_stance(void* raw,std::uintptr_t id,int* out){auto& self=*static_cast<CanonicalCharacterDeathV84*>(raw);
 if(!out||!self.record_.actor||id!=self.record_.actor->object->identity){self.error_="Death stance receiver changed";return -1;}
 if(!self.services_.stance){self.error_="Required actual Character.GetAnimStance";return -1;}return self.services_.stance(*out,self.error_)?0:-1;
}
bool CanonicalCharacterDeathV84::drop(std::uintptr_t& handle,std::string& e){
 if(!handle){e.clear();return true;}std::shared_ptr<void> pin;fx::CharacterMeshFxOwnerV4* manager{};
 if(!services_.fx||!services_.fx(pin,manager,e)||!pin||!manager){if(e.empty())e="Required SAME positive canonical death FX manager";return false;}return manager->drop(handle,e);
}
CanonicalCharacterDeathV84::CanonicalCharacterDeathV84(world::CanonicalCharacterCandidateRecordV60& record,
 CanonicalCharacterDeathServicesV84 services):record_(record),services_(std::move(services)){
 auto* a=record_.actor.get();auto* script=lifecycle();auto* view=properties();
 if(!a||!a->object||!a->machine||!view||!script||!record_.properties||!record_.services.design||!services_.provider||!services_.aggro.actor||!services_.aggro.on_deaggro||
  !a->source_aggro_v84()||view->resolved!=record_.properties->resolved.data()||a->machine->native_fsm().character!=a->object->identity)
  throw std::invalid_argument("Required SAME canonical Character death ownership graph");
 TimerStore32* timers=record_.player_script_owner_v62?&record_.player_script_owner_v62->session().timers():&a->session->timers();
 actor_={a->object->identity,reinterpret_cast<std::uintptr_t>(&a->machine->native_fsm()),timers};
 death_={a->ai_events.ai,&actor_,&a->object->target,a->source_ai_group34,0,nullptr,UINT32_MAX,UINT32_MAX,0};
 selector_=std::make_unique<NpcDeadStateV1>(NpcDeadStateBorrowV1{a->machine.get(),view,record_.services.animation_tables,this,dead_constant,dead_stance});
 recalc_=std::make_unique<CharacterMenuRecalcOwnerV1>(record_.services.design->borrow(),record_.properties,*view);
 auto* relations=a->source_aggro_v84();AggroClearActorBorrowV2 aggro{a->object->identity,relations->outgoing(),relations->incoming(),&a->object->binding};
 CharacterDeathServicesV56 source;source.aggro=services_.aggro;source.group=services_.group;
 source.ais=[this](auto receiver,auto method,auto attacker,const auto* scope,auto& e){
  if(record_.player_script_owner_v62)return character_ais_death_vm_v56(record_.player_script_owner_v62->session().owner(),receiver,method,attacker,scope,e)==0;
  if(!record_.actor->session){e="Released SAME NPC death VM";return false;}return character_ais_death_vm_v2(record_.actor->session->owner(),receiver,method,attacker,scope,e)==0;
 };
 source.set_dead=[this](auto mode,auto payload,auto force,auto& e){const auto result=selector_->set(mode,payload,force);if(result<0){e=selector_->error();return false;}e.clear();return true;};
 source.cleanup_skills=[this](auto& e){
  if(record_.player_script_owner_v62){if(record_.player_script_owner_v62->cleanup_skills()==1)return true;e=record_.player_script_owner_v62->error();return false;}
  if(!record_.npc_skills_v84){e="Required actual NPC C1 SkillOwner/InitSkills receiver for cleanup";return false;}
  if(record_.npc_skills_v84->cleanup_skills()==1)return true;e=record_.npc_skills_v84->error();return false;
 };
 source.cleanup_spells=[this](auto& e){
  if(record_.player_script_owner_v62){if(record_.player_script_owner_v62->cleanup_spells()==1)return true;e=record_.player_script_owner_v62->error();return false;}
  if(!record_.npc_skills_v84){e="Required actual NPC C1 Spell vector owner for cleanup";return false;}
  if(record_.npc_skills_v84->cleanup_spells()==1)return true;e=record_.npc_skills_v84->error();return false;
 };
 owner_=std::make_unique<CharacterDeathOwnerV56>(CharacterDeathBorrowV56{&death_,&a->ai_events,&a->object->binding,aggro,script},std::move(source));
 previous_body_=a->bodies;a->bodies={this,body};
}
CanonicalCharacterDeathV84::~CanonicalCharacterDeathV84(){
 auto* a=record_.actor.get();if(!a)return;
 if(a->bodies.context==this&&a->bodies.invoke==body)a->bodies=previous_body_;
 if(a->ai_events.ais_virtuals==ais_keys_.data())a->ai_events.ais_virtuals=previous_ais_;
}
bool CanonicalCharacterDeathV84::on_died(std::uintptr_t attacker,const dh2_script_callback_scope* scope,std::string& e){
 auto* a=record_.actor.get();auto* script=lifecycle();if(!a||!script){e="Released canonical death actor/session";return false;}
 a->ai_events.active=script->active;death_.group=a->source_ai_group34;
 if(a->ai_events.active){std::uint32_t method{};bool found=record_.player_script_owner_v62?
  record_.player_script_owner_v62->session().owner().source_death_callback(method):a->session->owner().source_death_callback(method);
  if(!found||!a->ai_events.ais_virtuals){e="Required actual selected canonical AIS death callable/table";return false;}
  if(a->ai_events.ais_virtuals!=ais_keys_.data())previous_ais_=a->ai_events.ais_virtuals;
  std::copy_n(a->ai_events.ais_virtuals,ais_keys_.size(),ais_keys_.begin());ais_keys_[0x24/4]=method;a->ai_events.ais_virtuals=ais_keys_.data();
 }
 AIDeathResult24 result{};if(owner_->on_died(result,attacker,scope)!=1){e=owner_->error();return false;}e.clear();return true;
}
int CanonicalCharacterDeathV84::event(void* raw,AIEventState64* state,const AIEventRequest40* q,std::uint32_t*){
 auto& self=*static_cast<CanonicalCharacterDeathV84*>(raw);auto& a=*self.record_.actor;
 if(!q||state!=&a.ai_events||q->event!=2){self.error_="Wrong canonical death event receiver";return -1;}
 if(q->service==ai_event_virtual&&q->operation==0x24&&q->callee==0x3d1000&&q->subject==a.ai_events.ai)
  return self.on_died(q->payload,self.scope_,self.error_)?0:-1;
 if(q->service==ai_event_state_event&&a.ai_events.owner&&q->subject==a.ai_events.owner->state_machine&&q->subject==reinterpret_cast<std::uintptr_t>(&a.machine->native_fsm())){
  const auto result=a.machine->event(2,q->payload);if(result<0)self.error_=a.machine->error();return result<0?-1:0;
 }
 self.error_="Required canonical death AI event selector "+std::to_string(q->service);return -1;
}
bool CanonicalCharacterDeathV84::raise(std::uintptr_t attacker,const dh2_script_callback_scope* scope,std::string& e){
 if(scope&&!dh2_script_callback_scope_valid(scope)){e="Expired actual canonical death callback scope";return false;}
 const auto* previous=scope_;scope_=scope;struct Restore{const dh2_script_callback_scope*& slot;const dh2_script_callback_scope* previous;~Restore(){slot=previous;}}restore{scope_,previous};
 auto& a=*record_.actor;auto* script=lifecycle();if(!script){e="Required actual death lifecycle";return false;}a.ai_events.active=script->active;
 const AIEventServices24 services{this,event,63,0};const AIEventPayload24 payload{attacker,0,0,0};AIEventResult16 result{};
 error_.clear();if(dh2_character_ai_event(&result,&a.ai_events,2,&payload,&services)){e=error_.empty()?"Required canonical RaiseAIEvent2 continuation":error_;return false;}e.clear();return true;
}
bool CanonicalCharacterDeathV84::set_dead(std::string& e){
 auto* a=record_.actor.get();auto* script=lifecycle();
 if(!a||!script||!owner_){e="Released SAME canonical AI_SetDead owner";return false;}
 a->ai_events.active=script->active;death_.group=a->source_ai_group34;
 AIDeathResult24 result{};if(owner_->set_dead(result)!=1){e=owner_->error();return false;}
 e.clear();return true;
}
bool CanonicalCharacterDeathV84::source_set_dead_state_v115(bool mode,std::uintptr_t payload,bool force,std::string& e){
 if(!selector_){e="Required SAME existing source death selector";return false;}
 if(selector_->set(mode,payload,force)<0){e=selector_->error();return false;}e.clear();return true;
}
void CanonicalCharacterDeathV84::body(void* raw,State* state,const Request* q){
 auto& self=*static_cast<CanonicalCharacterDeathV84*>(raw);if(!q)throw std::runtime_error("Missing canonical death body request");
 const auto result=self.source_body(state,*q,self.error_);if(result<0)throw std::runtime_error(self.error_);
 if(!result){if(!self.previous_body_.invoke)throw std::runtime_error("Required original canonical state body continuation");self.previous_body_.invoke(self.previous_body_.context,state,q);}
}
int CanonicalCharacterDeathV84::source_body(State* state,const Request& q,std::string& e){
 auto& a=*record_.actor;if(!a.machine||state!=&a.machine->state()){e="Wrong SAME canonical death FSM body";return -1;}
 bool okay{};switch(q.service){
 case remove_highlight:okay=drop(a.source_highlight14a0(),e);break;
 case disable_state_fx:okay=drop(a.source_state_fx148c(),e);break;
 case disable_self_fx:okay=drop(a.source_self_fx1484(),e);break;
 case set_death_filter:case reset_filter:
  // The original nullable physical2dc guard returns without a shape call.
  if(!a.position_fields_v7().physical2dc){e.clear();return 1;}
  if(!record_.physical_owner_v62){e="Required SAME positive Character physical2dc owner";return -1;}
  okay=q.service==set_death_filter?record_.physical_owner_v62->source_death_filter_v84(
   std::int16_t(q.argument[0]),std::uint16_t(q.argument[1]),std::uint16_t(q.argument[2])):record_.physical_owner_v62->source_reset_filter_v84();
  if(!okay)e=record_.physical_owner_v62->error();break;
 case remove_buffs:{
  auto* view=properties();if(!view){e="Required actual death property group map";return -1;}
  //3e0c1c..24 recalculates even when source map e18 starts empty.
  if(!view->group_count)return recalc_->recalculate(*view,1,e)?1:-1;
  BuffOwner* buffs=record_.player_script_owner_v62?record_.player_script_owner_v62->native_buffs():nullptr;
  if(!buffs&&services_.buffs&&!services_.buffs(buffs,e))return -1;
  if(!buffs){e="Required SAME positive canonical CharProperties Buff owner";return -1;}BuffResult24 result{};okay=dh2_character_buffs_remove_all(&result,buffs)==1;break;
 }
 case start_timer:{
  if(q.argument[2]!=0x2e)return 0;std::int32_t delay{};if(!constant("CharacterDesign","Despawn_Delay",delay,e))return -1;
  if(record_.player_script_owner_v62)okay=record_.player_script_owner_v62->session().start_timer(std::uint32_t(delay),q.argument[1],0x2e,q.identity)>=0;
  else if(a.session)okay=dh2_character_timer_start(&a.session->timers(),std::uint32_t(delay),q.argument[1],0x2e,q.identity,&a.session->native_timer_services())>=0;
  break;
 }
 case remove_body:
  if(!a.position_fields_v7().constructed){e="Unproduced Character physical2dc before death removal";return -1;}
  if(!a.position_fields_v7().physical2dc){e.clear();return 1;}
  if(!record_.physical_owner_v62){e="Required actual positive source physical owner for removal";return -1;}
  okay=record_.physical_owner_v62->release();if(!okay)e=record_.physical_owner_v62->error();break;
 default:return 0;
 }
 if(!okay){if(e.empty())e="Required actual canonical death body "+std::to_string(q.service);return -1;}e.clear();return 1;
}
}
