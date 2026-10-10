#include "retained_pose_playback.hpp"
#include <cmath>
#include <cstring>
#include <limits>

namespace dh::foundation {

bool RetainedPosePlayback::publish(OriginalPoseBlend& blend,
                                   std::array<RetainedPoseSlot,2>& slots,
                                   std::uint32_t timestamp,
                                   std::vector<RetainedPoseSample>& samples,
                                   std::string& error) {
    SkeletalPose pose;
    auto sample = [&](std::uint32_t index, SkeletalPose& output, std::string& problem) {
        auto& slot=slots[index];
        if (!slot.bound) { problem="Weighted retained clip slot is unbound"; return false; }
        const auto previous=slot.timeline.current_ms;
        const bool initialized=slot.timeline.initialized!=0, ended=slot.timeline.ended!=0;
        std::int32_t signed_time;
        std::memcpy(&signed_time,&timestamp,sizeof(signed_time));
        if (dh2_timeline_update(&slot.timeline,signed_time,nullptr)) {
            problem="Retained source timeline update rejected"; return false;
        }
        if (!visual_.sample_local_pose(slot.clip_id,slot.timeline.current_ms,output,problem)) return false;
        samples.push_back({index,slot.clip_id,slot.generation,previous,slot.timeline.current_ms,
                           timestamp,0,!initialized,slot.timeline.ended!=0&&!ended,slot.timeline.frame_seconds});
        return true;
    };
    if (!blend.evaluate(timestamp,sample,pose,error)) return false;
    for (auto& record:samples) record.weight=blend.state().weights[record.slot];
    return visual_.apply_local_pose(pose,error);
}

bool RetainedPosePlayback::select(const std::string& clip, bool loop, float rate,
                                  std::int32_t blend_out_ms, std::string& error,
                                  std::int32_t replay_extra_ms) {
    error.clear();
    if (clip.empty() || !std::isfinite(rate) || rate<0 || replay_extra_ms<0) {
        error="Retained clip selection metadata is invalid"; return false;
    }
    std::int32_t start=0,end=0;
    if (!visual_.animation_range(clip,start,end,error)) return false;
    if (end<start) { error="Retained clip source range is invalid"; return false; }
    auto blend=blender_; auto slots=slots_;
    if (!blend.select(blend_out_ms,error)) return false;
    auto& slot=slots[blend.current_slot()];
    if (slot.generation==std::numeric_limits<std::uint64_t>::max()) {
        error="Retained clip generation exhausted"; return false;
    }
    const bool same_clip=slot.bound && slot.clip_id==clip;
    slot.timeline.library_present=1;
    if (dh2_timeline_clip(&slot.timeline,static_cast<std::int32_t>(blend.current_slot()),start,end)
        || dh2_timeline_loop(&slot.timeline,loop?1:0)) {
        error="Retained source clip setup rejected"; return false;
    }
    // Source PlayClip checks the newly selected physical slot's prior clip,
    // then jumps start+applicator extra after selection cleared ended. No loop gate.
    if (same_clip) {
        const std::uint32_t word=std::uint32_t(start)+std::uint32_t(replay_extra_ms);
        std::int32_t replay; std::memcpy(&replay,&word,sizeof(replay));
        if (dh2_timeline_jump(&slot.timeline,replay)) { error="Same-slot source replay rejected"; return false; }
    }
    slot.clip_id=clip; slot.bound=true; ++slot.generation;
    for (auto& retained:slots) if (dh2_timeline_scale(&retained.timeline,rate)) {
        error="Retained source SetScale rejected"; return false;
    }
    const auto total_ms=static_cast<std::uint64_t>(std::floor(wall_seconds_*1000));
    std::vector<RetainedPoseSample> samples;
    if (!publish(blend,slots,static_cast<std::uint32_t>(total_ms),samples,error)) return false;
    blender_=std::move(blend);slots_=std::move(slots);samples_=std::move(samples);
    return true;
}

bool RetainedPosePlayback::set_source_rate(float rate,std::string& error) {
    error.clear();
    if (!std::isfinite(rate) || rate<0) { error="Retained source playback rate is invalid"; return false; }
    auto slots=slots_;
    for (auto& slot:slots) if (dh2_timeline_scale(&slot.timeline,rate)) {
        error="Retained source SetScale rejected"; return false;
    }
    slots_=std::move(slots);return true;
}

bool RetainedPosePlayback::advance(double seconds,std::string& error) {
    error.clear();
    // Integer absolute timestamps match the source ABI. Keep exact integer
    // conversion bounded and reject huge individual frames before pose mutation.
    const double next=wall_seconds_+seconds;
    if (!std::isfinite(seconds) || seconds<0 || seconds*1000>std::numeric_limits<std::int32_t>::max()
        || !std::isfinite(next) || next*1000>9007199254740991.0) {
        error="Retained pose wall interval is outside the source clock domain"; return false;
    }
    if (!slots_[current_slot()].bound) { error="Retained pose has no selected clip"; return false; }
    auto blend=blender_;auto slots=slots_;
    const auto timestamp=static_cast<std::uint32_t>(static_cast<std::uint64_t>(std::floor(next*1000)));
    std::vector<RetainedPoseSample> samples;
    if (!publish(blend,slots,timestamp,samples,error)) return false;
    blender_=std::move(blend);slots_=std::move(slots);samples_=std::move(samples);wall_seconds_=next;
    return true;
}

bool RetainedPosePlayback::current_ended() const noexcept {
    const auto& current=slots_[current_slot()];
    return current.bound && current.timeline.ended!=0;
}
void RetainedPosePlayback::clear() noexcept {
    blender_.clear();slots_={};samples_.clear();wall_seconds_=0;
}

} // namespace dh::foundation
