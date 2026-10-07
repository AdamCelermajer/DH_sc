#include "character_world_npc_collision_v1.hpp"
#include <stdexcept>
namespace dh2::character {
CharacterWorldNpcCollisionV1::CharacterWorldNpcCollisionV1(skills::CharacterWorldRuntimeV1& w,
 CharacterWorldNpcStateOwnerV1& m,CharacterScriptSession& s,ScriptCharacterObject& o,AIEventState64& ai,
 ControllerCommandState32& c,WorldNpcAISCollisionFieldsV1& f,WorldNpcCollisionGlobalsV1& g,
 DebugSwitches* d,const DebugFileServices24* files,WorldNpcCollisionServicesV1 svc):
 world_(w),machine_(m),session_(s),object_(o),ai_(ai),controller_(c),fields_(f),globals_(g),debug_(d),files_(files),services_(svc){}
bool CharacterWorldNpcCollisionV1::coherent(){
 ScriptSessionView active{};
 if(ai_.active&&(!session_.owner().active(active)||active.identity!=ai_.active)){
  error_="NPC collision active AIS must be the same published ScriptSession";return false;
 }
 if(world_.state_borrow(object_.identity)!=&machine_.state()||session_.properties().get()!=object_.properties.get()||
 session_.combat_state().get()!=object_.life.get()||!ai_.owner||ai_.owner->owner!=object_.identity||
 ai_.owner->state_machine!=reinterpret_cast<std::uintptr_t>(&machine_.native_fsm())||
 controller_.owner!=object_.identity||ai_.owner->controller!=controller_.controller||!ai_.ai||
 object_.target.owner!=&object_.owner||object_.binding.state!=&object_.target){
  error_="NPC collision requires the same registered FSM/script/AI/controller/property/life/target owners";return false;
 }
 return true;
}
bool CharacterWorldNpcCollisionV1::query(std::uint32_t service,std::uintptr_t subject,std::uintptr_t other,std::uint32_t& out){
 auto provider=world_.targets().query_services();target_providers::Request24 request{service,0,subject,other};std::uintptr_t result{};
 if(provider.invoke(provider.context,&request,&result)){error_=world_.targets().error();return false;}
 out=static_cast<std::uint32_t>(result);return true;
}
bool CharacterWorldNpcCollisionV1::handle_character(std::uintptr_t& identity){
 target_providers::Handle16* shared{};target_providers::Registry24* registry{};
 if(world_.handle_borrow(object_.identity,&shared,&registry)){error_=world_.error();return false;}
 auto provider=world_.targets().query_services();target_providers::Handle16 local{};
 if(dh2_target_handle_character(&identity,&local,shared,registry,&provider)){error_="Required source Character handle conversion";return false;}return true;
}
bool CharacterWorldNpcCollisionV1::debug_prefix(){
 std::uint32_t tracing{};
 if(!debug_||!files_||dh2_character_debug_load(debug_,files_)!=1||
 dh2_character_debug_get(&tracing,debug_,"isTracingPlayersCollision",files_)!=1){error_="Required POCharacter collision Debug prefix";return false;}
 if(tracing){std::uint32_t ignored{};if(!query(target_providers::virtual_player,object_.identity,0,ignored))return false;}
 return true;
}
std::uint32_t CharacterWorldNpcCollisionV1::collision_service(void* p,ScriptCollisionState72* state,ScriptCollisionObject16* object,const ScriptCollisionRequest32* r){
 auto& t=*static_cast<CharacterWorldNpcCollisionV1*>(p);std::uint32_t result{};
 // Publish original source counter/paused prefixes before any callback can
 // reenter the same AIS; then reload its real mutable projections afterward.
 t.fields_.collision_ms=state->collision_ms;t.fields_.last_collision_frame=state->last_collision_frame;t.ai_.paused=state->paused;
 switch(r->service){
 case script_collision_is_character:
  if(!t.query(target_providers::virtual_character,r->subject,0,result))throw std::runtime_error(t.error_);
  ++t.current_character_queries_;
  if(!result&&t.current_persist_&&t.current_character_queries_>1){
   if(!t.services_.object_type||!t.services_.object_type(t.services_.context,r->subject,object->type,t.error_))throw std::runtime_error(t.error_.empty()?"Required collided source ObjectBase+f4 type":t.error_);
  }break;
 case script_collision_owner_is_player:if(!t.query(target_providers::virtual_player,r->subject,0,result))throw std::runtime_error(t.error_);break;
 case script_collision_is_enemy:{std::uintptr_t enemy{};if(t.world_.relationship(t.object_.identity,r->target,true,&enemy))throw std::runtime_error(t.world_.error());result=static_cast<std::uint32_t>(enemy);break;}
 case script_collision_cancel_sneaking:
  if(!t.services_.cancel_sneaking||t.services_.cancel_sneaking(t.services_.context,r->subject,t.error_))throw std::runtime_error(t.error_.empty()?"Required actual CancelSneaking":t.error_);break;
 case script_collision_set_target:{
  // TargetOwner16 is a source projection. Commit its source halfword write
  // to the existing combat backing before/after every synchronous service.
  auto& combat=t.machine_.combat_fields();t.object_.owner.word14d0=combat.combo;
  struct Bridge {CharacterWorldNpcCollisionV1* owner;TargetServices16 original;};
  Bridge bridge{&t,t.object_.binding.services};
  const TargetServices16 svc{&bridge,[](void* ptr,TargetState48* target,const TargetRequest24* request,std::uint32_t* value)->int{
   auto& b=*static_cast<Bridge*>(ptr);auto& o=*b.owner;
   o.machine_.combat_fields().combo=o.object_.owner.word14d0;o.object_.life->combo_hits=o.object_.owner.word14d0;
   if(!b.original.invoke)return -1;
   const int status=b.original.invoke(b.original.context,target,request,value);
   o.object_.owner.word14d0=o.machine_.combat_fields().combo;return status;
  }};
  const int status=dh2_character_ai_set_target(&t.object_.target,r->target,r->arg0,&svc);
  combat.combo=t.object_.owner.word14d0;t.object_.life->combo_hits=combat.combo;
  if(status)throw std::runtime_error("Required genuine collision SetTarget continuation");break;
 }
 default:throw std::runtime_error("Unknown source AIS collision service");
 }
 state->collision_ms=t.fields_.collision_ms;state->last_collision_frame=t.fields_.last_collision_frame;state->paused=t.ai_.paused;
 state->current_target=t.object_.target.target;state->preferred_target=t.object_.target.last_target;
 return result;
}
bool CharacterWorldNpcCollisionV1::script_collision(std::uintptr_t collider,std::uint32_t persist){
 std::uint32_t frame{},dt{};if(!services_.clock||!services_.clock(services_.context,frame,dt,error_)){if(error_.empty())error_="Required Application frame/GetDt";return false;}
 ScriptCollisionObject16 object{collider,0,0}; // type read only at its source branch below
 std::int32_t state_id{};if(dh2_character_native_fsm_get_integer(&state_id,&machine_.native_fsm(),0)!=1){error_="Required same collision FSM";return false;}
 ScriptCollisionState72 state{ai_.active,object_.identity,ai_.ai,reinterpret_cast<std::uintptr_t>(&machine_.native_fsm()),
  object_.target.target,object_.target.last_target,fields_.collision_ms,fields_.last_collision_frame,ai_.paused,static_cast<std::uint32_t>(state_id),frame,dt};
 auto* previous=current_;const auto previous_queries=current_character_queries_,previous_persist=current_persist_;
 current_=&state;current_character_queries_=0;current_persist_=persist;const ScriptCollisionServices16 svc{this,collision_service};bool success=true;
 try{if(dh2_character_script_collision(&state,&object,persist,&svc)!=1){error_="Malformed source AIS collision borrow";success=false;}}
 catch(const std::exception& e){error_=e.what();success=false;}
 fields_.collision_ms=state.collision_ms;fields_.last_collision_frame=state.last_collision_frame;ai_.paused=state.paused;
 current_=previous;current_character_queries_=previous_queries;current_persist_=previous_persist;return success;
}
bool CharacterWorldNpcCollisionV1::ais_collision(std::uint32_t slot,std::uintptr_t collider,std::uint32_t persist){
 if(!ai_.active)return true;
 if(!ai_.ais_virtuals){error_="Required active AIS collision callable table";return false;}
 const auto method=ai_.ais_virtuals[slot/4];
 if(slot==0xbc&&method==0x3dbf08u){++globals_.collisions;return script_collision(collider,persist);}
 if(slot==0xc0&&method==0x3dbfa0u)return script_collision(collider,persist);
 if(slot==0xc4&&method==0x3dbf40u){--globals_.collisions;return true;}
 if(slot==0xc8&&method==0x3dbf68u)return true;
 if(!method||!services_.ais_method||services_.ais_method(services_.context,ai_.active,method,collider,persist,error_)){
  if(error_.empty())error_="Required source selected AIS collision override";return false;
 }return true;
}
int CharacterWorldNpcCollisionV1::event_service(void* p,AIEventState64* state,const AIEventRequest40* r,std::uint32_t*){
 auto& t=*static_cast<CharacterWorldNpcCollisionV1*>(p);if(state!=&t.ai_)return -1;
 if(r->service==ai_event_state_event)return t.machine_.event(static_cast<std::int32_t>(r->event),r->payload)<0?-1:0;
 if(r->service!=ai_event_virtual)return -1;
 const std::uintptr_t expected=r->operation==0xbc?0x3d0decu:r->operation==0xc0?0x3d0e10u:r->operation==0xc4?0x3d0e34u:r->operation==0xc8?0x3d0e58u:0;
 if(!expected||r->callee!=expected)return -1;
 return t.ais_collision(r->operation,r->payload,r->argument)?0:-1;
}
bool CharacterWorldNpcCollisionV1::physical_event(WorldNpcPhysicalEventV1 event,std::uintptr_t peer,std::uint32_t persist){
 error_.clear();if(!coherent())return false;
 if(event==WorldNpcPhysicalEventV1::result)return true; // Whole POCharacter body: bx lr.
 if(!peer)return true;
 std::uintptr_t character{};
 if(event==WorldNpcPhysicalEventV1::persist){if(!handle_character(character)||!debug_prefix())return false;}
 else if(!debug_prefix()||!handle_character(character))return false;
 if(!character)return true;
 ai_.owner->forced=controller_.forced;ai_.owner->locked=controller_.locked;ai_.global_blocked=controller_.global_blocked;
 const std::uint32_t code=event==WorldNpcPhysicalEventV1::begin?0x37:event==WorldNpcPhysicalEventV1::persist?0x39:0x3b;
 const AIEventPayload24 payload{peer,0,0,0};const AIEventServices24 svc{this,event_service,3,0};AIEventResult16 result{};
 if(dh2_character_ai_event(&result,&ai_,code+(persist?0:1),&payload,&svc)){
  if(error_.empty())error_="Required same Character collision RaiseEvent continuation: "+machine_.error();return false;
 }return true;
}
bool CharacterWorldNpcCollisionV1::permits_filter(std::uint16_t category,bool& allowed){
 error_.clear();allowed=false;if(!coherent())return false;std::uintptr_t character{};
 if(!handle_character(character))return false;
 if(character){std::int32_t state{};if(dh2_character_native_fsm_get_integer(&state,&machine_.native_fsm(),0)!=1){error_="Required same physical filter FSM";return false;}
  if(state==0||(state==10&&!(category&3)))return true;
 }
 allowed=true;return true;
}
}
