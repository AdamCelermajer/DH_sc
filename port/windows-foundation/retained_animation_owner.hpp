#pragma once

#include "retained_pose_playback.hpp"
#include "../level-world/visual_motion.hpp"
#include "../engine-animation/events.hpp"
#include <memory>
#include <map>

namespace dh::foundation {

struct RetainedAnimationEvent {
    std::string name, clip_id;
    std::uint32_t slot=0, wall_timestamp_ms=0;
    std::uint64_t generation=0;
    std::int32_t lag_ms=0;
};
struct RetainedAnimationFrame {
    std::vector<RetainedAnimationEvent> events;
    Vec3 authored_motion{};
    bool move_go=false;
    dh2::timeline::Completion completion{};
};

class RetainedAnimationOwner {
public:
    // Source default-less root bindings deliberately retain incoming scratch.
    // Callback must write when an authored/default root channel exists, or leave
    // scratch unchanged when source sampling has no value. Never derive it from
    // a blended pose. Caller and visual must outlive this owner.
    using RootSampler=std::function<bool(const std::string& clip,std::int32_t source_ms,
                                         std::array<float,3>& scratch,std::string& error)>;
    RetainedAnimationOwner(CharacterVisual& visual,RootSampler root)
        : visual_(visual),pose_(visual),root_(std::move(root)) {}
    // Owns exact serialized event names/times (including original frame encoding).
    // Bind every named clip before selection. No presets or guessed marker times.
    bool bind_events(const std::string& clip,const std::uint8_t* bres,std::size_t size,std::string& error);
    bool select(const std::string& clip,bool loop,float rate,std::int32_t blend_out_ms,
                bool move_go,RetainedAnimationFrame& output,std::string& error,
                std::int32_t same_slot_replay_extra_ms=0);
    bool advance(double seconds,RetainedAnimationFrame& output,std::string& error);
    bool set_source_rate(float rate,std::string& error) { return pose_.set_source_rate(rate,error); }
    const RetainedPosePlayback& pose() const noexcept { return pose_; }
    const std::array<dh2::visual::Delta,2>& root_histories() const noexcept { return histories_; }
    // Original completion is latched for the animator/sequence owner to consume.
    dh2::timeline::Completion take_completion() noexcept;
    void clear() noexcept;

private:
    bool collect(const std::array<RetainedPoseSlot,2>& before,bool selection,
                 RetainedAnimationFrame& output,std::string& error);
    CharacterVisual& visual_;
    RetainedPosePlayback pose_;
    RootSampler root_;
    std::map<std::string,std::shared_ptr<dh2::animation::EventTrack>> events_;
    std::array<dh2::animation::EventCursor,2> cursors_{};
    std::array<std::uint64_t,2> generations_{};
    std::array<dh2::visual::Delta,2> histories_{};
    dh2::timeline::Completion completion_{};
    bool move_go_=false;
};

// Complete-pose convenience policy. Source dynamic/default-less registration
// needs its own RootSampler rather than claiming that full rest poses reproduce
// missing-channel retention semantics.
inline RetainedAnimationOwner::RootSampler complete_pose_root_sampler(CharacterVisual& visual) {
    return [&visual](const std::string& clip,std::int32_t time,std::array<float,3>& scratch,std::string& error) {
        Vec3 point;if(!visual.sample_root_translation(clip,time,point,error)) return false;
        scratch={point.x,point.y,point.z};return true;
    };
}

} // namespace dh::foundation
