#include "audio_control_v40.hpp"
namespace dh2::audio {
bool AudioControlV40::tick(std::string& error) {
    if(stopped_){error="Native audio control has stopped";return false;}
    if(!output_.recover(error)||!output_.apply(error)) {
        ready_.store(false,std::memory_order_release);
        clock_.invalidate(std::uint64_t(gate_.source_epoch())<<32);return false;
    }
    AudioDeviceClockSnapshotV40 sample;
    if(output_.snapshot(sample)) {
        generation_=sample.generation;
        clock_.publish({sample.position,sample.monotonic_ns,sample.base_frame,sample.generation,std::uint32_t(sample.rate),true});
        applied_source_epoch_.store(std::uint32_t(sample.generation>>32),std::memory_order_release);
        ready_.store(true,std::memory_order_release);
    } else {
        ready_.store(false,std::memory_order_release);
        clock_.invalidate(std::uint64_t(gate_.source_epoch())<<32);
    }
    return true;
}
bool AudioControlV40::shutdown(std::string& error) {
    stopped_=true;ready_.store(false,std::memory_order_release);clock_.invalidate(generation_);
    if(!output_.close(error))return false;
    closed_.store(true,std::memory_order_release);return true;
}
}
