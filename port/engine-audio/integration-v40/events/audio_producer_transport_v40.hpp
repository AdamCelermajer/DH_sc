#pragma once
#include "../../../level-world/character_combat_sound_v1.hpp"
#include <cstdint>
#include <string>
namespace dh2::audio {
// Borrowed synchronous services to ONE AudioGameplayRuntimeV40 and the
// actual producer event-clock scope. Root owns both contexts/lifetimes.
// Event time already resolves scheduler lag. No helper private now()/frame
// clock, Level phase, listener, binding selection or clip fallback exists.
struct AudioProducerTransportV40 {
 void* runtime_context{};
 bool(*submit_actual_play)(void*,const character::CombatSoundPlayV1&,
                          std::int64_t,std::string&){};
 void* event_clock_context{};
 std::int64_t(*actual_event_monotonic_ns)(void*){};
};
inline bool audio_submit_actual_producer_v40(const AudioProducerTransportV40& s,
 const character::CombatSoundPlayV1& request,std::string& error){
 if(!s.actual_event_monotonic_ns){error="Required actual producer event-clock authority";return false;}
 if(!s.submit_actual_play){error="Required same AudioGameplayRuntimeV40 transport";return false;}
 const auto event=s.actual_event_monotonic_ns(s.event_clock_context);
 if(event<=0){error="Required positive actual source-event monotonic timestamp";return false;}
 // Forward every source request bit and signed ID, including negative IDs.
 // Runtime owns original gates and returns true for source early returns.
 return s.submit_actual_play(s.runtime_context,request,event,error);
}
}
