#include "character_timers.hpp"
#include <limits>
namespace {
using namespace dh2::character;
bool valid(const TimerStore32* s){
 if(!s||s->reserved||s->count>s->capacity||s->capacity>std::uint32_t(std::numeric_limits<std::int32_t>::max())||
    (s->capacity&&!s->slots))return false;
 for(std::uint32_t i=0;i<s->count;++i)if(s->slots[i].reserved||s->slots[i].id!=i)return false;
 return true;
}
bool services(const TimerServices32* c){return c&&!c->reserved;}
}
extern "C" std::int32_t dh2_character_timer_start(TimerStore32* s,std::uint32_t duration,
 std::int32_t repeat,std::int32_t event,std::uintptr_t ref,const TimerServices32* c){
 if(!valid(s)||!services(c))return -1;
 std::uint32_t index=0;while(index<s->count&&s->slots[index].active)++index;
 if(index==s->count){
  if(s->count==s->capacity){
   if(s->update_depth)return -3;
   if(!c->grow||s->count==std::uint32_t(std::numeric_limits<std::int32_t>::max()))return -2;
   const auto count=s->count;const auto owner=s->owner;
   if(c->grow(c->context,s,count+1)!=1)return -2;
   if(!valid(s)||s->count!=count||s->owner!=owner||s->update_depth||s->capacity<=count)return -1;
  }
  s->slots[index]=Timer32{};s->slots[index].id=index;++s->count;
 }
 auto& t=s->slots[index];t.active=1;t.paused=0;t.repeat=repeat;t.duration_ms=duration;
 t.elapsed_ms=0;t.event=event;t.user_ref=ref;return std::int32_t(index);
}
extern "C" int dh2_character_timers_update(TimerStore32* s,std::uint32_t dt,std::uint32_t blocked,const TimerServices32* c){
 if(!valid(s)||!services(c)||!c->expired||s->update_depth==std::numeric_limits<std::uint32_t>::max())return -1;
 if(blocked)return 1;
 const auto count=s->count;++s->update_depth;
 for(std::uint32_t i=0;i<count;++i){
  auto* t=s->slots+i;if(!t->active||t->paused)continue;
  t->elapsed_ms+=dt;
  while(t->active&&t->duration_ms&&t->elapsed_ms>=t->duration_ms){
   if(!t->repeat)t->active=0;
   else{t->elapsed_ms-=t->duration_ms;if(t->repeat>0)--t->repeat;}
   c->expired(c->context,s->owner,t->event==-1?0x29:t->event,t);
  }
 }
 --s->update_depth;return 1;
}
extern "C" int dh2_character_timer_pause(TimerStore32* s,std::uint32_t id,std::uint32_t paused){
 if(!valid(s)||paused>1)return -1;
 if(id>=s->count)return 0;
 s->slots[id].paused=std::uint8_t(paused);return 1;
}
extern "C" int dh2_character_timer_stop(TimerStore32* s,std::uint32_t id){
 if(!valid(s))return -1;
 if(id>=s->count)return 0;
 s->slots[id].active=0;return 1;
}
extern "C" int dh2_character_timers_stop_all(TimerStore32* s){
 if(!valid(s))return -1;
 for(std::uint32_t i=0;i<s->count;++i)s->slots[i].active=0;
 return 1;
}
extern "C" int dh2_character_timer_time_left(std::uint32_t* elapsed,std::uint32_t* duration,const TimerStore32* s,std::uint32_t id){
 if(!elapsed||!duration||!valid(s))return -1;
 if(id>=s->count||!s->slots[id].active)return 0;
 *elapsed=s->slots[id].elapsed_ms;*duration=s->slots[id].duration_ms;return 1;
}
