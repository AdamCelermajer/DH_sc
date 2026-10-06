#include "character_state_owner_behavior.hpp"
namespace {
using namespace dh2::character;
bool context_valid(const StateOwnerBehaviorContext40* c){return c&&c->facts&&c->bodies&&c->bodies->invoke&&c->predicates&&c->remaining.invoke&&!c->facts->reserved&&c->facts->is_player<=1&&c->facts->is_at_destination<=1&&c->facts->following_path<=1&&c->facts->has_ranged_weapon<=1&&!c->predicates->reserved[0]&&!c->predicates->reserved[1]&&!c->predicates->reserved[2];}
bool bounded(std::int32_t id){return id==3||id==4||id==5||id==12;}
bool pure(std::uint32_t f){return f==0x3ad22c||f==0x3ad23c||f==0x3ad244||f==0x3ad280||f==0x3ad290||f==0x3ad29c||f==0x3ad2c8;}
const StateOwnerInfo40* info(const StateOwnerMachine40* m,std::int32_t id){for(std::uint32_t i=0;i<m->state_count;++i)if(m->states[i].id==id)return &m->states[i];return nullptr;}
bool overlaps(const void* a,std::uintptr_t n,const void* b,std::uintptr_t k){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<k;}
}
extern "C" int dh2_character_state_owner_behavior_bind(StateOwnerServices16* out,StateOwnerBehaviorContext40* c){if(!out||!context_valid(c)||overlaps(out,16,c,40)||overlaps(out,16,c->facts,96)||overlaps(out,16,c->bodies,16)||overlaps(out,16,c->predicates,8))return -1;*out={c,dh2_character_state_owner_behavior_invoke};return 1;}
extern "C" int dh2_character_state_owner_behavior_invoke(void* context,StateOwnerMachine40* m,const StateOwnerRequest48* r,StateOwnerResponse8* out){
 auto* c=static_cast<StateOwnerBehaviorContext40*>(context);
 if(!context_valid(c)||!m||!m->fsm||!m->fsm->state||!r||!out||r->reserved[0]||r->reserved[1]||m->state_count>20||(!m->states&&m->state_count))return -1;
 const auto s=m->fsm->state;
 if(r->operation==state_owner_predicate&&pure(r->source_function)){
  if(r->character!=m->fsm->character||r->adjustment||s->stop_attack_allowed>255)return -1;
  const StateOwnerPredicateFacts16 f{s->flags,s->attack_gate,c->predicates->interaction,c->predicates->can_interrupt,static_cast<std::uint8_t>(s->stop_attack_allowed),{0,0}};
  return dh2_character_state_owner_predicate(out,r->source_function,r->state,&f)==1?0:-1;
 }
 if(bounded(r->state)&&(r->operation==state_owner_blur||r->operation==state_owner_focus||r->operation==state_owner_event)){
  const auto i=info(m,r->state);if(!i||r->character!=m->fsm->character||s->current!=r->state||!m->fsm->current_present)return -1;
  const auto source=r->operation==state_owner_blur?i->blur:r->operation==state_owner_focus?i->focus:i->on_event;if(source!=r->source_function)return -1;
  int result=0;
  if(r->operation==state_owner_blur)result=dh2_character_state_blur_body(s,c->facts,c->bodies);
  else if(r->operation==state_owner_focus)result=dh2_character_state_focus_body(s,c->facts,r->other,r->payload,c->bodies);
  else result=dh2_character_state_event_body(s,c->facts,r->event,c->bodies);
  return result==1?0:-1;
 }
 return c->remaining.invoke(c->remaining.context,m,r,out);
}
