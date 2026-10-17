#pragma once

#include "camera_math.hpp"

#include <cstddef>
#include <cstdint>

namespace dh2::engine_camera::camera_node_transform {

using Word = std::uint32_t;

// Only the fields read/written by CCameraSceneNode::recalculateMatrices are
// typed. Opaque bytes preserve the source object's observed field offsets.
struct CameraNode {
    std::uint8_t prefix_0000[0x54];
    camera_math::Vector3 absolute_position; // ISceneNode cached +0x54
    std::uint8_t prefix_0060[0x138 - 0x60];
    camera_math::Vector3 target;            // +0x138
    camera_math::Vector3 up;                // +0x144
    std::uint8_t prefix_0150[0x1ec - 0x150];
    camera_math::Matrix4 view_matrix;       // +0x1ec
};
static_assert(offsetof(CameraNode, absolute_position) == 0x54);
static_assert(offsetof(CameraNode, target) == 0x138);
static_assert(offsetof(CameraNode, up) == 0x144);
static_assert(offsetof(CameraNode, view_matrix) == 0x1ec);

struct SceneServices {
    void* context = nullptr;
    std::size_t context_extent = 0;
    // Source call boundaries made by recalculateMatrices after committing the
    // view cache. Both are synchronous and may update opaque frustum/node data.
    // transform_state is always called with source state 0 in this slice.
    void (*transform_state)(void*, void*, std::uint32_t) noexcept = nullptr;
    void (*recalculate_view_area)(void*, CameraNode*) noexcept = nullptr;
};

enum class Status : std::int32_t {
    complete,
    invalid_argument,
    reentrant_call
};

// Source CCameraSceneNode::recalculateMatrices at 0x583280. The absolute
// position is the existing ISceneNode cache at +0x54; scene-tree ownership or
// parent/world transform calculation is not provided by this slice.
Status recalculate_matrices(CameraNode*, const camera_math::MathServices*,
                           const SceneServices*);

} // namespace dh2::engine_camera::camera_node_transform
