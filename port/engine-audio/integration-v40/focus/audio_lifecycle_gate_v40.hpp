#pragma once
#include <atomic>
#include <cstdint>
namespace dh2::audio {
// JNI/main thread publishes activity snapshots; actual source initialization
// publishes its epoch separately. No AudioManager or output calls on callback.
class AudioLifecycleGateV40 {
    std::atomic<std::uint64_t> activity_{0},source_{0};
public:
    bool publish_activity(std::uint32_t owner,std::uint32_t sequence,bool resumed,
                          bool window_focused,bool focus_granted,bool destroyed) noexcept;
    std::uint32_t begin_source() noexcept;
    bool publish_source(std::uint32_t epoch,bool actually_ready) noexcept;
    bool source_ready() const noexcept { return (source_.load(std::memory_order_acquire)&1)!=0; }
    std::uint32_t source_epoch() const noexcept { return std::uint32_t(source_.load(std::memory_order_acquire)>>1); }
    bool permitted() const noexcept;
    bool permitted_for(std::uint32_t source_epoch) const noexcept;
    bool destroyed() const noexcept { return (activity_.load(std::memory_order_acquire)&8)!=0; }
};
AudioLifecycleGateV40& application_audio_gate_v40() noexcept;
}
