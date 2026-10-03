#include "combat_events.hpp"
#include <cstring>
extern "C" unsigned dh2_combat_event_route(dh2::data::CombatEventAction* out,const dh2::data::CombatEventContext* context,const char* name){
 if(!out||!context||!name||context->can_range>1)return 1;
 dh2::data::CombatEventAction action;
 if(context->state==5){
  const bool main=std::strcmp(name,"attack_mainhand")==0;
  if(context->can_range){
   if(main||std::strcmp(name,"attack_ranged")==0||std::strcmp(name,"do_skill")==0){action.kind=dh2::data::CombatEventKind::projectile;action.sequence_step=context->projectile;}
  }else if(main||std::strcmp(name,"attack_offhand")==0){
   action.kind=dh2::data::CombatEventKind::melee;action.sequence_step=context->sequence_step;
   const auto raw=std::uint32_t(context->clip_step)-1;std::memcpy(&action.attack_step,&raw,4);action.offhand=!main;
  }
 }
 *out=action;return 0;
}
