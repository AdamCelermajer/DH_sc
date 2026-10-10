#pragma once
#include "../audio_gameplay_runtime_v42.hpp"
#include "../audio_clock_v40.hpp"
#include "../integration-v40/focus/audio_lifecycle_gate_v40.hpp"
#include "../integration-v40/focus/audio_native_session_v40.hpp"
#include <memory>
#include <string>

namespace dh2::audio {
// Production platform factory. The shared context owner is pinned by the
// native session until checked output closure, control-thread join, and drain.
struct AudioSessionControlFactoryV42 {
    using Construct=std::unique_ptr<AudioSessionControlOwnerV40>(*)(
        void*,AudioMixerV34&,AudioClockV40&,AudioLifecycleGateV40&,std::string&);
    Construct construct{};
    void* context{};
    std::shared_ptr<void> context_owner;
    bool valid()const noexcept{return construct&&(!context||bool(context_owner));}
    explicit operator bool()const noexcept{return valid();}
};

// Empty on platforms with no production control default. Windows callers must
// pass an explicit WinMM factory; Android keeps its existing AAudio default.
AudioSessionControlFactoryV42 default_audio_session_control_factory_v42() noexcept;
}
