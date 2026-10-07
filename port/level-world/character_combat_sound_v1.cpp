#include "character_combat_sound_v1.hpp"
namespace dh2::character {
int character_combat_sound_v1(CombatSoundOutputV1* out,const data::CombatResult* result,
 std::uintptr_t attacker,std::uintptr_t target,bool character_attacker,const CombatSoundServicesV1* s){
 if(!out||!result||!attacker||!target||!s)return -1;
 *out={};const CombatSoundRowV1 *a=nullptr,*t=nullptr;
 auto phase=[&](unsigned p){out->phase=p;++out->calls;};
 if(character_attacker){phase(1);if(!s->row||s->row(s->context,attacker,&a)||!a)return -2;}
 phase(2);if(!s->row||s->row(s->context,target,&t)||!t)return -2;
 phase(3);std::uint32_t minimal=0;if(!s->minimal_randoms||s->minimal_randoms(s->context,&minimal))return -2;
 if(minimal)return 0;
 phase(4);bool dead=false;if(!s->dead||s->dead(s->context,target,&dead))return -2;
 auto play=[&](const CombatSoundListV1& list)->bool{
  if(!list.count)return true;
  if(!list.ids)return false;
  phase(5);std::uintptr_t manager=0;if(!s->manager||s->manager(s->context,&manager)||!manager)return false;
  phase(6);std::uint32_t index=0;if(!s->random||s->random(s->context,list.count,&index)||index>=list.count)return false;
  CombatSoundPlayV1 request{manager,target,list.ids[index],{},false,1,-1.f,-1.f};
  phase(7);if(!s->position||s->position(s->context,target,&request.position))return false;
  phase(8);if(!s->play||s->play(s->context,&request))return false;
  ++out->played;return true;
 };
 if(dead){if(!play(t->death))return -2;}
 else if(result->amount>0){if(!play(t->hit))return -2;}
 if(character_attacker&&result->amount>0){
  if(t->use_impact1){if(!play(a->impact1))return -2;}
  else if(t->use_impact2){if(!play(a->impact2))return -2;}
 }
 return 0;
}
}
