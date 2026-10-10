#pragma once

#include "../scene-materials/scene.hpp"
#include "../level-world/animation_blender.hpp"
#include <functional>

namespace dh::foundation {

struct LocalNodePose {
    std::string id;
    std::array<float,3> translation{0,0,0};
    std::array<float,4> quaternion{0,0,0,1};
    std::array<float,3> scale{1,1,1};
};
using SkeletalPose = std::vector<LocalNodePose>;

bool capture_scene_pose(const dh2::scene::Scene&, SkeletalPose&, std::string& error);
// Updates existing node storage and instance worlds; no mesh morphing or skinning.
// The caller then constructs skin palettes from these world transforms.
bool apply_scene_pose(const SkeletalPose&, dh2::scene::Scene&, std::string& error);

class OriginalPoseBlend {
public:
    using PoseSampler = std::function<bool(std::uint32_t slot, SkeletalPose&, std::string&)>;
    // Source AnimatorBlender::PlayClip: OUTGOING stored BlendOut determines this
    // transition's fade. Incoming metadata is retained for its later departure.
    // Callers bind/select the returned current slot's resource before evaluate.
    bool select(std::int32_t incoming_blend_out_ms, std::string& error);
    // Absolute uint32 wall milliseconds, matching original timestamp wrap.
    // Samples slot0 then1 only when nonzero weighted, normalizes after sampling,
    // then mixes local positions/scales and original quaternion slerp kernels.
    // Samplers retain both clip clocks; they must not reenter this object. Events,
    // root delta histories and completion notifications belong to their owner.
    // Failure preserves blender/cache/output; samplers own their side effects.
    bool evaluate(std::uint32_t timestamp, const PoseSampler&, SkeletalPose& output,
                  std::string& error);
    std::uint32_t current_slot() const noexcept { return state_.current; }
    const dh2::animation::BlenderState& state() const noexcept { return state_; }
    void clear() noexcept;

private:
    dh2::animation::BlenderState state_{0,0,0,0,0,0,{1,0}};
    std::array<SkeletalPose,2> cached_;
};

} // namespace dh::foundation
