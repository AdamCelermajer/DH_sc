#include "character_ai_death.hpp"
namespace {
using namespace dh2::character;
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return n&&m&&(x<=y?y-x<n:x-y<m);}
bool owner(const AIDeathState64* s){
 if(!aligned(s->owner)||!s->owner->character||!s->owner->state_machine||!aligned(s->owner->timers))return false;
 const auto& t=*s->owner->timers;if(t.reserved||t.count>t.capacity||t.capacity>0x7fffffffu||(t.capacity&&!aligned(t.slots)))return false;
 for(unsigned i=0;i<t.count;i++)if(t.slots[i].reserved||t.slots[i].id!=i)return false;
 const auto* p=s->target;return aligned(p)&&p->identity==s->ai&&!p->reserved&&!p->reserved8&&aligned(p->owner)&&p->owner->identity==s->owner->character&&!p->owner->reserved&&!p->owner->reserved16;
}
bool valid(AIDeathResult24* o,AIDeathState64* s,const TargetServices16* t,const AIDeathServices16* c){
 if(!aligned(o)||!aligned(s)||!s->ai||s->reserved||!owner(s)||!aligned(t)||!t->invoke||!aligned(c)||!c->invoke||(s->active&&!aligned(s->ais_virtuals)))return false;
 const void* p[]={o,s,t,c,s->owner,s->target,s->target->owner,s->owner->timers};const std::size_t z[]={sizeof(*o),sizeof(*s),sizeof(*t),sizeof(*c),sizeof(*s->owner),sizeof(*s->target),sizeof(*s->target->owner),sizeof(*s->owner->timers)};
 for(unsigned i=0;i<8;i++){for(unsigned j=i+1;j<8;j++)if(overlap(p[i],z[i],p[j],z[j]))return false;if(overlap(p[i],z[i],s->owner->timers->slots,std::size_t(s->owner->timers->capacity)*sizeof(Timer32)))return false;}
 if(s->active)for(unsigned i=0;i<8;i++)if(overlap(p[i],z[i],s->ais_virtuals,51*sizeof(std::uintptr_t)))return false;
 return true;
}
struct Run {
 AIDeathResult24 result{};AIDeathState64* state;const TargetServices16* targets;const AIDeathServices16* services;
 bool call(unsigned phase,AIDeathService service,std::uintptr_t subject,std::uintptr_t owner=0,std::uintptr_t payload=0,std::uintptr_t callee=0,unsigned operation=0,unsigned argument=0){result.phase=phase;++result.callbacks;const AIDeathRequest48 request{service,argument,subject,owner,payload,callee,operation,0};return services->invoke(services->context,state,&request)==0;}
 int dead(){
  result.phase=3;if(!owner(state)||dh2_character_ai_set_target(state->target,0,0,targets))return -2;
  result.phase=4;if(dh2_character_ai_sync_last_target(state->target))return -2;
  result.phase=5;if(!owner(state)||!call(5,ai_death_state,state->owner->state_machine,0,0,0x3c58c8,0,1))return -2;
  result.phase=6;if(!owner(state))return -2;int status=dh2_character_timer_stop(state->owner->timers,state->timer0);if(status<0)return -2;result.stopped_timers+=unsigned(status);
  result.phase=7;if(!owner(state))return -2;status=dh2_character_timer_stop(state->owner->timers,state->timer1);if(status<0)return -2;result.stopped_timers+=unsigned(status);
  result.phase=8;state->timer1=state->timer0=0xffffffffu;result.reset_ids=1;
  if(!call(9,ai_death_clear_all_aggro,state->ai,0,0,0x3d5fa8))return -2;
  if(!call(10,ai_death_clear_aggro_toward_me,state->ai,0,0,0x3d6abc))return -2;
  if(!call(11,ai_death_skill_cleanup,state->ai,0,0,0x3d8ae0))return -2;
  if(!call(12,ai_death_spell_cleanup,state->ai,0,0,0x3d8a98))return -2;
  return 1;
 }
 int end(AIDeathResult24* out,int status){result.status=status;if(status==1){result.phase=13;result.completed=1;}*out=result;return status;}
};
}
extern "C" int dh2_character_ai_set_dead(AIDeathResult24* out,AIDeathState64* state,const TargetServices16* targets,const AIDeathServices16* services){if(!valid(out,state,targets,services))return -1;Run run{{},state,targets,services};return run.end(out,run.dead());}
extern "C" int dh2_character_ai_on_died(AIDeathResult24* out,AIDeathState64* state,std::uintptr_t attacker,const TargetServices16* targets,const AIDeathServices16* services){
 if(!valid(out,state,targets,services))return -1;Run run{{},state,targets,services};
 if(state->group&&!run.call(1,ai_death_group,state->group,state->owner->character,attacker,0x3d2628))return run.end(out,-2);
 if(state->active){if(!aligned(state->ais_virtuals))return run.end(out,-2);const auto active=state->active,callee=state->ais_virtuals[9];if(!callee||!run.call(2,ai_death_ais,active,0,attacker,callee,0x24))return run.end(out,-2);}
 return run.end(out,run.dead());
}
