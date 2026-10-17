#pragma once

#include "navigation_heading.hpp"

#include <cmath>

namespace dh2::native::camera_input_v1 {

inline constexpr float kSwampDefaultCameraEyeOffset[3]{1380.0f,-1380.0f,2450.0f};

// Convert an authored camera-to-target offset (renderer world axes) to the
// orbit angles used by the native renderer. This is a coordinate conversion;
// source joystick conditioning and rotation remain in the functions below.
inline int orbit_from_eye_offset(const float* eye_offset,
                                 float* camera_yaw, float* camera_pitch,
                                 float* camera_distance = nullptr) {
    if (!eye_offset || !camera_yaw || !camera_pitch ||
        !std::isfinite(eye_offset[0]) || !std::isfinite(eye_offset[1]) ||
        !std::isfinite(eye_offset[2])) return 1;
    const float horizontal = std::hypot(eye_offset[0], eye_offset[1]);
    const float length = std::hypot(horizontal, eye_offset[2]);
    if (!std::isfinite(length) || length <= 0.0f) return 1;
    *camera_yaw = std::atan2(eye_offset[1], eye_offset[0]);
    *camera_pitch = std::atan2(eye_offset[2], horizontal);
    if (camera_distance) *camera_distance = length;
    return 0;
}

// Reconstruct the camera-relative part of v2GamepadController::Update
// (0x406c2c). CameraBase::GetCameraLookAtVec (0x40e8a8) supplies a full 3D
// direction. The orbit adapter below derives center-eye from the renderer's
// yaw/pitch camera; navigation_heading supplies the IDA-matched
// Point3D::angle/rotateXY arithmetic. Returns 0 on success and 1 on bad input.
inline int camera_look_at_from_orbit(float camera_yaw, float camera_pitch, float* look_at) {
    if (!look_at || !std::isfinite(camera_yaw) || !std::isfinite(camera_pitch)) return 1;
    const float cos_pitch = std::cos(camera_pitch);
    float forward[3]{-std::cos(camera_yaw) * cos_pitch,
                     -std::sin(camera_yaw) * cos_pitch,
                     -std::sin(camera_pitch)};
    const float length = std::sqrt(forward[0] * forward[0] + forward[1] * forward[1] +
                                   forward[2] * forward[2]);
    if (!std::isfinite(length) || length <= 0.0f) return 1;
    for (unsigned i = 0; i < 3; ++i) look_at[i] = forward[i] / length;
    return 0;
}

inline int rotate_ground_input(float* direction, float camera_yaw, float camera_pitch = 0.0f) {
    float look_at[3]{};
    if (camera_look_at_from_orbit(camera_yaw, camera_pitch, look_at)) return 1;
    return dh2_nav_rotate_input_for_camera(direction, look_at);
}

// v2GamepadController::Update sends the conditioned vector through
// v2Controller::Cmd_HeadTowards -> Character::Ctrl_HeadTowards ->
// GameObject::SetHeadingDirection (IDA: 0x4082b0, 0x405374, 0x3adb60,
// 0x393be8). This operation updates heading/activity/angle only. The source
// HeadTowards path does not call PathTo or SetDestination; destination and
// route ownership remain with the existing world/path owner.
inline int apply_head_towards(dh2::navigation::HeadingState* heading,
                              float* movement_heading_angle,
                              const float* mapped_direction) {
    if (!heading || !movement_heading_angle || heading->reserved ||
        !std::isfinite(heading->angle) || !std::isfinite(*movement_heading_angle) ||
        !mapped_direction || !std::isfinite(mapped_direction[0]) ||
        !std::isfinite(mapped_direction[1]) || !std::isfinite(mapped_direction[2])) return 1;
    auto next = *heading;
    if (dh2_nav_set_heading(&next, mapped_direction, 1)) return 1;
    *heading = next;
    *movement_heading_angle = next.angle;
    return 0;
}

// v2GamepadController::Update converts calibrated analog axes into a radial
// stick vector: deadzone length 0.25, normalize, then scale the sub-unit range
// linearly to [0,1]. CameraBase rotation is applied afterward. `active` reports
// whether the resulting vector is nonzero so callers can gate Move/state events
// using the same conditioned input rather than a separate raw-axis threshold.
// This returns a direction per controller update; do not multiply by dt here.
// The recovered dt*1.25 accumulation is a separate `Character+5320` branch.
inline int map_touch_ground_input(float* direction, float camera_yaw, float camera_pitch,
                                  bool camera_active, bool* active) {
    if (!direction || !active || !std::isfinite(direction[0]) ||
        !std::isfinite(direction[1]) || !std::isfinite(direction[2])) return 1;
    float mapped[3]{direction[0], direction[1], 0.0f};
    const float length = std::sqrt(mapped[0] * mapped[0] + mapped[1] * mapped[1]);
    if (!std::isfinite(length)) return 1;
    bool next_active = false;
    if (length >= 0.25f) {
        mapped[0] /= length;
        mapped[1] /= length;
        if (length < 1.0f) {
            const float scale = (length - 0.25f) / 0.75f;
            mapped[0] *= scale;
            mapped[1] *= scale;
        }
        next_active = mapped[0] != 0.0f || mapped[1] != 0.0f;
    } else {
        mapped[0] = mapped[1] = 0.0f;
    }
    if (camera_active && next_active && rotate_ground_input(mapped, camera_yaw, camera_pitch)) return 1;
    direction[0] = mapped[0];
    direction[1] = mapped[1];
    direction[2] = 0.0f;
    *active = next_active;
    return 0;
}

// Match HUDControls::OnEvent (IDA 0x4197f4-0x419970) for the Android
// virtual stick. The source rotates its normalized (1,-1) basis by
// 90deg-atan2(screen_dy,screen_dx); native move_axis supplies screen-up as
// positive Y, so screen_dy=-direction[1]. This simplifies to a fixed +45deg
// basis transform. HUDControls preserves radial magnitude and the downstream
// SetHeadingDirection considers XY active only above squared length 1e-4.
// Inputs are the normalized/clamped axes already produced by MainActivity.
inline int map_hud_touch_input(float* direction, bool* active) {
    if (!direction || !active || !std::isfinite(direction[0]) ||
        !std::isfinite(direction[1]) || !std::isfinite(direction[2])) return 1;
    constexpr float kInvSqrtTwo = 0.7071067811865475244f;
    const float x = direction[0];
    const float y = direction[1];
    const float mapped_x = (x - y) * kInvSqrtTwo;
    const float mapped_y = (x + y) * kInvSqrtTwo;
    const float magnitude_squared = x * x + y * y;
    if (!std::isfinite(mapped_x) || !std::isfinite(mapped_y) ||
        !std::isfinite(magnitude_squared)) return 1;
    direction[0] = mapped_x;
    direction[1] = mapped_y;
    direction[2] = 0.0f;
    *active = magnitude_squared > 0.0001f;
    return 0;
}

// MainActivity's MovementControl mirrors HUDControls::OnEvent/Update. It
// submits a normalized screen vector with screen-up as +Y; source HUD clamps
// both axes then projects over-radius input back along its original angle.
// That yields the same unit-circle projection here. Unlike v2GamepadController,
// this route has no deadzone and is not camera-relative.
inline int map_movement_control_input(float* direction, bool* active) {
    if (!direction || !active || !std::isfinite(direction[0]) ||
        !std::isfinite(direction[1]) || !std::isfinite(direction[2])) return 1;
    float length = std::sqrt(direction[0] * direction[0] +
                             direction[1] * direction[1]);
    if (!std::isfinite(length)) return 1;
    if (length > 1.0f) {
        direction[0] /= length;
        direction[1] /= length;
    }
    direction[2] = 0.0f;
    return map_hud_touch_input(direction, active);
}

}  // namespace dh2::native::camera_input_v1
