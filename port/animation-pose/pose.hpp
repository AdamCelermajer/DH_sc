#pragma once
#include "../skin-payloads/skin.hpp"

// An absolute-key diagnostic pose evaluator. This does not implement the
// original animator's default-relative blending, transitions or state machine.
namespace dh2::pose {
enum class Error : std::uint32_t {
    ok,
    argument,
    range,
    unsupported,
    nonfinite,
    keys,
    duplicate,
    scene,
    joint,
    limit
};
struct Clip {
    dh2::assets::Animation tracks[128];
    std::uint32_t count;
    std::int32_t start, end;
};
} // namespace dh2::pose
extern "C" {
dh2::pose::Error dh2_pose_clip_open(dh2::pose::Clip *, const dh2::resources::BresView *,
                                    std::int32_t segment);
dh2::pose::Error dh2_pose_sample(const dh2::pose::Clip *, std::uint32_t track,
                                 std::int32_t milliseconds, float *value4);
dh2::pose::Error dh2_pose_node(const dh2::pose::Clip *, std::int32_t milliseconds,
                               const dh2::scene::Node *, dh2::scene::Node *);
dh2::pose::Error dh2_pose_skin_palette(const dh2::pose::Clip *, std::int32_t milliseconds,
                                       const dh2::skin::Skin *, const dh2::scene::Visual *,
                                       dh2::math::Matrix4f *, std::size_t capacity);
}
