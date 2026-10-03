#include "character_animation_events.hpp"

extern "C" int dh2_character_animation_event_route(
 const dh2::character::AnimationEventFacts* facts,
 const dh2::character::AnimationEventServices* services){
 using namespace dh2::character;
 if(!facts||!services||!services->invoke||facts->event<0x22||facts->event>0x27||
    facts->global_blocked>1||facts->controller_locked>1||facts->controller_forced>1)return -1;
 // The original saves event/payload in registers before synchronous delivery.
 const auto event=facts->event;const auto payload=facts->payload;
 auto invoke=[&](AnimationEventService kind){
  const AnimationEventRequest request{kind,event,payload};
  return services->invoke(services->context,&request);
 };
 if(event==0x22||event==0x23){
  // The two switch branches run before generic controller/global gating.
  invoke(animation_end_virtual);
  invoke(animation_state_event);return 1;
 }
 if(!facts->controller_forced&&(facts->global_blocked||facts->controller_locked)){
  invoke(animation_state_event);return 1;
 }
 const auto state=invoke(animation_state_getter);
 if(event==0x24||event==0x25){invoke(animation_state_event);return 1;}
 AnimationEventService kind;
 if(event==0x27){
  if(state==5)kind=animation_attack_end;
  else if(state==6||state==7)kind=animation_skill_end;
  else {invoke(animation_state_event);return 1;}
 }else{
  if(state==4)kind=animation_move_begin;
  else if(state==5)kind=animation_attack_begin;
  else if(state==6||state==7)kind=animation_skill_begin;
  else {invoke(animation_state_event);return 1;}
 }
 if(invoke(kind))invoke(animation_state_event);
 return 1;
}
