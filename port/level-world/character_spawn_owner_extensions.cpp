#include "character_spawn_owner_extensions.hpp"
namespace {
using namespace dh2::character;
bool valid(const SpawnOwnerExtensions72* c,const StateOwnerMachine40* m){
 return c&&m&&m==c->machine&&m->fsm&&m->fsm->state&&m->fsm->character&&
  !m->reserved&&!m->fsm->reserved&&m->fsm->current_present<=1&&
  m->state_count<=20&&(!m->state_count||m->states)&&c->remaining&&
  c->remaining->pre_spawn&&c->remaining->spawn_services&&c->remaining->spawn_services->invoke&&
  c->remaining->pre_spawn->state==m->fsm->state&&c->remaining->pre_spawn->character==m->fsm->character&&
  c->spawn&&c->spawn->state==m->fsm->state&&c->spawn->character==m->fsm->character&&
  c->spawn_services&&c->spawn_services->invoke&&c->selection&&c->selection->fsm==m->fsm&&
  c->selection->random&&c->permission&&!c->permission->reserved&&c->permission->auto_spawn<=255&&
  c->timers&&c->timers->owner==m->fsm->character&&c->timer_services&&
  !c->timer_services->reserved&&c->timer_services->expired&&
  c->outer_methods&&c->outer_methods->invoke;
}
int select_service(void* p,NativeSpawn24*,const SpawnSelectRequest32* r,std::uint32_t* out){
 auto* c=static_cast<SpawnOwnerExtensions72*>(p);
 if(r->service==spawn_select_state)
  return dh2_character_state_owner_transition(c->machine,1,-1,0,c->outer_methods)==1?0:1;
 if(r->service!=spawn_select_timer)return 1;
 const auto id=dh2_character_timer_start(c->timers,r->argument0,0,0x2d,0,c->timer_services);
 if(id<0)return 1;
 *out=static_cast<std::uint32_t>(id);return 0;
}
int pre_service(void* p,PreSpawnState48* s,const PreSpawnRequest32* r,PreSpawnResponse8* out){
 auto* c=static_cast<SpawnOwnerExtensions72*>(p);
 // Entry validation is done by the body adapter. After nested selection the
 // original PreSpawn body still resumes, even when the live state is no longer17.
 if(r->service==pre_spawn_predicate)
  return dh2_character_pre_spawn_permission(&out->word,c->permission)==1?0:1;
 if(r->service==pre_spawn_set_spawn)
  return dh2_character_spawn_owner_select(c,r->argument0,r->argument1)==1?0:1;
 const auto* services=c->remaining->spawn_services;
 return services->invoke(services->context,s,r,out);
}
bool current(const StateOwnerMachine40* m,const StateOwnerRequest48* r){
 if(!m->fsm->current_present||m->current_index<0||
  m->current_index>=static_cast<std::int32_t>(m->state_count))return false;
 const auto& row=m->states[m->current_index];
 return !row.reserved&&row.id==m->fsm->state->current&&row.id==r->state&&r->character==m->fsm->character;
}
}
extern "C" int dh2_character_spawn_owner_select(SpawnOwnerExtensions72* c,std::uint32_t delay,std::uint32_t ignored){
 if(!valid(c,c?c->machine:nullptr))return -1;
 const SpawnSelectServices16 services{c,select_service};
 return dh2_character_spawn_select(c->selection,delay,ignored,&services);
}
extern "C" int dh2_character_spawn_owner_extensions_method(void* p,StateOwnerMachine40* m,
 const StateOwnerRequest48* r,StateOwnerResponse8* out){
 auto* c=static_cast<SpawnOwnerExtensions72*>(p);
 if(!valid(c,m)||!r||!out||r->reserved[0]||r->reserved[1])return 1;
 if(r->state==1&&(r->operation==state_owner_focus||r->operation==state_owner_blur||r->operation==state_owner_event)){
  if(!current(m,r))return 1;
  const auto& row=m->states[m->current_index];
  const auto source=r->operation==state_owner_focus?row.focus:r->operation==state_owner_blur?row.blur:row.on_event;
  const auto expected=r->operation==state_owner_focus?0x3c35ecu:r->operation==state_owner_blur?0x3c2f7cu:0x3c0b04u;
  if(source!=expected||r->source_function!=source)return 1;
  const auto op=r->operation==state_owner_focus?0u:r->operation==state_owner_blur?1u:3u;
  return dh2_character_spawn_body(c->spawn,op,r->other,r->event,r->payload,c->spawn_services)==1?0:1;
 }
 // Copy only the wrapper. Persistent source projections and providers remain
 // borrowed; nested calls construct independent wrappers, never modify base.
 auto remaining=*c->remaining;
 const PreSpawnServices16 services{c,pre_service};remaining.spawn_services=&services;
 return dh2_character_state_owner_extensions_method(&remaining,m,r,out);
}
extern "C" int dh2_character_spawn_owner_extensions_update(void* p,StateOwnerMachine40* m,
 const StateOwnerUpdateRequest24* r){
 auto* c=static_cast<SpawnOwnerExtensions72*>(p);
 if(!valid(c,m))return 1;
 auto remaining=*c->remaining;
 return dh2_character_state_owner_extensions_update(&remaining,m,r);
}
