#pragma once
#include "audio_producer_transport_v40.hpp"
#include "../focus/audio_native_session_v40.hpp"
namespace dh2::audio {
// PRODUCTION endpoint: the sole owning session enforces producer/thread,
// accepting and independent UI close exclusion before its runtime receives
// a submission. Borrowed session/event scope must outlive producer calls.
inline AudioProducerTransportV40 audio_session_transport_v40(
 AudioNativeSessionV40& actual_session,void* actual_event_scope,
 std::int64_t(*actual_event_time)(void*))noexcept{
 return {&actual_session,
  [](void* context,const character::CombatSoundPlayV1& request,std::int64_t time,std::string& error){
   return static_cast<AudioNativeSessionV40*>(context)->submit_actual_play(request,time,error);
  },actual_event_scope,actual_event_time};
}
}
