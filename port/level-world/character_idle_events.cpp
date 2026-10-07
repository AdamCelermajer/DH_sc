#include "character_idle_events.hpp"
#include "character_ai_state_changed.hpp"
#include "character_ai_state_changed_vm.hpp"
#include <array>
#include <cstring>
namespace {
#include "character_idle_event_tables.inc"
std::int32_t signed_word(std::uintptr_t word){const auto raw=std::uint32_t(word);std::int32_t result;std::memcpy(&result,&raw,4);return result;}
}
namespace dh2::character {
const std::uintptr_t* character_idle_ai_keys() noexcept{return ai_keys.data();}
const std::uintptr_t* character_idle_external_keys() noexcept{return external_keys.data();}
int StateOwnerDebugDiagnostics::begin(const char* name){
 ++loads_;const int loaded=dh2_character_debug_load(&debug_,&files_);if(loaded!=1)return loaded;
 temporary_.emplace_back(name);std::uint32_t ignored=0;
 ++queries_;const int queried=dh2_character_debug_get(&ignored,&debug_,temporary_.back().c_str(),&files_);
 return queried==1?0:queried;
}
int StateOwnerDebugDiagnostics::end(){
 if(temporary_.empty())return -1;
 temporary_.pop_back();++destructions_;return 0;
}
int StateOwnerDebugDiagnostics::invoke(const StateOwnerRequest48& request){
 if(request.reserved[0]||request.reserved[1])return -1;
 if(request.operation==state_owner_profile_begin&&request.source_function==0x337888)return begin("isTracingCharSM");
 if(request.operation==state_owner_profile_end&&request.source_function==0x318254)return end();
 return -1;
}
int StateOwnerDebugDiagnostics::idle_behavior(std::uint32_t source){
 if(source!=0x3c3020&&source!=0x3c2d3c)return -1;
 const int status=begin("isTracingCharState");return status?status:end();
}
CharacterIdleEvents::CharacterIdleEvents(CharacterStateOwner& states,CharacterAnimationInstance& animation,
 const data::AnimationTables& tables,data::AnimationRandom& random,const Facts& facts,
 AIEventState64& ai,AnimationAIState96& animation_ai,float speed,const IdleEventProviders& providers)
 :states_(states),animation_(animation),tables_(tables),random_(random),facts_(facts),ai_(ai),
 animation_ai_(animation_ai),providers_(providers),global_speed_(speed){
 bodies_={this,body};behavior_={&facts_,&bodies_,&predicates_,{this,remaining}};
 ai_services_={this,ai_service,63,0};animation_services_={this,animation_service};
 if(dh2_character_state_owner_behavior_bind(&state_services_,&behavior_)!=1)fail("Malformed Idle behavior facts");
 else state_services_={this,state_service};
 previous_observer_=animation_.playback().observer;
 if(previous_observer_.invoke)fail("Animation instance already has an observer");
 else animation_.playback().observer={this,observe};
}
CharacterIdleEvents::~CharacterIdleEvents(){auto& observer=animation_.playback().observer;if(observer.context==this&&observer.invoke==observe)observer=previous_observer_;}
int CharacterIdleEvents::fail(const char* message){if(error_.empty())error_=message;return -1;}
bool CharacterIdleEvents::coherent() const{
 const auto& fsm=states_.machine().fsm;
 return fsm&&ai_.ai&&ai_.owner&&ai_.owner->owner==fsm->character&&
  ai_.owner->state_machine==reinterpret_cast<std::uintptr_t>(fsm)&&
  ai_.owner->controller&&ai_.owner->properties&&
  animation_ai_.owner==fsm->character&&animation_ai_.controller==ai_.owner->controller;
}
bool CharacterIdleEvents::bind_input(CharacterScriptSessionInput& input){
 if(!error_.empty()||!coherent()||input.identity!=states_.native_fsm().character)return fail("Incoherent session/FSM input") == 0;
 if(input.state_machine&&input.state_machine!=&states_.native_fsm())return fail("Session already binds another FSM") == 0;
 input.state_machine=&states_.native_fsm();input_bound_=true;return true;
}
bool CharacterIdleEvents::attach(CharacterScriptSession& session){
 if(!error_.empty()||!input_bound_||!coherent()||session.owner().lifecycle().owner!=states_.native_fsm().character||session_)
  return fail("Incoherent or already attached script session") == 0;
 session_=&session;return true;
}
bool CharacterIdleEvents::publish_external(){
 ScriptSessionView view{};
 if(!error_.empty()||!session_||!coherent()||!session_->owner().active(view)||view.kind!=script_external||
  session_->owner().last_source_load_status()!=0||!session_->error().empty())return fail("Successful genuine External Init is required") == 0;
 ai_.active=view.identity;ai_.ais_virtuals=character_idle_external_keys();return true;
}
bool CharacterIdleEvents::initialize_idle(){
 ScriptSessionView view{};
 if(!error_.empty()||!session_||!coherent()||!session_->owner().active(view)||view.kind!=script_external||
  ai_.active!=view.identity||ai_.ais_virtuals!=character_idle_external_keys())return fail("External Init/publication must precede LevelLoadStates") == 0;
 const int status=states_.initialize_level(3,state_services_);
 if(status!=1)fail("Native Idle LevelLoadStates failed at delivered prefix");
 return status==1&&error_.empty();
}
bool CharacterIdleEvents::raise(std::uint32_t event,std::uintptr_t payload,const dh2_script_callback_scope* scope){
 if(!error_.empty())return false;
 if(!session_||!coherent())return fail("Incoherent live Character event receiver") == 0;
 // Character::RaiseEvent3a4d5c branches directly to embedded CharAI except36.
 if(event==0x36)return fail("Property event36 requires its source property backend") == 0;
 const auto previous=scope_;scope_=scope;AIEventResult16 result{};AIEventPayload24 argument{payload,0,0,0};
 ++counts_.character_events;const int status=dh2_character_ai_event(&result,&ai_,event,&argument,&ai_services_);scope_=previous;
 if(status)fail("CharAI event service failed at delivered prefix");
 return !status&&error_.empty();
}
bool CharacterIdleEvents::scene_phase(std::uint32_t timestamp){
 if(!error_.empty())return false;
 std::string error;const bool done=animation_.scene_phase(timestamp,error);
 if(!done)fail(error.c_str());
 return done&&error_.empty();
}
bool CharacterIdleEvents::animator_phase(){
 if(!error_.empty())return false;
 std::string error;const bool done=animation_.animator_phase(tables_,random_,global_speed_,error);
 if(!done)fail(error.c_str());
 return done&&error_.empty();
}
void CharacterIdleEvents::body(void* opaque,State* state,const Request* request){
 auto& self=*static_cast<CharacterIdleEvents*>(opaque);if(!self.error_.empty())return;
 if(state==&self.states_.state()&&state->current==3&&request->service==set_animation){
  ++self.counts_.animation_selections;std::string error;
  if(!self.animation_.start(self.tables_,request->argument[0],self.random_,self.global_speed_,error))self.fail(error.c_str());
 }else if(!self.providers_.body||self.providers_.body(self.providers_.context,state,request))self.fail("Required non-Idle body provider unavailable");
}
int CharacterIdleEvents::state_service(void* opaque,StateOwnerMachine40* machine,const StateOwnerRequest48* request,StateOwnerResponse8* response){
 auto& self=*static_cast<CharacterIdleEvents*>(opaque);if(!self.error_.empty())return -1;
 if(machine==&self.states_.machine()&&request&&response&&request->state==3&&request->character==self.states_.native_fsm().character&&
  !request->reserved[0]&&!request->reserved[1]&&self.states_.native_fsm().current_present&&self.states_.state().current==3&&
  ((request->operation==state_owner_focus&&request->source_function==0x3c3020)||
   (request->operation==state_owner_blur&&request->source_function==0x3c2d3c))){
  if(!self.providers_.diagnostics||self.providers_.diagnostics->idle_behavior(request->source_function))
   return self.fail("Required source Idle Focus/Blur diagnostics failed");
 }
 const int status=dh2_character_state_owner_behavior_invoke(&self.behavior_,machine,request,response);
 return status?status:self.error_.empty()?0:-1;
}
int CharacterIdleEvents::remaining(void* opaque,StateOwnerMachine40* machine,const StateOwnerRequest48* request,StateOwnerResponse8* response){
 auto& self=*static_cast<CharacterIdleEvents*>(opaque);if(!self.error_.empty())return -1;
 if(machine==&self.states_.machine()&&request->operation==state_owner_character_event&&request->source_function==0x3a4d5c&&request->character==self.states_.native_fsm().character)
  return self.raise(request->event,request->payload,self.scope_)?0:-1;
 if(self.providers_.diagnostics&&(request->operation==state_owner_profile_begin||request->operation==state_owner_profile_end)){
  if(self.providers_.diagnostics->invoke(*request))return self.fail("Source DebugSwitches/string diagnostic service failed");
  return 0;
 }
 if(!self.providers_.state||self.providers_.state(self.providers_.context,machine,request,response))return self.fail("Required source state/debug/pin provider unavailable");
 return 0;
}
int CharacterIdleEvents::fallback(AIEventState64* ai,const AIEventRequest40* request,std::uint32_t* result){
 if(!providers_.ai||providers_.ai(providers_.context,ai,request,result))return fail("Required nonempty AI/AIS service unavailable");
 return 0;
}
int CharacterIdleEvents::ai_service(void* opaque,AIEventState64* ai,const AIEventRequest40* request,std::uint32_t* result){
 auto& self=*static_cast<CharacterIdleEvents*>(opaque);if(!self.error_.empty())return -1;++self.counts_.AI_services;
 if(ai!=&self.ai_)return self.fail("Wrong AI receiver");
 if(request->service==ai_event_state_getter&&request->subject==reinterpret_cast<std::uintptr_t>(&self.states_.native_fsm())){
  std::int32_t value;if(dh2_character_native_fsm_get_integer(&value,&self.states_.native_fsm(),0)!=1)return self.fail("Native FSM getter failed");
  *result=std::uint32_t(value);++self.counts_.state_getters;return 0;
 }
 if(request->service==ai_event_state_event&&request->subject==reinterpret_cast<std::uintptr_t>(&self.states_.native_fsm())){
  ++self.counts_.FSM_events;const int status=self.states_.event(signed_word(request->event),request->payload,self.state_services_);
  if(status<0)return self.fail("Required native FSM event service failed");
  *result=std::uint32_t(status);return self.error_.empty()?0:-1;
 }
 if(request->service==ai_event_virtual&&request->subject==ai->ai){
  if(request->operation==0x20&&request->callee==0x3d0bec){++self.counts_.state_changed;return dh2_character_ai_state_changed(ai,signed_word(request->argument),signed_word(request->payload),&self.ai_services_);}
  if(request->operation==0x98&&request->callee==0x3d0ce8){++self.counts_.end_relays;return dh2_character_ai_end_anim(ai,&self.ai_services_);}
 }
 if(request->service==ai_event_ais_virtual&&request->operation==0x98&&request->callee==0x3dccd0&&self.session_){
  ++self.counts_.external_end_calls;const int status=character_ais_external_end_anim(self.session_->owner(),request->subject,self.scope_);
  if(status)return self.fail("Private External OnEndOfAnim failed");
  return 0;
 }
 if(request->service==ai_event_helper&&request->subject==ai->ai&&
  (request->operation==0x3d3d4c||request->operation==0x3d3d30||request->operation==0x3d4204||request->operation==0x3d3ff8)){
  const auto fsm=&self.states_.native_fsm();
  if(!ai->owner||ai->owner->state_machine!=reinterpret_cast<std::uintptr_t>(fsm))return self.fail("Animation helper current owner/FSM mismatch");
  ++self.counts_.animation_helpers;
  // Sequence begin/end always query state and return1. Step helpers branch to
  // nonempty Move/Attack/Skill consumers; only actual Idle is handled here.
  if(request->operation==0x3d3d4c||request->operation==0x3d3d30){
   std::int32_t ignored;if(dh2_character_native_fsm_get_integer(&ignored,fsm,0)!=1)return self.fail("Animation sequence helper getter failed");
   ++self.counts_.state_getters;*result=1;return 0;
  }
  // This is a native domain guard, not an extra source service invocation.
  // Actual consumer executes its one current-state getter below.
  if(!fsm->current_present||!fsm->state||fsm->state->current!=3)return self.fallback(ai,request,result);
  const int status=dh2_character_animation_ai(&self.animation_ai_,request->operation==0x3d4204?ai_step_begin:ai_step_end,&self.animation_services_);
  if(status!=1)return self.fail("Native Idle animation consumer failed");
  *result=1;return self.error_.empty()?0:-1;
 }
 return self.fallback(ai,request,result);
}
void CharacterIdleEvents::animation_service(void* opaque,AnimationAIState96* ai,const AnimationAIRequest32* request,AnimationAIResponse16* response){
 auto& self=*static_cast<CharacterIdleEvents*>(opaque);
 if(ai==&self.animation_ai_&&request->service==ai_animation_state&&request->subject==self.states_.native_fsm().character){
  std::int32_t state;if(dh2_character_native_fsm_get_integer(&state,&self.states_.native_fsm(),0)==1){++self.counts_.state_getters;response->word=std::uint32_t(state);return;}
 }
 self.fail("Non-Idle animation consumer service unavailable");
}
void CharacterIdleEvents::observe(void* opaque,actor::BlendedPlayback&,const actor::BlendedPlaybackEvent& event){
 auto& self=*static_cast<CharacterIdleEvents*>(opaque);++self.counts_.animation_events;
 const auto& handoff=event.event.handoff;
 if(handoff.event_id==0x28){const animation::TriggeredEvent trigger{handoff.lag_ms,handoff.payload};self.raise(0x28,reinterpret_cast<std::uintptr_t>(&trigger),self.scope_);}
 else self.raise(handoff.event_id,0,self.scope_);
}
}
