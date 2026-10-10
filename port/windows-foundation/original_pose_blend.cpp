#include "original_pose_blend.hpp"
#include "../engine-animation/animation_blend.hpp"
#include <algorithm>
#include <cmath>

namespace dh::foundation {
namespace {
bool valid_pose(const SkeletalPose& pose, std::string& error) {
    if (pose.empty() || pose.size() > 65536) { error = "Skeletal pose node domain is invalid"; return false; }
    for (const auto& node : pose) {
        for (float value : node.translation) if (!std::isfinite(value)) { error="Nonfinite pose position"; return false; }
        for (float value : node.scale) if (!std::isfinite(value)) { error="Nonfinite pose scale"; return false; }
        for (float value : node.quaternion) if (!std::isfinite(value)) { error="Nonfinite pose quaternion"; return false; }
    }
    return true;
}
bool same_domain(const SkeletalPose& a, const SkeletalPose& b) {
    if (a.size()!=b.size()) return false;
    for (std::size_t index=0; index<a.size(); ++index) if (a[index].id!=b[index].id) return false;
    return true;
}
}

bool capture_scene_pose(const dh2::scene::Scene& scene, SkeletalPose& output, std::string& error) {
    error.clear(); SkeletalPose pose;
    pose.reserve(scene.graph.size());
    for (const auto& node : scene.graph) {
        LocalNodePose local; local.id=node.id;
        std::copy(node.translation,node.translation+3,local.translation.begin());
        std::copy(node.quaternion,node.quaternion+4,local.quaternion.begin());
        std::copy(node.scale,node.scale+3,local.scale.begin());
        pose.push_back(std::move(local));
    }
    if (!valid_pose(pose,error)) return false;
    output=std::move(pose); return true;
}

bool apply_scene_pose(const SkeletalPose& pose, dh2::scene::Scene& scene, std::string& error) {
    error.clear();
    if (!valid_pose(pose,error)) return false;
    if (pose.size()!=scene.graph.size()) { error="Pose scene node count differs"; return false; }
    for (std::size_t index=0; index<pose.size(); ++index)
        if (pose[index].id!=scene.graph[index].id) { error="Pose scene node identity differs"; return false; }
    // A separate scene computes all hierarchy results before modifying existing
    // source-owned node cells. Assigning a whole Scene would replace those owners.
    auto candidate=scene;
    for (std::size_t index=0; index<pose.size(); ++index) {
        auto& node=candidate.graph[index]; const auto& local=pose[index];
        std::copy(local.translation.begin(),local.translation.end(),node.translation);
        std::copy(local.quaternion.begin(),local.quaternion.end(),node.quaternion);
        std::copy(local.scale.begin(),local.scale.end(),node.scale);
    }
    if (!dh2::scene::update_world(candidate,error)) return false;
    for (const auto& node : candidate.graph)
        for (float value : node.world) if (!std::isfinite(value)) { error="Mixed pose hierarchy overflow"; return false; }
    for (const auto& instance : candidate.instances)
        for (float value : instance.world) if (!std::isfinite(value)) { error="Mixed pose instance overflow"; return false; }
    for (std::size_t index=0; index<pose.size(); ++index) {
        auto& node=scene.graph[index]; const auto& next=candidate.graph[index];
        std::copy(next.translation,next.translation+3,node.translation);
        std::copy(next.quaternion,next.quaternion+4,node.quaternion);
        std::copy(next.scale,next.scale+3,node.scale);
        node.world=next.world;
    }
    for (std::size_t index=0; index<scene.instances.size(); ++index) scene.instances[index].world=candidate.instances[index].world;
    return true;
}

bool OriginalPoseBlend::select(std::int32_t incoming_blend_out_ms, std::string& error) {
    error.clear();
    if (dh2_blender_begin(&state_,incoming_blend_out_ms)) { error="Source pose blend selection rejected"; return false; }
    return true;
}

bool OriginalPoseBlend::evaluate(std::uint32_t timestamp, const PoseSampler& sample,
                                 SkeletalPose& output, std::string& error) {
    error.clear();
    if (!sample) { error="Pose sampler is absent"; return false; }
    auto state=state_; auto cached=cached_;
    if (dh2_blender_update_weights(&state,timestamp)) { error="Source pose blend weight update rejected"; return false; }
    for (std::uint32_t slot=0; slot<2; ++slot) {
        if (state.weights[slot]==0) continue;
        if (!sample(slot,cached[slot],error) || !valid_pose(cached[slot],error)) return false;
    }
    if (dh2_blender_normalize(&state)) { error="Source pose blend normalization rejected"; return false; }
    const auto populated = cached[0].empty() ? 1u : 0u;
    if (cached[populated].empty()) { error="Blend has no sampled pose"; return false; }
    // Initial inactive AnimatorSet has no sample. Its zero-weight storage cannot
    // affect output; provide a finite same-domain placeholder for typed kernels.
    for (std::uint32_t slot=0;slot<2;++slot) {
        if (cached[slot].empty()) {
            if (state.weights[slot]!=0) { error="Weighted blend slot has no pose"; return false; }
            cached[slot]=cached[populated];
        }
    }
    if (!same_domain(cached[0],cached[1])) { error="Blend skeletal node identities differ"; return false; }
    SkeletalPose pose=cached[0];
    for (std::size_t index=0; index<pose.size(); ++index) {
        float positions[6],scales[6],quaternions[8];
        for (std::uint32_t slot=0;slot<2;++slot) {
            const auto& node=cached[slot][index];
            std::copy(node.translation.begin(),node.translation.end(),positions+slot*3);
            std::copy(node.scale.begin(),node.scale.end(),scales+slot*3);
            std::copy(node.quaternion.begin(),node.quaternion.end(),quaternions+slot*4);
        }
        auto& node=pose[index];
        if (dh2_animation_blend_vector3(node.translation.data(),positions,state.weights,2)
            || dh2_animation_blend_vector3(node.scale.data(),scales,state.weights,2)
            || dh2_animation_blend_quaternion(node.quaternion.data(),quaternions,state.weights,2)) {
            error="Source typed pose contribution rejected"; return false;
        }
    }
    if (!valid_pose(pose,error)) return false;
    state.last_time=timestamp;
    state_=state; cached_=std::move(cached); output=std::move(pose);
    return true;
}

void OriginalPoseBlend::clear() noexcept {
    state_={0,0,0,0,0,0,{1,0}}; cached_[0].clear(); cached_[1].clear();
}

} // namespace dh::foundation
