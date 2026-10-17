#pragma once

#include "camera_math.hpp"

#include <cstddef>
#include <cstdint>

namespace dh2::engine_camera::camera_projection {

struct CameraNode {
    std::uint8_t prefix_0000[0x150];
    std::uint32_t orthographic_half_height; // source +0x150
    std::uint32_t fov;                       // source +0x154
    std::uint32_t aspect;                    // source +0x158
    std::uint32_t near_plane;                // source +0x15c
    std::uint32_t far_plane;                 // source +0x160
    std::uint8_t far_at_infinity;            // source +0x164
    std::uint8_t prefix_0165[0x274 - 0x165];
    camera_math::Matrix4 projection_matrix;  // source +0x274; copied as 0x41 bytes
};
static_assert(offsetof(CameraNode, orthographic_half_height) == 0x150);
static_assert(offsetof(CameraNode, fov) == 0x154);
static_assert(offsetof(CameraNode, aspect) == 0x158);
static_assert(offsetof(CameraNode, near_plane) == 0x15c);
static_assert(offsetof(CameraNode, far_plane) == 0x160);
static_assert(offsetof(CameraNode, far_at_infinity) == 0x164);
static_assert(offsetof(CameraNode, projection_matrix) == 0x274);

struct Services {
    void* context = nullptr;
    std::size_t context_extent = 0;
    // Source CCameraSceneNode virtual query at +0x150.
    bool (*is_orthogonal)(void*, const CameraNode*) noexcept = nullptr;
    // Source calls SViewFrustum::setTransformState(2) on camera+0x168.
    void (*transform_state)(void*, void*, std::uint32_t) noexcept = nullptr;
};

enum class Status : std::int32_t { complete, invalid_argument };

// Source CCameraSceneNode::recalculateProjectionMatrix at 0x58364c.
// The cache is committed before the source transformation-state callback.
Status recalculate_projection_matrix(CameraNode*, const Services*) noexcept;

} // namespace dh2::engine_camera::camera_projection
