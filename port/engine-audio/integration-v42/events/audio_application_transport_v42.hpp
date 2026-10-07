#pragma once
#include "../audio_application_manager_v42.hpp"
namespace dh2::audio {
struct AudioActualEventClockV42 {
 void* scope{};
 std::int64_t(*actual_event_monotonic_ns)(void*){};
};
// Capture the Application owner before producer position/getter callbacks.
// This copy keeps the actual manager alive across nested synchronous callbacks.
inline bool audio_submit_application_v42(const AudioApplicationBorrowV42& captured,
 const AudioActualEventClockV42& clock,const character::CombatSoundPlayV1& request,std::string& error){
 if(!captured.manager){error="Required captured actual Application SoundManager";return false;}
 if(request.manager!=captured.identity()){error="Required actual captured Application SoundManager identity";return false;}
 if(!clock.actual_event_monotonic_ns){error="Required actual source event timestamp provider";return false;}
 const auto timestamp=clock.actual_event_monotonic_ns(clock.scope);
 if(timestamp<=0){error="Required positive actual source event monotonic timestamp";return false;}
 return captured.manager->submit_actual_play(request,timestamp,error);
}
}
