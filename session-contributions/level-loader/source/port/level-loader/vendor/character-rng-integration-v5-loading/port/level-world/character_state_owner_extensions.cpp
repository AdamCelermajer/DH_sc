#include "character_state_owner_extensions.hpp"
namespace {
using namespace dh2::character;
bool valid(const StateOwnerMachine40* m){
 if(!m||!m->fsm||!m->fsm->state||!m->fsm->character||m->reserved||m->fsm->reserved||
    m->fsm->current_present>1||m->state_count>20||(!m->states&&m->state_count)||
    m->current_index<0||m->current_index>=static_cast<std::int32_t>(m->state_count)||
    !m->fsm->current_present)return false;
 const auto& row=m->states[m->current_index];
 return !row.reserved&&row.id==m->fsm->state->current;
}
bool pre_spawn(StateOwnerExtensions48* c,StateOwnerMachine40* m){
 return c->pre_spawn&&c->spawn_services&&c->spawn_services->invoke&&
  c->pre_spawn->state==m->fsm->state&&c->pre_spawn->character==m->fsm->character;
}
}
extern "C" int dh2_character_state_owner_extensions_method(void* p,StateOwnerMachine40* m,
 const StateOwnerRequest48* r,StateOwnerResponse8* out){
 auto* c=static_cast<StateOwnerExtensions48*>(p);
 if(!c||!m||!r||!out||r->reserved[0]||r->reserved[1])return 1;
 std::uint32_t op=0;
 if(r->operation==state_owner_focus)op=state_method_focus;
 else if(r->operation==state_owner_blur)op=state_method_blur;
 else if(r->operation==state_owner_event)op=state_method_event;
 else return c->remaining_methods.invoke?
  c->remaining_methods.invoke(c->remaining_methods.context,m,r,out):1;
 if(!valid(m)||r->state!=m->fsm->state->current||r->character!=m->fsm->character)return 1;
 const auto& row=m->states[m->current_index];
 const auto source=op==state_method_focus?row.focus:op==state_method_blur?row.blur:row.on_event;
 if(r->source_function!=source)return 1;
 const int empty=dh2_character_state_empty_body(row.id,op,source);
 if(empty<0)return 1;
 if(row.id==17){
  if(!pre_spawn(c,m))return 1;
  return dh2_character_pre_spawn_body(c->pre_spawn,op,r->event,r->payload,c->spawn_services)==1?0:1;
 }
 if(empty==1)return 0;
 return c->remaining_methods.invoke?
  c->remaining_methods.invoke(c->remaining_methods.context,m,r,out):1;
}
extern "C" int dh2_character_state_owner_extensions_update(void* p,StateOwnerMachine40* m,
 const StateOwnerUpdateRequest24* r){
 auto* c=static_cast<StateOwnerExtensions48*>(p);
 if(!c||!r||r->reserved||!valid(m)||r->state!=m->fsm->state->current||
    r->character!=m->fsm->character||r->elapsed_ms!=m->fsm->state->elapsed_ms)return 1;
 const auto& row=m->states[m->current_index];
 if(r->source_function!=row.update)return 1;
 const int empty=dh2_character_state_empty_body(row.id,state_method_update,row.update);
 if(empty<0)return 1;
 if(empty==1)return 0;
 return c->remaining_updates.invoke?
  c->remaining_updates.invoke(c->remaining_updates.context,m,r):1;
}
