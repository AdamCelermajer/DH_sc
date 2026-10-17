#pragma once

#include <cstdint>

namespace dh2::animation::root_motion_applicators_v1 {

// Borrowed view of one source RootSceneNode animator and its current
// GetApplicator result. The identity pair prevents a provider from silently
// substituting an applicator owned by another animator.
struct AnimatorApplicator {
    std::uintptr_t animator_identity;
    std::uintptr_t applicator_identity;
};

struct Services {
    void* context;
    // Copies the already-updated applicator's XYZ displacement into `out`.
    // This is a read only snapshot: it must not advance the animator.
    std::int32_t (*read_delta)(void*, std::uintptr_t animator_identity,
                              std::uintptr_t applicator_identity, float out[3]);
};

struct Request {
    const AnimatorApplicator* animators;
    std::uint32_t count;
    const Services* services;
    float* output_xyz;
    std::uint32_t reserved;
};

// Mirrors RootSceneNode::_CalcDelta @ 0x35cd5c: visit the attached animator
// list in source order and add each current applicator XYZ delta. No animation
// step, weight update, transform, or owner lifetime is created here.
// Returns 0 on success, 1 for malformed input, or 2 if a provider cannot
// supply an exact attached animator/applicator pair. Output is unchanged on
// failure. IEEE float values are retained as in the source function.
int collect(const Request* request);

} // namespace dh2::animation::root_motion_applicators_v1
