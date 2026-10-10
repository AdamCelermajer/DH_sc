#pragma once
#include "retained_frame_audio_clock.hpp"
#include "../../retained_animation_owner.hpp"
#include <exception>
#include <utility>

namespace dh::foundation::audio {
inline RetainedFrameAudioObserverDiagnostic observe_retained_frame_audio(
    const RetainedFrameAudioObserver& observer,ActorId actor,const RetainedAnimationEvent& event,
    std::uint32_t event_index,const RetainedFrameAudioClock* clock) {
    RetainedFrameAudioObserverDiagnostic result;
    result.actor=actor;result.event_index=event_index;result.event_name=event.name;
    if(!observer){result.status=RetainedFrameAudioObserverStatus::not_applicable;return result;}
    if(!clock||!clock->valid()){
        result.status=RetainedFrameAudioObserverStatus::unavailable_clock;
        result.detail="No valid caller-supplied WinMM TIME_SAMPLES/QPC pair";
        return result;
    }
    try {
        std::string detail;
        result.status=observer(actor,event,event_index,clock,detail);
        result.detail=std::move(detail);
        if(result.status==RetainedFrameAudioObserverStatus::dispatched&&!result.detail.empty())
            result.status=RetainedFrameAudioObserverStatus::playback_diagnostic;
    } catch(const std::exception& ex) {
        result.status=RetainedFrameAudioObserverStatus::playback_diagnostic;
        result.detail=ex.what();
    } catch(...) {
        result.status=RetainedFrameAudioObserverStatus::playback_diagnostic;
        result.detail="Retained-frame audio observer threw an unknown exception";
    }
    return result;
}

template<class GameplayEvent>
bool dispatch_retained_frame_event_with_audio_observer(
    GameplayEvent&& gameplay_event,const RetainedFrameAudioObserver& observer,ActorId actor,
    const RetainedAnimationEvent& event,std::uint32_t event_index,
    const RetainedFrameAudioClock* clock,RetainedFrameAudioObserverDiagnostic& diagnostic,
    std::string& error) {
    if(!gameplay_event(error))return false;
    diagnostic=observe_retained_frame_audio(observer,actor,event,event_index,clock);
    return true;
}
}
