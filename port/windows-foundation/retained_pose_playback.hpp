#pragma once

#include "original_character.hpp"
#include "original_pose_blend.hpp"
#include "../level-world/visual_timeline.hpp"

namespace dh::foundation {

struct RetainedPoseSlot {
    std::string clip_id;
    dh2::timeline::State timeline{};
    std::uint64_t generation = 0;
    bool bound = false;
};
struct RetainedPoseSample {
    std::uint32_t slot = 0;
    std::string clip_id;
    std::uint64_t generation = 0;
    std::int32_t previous_ms = 0, current_ms = 0;
    std::uint32_t wall_timestamp_ms = 0;
    float weight = 0;
    bool initialized_now = false;
    bool ended_now = false;
    // Source scaled frame duration before clamp/wrap; owners need this to handle
    // more than one loop rather than guessing from previous/current integers.
    float source_frame_seconds = 0;
};

// Pose-only original two-slot owner. While bound, do not call CharacterVisual's
// ordinary update/select: this owner independently samples and publishes pose.
// Marker dispatch, root delta histories, completion policy and gameplay state
// transitions remain explicit responsibilities of the host using sample records.
class RetainedPosePlayback {
public:
    explicit RetainedPosePlayback(CharacterVisual& visual) : visual_(visual) {}
    bool select(const std::string& clip, bool loop, float source_rate,
                std::int32_t blend_out_ms, std::string& error,
                std::int32_t same_slot_replay_extra_ms = 0);
    // Source SetScale visits both retained timelines, including zero-weight slots.
    // Rate zero pauses source clips; outgoing BlendOut still uses wall time.
    bool set_source_rate(float rate, std::string& error);
    bool advance(double wall_seconds, std::string& error);
    std::uint32_t current_slot() const noexcept { return blender_.current_slot(); }
    const std::array<RetainedPoseSlot,2>& slots() const noexcept { return slots_; }
    const dh2::animation::BlenderState& blend_state() const noexcept { return blender_.state(); }
    const std::vector<RetainedPoseSample>& samples() const noexcept { return samples_; }
    bool current_ended() const noexcept;
    // Clear does not modify the visual's published pose.
    void clear() noexcept;

private:
    bool publish(OriginalPoseBlend&, std::array<RetainedPoseSlot,2>&, std::uint32_t timestamp,
                 std::vector<RetainedPoseSample>&, std::string& error);
    CharacterVisual& visual_;
    OriginalPoseBlend blender_;
    std::array<RetainedPoseSlot,2> slots_{};
    std::vector<RetainedPoseSample> samples_;
    double wall_seconds_ = 0;
};

} // namespace dh::foundation
