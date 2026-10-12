#include "character_dead_select.hpp"
#include <cstring>
namespace {
using namespace dh2::character;
std::int32_t integer(std::uint32_t v){std::int32_t r;std::memcpy(&r,&v,4);return r;}
bool valid(const DeadSelect32* d,const DeadSelectServices16* c,const StateOwnerServices16* s){
 if(!d||!d->machine||!c||!c->invoke||!s||!s->invoke||d->reserved||d->pending_alternate>255||d->count>0x7fffffffu||(!d->rows&&d->count))return false;
 const auto m=d->machine;const auto f=m->fsm;
 if(!f||!f->state||!f->character||f->reserved||f->current_present>1||m->reserved||m->state_count>20||(!m->states&&m->state_count)||m->current_index< -1||m->current_index>=static_cast<std::int32_t>(m->state_count))return false;
 if((m->current_index>=0)!=bool(f->current_present)||(f->current_present&&f->state->current!=m->states[m->current_index].id))return false;
 for(std::uint32_t i=0;i<m->state_count;++i){const auto& info=m->states[i];if(info.reserved||info.event_count>1024||(!info.events&&info.event_count)||(i&&m->states[i-1].id>=info.id))return false;
  for(std::uint32_t j=1;j<info.event_count;++j)if(info.events[j-1].event>=info.events[j].event)return false;
 }
 return true;
}
int call(DeadSelect32* d,const DeadSelectServices16* c,DeadSelectOperation op,std::uint32_t mask,std::uint32_t* out){const DeadSelectRequest24 r{op,mask,d->machine->fsm->character,{0,0}};return c->invoke(c->context,d,&r,out)?-2:1;}
int modifier(DeadSelect32* d,const DeadSelectServices16* c,std::uint32_t mask,std::uint32_t& value){auto r=call(d,c,dead_stance_bits,mask,&value);if(r<0)return r;if(value&mask)return call(d,c,dead_anim_stance,0,&value);value=0;return 1;}
}
extern "C" int dh2_character_dead_select(DeadSelect32* d,std::uint32_t mode,std::uintptr_t payload,std::uint32_t force,const DeadSelectServices16* c,const StateOwnerServices16* s){
 if(!valid(d,c,s))return -1;
 std::uint32_t index=0;auto r=call(d,c,dead_animation_index,0,&index);if(r<0)return r;
 // Getter and outer gate both read the current global count after the index.
 if(integer(index)<0||integer(index)>=static_cast<std::int32_t>(d->count))index=17;
 if(index>=d->count)return 0;
 if(!d->rows||d->count>0x7fffffffu)return -2;
 const auto row=d->rows+index;const auto first=static_cast<std::uint32_t>(d->pending_alternate?row->deadly_great_kb:row->died);
 std::uint32_t value=0;r=modifier(d,c,d->pending_alternate?0x20000u:0x8000u,value);if(r<0)return r;
 d->machine->fsm->state->animation_override=integer(first+value);
 // Original retains the row pointer but re-reads pending_alternate here.
 const auto second=static_cast<std::uint32_t>(d->pending_alternate?row->despawn_great_kb:row->despawn);
 r=modifier(d,c,d->pending_alternate?0x40000u:0x10000u,value);if(r<0)return r;
 d->secondary_animation=integer(second+value);
 auto m=d->machine;auto f=m->fsm;f->state->dead_alternate=mode&255u;d->pending_alternate=0;
 // SM_IsAwaitingToRevive: Limbus0,PreSpawn17,Reviving16. This clears
 // the current StateInfo pointer, not elapsed time. The owner dispatch then
 // sees previous=-1, skips Blur, and owns any subsequent elapsed reset.
 if(f->current_present&&(f->state->current==0||f->state->current==17||f->state->current==16)){m->current_index=-1;f->current_present=0;f->state->current=-1;}
 r=force?dh2_character_state_owner_transition(m,12,0xc358,payload,s):dh2_character_state_owner_event(m,0xc358,payload,s);
 return r<0?r:1;
}
