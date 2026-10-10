#pragma once
#include "linux_sdl2_output.hpp"
#include "../../../engine-audio/integration-v40/focus/audio_lifecycle_gate_v40.hpp"
#include "../../../engine-audio/integration-v42/audio_session_control_factory_v42.hpp"
#include <atomic>

namespace dh::foundation::audio {

// Control-thread owner for the V42 session's one shared V34 mixer and clock.
class LinuxSdl2SourceSessionControlV1 final:public dh2::audio::AudioSessionControlOwnerV40 {
 dh2::audio::AudioClockV40& clock_;
 dh2::audio::AudioLifecycleGateV40& gate_;
 LinuxSdl2AudioOutput output_;
 std::uint32_t epoch_{};
 std::uint64_t generation_{};
 std::atomic<bool> closed_{false},ready_{false};
 bool opened_{},focused_{},stopped_{};
public:
 LinuxSdl2SourceSessionControlV1(dh2::audio::AudioMixerV34&,dh2::audio::AudioClockV40&,
                                  dh2::audio::AudioLifecycleGateV40&);
 bool tick(std::string&)override;
 bool shutdown(std::string&)override;
 bool close_succeeded()const noexcept override{return closed_.load(std::memory_order_acquire);}
 bool ready()const noexcept override{return ready_.load(std::memory_order_acquire);}
};

}

namespace dh2::audio {
AudioSessionControlFactoryV42 linux_sdl2_source_session_control_v1();
}
