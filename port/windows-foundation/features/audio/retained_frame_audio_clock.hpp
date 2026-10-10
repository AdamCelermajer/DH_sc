#pragma once
#include "../../actor_state.hpp"
#include <cstdint>
#include <functional>
#include <string>

namespace dh::foundation {
// Caller-owned exact device sample paired to the output's QPC observation.
// device_samples is WinMM TIME_SAMPLES; it is not a synthesized mixer frame.
struct RetainedFrameAudioClock {
    std::uint64_t output_generation=0;
    std::int64_t device_samples=-1;
    std::int64_t qpc_monotonic_ns=0;
    bool ready=false;
    bool valid()const noexcept{return ready&&output_generation!=0&&device_samples>=0&&qpc_monotonic_ns>0;}
};
struct RetainedAnimationEvent;
enum class RetainedFrameAudioObserverStatus {
    not_applicable,dispatched,unavailable_clock,required_owner_unavailable,playback_diagnostic
};
struct RetainedFrameAudioObserverDiagnostic {
    ActorId actor=invalid_actor_id;
    std::uint32_t event_index=0;
    RetainedFrameAudioObserverStatus status=RetainedFrameAudioObserverStatus::not_applicable;
    std::string event_name,detail;
};
using RetainedFrameAudioObserver=std::function<RetainedFrameAudioObserverStatus(
    ActorId,const RetainedAnimationEvent&,std::uint32_t,const RetainedFrameAudioClock*,std::string&)>;
}
