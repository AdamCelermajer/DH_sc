#include "character_spawn_select.hpp"
using namespace dh2::character;
extern "C" int dh2_character_spawn_select(NativeSpawn24* s,std::uint32_t delayed,std::uint32_t ignored,const SpawnSelectServices16* c){
 (void)ignored;
 if(!s||!s->fsm||!s->fsm->state||!s->fsm->character||s->fsm->reserved||s->fsm->current_present>1||!s->random||!c||!c->invoke)return -1;
 std::uint32_t duration=0;bool timer=false;
 if(delayed){
  if(s->minimum_ms<0)s->minimum_ms=0;
  if(s->maximum_ms<s->minimum_ms)s->maximum_ms=s->minimum_ms;
  timer=s->maximum_ms!=0;
  duration=static_cast<std::uint32_t>(s->minimum_ms);
  if(s->maximum_ms!=s->minimum_ms)duration+=static_cast<std::uint32_t>(dh2_combat_random(s->random,s->maximum_ms-s->minimum_ms));
 }
 const SpawnSelectRequest32 r{timer?spawn_select_timer:spawn_select_state,timer?duration:1u,timer?0u:~0u,timer?0x2du:0u,s->fsm->character,0};
 std::uint32_t ignored_response=0;
 return c->invoke(c->context,s,&r,&ignored_response)?-2:1;
}
