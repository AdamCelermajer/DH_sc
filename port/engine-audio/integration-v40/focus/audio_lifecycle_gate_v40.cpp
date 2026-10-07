#include "audio_lifecycle_gate_v40.hpp"
namespace dh2::audio {
bool AudioLifecycleGateV40::publish_activity(std::uint32_t owner,std::uint32_t sequence,
 bool resumed,bool window,bool focus,bool dead) noexcept {
    if(!owner||!sequence||sequence>0xffffffu)return false;
    const auto flags=std::uint64_t(resumed)|(std::uint64_t(window)<<1)|(std::uint64_t(focus)<<2)|(std::uint64_t(dead)<<3);
    const auto next=(std::uint64_t(owner)<<32)|(std::uint64_t(sequence)<<8)|flags;
    auto previous=activity_.load(std::memory_order_acquire);
    for(;;) {
        const auto preceding_owner=std::uint32_t(previous>>32);
        const auto preceding_sequence=std::uint32_t((previous>>8)&0xffffffu);
        if(owner<preceding_owner||(owner==preceding_owner&&(sequence<=preceding_sequence||(previous&8))))return false;
        if(activity_.compare_exchange_weak(previous,next,std::memory_order_release,std::memory_order_acquire))return true;
    }
}
std::uint32_t AudioLifecycleGateV40::begin_source() noexcept {
    auto previous=source_.load(std::memory_order_acquire);
    for(;;) {
        const auto epoch=std::uint32_t(previous>>1);
        if(epoch==0xffffffffu)return 0;
        const auto next=std::uint64_t(epoch+1)<<1;
        if(source_.compare_exchange_weak(previous,next,std::memory_order_release,std::memory_order_acquire))return epoch+1;
    }
}
bool AudioLifecycleGateV40::publish_source(std::uint32_t epoch,bool ready) noexcept {
    if(!epoch)return false;
    auto previous=source_.load(std::memory_order_acquire);
    while(std::uint32_t(previous>>1)==epoch) {
        if(source_.compare_exchange_weak(previous,(std::uint64_t(epoch)<<1)|std::uint64_t(ready),std::memory_order_release,std::memory_order_acquire))return true;
    }
    return false;
}
bool AudioLifecycleGateV40::permitted() const noexcept {
    const auto state=activity_.load(std::memory_order_acquire);
    return (state&15)==7&&source_ready();
}
bool AudioLifecycleGateV40::permitted_for(std::uint32_t epoch) const noexcept {
    const auto state=activity_.load(std::memory_order_acquire);
    return epoch&&(state&15)==7&&source_.load(std::memory_order_acquire)==((std::uint64_t(epoch)<<1)|1);
}
AudioLifecycleGateV40& application_audio_gate_v40() noexcept { static AudioLifecycleGateV40 gate;return gate; }
}
