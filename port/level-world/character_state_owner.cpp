#include "character_state_owner.hpp"
#include <algorithm>
namespace {
using namespace dh2::character;
#include "character_state_owner_data.inc"
bool valid(const StateOwnerMachine40* m,const StateOwnerServices16* c){
 if(!m||!m->fsm||!m->fsm->state||!m->fsm->character||!c||!c->invoke||m->reserved||m->fsm->reserved||m->fsm->current_present>1||m->state_count>20||(!m->states&&m->state_count))return false;
 if(m->current_index < -1 || m->current_index>=static_cast<std::int32_t>(m->state_count))return false;
 if((m->current_index>=0)!=bool(m->fsm->current_present))return false;
 if(m->current_index>=0&&m->fsm->state->current!=m->states[m->current_index].id)return false;
 for(std::uint32_t i=0;i<m->state_count;++i){const auto& s=m->states[i];if(s.reserved||(!s.events&&s.event_count)||s.event_count>1024||(i&&m->states[i-1].id>=s.id))return false;
  for(std::uint32_t j=1;j<s.event_count;++j)if(s.events[j-1].event>=s.events[j].event)return false;}
 return true;
}
int send(StateOwnerMachine40* m,const StateOwnerServices16* c,StateOwnerOperation op,std::uint32_t fn,std::int32_t state,std::int32_t other,std::int32_t event,std::uintptr_t payload,StateOwnerResponse8* response=nullptr,std::uint32_t adjustment=0){
 StateOwnerResponse8 ignored{};StateOwnerRequest48 r{op,fn,state,other,static_cast<std::uint32_t>(event),adjustment,op==state_owner_pin?m->physical:m->fsm->character,payload,{0,0}};
 return c->invoke(c->context,m,&r,response?response:&ignored)?-2:1;
}
std::int32_t find(const StateOwnerMachine40* m,std::int32_t id){for(std::uint32_t i=0;i<m->state_count;++i)if(m->states[i].id==id)return static_cast<std::int32_t>(i);return -1;}
const StateOwnerInfo40* current(const StateOwnerMachine40* m){return m->current_index<0?nullptr:&m->states[m->current_index];}
const StateOwnerEvent16* find_event(const StateOwnerInfo40* s,std::int32_t id){for(std::uint32_t i=0;i<s->event_count;++i)if(s->events[i].event==id)return &s->events[i];return nullptr;}
}
extern "C" int dh2_character_state_owner_transition(StateOwnerMachine40* m,std::int32_t next,std::int32_t event,std::uintptr_t payload,const StateOwnerServices16* c){
 if(!valid(m,c))return -1;
 const auto prior=current(m);const std::int32_t previous=prior?prior->id:-1;
 if(prior){const auto r=send(m,c,state_owner_blur,prior->blur,previous,next,0,0);if(r<0)return r;}
 //Original captures requested next and previous before blur, then reloads owner.
 const auto index=find(m,next);m->current_index=index;m->fsm->current_present=index>=0;m->fsm->state->current=index>=0?m->states[index].id:-1;
 if(index>=0){if(previous!=next)m->fsm->state->elapsed_ms=0;
  const auto r=send(m,c,state_owner_focus,m->states[index].focus,next,previous,event,payload);if(r<0)return r;}
 return send(m,c,state_owner_character_event,0x3a4d5c,previous,0,0x1d,static_cast<std::uintptr_t>(static_cast<std::intptr_t>(previous)));
}
extern "C" int dh2_character_state_owner_event(StateOwnerMachine40* m,std::int32_t event,std::uintptr_t payload,const StateOwnerServices16* c){
 if(!valid(m,c))return -1;
 if(event>=0x2a&&event<=0x2c)m->fsm->state->attack_gate&=~(1u<<static_cast<unsigned>(event-0x2a));
 const auto state=current(m);
 if(event==0x30&&state&&(state->id==3||state->id==13||state->id==18)&&m->physical){const auto r=send(m,c,state_owner_pin,0x46eb20,state->id,0,event,0);if(r<0)return r;}
 const auto entry=current(m);
 if(!entry)return 0;
 auto r=send(m,c,state_owner_event,entry->on_event,entry->id,0,event,payload);if(r<0)return r;
 if(!valid(m,c))return -3;
 const auto after=current(m);if(!after)return -3;
 if(!find_event(after,event))return 0;
 r=send(m,c,state_owner_profile_begin,0x337888,0,0,0,0);if(r<0)return r;
 r=send(m,c,state_owner_profile_end,0x318254,0,0,0,0);if(r<0)return r;
 if(!valid(m,c))return -3;
 const auto live=current(m);if(!live)return -3;
 const auto record=find_event(live,event);
 //Original const _GetEvent does not insert. After its assertions, a miss
 //dereferences the map sentinel. Refuse this invalidated lookup explicitly.
 if(!record)return -3;
 const auto selected=*record;
 StateOwnerResponse8 response{selected.target,1};
 if(selected.predicate||(selected.adjustment&1)){r=send(m,c,state_owner_predicate,selected.predicate,live->id,selected.target,event,payload,&response,selected.adjustment);if(r<0)return r;if(!response.accepted)return 0;}
 return dh2_character_state_owner_transition(m,response.next,event,payload,c);
}
extern "C" int dh2_character_state_owner_initialize_level(StateOwnerMachine40* m,std::int32_t preset,const StateOwnerServices16* c){
 if(!valid(m,c))return -1;
 const auto next=preset==0||preset==17?17:3;
 if(next==3)m->fsm->state->idle_suppressed=0;
 return dh2_character_state_owner_transition(m,next,-1,0,c);
}
extern "C" std::uint32_t dh2_character_state_owner_registration_count(){return sizeof(registrations)/sizeof(*registrations);}
extern "C" int dh2_character_state_owner_registration(std::uint32_t i,std::int32_t* id,StateOwnerEvent16* event){if(!id||!event||i>=dh2_character_state_owner_registration_count())return -1;*id=registrations[i].state;*event=registrations[i].event;return 1;}
extern "C" int dh2_character_state_owner_predicate(StateOwnerResponse8* out,std::uint32_t source,std::int32_t current,const StateOwnerPredicateFacts16* f){
 if(!out||!f||f->reserved[0]||f->reserved[1])return -1;
 std::uint32_t accepted=0;
 switch(source){
 case 0x3ad22c:accepted=(f->mask&1)^1;break;
 case 0x3ad23c:accepted=1;break;
 case 0x3ad244:accepted=current==6?(f->flags>>16)&1:current==10?((f->mask^0x20)>>5)&1:current==5?f->can_interrupt:1;break;
 case 0x3ad280:accepted=current==5?f->stopped_attacking:0;break;
 case 0x3ad290:accepted=(f->flags>>15)&1;break;
 case 0x3ad29c:accepted=f->interaction==4||f->interaction==5?0:1;break;
 case 0x3ad2c8:accepted=1;if(f->interaction==4)out->next=18;break;
 default:return -2;
 }
 out->accepted=accepted;return 1;
}
namespace dh2::character {
CharacterStateOwner::CharacterStateOwner(std::uintptr_t character){
 fsm_={&state_,character,0,0};machine_={&fsm_,infos_.data(),20,-1,0,0};
 for(const auto& r:registrations)events_[static_cast<unsigned>(r.state)].push_back(r.event);
 for(unsigned i=0;i<20;++i){auto& e=events_[i];std::sort(e.begin(),e.end(),[](const auto& a,const auto& b){return a.event<b.event;});infos_[i]=source_infos[i];infos_[i].events=e.data();infos_[i].event_count=static_cast<std::uint32_t>(e.size());}
}
const StateOwnerInfo40* CharacterStateOwner::info(std::int32_t id)const noexcept{return id>=0&&id<20?&infos_[static_cast<unsigned>(id)]:nullptr;}
int CharacterStateOwner::initialize_level(std::int32_t p,const StateOwnerServices16& c){return dh2_character_state_owner_initialize_level(&machine_,p,&c);}
int CharacterStateOwner::transition(std::int32_t n,std::int32_t e,std::uintptr_t p,const StateOwnerServices16& c){return dh2_character_state_owner_transition(&machine_,n,e,p,&c);}
int CharacterStateOwner::event(std::int32_t e,std::uintptr_t p,const StateOwnerServices16& c){return dh2_character_state_owner_event(&machine_,e,p,&c);}
}
