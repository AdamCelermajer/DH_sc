#pragma once
#include "audio_producer_transport_v40.hpp"
#include "../../audio_named_animation_sound_v38.hpp"
#include "../../../level-world/character_melee_animation_event_v1.hpp"
namespace dh2::audio {
struct AudioAuthoredEventBorrowV40 {
 const AudioSourceBindingsV38* actual_names{};
 AudioNamedAnimationSoundServicesV38 actual_named_source;
 AudioProducerTransportV40 transport;
 // Genuine existing actor-specific providers, including step/count,
 // state/range/attack/skill/spell/interaction/FX/Lua and source Debug.
 // This adapter supplies ONLY named sound, never accepts missing owners.
 character::skills::MeleeAnimationServicesV1 remaining;
};
inline int audio_dispatch_actual_authored_event_v40(
 character::skills::MeleeAnimationOutputV1& output,const char* event,
 const AudioAuthoredEventBorrowV40& borrow,std::string& error){
 struct Frame {const AudioAuthoredEventBorrowV40& b;std::string& error;};
 Frame frame{borrow,error};
 character::skills::MeleeAnimationServicesV1 services{&frame,
 [](void* raw,const character::skills::MeleeAnimationRequestV1* q,
    character::skills::MeleeAnimationResponseV1* response){
  auto& f=*static_cast<Frame*>(raw);if(!q||!response)return -1;
  if(q->operation!=character::skills::melee_event_sound_fx){
   if(!f.b.remaining.invoke){f.error="Required actual actor authored-event provider";return -1;}
   return f.b.remaining.invoke(f.b.remaining.context,q,response);
  }
  if(!f.b.actual_names){f.error="Required genuine generated source sound names";return -1;}
  struct Named {Frame& f;};Named named{f};
  auto source=f.b.actual_named_source;
  // Actual manager/position callbacks retain their own original contexts.
  // Only the reached Play3D delivery is changed to the common transport.
  AudioNamedAnimationSoundServicesV38 routed{};routed.context=&named;
  routed.actual_character=source.actual_character;
  routed.manager=[](void* raw,std::uintptr_t& value){auto& n=*static_cast<Named*>(raw);const auto& s=n.f.b.actual_named_source;return s.manager?s.manager(s.context,value):-1;};
  routed.target_position=[](void* raw,std::uintptr_t actor,std::array<float,3>& value){auto& n=*static_cast<Named*>(raw);const auto& s=n.f.b.actual_named_source;return s.target_position?s.target_position(s.context,actor,value):-1;};
  routed.play=[](void* raw,const character::CombatSoundPlayV1& request){auto& n=*static_cast<Named*>(raw);return audio_submit_actual_producer_v40(n.f.b.transport,request,n.f.error)?0:-1;};
  AudioNamedAnimationSoundResultV38 result{};
  const int status=audio_named_animation_sound_v38(q->text,*f.b.actual_names,routed,result);
  if(status&&f.error.empty())f.error=result.required?result.required:"Required named source sound leaf";
  return status;
 },borrow.remaining.debug};
 return character::skills::dh2_character_melee_animation_event_v1(&output,event,&services);
}
}
