#pragma once
#include "audio_producer_transport_v40.hpp"
#include "../../audio_gameplay_runtime_v40.hpp"
namespace dh2::audio {
// CONTROLLED INDEPENDENT FIXTURES ONLY. Production uses
// audio_session_transport_v40.hpp to enforce session accepting/UI-close
// exclusion. A direct runtime bridge bypasses that outer lifecycle owner.
inline AudioProducerTransportV40 audio_runtime_transport_v40(
 AudioGameplayRuntimeV40& runtime,void* actual_event_scope,
 std::int64_t(*actual_event_time)(void*))noexcept{
 return {&runtime,
  [](void* context,const character::CombatSoundPlayV1& request,std::int64_t time,std::string& error){
   return static_cast<AudioGameplayRuntimeV40*>(context)->submit_actual_play(request,time,error);
  },actual_event_scope,actual_event_time};
}
}
