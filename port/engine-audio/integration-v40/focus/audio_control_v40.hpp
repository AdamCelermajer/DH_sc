#pragma once
#include "audio_output_v40.hpp"
#include "../../audio_clock_v40.hpp"
namespace dh2::audio {
// Construct, tick and shutdown on the dedicated control thread. Borrowed
// mixer/clock/gate must survive it. Never writes the mixer's SPSC command ring.
class AudioControlV40 {
    AudioClockV40& clock_;
    AudioLifecycleGateV40& gate_;
    AndroidAudioOutputV40 output_;
    std::atomic<bool> closed_{false};
    std::atomic<bool> ready_{false};
    std::atomic<std::uint32_t> applied_source_epoch_{0};
    bool stopped_{};
    std::uint64_t generation_{};
public:
    AudioControlV40(AudioMixerV34& mixer,AudioClockV40& clock,AudioLifecycleGateV40& gate)
        :clock_(clock),gate_(gate),output_(mixer,gate){}
    bool tick(std::string&);
    bool shutdown(std::string&);
    bool close_succeeded()const noexcept { return closed_.load(std::memory_order_acquire); }
    bool ready_for_current_source()const noexcept {
        return ready_.load(std::memory_order_acquire)&&output_.audible_requested()&&gate_.permitted_for(applied_source_epoch_.load(std::memory_order_acquire));
    }
};
}
