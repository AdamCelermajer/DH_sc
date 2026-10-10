#pragma once
#include "winmm_output.hpp"
#include "../../../engine-audio/integration-v42/audio_session_control_factory_v42.hpp"

namespace dh::foundation::audio {
// Dedicated control-thread owner for the SAME V42 session mixer and clock.
class WindowsSourceSessionControlV1 final:public dh2::audio::AudioSessionControlOwnerV40 {
    dh2::audio::AudioClockV40& clock_;
    dh2::audio::AudioLifecycleGateV40& gate_;
    WinmmAudioOutput output_;
    std::uint32_t epoch_{};
    std::uint64_t generation_{};
    std::atomic<bool> closed_{false},ready_{false};
    bool opened_{},focused_{},stopped_{};
public:
    WindowsSourceSessionControlV1(dh2::audio::AudioMixerV34&,dh2::audio::AudioClockV40&,
                                  dh2::audio::AudioLifecycleGateV40&);
    bool tick(std::string&)override;
    bool shutdown(std::string&)override;
    bool close_succeeded()const noexcept override{return closed_.load(std::memory_order_acquire);}
    bool ready()const noexcept override{return ready_.load(std::memory_order_acquire);}
};
}

namespace dh2::audio {
// Creates a production descriptor whose shared context is pinned by the
// AudioNativeSessionV42 until checked close/join/drain succeeds.
AudioSessionControlFactoryV42 windows_source_session_control_v1();
}
