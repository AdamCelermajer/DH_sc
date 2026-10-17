#pragma once

#include "native_character_controller_v1.hpp"

#include <cmath>
#include <cstdint>

namespace dh2::native::player_input_controller_v1 {

enum class Source : std::uint32_t {
    android_movement_control,
    authored_hud_projected,
    v2_gamepad,
};

enum class Status : std::uint8_t { complete, invalid_argument, controller_failed };

// v2Controller::Cmd_* permits its +9 forced route, otherwise both the global
// s_blocked flag and per-controller +8 lock must be clear.
inline bool source_controller_allowed(bool forced, bool globally_blocked,
                                     bool controller_locked) noexcept {
    return forced || (!globally_blocked && !controller_locked);
}

struct Input {
    const float* direction = nullptr;
    Source source = Source::android_movement_control;
    // Used by authored_hud_projected, whose source Joystick.Update already
    // rotated and scaled its vector. For the other sources, activity is derived
    // by the matching source input adapter.
    bool active = false;
    float camera_yaw = 0.0f;
    float camera_pitch = 0.0f;
    bool camera_active = false;
    // Authored SWF pointer-up calls v2Controller::Cmd_Stop directly. Keep that
    // command distinct from HeadTowards(0), whose source stop is conditional
    // on the Character already having an active heading.
    bool stop_requested = false;
};

struct Projection {
    float direction[3]{};
    bool active = false;
    bool stop_requested = false;
};

struct DispatchResult {
    Projection input{};
    character_controller_v1::Outcome outcome = character_controller_v1::Outcome::unchanged;
};

// Renderer queries source skill/cast state only for a controller-enabled
// nonzero HeadTowards. This shares the exact vector threshold with the
// canonical dispatcher, so release/tiny vectors bypass those queries.
inline bool requires_skill_cast_gates(const Projection& input,
                                     bool controller_enabled) noexcept {
    const float length_squared = input.direction[0] * input.direction[0] +
        input.direction[1] * input.direction[1] +
        input.direction[2] * input.direction[2];
    return character_controller_v1::has_source_head_towards_vector(
        controller_enabled, input.active, length_squared);
}

// Character::Ctrl_HeadTowards clears CharAI's current target when the new
// heading differs from target direction by more than 1.5708 radians. Keep the
// exact source comparison (angleCos -> acosf -> strict threshold), including
// its NaN behavior for a zero-length target vector. Call only after the same
// controller and skill/cast gates accepted this HeadTowards command.
inline bool should_clear_ai_target(const Projection& input,
                                  const float* target_direction,
                                  bool controller_enabled,
                                  bool using_skill,
                                  bool casting) noexcept {
    if (!target_direction || !requires_skill_cast_gates(input, controller_enabled) ||
        using_skill || casting) return false;
    for (unsigned i = 0; i < 3; ++i)
        if (!std::isfinite(target_direction[i])) return false;
    const float target_length_squared = target_direction[0] * target_direction[0] +
        target_direction[1] * target_direction[1] +
        target_direction[2] * target_direction[2];
    if (!(target_length_squared > 0.0f)) return false;
    const float dot = input.direction[0] * target_direction[0] +
        input.direction[1] * target_direction[1] +
        input.direction[2] * target_direction[2];
    const float input_length = std::sqrt(input.direction[0] * input.direction[0] +
        input.direction[1] * input.direction[1] +
        input.direction[2] * input.direction[2]);
    const float target_length = std::sqrt(target_length_squared);
    return std::acos(dot / (input_length * target_length)) > 1.5708f;
}

// Project input into the original controller vector. MovementControl uses
// HUDControls' fixed +45-degree screen basis. The authored SWF joystick passes
// through its already-rotated vector. The v2 gamepad keeps its radial deadzone
// and camera-relative transform. None of these routes takes PathTo ownership.
inline Status project(const Input& input, Projection* out) {
    if (!out || !input.direction) return Status::invalid_argument;
    if (input.stop_requested && input.source == Source::v2_gamepad)
        return Status::invalid_argument;
    Projection next{};
    int status = 0;
    switch (input.source) {
    case Source::android_movement_control:
        if (input.stop_requested) {
            next.stop_requested = true;
            break;
        }
        for (unsigned i = 0; i < 3; ++i) next.direction[i] = input.direction[i];
        status = camera_input_v1::map_movement_control_input(next.direction, &next.active);
        break;
    case Source::authored_hud_projected:
        if (input.stop_requested) {
            next.stop_requested = true;
            break;
        }
        if (input.active) {
            for (unsigned i = 0; i < 3; ++i) {
                if (!std::isfinite(input.direction[i])) return Status::invalid_argument;
                next.direction[i] = input.direction[i];
            }
            next.active = true;
        } else {
            next.direction[0] = next.direction[1] = next.direction[2] = 0.0f;
        }
        break;
    case Source::v2_gamepad:
        for (unsigned i = 0; i < 3; ++i) next.direction[i] = input.direction[i];
        status = camera_input_v1::map_touch_ground_input(
            next.direction, input.camera_yaw, input.camera_pitch,
            input.camera_active, &next.active);
        break;
    default:
        return Status::invalid_argument;
    }
    if (status) return Status::invalid_argument;
    *out = next;
    return Status::complete;
}

// Dispatch the projected vector through the existing Character controller
// owner. Callers query source skill/cast gates after projection and before this
// call, matching the original Update order. Path movement remains on Cmd_MoveTo.
inline Status dispatch(const Projection& input,
                       dh2::navigation::HeadingState* heading,
                       float* movement_heading_angle,
                       bool controller_enabled,
                       bool using_skill,
                       bool casting,
                       const character_controller_v1::Services* services,
                       DispatchResult* out) {
    if (!heading || !movement_heading_angle || !services || !out)
        return Status::invalid_argument;
    if (input.stop_requested) {
        if (!controller_enabled) {
            *out = DispatchResult{input, character_controller_v1::Outcome::unchanged};
            return Status::complete;
        }
        if (services->stop_game_object(services->context) < 0)
            return Status::controller_failed;
        if (services->raise_character_event(services->context, 63) < 0)
            return Status::controller_failed;
        *out = DispatchResult{input, character_controller_v1::Outcome::stopped};
        return Status::complete;
    }
    character_controller_v1::Outcome outcome{};
    const auto status = character_controller_v1::dispatch_head_towards(
        heading, movement_heading_angle, input.direction, input.active,
        controller_enabled, using_skill, casting, services, &outcome);
    if (status == character_controller_v1::Status::invalid_argument)
        return Status::invalid_argument;
    if (status != character_controller_v1::Status::complete)
        return Status::controller_failed;
    *out = DispatchResult{input, outcome};
    return Status::complete;
}

} // namespace dh2::native::player_input_controller_v1
